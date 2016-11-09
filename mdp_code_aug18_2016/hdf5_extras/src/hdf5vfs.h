#ifndef HDF5VFS_H_
#define HDF5VFS_H_

#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <complex.h>

#include <hdf5.h>

#include "sqlite3.h"
#include <dlfcn.h>
#include <spatialite.h>

#include "ifile.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file */

#define TRUE 1
#define FALSE 0

/*
** Size of the write buffer used by journal files in bytes.
*/
#ifndef SQLITE_DEMOVFS_BUFFERSZ
# define SQLITE_DEMOVFS_BUFFERSZ 8192
#endif

/*
** The maximum pathname length supported by this VFS.
*/
#define MAXPATHNAME 512

/*
** When using this VFS, the sqlite3_file* handles that SQLite uses are
** actually pointers to instances of type HDFFile.
*/
typedef struct HDFFile HDFFile;
struct HDFFile {
  sqlite3_file base;              /* Base class. Must be first. */
  IFILE *ifilep;                  /* the IFILE struct */

  char *aBuffer;                  /* Pointer to malloc'd buffer */
  int nBuffer;                    /* Valid bytes of data in zBuffer */
  sqlite3_int64 iBufferOfst;      /* Offset in file of zBuffer[0] */
};


static int HDFDirectWrite(
  HDFFile *p,                    /* File handle */
  const void *zBuf,               /* Buffer containing data to write */
  int iAmt,                       /* Size of data to write in bytes */
  sqlite_int64 iOfst              /* File offset to write to */
);

static int HDFFlushBuffer(HDFFile *p);

static int HDFClose(sqlite3_file *pFile);

static int HDFRead(
  sqlite3_file *pFile, 
  void *zBuf, 
  int iAmt, 
  sqlite_int64 iOfst
);

static int HDFWrite(
  sqlite3_file *pFile, 
  const void *zBuf, 
  int iAmt, 
  sqlite_int64 iOfst
);

static int HDFTruncate(sqlite3_file *pFile, sqlite_int64 size);

static int HDFSync(sqlite3_file *pFile, int flags);

static int HDFFileSize(sqlite3_file *pFile, sqlite_int64 *pSize);

static int HDFLock(sqlite3_file *pFile, int eLock);
static int HDFUnlock(sqlite3_file *pFile, int eLock);
static int HDFCheckReservedLock(sqlite3_file *pFile, int *pResOut);
static int HDFFileControl(sqlite3_file *pFile, int op, void *pArg);


static int HDFSectorSize(sqlite3_file *pFile);
static int HDFDeviceCharacteristics(sqlite3_file *pFile);

static int HDFOpen(
  sqlite3_vfs *pVfs,              /* VFS */
  const char *zName,              /* File to open, or 0 for a temp file */
  sqlite3_file *pFile,            /* Pointer to HDFFile struct to populate */
  int flags,                      /* Input SQLITE_OPEN_XXX flags */
  int *pOutFlags                  /* Output SQLITE_OPEN_XXX flags (or NULL) */
);


static int HDFDelete(sqlite3_vfs *pVfs, const char *zPath, int dirSync);

static int HDFAccess(
  sqlite3_vfs *pVfs, 
  const char *zPath, 
  int flags, 
  int *pResOut
);

static int HDFFullPathname(
  sqlite3_vfs *pVfs,              /* VFS */
  const char *zPath,              /* Input path (possibly a relative path) */
  int nPathOut,                   /* Size of output buffer in bytes */
  char *zPathOut                  /* Pointer to output buffer */
);

static void *HDFDlOpen(sqlite3_vfs *pVfs, const char *zPath);
static void HDFDlError(sqlite3_vfs *pVfs, int nByte, char *zErrMsg);
static void (*HDFDlSym(sqlite3_vfs *pVfs, void *pH, const char *z))(void);
static void HDFDlClose(sqlite3_vfs *pVfs, void *pHandle);
static int HDFRandomness(sqlite3_vfs *pVfs, int nByte, char *zByte);


static int HDFSleep(sqlite3_vfs *pVfs, int nMicro);
static int HDFCurrentTime(sqlite3_vfs *pVfs, double *pTime);

sqlite3_vfs *sqlite3_HDFvfs(void);





#ifdef __cplusplus
}
#endif

#endif
