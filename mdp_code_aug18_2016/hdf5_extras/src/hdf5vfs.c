/* hdf5vfs.c

   by Leland Pierce, Jan 25, 2014.
  
*/

/* 
   This file is a re-working of the file test_demovfs.c
   that comes with sqlite.

   This uses the functions written for interacting with HDF5
   files and objects in order to implement all the functionality
   needed by an sqlite vfs.


 */

#include "hdf5vfs.h"
#include "string_funcs.h"
#include "gmalloc.h"
#include "bstrlib.h"
#include "hdf5mine.h"

static hid_t decompose_filename(const char *in_filename, char **dataset_name);

//#define DEBUG


/*
** 2010 April 7
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
**
** This file implements an example of a simple VFS implementation that 
** omits complex features often not required or not possible on embedded
** platforms.  Code is included to buffer writes to the journal file, 
** which can be a significant performance improvement on some embedded
** platforms.
**
** OVERVIEW
**
**   The code in this file implements a minimal SQLite VFS that can be 
**   used on Linux and other posix-like operating systems. The following 
**   system calls are used:
**
**    File-system: access(), unlink(), getcwd()
**    File IO:     open(), read(), write(), fsync(), close(), fstat()
**    Other:       sleep(), usleep(), time()
**
**   The following VFS features are omitted:
**
**     1. File locking. The user must ensure that there is at most one
**        connection to each database when using this VFS. Multiple
**        connections to a single shared-cache count as a single connection
**        for the purposes of the previous statement.
**
**     2. The loading of dynamic extensions (shared libraries).
**
**     3. Temporary files. The user must configure SQLite to use in-memory
**        temp files when using this VFS. The easiest way to do this is to
**        compile with:
**
**          -DSQLITE_TEMP_STORE=3
**
**     4. File truncation. As of version 3.6.24, SQLite may run without
**        a working xTruncate() call, providing the user does not configure
**        SQLite to use "journal_mode=truncate", or use both
**        "journal_mode=persist" and ATTACHed databases.
**
**   It is assumed that the system uses UNIX-like path-names. Specifically,
**   that '/' characters are used to separate path components and that
**   a path-name is a relative path unless it begins with a '/'. And that
**   no UTF-8 encoded paths are greater than 512 bytes in length.
**
** JOURNAL WRITE-BUFFERING
**
**   To commit a transaction to the database, SQLite first writes rollback
**   information into the journal file. This usually consists of 4 steps:
**
**     1. The rollback information is sequentially written into the journal
**        file, starting at the start of the file.
**     2. The journal file is synced to disk.
**     3. A modification is made to the first few bytes of the journal file.
**     4. The journal file is synced to disk again.
**
**   Most of the data is written in step 1 using a series of calls to the
**   VFS xWrite() method. The buffers passed to the xWrite() calls are of
**   various sizes. For example, as of version 3.6.24, when committing a 
**   transaction that modifies 3 pages of a database file that uses 4096 
**   byte pages residing on a media with 512 byte sectors, SQLite makes 
**   eleven calls to the xWrite() method to create the rollback journal, 
**   as follows:
**
**             Write offset | Bytes written
**             ----------------------------
**                        0            512
**                      512              4
**                      516           4096
**                     4612              4
**                     4616              4
**                     4620           4096
**                     8716              4
**                     8720              4
**                     8724           4096
**                    12820              4
**             ++++++++++++SYNC+++++++++++
**                        0             12
**             ++++++++++++SYNC+++++++++++
**
**   On many operating systems, this is an efficient way to write to a file.
**   However, on some embedded systems that do not cache writes in OS 
**   buffers it is much more efficient to write data in blocks that are
**   an integer multiple of the sector-size in size and aligned at the
**   start of a sector.
**
**   To work around this, the code in this file allocates a fixed size
**   buffer of SQLITE_DEMOVFS_BUFFERSZ using sqlite3_malloc() whenever a 
**   journal file is opened. It uses the buffer to coalesce sequential
**   writes into aligned SQLITE_DEMOVFS_BUFFERSZ blocks. When SQLite
**   invokes the xSync() method to sync the contents of the file to disk,
**   all accumulated data is written out, even if it does not constitute
**   a complete block. This means the actual IO to create the rollback 
**   journal for the example transaction above is this:
**
**             Write offset | Bytes written
**             ----------------------------
**                        0           8192
**                     8192           4632
**             ++++++++++++SYNC+++++++++++
**                        0             12
**             ++++++++++++SYNC+++++++++++
**
**   Much more efficient if the underlying OS is not caching write 
**   operations.
*/



/*
** Write directly to the file passed as the first argument. Even if the
** file has a write-buffer (DemoFile.aBuffer), ignore it.
*/
static int HDFDirectWrite(
  HDFFile *p,                    /* File handle */
  const void *zBuf,               /* Buffer containing data to write */
  int iAmt,                       /* Size of data to write in bytes */
  sqlite_int64 iOfst              /* File offset to write to */
){
  size_t nWrite;                  /* Return value from write() */

#ifdef DEBUG
  printf("HDFDirectWrite: about to seek to %ld\n",(long int)iOfst);
#endif

  if(!IFileSeek(p->ifilep, iOfst, SEEK_SET)){
    return SQLITE_IOERR_WRITE;
  }

#ifdef DEBUG
  printf("HDFDirectWrite: about to write %d bytes, p->ifilep=%p\n",iAmt,p->ifilep);
#endif


  nWrite = IFileWrite(p->ifilep, zBuf, sizeof(char), iAmt);
  if( nWrite!=iAmt ){
#ifdef DEBUG
    printf("HDFDirectWrite: error: nWrite=%d, \n",(int)nWrite);
    printf("                %s\n",error_string);
#endif

    return SQLITE_IOERR_WRITE;
  }

#ifdef DEBUG
  printf("HDFDirectWrite: success\n");
#endif

  return SQLITE_OK;
}

/*
** Flush the contents of the HDFFile.aBuffer buffer to disk. This is a
** no-op if this particular file does not have a buffer (i.e. it is not
** a journal file) or if the buffer is currently empty.
*/
static int HDFFlushBuffer(HDFFile *p){
  int rc = SQLITE_OK;

#ifdef DEBUG
  printf("HDFFlushBuffer: top\n");
#endif

  if( p->nBuffer ){
    rc = HDFDirectWrite(p, p->aBuffer, p->nBuffer, p->iBufferOfst);
    p->nBuffer = 0;
  }
#ifdef DEBUG
  printf("HDFFlushBUffer: returning rc=%d\n",rc);
#endif

  return rc;
}

/*
** Close a file.
*/
static int HDFClose(sqlite3_file *pFile){
  int rc;
  HDFFile *p = (HDFFile*)pFile;
  rc = HDFFlushBuffer(p);
  sqlite3_free(p->aBuffer);
#ifdef DEBUG
  printf("HDFClose: about to call IFileClose()\n");
#endif
  IFileClose(p->ifilep);
  return rc;
}

/*
** Read data from a file.
*/
static int HDFRead(
  sqlite3_file *pFile, 
  void *zBuf, 
  int iAmt, 
  sqlite_int64 iOfst
){
  HDFFile *p = (HDFFile*)pFile;
  off_t ofst;                     /* Return value from lseek() */
  int nRead;                      /* Return value from read() */
  int rc;                         /* Return code from demoFlushBuffer() */

  /* Flush any data in the write buffer to disk in case this operation
  ** is trying to read data the file-region currently cached in the buffer.
  ** It would be possible to detect this case and possibly save an 
  ** unnecessary write here, but in practice SQLite will rarely read from
  ** a journal file when there is data cached in the write-buffer.
  */

#ifdef DEBUG
  printf("HDFRead: 1\n");
#endif

  rc = HDFFlushBuffer(p);
  if( rc!=SQLITE_OK ){
    return rc;
  }

#ifdef DEBUG
  printf("HDFRead: 2\n");
#endif


  if(!IFileSeek(p->ifilep, iOfst, SEEK_SET)){

#ifdef DEBUG
    printf("HDFRead: 2.5. seek failed: returning error\n");
#endif

    return SQLITE_IOERR_READ;
  }

#ifdef DEBUG
  printf("HDFRead: 3\n");
#endif

  nRead = IFileRead(p->ifilep, zBuf, sizeof(char), iAmt);

#ifdef DEBUG
  printf("HDFRead: 4\n");
#endif


  if( nRead==iAmt ){
#ifdef DEBUG
  printf("HDFRead: 5\n");
#endif

    return SQLITE_OK;
  }else if( nRead>=0 ){

#ifdef DEBUG
  printf("HDFRead: 6\n");
#endif

    return SQLITE_IOERR_SHORT_READ;
  }

#ifdef DEBUG
  printf("HDFRead: 7\n");
#endif

  return SQLITE_IOERR_READ;
}

/*
** Write data to a crash-file.
*/
static int HDFWrite(
  sqlite3_file *pFile, 
  const void *zBuf, 
  int iAmt, 
  sqlite_int64 iOfst
){
  HDFFile *p = (HDFFile*)pFile;
  
  if( p->aBuffer ){
    char *z = (char *)zBuf;       /* Pointer to remaining data to write */
    int n = iAmt;                 /* Number of bytes at z */
    sqlite3_int64 i = iOfst;      /* File offset to write to */

#ifdef DEBUG
  printf("HDFWrite: 1\n");
#endif

    while( n>0 ){
      int nCopy;                  /* Number of bytes to copy into buffer */

#ifdef DEBUG
  printf("HDFWrite: 2\n");
#endif

      /* If the buffer is full, or if this data is not being written directly
      ** following the data already buffered, flush the buffer. Flushing
      ** the buffer is a no-op if it is empty.  
      */
      if( p->nBuffer==SQLITE_DEMOVFS_BUFFERSZ || p->iBufferOfst+p->nBuffer!=i ){
        int rc = HDFFlushBuffer(p);
        if( rc!=SQLITE_OK ){
          return rc;
        }
      }
      //assert( p->nBuffer==0 || p->iBufferOfst+p->nBuffer==i );
      p->iBufferOfst = i - p->nBuffer;

      /* Copy as much data as possible into the buffer. */
      nCopy = SQLITE_DEMOVFS_BUFFERSZ - p->nBuffer;
      if( nCopy>n ){
        nCopy = n;
      }
      memcpy(&p->aBuffer[p->nBuffer], z, nCopy);
      p->nBuffer += nCopy;

      n -= nCopy;
      i += nCopy;
      z += nCopy;
    }
  }else{
#ifdef DEBUG
  printf("HDFWrite: 3\n");
#endif

    return HDFDirectWrite(p, zBuf, iAmt, iOfst);
  }

#ifdef DEBUG
  printf("HDFWrite: 4\n");
#endif

  return SQLITE_OK;
}

/*
** Truncate a file. 
*/
static int HDFTruncate(sqlite3_file *pFile, sqlite_int64 size){

#ifdef DEBUG
  printf("HDFTruncate: 1\n");
#endif

  if( !IFileTruncate(((HDFFile *)pFile)->ifilep, size) ) {
#ifdef DEBUG
  printf("HDFTruncate: 2\n");
#endif
    return SQLITE_IOERR_TRUNCATE;
  }
#ifdef DEBUG
  printf("HDFTruncate: 3\n");
#endif

  return SQLITE_OK;
}

/*
** Sync the contents of the file to the persistent media.
*/
static int HDFSync(sqlite3_file *pFile, int flags){
  HDFFile *p = (HDFFile*)pFile;
  int rc;

#ifdef DEBUG
  printf("HDFSync: 1\n");
#endif

  rc = HDFFlushBuffer(p);

  if( rc!=SQLITE_OK ){
#ifdef DEBUG
  printf("HDFSync: 2\n");
#endif

    return rc;
  }

#ifdef DEBUG
  printf("HDFSync: 3\n");
#endif

  return SQLITE_OK;
}

/*
** Write the size of the file in bytes to *pSize.
*/
static int HDFFileSize(sqlite3_file *pFile, sqlite_int64 *pSize){
  HDFFile *p = (HDFFile*)pFile;
  int rc;                         /* Return code from fstat() call */

  /* Flush the contents of the buffer to disk. As with the flush in the
  ** demoRead() method, it would be possible to avoid this and save a write
  ** here and there. But in practice this comes up so infrequently it is
  ** not worth the trouble.
  */

#ifdef DEBUG
  printf("HDFFileSize: 1\n");
#endif

  rc = HDFFlushBuffer(p);
  if( rc!=SQLITE_OK ){

#ifdef DEBUG
  printf("HDFFileSize: 2\n");
#endif

    return rc;
  }

#ifdef DEBUG
  printf("HDFFileSize: 3\n");
#endif

  *pSize = IFileSize(p->ifilep);

#ifdef DEBUG
  printf("HDFFileSize: returning OK, size=%ld\n",(long int)*pSize);
#endif

  return SQLITE_OK;
}

/*
** Locking functions. The xLock() and xUnlock() methods are both no-ops.
** The xCheckReservedLock() always indicates that no other process holds
** a reserved lock on the database file. This ensures that if a hot-journal
** file is found in the file-system it is rolled back.
*/
static int HDFLock(sqlite3_file *pFile, int eLock){
  return SQLITE_OK;
}
static int HDFUnlock(sqlite3_file *pFile, int eLock){
  return SQLITE_OK;
}
static int HDFCheckReservedLock(sqlite3_file *pFile, int *pResOut){
  *pResOut = 0;
  return SQLITE_OK;
}

/*
** No xFileControl() verbs are implemented by this VFS.
*/
static int HDFFileControl(sqlite3_file *pFile, int op, void *pArg){
  return SQLITE_NOTFOUND;
}

/*
** The xSectorSize() and xDeviceCharacteristics() methods. These two
** may return special values allowing SQLite to optimize file-system 
** access to some extent. But it is also safe to simply return 0.
*/
static int HDFSectorSize(sqlite3_file *pFile){
  return 0;
}
static int HDFDeviceCharacteristics(sqlite3_file *pFile){
  return 0;
}

/* 
** deal with filenames encoding both file_id and dataset_name
** format is : 123456:/dataset/name
*/
static hid_t decompose_filename(const char *in_filename, char **dataset_name)
{
  const char *the_semicolon;
  hid_t file_id;
  int L;
  char *the_filename;

  // save a mutable version of the input filename:
  L = strlen(in_filename);
  the_filename = (char *)GMalloc(sizeof(char)*(L+10));
  if(!the_filename) {
    *dataset_name = NULL;
    return -1;
  }
  strcpy(the_filename, in_filename);

  // find the semicolon:
  the_semicolon = find_first_char(the_filename, ':');
  if(!the_semicolon) {
    GFree(the_filename);
    *dataset_name = NULL;
    return -1;
  }// endif

  // copy the filename piece:
  L = strlen(the_semicolon+1);
  *dataset_name = (char *)GMalloc(sizeof(char)*(L+10));
  strcpy(*dataset_name, the_semicolon+1);

  // copy the file_id piece:
  if(sscanf(the_filename,"%d",&file_id) != 1) {
    GFree(the_filename);
    *dataset_name = NULL;
    return -1;
  }// endif

  GFree(the_filename);
  return file_id;
}








/*
** Open a file handle.
*/
static int HDFOpen(
  sqlite3_vfs *pVfs,              /* VFS */
  const char *zName,              /* File to open, or 0 for a temp file */
  sqlite3_file *pFile,            /* Pointer to HDFFile struct to populate */
  int flags,                      /* Input SQLITE_OPEN_XXX flags */
  int *pOutFlags                  /* Output SQLITE_OPEN_XXX flags (or NULL) */
){
  char mode[100];

  static const sqlite3_io_methods HDFio = {
    1,                            /* iVersion */
    HDFClose,                    /* xClose */
    HDFRead,                     /* xRead */
    HDFWrite,                    /* xWrite */
    HDFTruncate,                 /* xTruncate */
    HDFSync,                     /* xSync */
    HDFFileSize,                 /* xFileSize */
    HDFLock,                     /* xLock */
    HDFUnlock,                   /* xUnlock */
    HDFCheckReservedLock,        /* xCheckReservedLock */
    HDFFileControl,              /* xFileControl */
    HDFSectorSize,               /* xSectorSize */
    HDFDeviceCharacteristics     /* xDeviceCharacteristics */
  };


  HDFFile *p = (HDFFile*)pFile; /* Populate this structure */
  int oflags = 0;                 /* flags to pass to open() call */
  char *aBuf = 0;


#ifdef DEBUG
  printf("HDFOpen: zName=|%s|\n",zName);
#endif

  if( zName==0 ){
    return SQLITE_IOERR;
  }

  if( flags&SQLITE_OPEN_MAIN_JOURNAL ){
    aBuf = (char *)sqlite3_malloc(SQLITE_DEMOVFS_BUFFERSZ);
    if( !aBuf ){
      return SQLITE_NOMEM;
    }
  }

  hid_t file_id;
  char *dataset_name=0;
  file_id = decompose_filename(zName,&dataset_name);
  if(file_id <0) {
    GFree(dataset_name);
    return SQLITE_IOERR;
  }

  if( flags&SQLITE_OPEN_CREATE &&
      flags&SQLITE_OPEN_READWRITE ) {
    // this mode means to create if doesnt exist,
    // and open for read and write anywhere.
    // figure out if it exists:
    bstring dname = bfromcstr(dataset_name);
    if(GS_ObjectIsIFile(file_id,dname)){
      // already exists:
      strcpy(mode,"r+");
    } else {
      // need to create:
      strcpy(mode,"w+");
    }// endif
    bdestroy(dname);
  } else if ( flags&SQLITE_OPEN_CREATE ){
    strcpy(mode,"w+");
  } else if( flags&SQLITE_OPEN_READONLY ) {
    strcpy(mode,"r");
  } else if( flags&SQLITE_OPEN_READWRITE ) {
    strcpy(mode,"r+");
  } else {
    strcpy(mode,"r+");    
  }// endif
  
  //printf("hdf5vfs: ifilename=%s, mode=%s\n",dataset_name, mode);

  memset(p, 0, sizeof(HDFFile));


#ifdef DEBUG
  printf("HDFOpen: after decompose_filename: file_id=%d, dataset_name=|%s|\n",
         file_id, dataset_name);
#endif

  if(file_id < 0) {
    GFree(dataset_name);
    return SQLITE_IOERR;
  }// endif



  p->ifilep = IFileOpen(file_id, dataset_name, mode);

#ifdef DEBUG
  printf("HDFOpen: mode=|%s|, p->ifilep = %p\n",mode,p->ifilep);
#endif

  if( !p->ifilep ){
    sqlite3_free(aBuf);
    GFree(dataset_name);
    return SQLITE_CANTOPEN;
  }
  p->aBuffer = aBuf;

  if( pOutFlags ){
    *pOutFlags = flags;
  }
  p->base.pMethods = &HDFio;

#ifdef DEBUG
  printf("HDFOpen: returning OK\n");
#endif

  GFree(dataset_name);
  return SQLITE_OK;
}

/*
** Delete the file identified by argument zPath. If the dirSync parameter
** is non-zero, then ensure the file-system modification to delete the
** file has been synced to disk before returning.
*/
static int HDFDelete(sqlite3_vfs *pVfs, const char *zPath, int dirSync){
  int rc;                         /* Return code */

  hid_t file_id;
  char *dataset_name=0;

#ifdef DEBUG
  printf("HDFDelete: 1\n");
#endif

  file_id = decompose_filename(zPath,&dataset_name);
  if(file_id < 0) {
    GFree(dataset_name);
    return SQLITE_IOERR_DELETE;
  }// endif
#ifdef DEBUG
  printf("HDFDelete: 2, file_id=%d, dataset_name=|%s|\n",file_id,dataset_name);
#endif

  bstring dname = bfromcstr(dataset_name);
  GFree(dataset_name);
  if(!GS_ObjectIsIFile( file_id, dname )){
    bdestroy(dname);
    return SQLITE_OK;
  } 
  
#ifdef DEBUG
  printf("HDFDelete: 3, objisdataset...\n");
#endif


    rc = GS_DatasetDelete(file_id,dname);
#ifdef DEBUG
  printf("HDFDelete: 4, rc=%d\n",rc);
#endif

    bdestroy(dname);

  return (rc== TRUE ? SQLITE_OK : SQLITE_IOERR_DELETE);
}

#ifndef F_OK
# define F_OK 0
#endif
#ifndef R_OK
# define R_OK 4
#endif
#ifndef W_OK
# define W_OK 2
#endif

/*
** Query the file-system to see if the named file exists, is readable or
** is both readable and writable.
*/
static int HDFAccess(
  sqlite3_vfs *pVfs, 
  const char *zPath, 
  int flags, 
  int *pResOut
){
  int rc;                         /* access() return code */
  hid_t file_id;
  char *dataset_name;
  // need to do all these without IFileOpen'ing the Dataset
  // (because I don't know if the dataset is already 
  //  open or not when this is called.)
  // hid_t file_id, char *name


#ifdef DEBUG
  printf("HDFAccess: 1\n");
#endif

  rc =0;

  file_id = decompose_filename(zPath,&dataset_name);
  if(file_id < 0) {
    rc = -1;
    *pResOut = (rc==0);
    GFree(dataset_name);
#ifdef DEBUG
  printf("HDFAccess: 2\n");
#endif
    return SQLITE_OK;
  }// endif


#ifdef DEBUG
  printf("HDFAccess: 3, file_id=%d, dataset_name=|%s|\n",file_id,dataset_name);
#endif

  // hdf5 file must be writeable for the IFIle to be accessible at all:
  if(!GS_FileIsWriteable(file_id)){
    rc =-1;
    *pResOut = (rc==0);
    GFree(dataset_name);
#ifdef DEBUG
  printf("HDFAccess: 4\n");
#endif
    return SQLITE_OK;

  }// endif

#ifdef DEBUG
  printf("HDFAccess: 5\n");
#endif


  // name must be an IFile
  bstring dname = bfromcstr(dataset_name);
  if(!GS_ObjectIsIFile( file_id, dname )){
    rc = -1;
    *pResOut = (rc==0);
    GFree(dataset_name);
    bdestroy(dname);
#ifdef DEBUG
    printf("HDFAccess: 6, *pResOut=%d\n",*pResOut);
#endif
    return SQLITE_OK;

  }// endif

#ifdef DEBUG
  printf("HDFAccess: 7, rc=%d, file_id=%d, dataset_name=|%s|\n",
         rc, file_id, dataset_name);
#endif


  // dataset must be openable
  hid_t dataset_id;
  bassigncstr(dname,dataset_name);
  dataset_id = GS_DatasetOpen(file_id, dname);
  if(!dataset_id) {
    rc = -1;
    *pResOut = (rc==0);
    GFree(dataset_name);
    bdestroy(dname);
#ifdef DEBUG
  printf("HDFAccess: 8\n");
#endif
    return SQLITE_OK;

  }// endif

#ifdef DEBUG
  printf("HDFAccess: 9, dataset_id=%d\n",dataset_id);
#endif

  
  // dataset type must be "internal-file"
  if(GS_DatasetGetType(dataset_id) != GS_OBJECT_TYPE_IFILE){
    rc =-1;
    *pResOut = (rc==0);
    GFree(dataset_name);
    bdestroy(dname);
#ifdef DEBUG
  printf("HDFAccess: 10\n");
#endif
    return SQLITE_OK;

  }// endif
  GFree(dataset_name);
  bdestroy(dname);

#ifdef DEBUG
  printf("HDFAccess: 11\n");
#endif

  
  // at this point, the IFile exists
  if(flags==SQLITE_ACCESS_EXISTS){
    rc =0;  
    *pResOut = (rc==0);
#ifdef DEBUG
  printf("HDFAccess: 12\n");
#endif
    return SQLITE_OK;
      
  }// endif  

#ifdef DEBUG
  printf("HDFAccess: 13\n");
#endif
     

  int writeable;
  writeable = IFileGetWriteDataset(dataset_id);
  GS_DatasetClose(dataset_id);

#ifdef DEBUG
  printf("HDFAccess: 9, writeable=%d\n",writeable);
#endif


  if( flags==SQLITE_ACCESS_READ ) {
    rc =0;
    *pResOut = (rc==0);
#ifdef DEBUG
  printf("HDFAccess: 14\n");
#endif
    return SQLITE_OK;

  }// endif
  if( flags==SQLITE_ACCESS_READWRITE ) {
    if(writeable==TRUE){
      rc =0;
    *pResOut = (rc==0);
#ifdef DEBUG
  printf("HDFAccess: 15\n");
#endif
    return SQLITE_OK;

    } else {
      rc =-1;
    *pResOut = (rc==0);
#ifdef DEBUG
  printf("HDFAccess: 16\n");
#endif
    return SQLITE_OK;

    }
  }// endif

#ifdef DEBUG
  printf("HDFAccess: 17, rc=%d\n",rc);
#endif


  *pResOut = (rc==0);
  return SQLITE_OK;

}

/*
** Argument zPath points to a nul-terminated string containing a file path.
** If zPath is an absolute path, then it is copied as is into the output 
** buffer. Otherwise, if it is a relative path, then the equivalent full
** path is written to the output buffer.
**
** This function assumes that paths are UNIX style. Specifically, that:
**
**   1. Path components are separated by a '/'. and 
**   2. Full paths begin with a '/' character.
**
**
**  FOr my HDF5 implementation, all filenames must be file_id:full-hdf5-path
**  so, this func just copies input to output
*/
static int HDFFullPathname(
  sqlite3_vfs *pVfs,              /* VFS */
  const char *zPath,              /* Input path (possibly a relative path) */
  int nPathOut,                   /* Size of output buffer in bytes */
  char *zPathOut                  /* Pointer to output buffer */
){

  strncpy(zPathOut, zPath, nPathOut);

  return SQLITE_OK;
}

/*
** The following four VFS methods:
**
**   xDlOpen
**   xDlError
**   xDlSym
**   xDlClose
**
** are supposed to implement the functionality needed by SQLite to load
** extensions compiled as shared objects. This simple VFS does not support
** this functionality, so the following functions are no-ops.
*/
static void *HDFDlOpen(sqlite3_vfs *pVfs, const char *zPath){
  return 0;
}
static void HDFDlError(sqlite3_vfs *pVfs, int nByte, char *zErrMsg){
  sqlite3_snprintf(nByte, zErrMsg, "Loadable extensions are not supported");
  zErrMsg[nByte-1] = '\0';
}
static void (*HDFDlSym(sqlite3_vfs *pVfs, void *pH, const char *z))(void){
  return 0;
}
static void HDFDlClose(sqlite3_vfs *pVfs, void *pHandle){
  return;
}

/*
** Parameter zByte points to a buffer nByte bytes in size. Populate this
** buffer with pseudo-random data.
*/
static int HDFRandomness(sqlite3_vfs *pVfs, int nByte, char *zByte){
  return SQLITE_OK;
}

/*
** Sleep for at least nMicro microseconds. Return the (approximate) number 
** of microseconds slept for.
*/
static int HDFSleep(sqlite3_vfs *pVfs, int nMicro){
  sleep(nMicro / 1000000);
  usleep(nMicro % 1000000);
  return nMicro;
}

/*
** Set *pTime to the current UTC time expressed as a Julian day. Return
** SQLITE_OK if successful, or an error code otherwise.
**
**   http://en.wikipedia.org/wiki/Julian_day
**
** This implementation is not very good. The current time is rounded to
** an integer number of seconds. Also, assuming time_t is a signed 32-bit 
** value, it will stop working some time in the year 2038 AD (the so-called
** "year 2038" problem that afflicts systems that store time this way). 
*/
static int HDFCurrentTime(sqlite3_vfs *pVfs, double *pTime){
  time_t t = time(0);
  *pTime = t/86400.0 + 2440587.5; 
  return SQLITE_OK;
}

/*
** This function returns a pointer to the VFS implemented in this file.
** To make the VFS available to SQLite:
**
**   sqlite3_vfs_register(sqlite3_HDFvfs(), 1);
*/
sqlite3_vfs *sqlite3_HDFvfs(void){
  static sqlite3_vfs HDFvfs = {
    1,                            /* iVersion */
    sizeof(HDFFile),              /* szOsFile */
    MAXPATHNAME,                  /* mxPathname */
    0,                            /* pNext */
    "hdf",                        /* zName */
    0,                            /* pAppData */
    HDFOpen,                     /* xOpen */
    HDFDelete,                   /* xDelete */
    HDFAccess,                   /* xAccess */
    HDFFullPathname,             /* xFullPathname */
    HDFDlOpen,                   /* xDlOpen */
    HDFDlError,                  /* xDlError */
    HDFDlSym,                    /* xDlSym */
    HDFDlClose,                  /* xDlClose */
    HDFRandomness,               /* xRandomness */
    HDFSleep,                    /* xSleep */
    HDFCurrentTime,              /* xCurrentTime */
  };
  return &HDFvfs;
}
