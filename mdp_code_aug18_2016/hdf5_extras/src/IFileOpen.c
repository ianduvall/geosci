/* IFileOpen.c

   by Leland Pierce, Jan 20, 2014.
  
*/

#include "ifile.h"



IFILE *ifileopen_setup_read(hid_t ifile_id,const char *ifilename, int access);
IFILE *ifileopen_setup_write(hid_t ifile_id,const char *ifilename, int access);
IFILE *ifileopen_setup_append(hid_t ifile_id,const char *ifilename, int access);

IFILE *IFileCreate(hid_t ifile_id, const char *ifilename);



/** \brief IFileOpen is used to open an internal file in a GeoSci File

\param[in] file_id
           The handle of the open GeoSci file.
\param[in] ifilename
                   The name of the internal file in an existing GeoSci 
                   file to be opened.
                   Use Unix filenaming conventions, giving the full 
                   pathname, starting with "/".
                   All but the last component of this name must already
                   exist.
\param[in]  access 
                Access mode for the IFile, similar to that used for files:
                <br>
                \parblock
                - "r"       read: 
                                   Open internal file for input operations.
                                   Must already exist.
                - "w"       write: 
                                   Create an empty file for output 
                                    operations. If an internal file with 
                                    the same name already exists, its contents
                                    are discarded and the file is treated as 
                                    a new empty internal file.
                - "a"       append: 
                                     Open internal file for output at the end
                                     of a the file. Output operations always 
                                     write data at the end of the file, 
                                     expanding it. Repositioning operations 
                                     (IFileSeek, IFileSetpos, IFileRewind) 
                                     are silently ignored. The internal file 
                                     is created if it does not exist.
                - "r+"      read/update: 
                                     Open an internal file for update 
                                     (both for input and output). The file 
                                     must already exist.
                - "w+"      write/update: 
                                     Create an empty internal file and 
                                     open it for update (both for input and 
                                     output). If an internal file with the same 
                                     name already exists its contents are 
                                     discarded and the internal file is treated 
                                     as a new empty file.
                - "a+"      append/update: 
                                     Open an internal file for update 
                                     (both for input and output) with all 
                                     output operations writing data at the end 
                                     of the file. Repositioning operations 
                                     (IFileSeek, IFileSetpos, IFileRewind) 
                                     affect the next input operations, but 
                                     output operations move the position back 
                                     to the end of internal file. The internal 
                                     file is created if it does not exist.
               
               The letter "x" can be appended to any "w" specifier (to form 
               "wx" or "w+x"). This subspecifier forces the function to fail 
               if the internal file exists, instead of overwriting it.
               <br>
               Sometimes a "b" is appended to indicate binary mode for
               reading and writing, but that is not necessary, so it is
               ignored.
              \endparblock

\returns
If the internal file is successfully opened, the function returns a 
pointer to a structure, otherwise a NULL pointer is returned.

\par Example 1
Let's assume that one already has an GeoSci file,
and one wants to add an internal file, named "somename", off of the root:
\code
       hid_t file_id;
       IFILE *ifilep;

       ifilep = IFileOpen(file_id,"/somename","w");
       if(!ifilep) {
          printf("IFileOpen failed on /somename\n");
       }

\endcode

\par Example 2
Let's assume that one already has an GeoSci file, and one wants
to add an internal file inside another group:
\code
       hid_t file_id, group_id;
       IFILE *ifilep;
       bstring name=bfromcstr("/group1");
       group_id = GS_GroupCreate(file_id,name);
       bdestroy(name);
       if(group_id < 0) {
          printf("GroupCreate failed on /group1\n");
          exit(-1);
       }
       ifilep = IFileOpen(file_id,"/group1/somename","w");
       if(!ifilep) {
          printf("IFileOpen failed on /group1/somename\n");
          exit(-1);
       }

\endcode

\par Details
\parblock
       This function is meant to emulate as close as possible the standard
       C fopen() function.

       For internal files open for update (those which include a "+" sign), 
       on which both input and output operations are allowed, the file
       should be flushed (IFileFlush) or repositioned (IFileSeek, IFileSetpos, 
       IFileRewind) between either a writing operation followed by a reading 
       operation or a reading operation which did not reach the end-of-file 
       followed by a writing operation.

       When appropriate, this routine creates a single-dimensional 
       unsigned-8-bit dataset with the given name. 
       The components of the pathname above the filename must already exist, 
       and be HDF5 groups in order for this to work.
       
       Note that even if you intend to only read from an IFile, you need to
       open the GeoSci file for reading AND writing ("r+").
       This is because the state of the internal file is written to metadata
       in the GeoSci file, even if one is only reading from the IFile.
\endparblock

\par Implementation
\parblock
       For developers, the details of the implementation are presented here.

       The complete list of all 34 IFile-related functions is given below:

       - GS_ObjectIsIFile (also contains ObjectIsIFileByID)
       - GS_ObjectIsMetadataIFile
       - DatasetGetType
       - DatasetSetType
       - IFileAllocate
       - IFileClearError
       - IFileClose
       - IFileEOF
       - IFileError
       - IFileFlush
       - IFileGetc
       - IFileGets
       - IFileGetWrite
       - IFileOpen
       - IFilePerror
       - IFilePrintf
       - IFilePutc
       - IFilePuts
       - IFileReadAccess
       - IFileRead
       - IFileReadStatus
       - IFileRewind
       - IFileScanf
       - IFileSeek
       - IFileSetEOF
       - IFileSetWriteability
       - IFileSetWrite
       - IFileSize
       - IFileTell
       - IFileTruncate
       - IFileWriteAccess
       - IFileWrite
       - IFileWriteStatus
       - IFileWriteStatusMessage

       Some of these are named based on the standard C library functions that
       they emulate. Others are implementation-specific functions.

       The overall idea of a IFile is that of a HDF5 dataset.
       This dataset is 1-dimensional, with a datatype of unsigned-8-bit.
       It is infinitely extendable.
       It has three string attributes:

       - writeable: "TRUE" or "FALSE". The user can set this so that
                      don't accidentally over-write it or delete it.
                      All IFiles are created with writeable="TRUE"

       - access: an integer betwen 0 and 6, converted to a string.
                      Corresponds to one of the following #define'd
                      parameters:
                      \li IFILE_CLOSED 0
                      \li IFILE_R   1
                      \li IFILE_W   2
                      \li IFILE_A   3
                      \li IFILE_RP  4
                      \li IFILE_WP  5
                      \li IFILE_AP  6

                      These are named based on the access string used
                      during the IFileOpen() function-call, the "P"
                      meaning "+". 
                      This keeps track of how the file was opened so
                      only appropriate operations are allowed.
                      Note that a file that is open cannot be opened by
                      another command: the file must be CLOSED in order
                      to be opened. This implements a very simple locking
                      mechanism

       - dataset_type: for an IFile this must be "10", which corresponds
                     to the C #define GS_OBJECT_TYPE_IFILE
                     This is needed because image rasters are stored
                     as datasets as well, and we need to distinguish
                     between these.

       The returned data structure is defined as:
\code
       typedef struct {
         hid_t ifile_id;
         long int file_position;
         int readPastEOF;
         int last_operation_status;
         bstring last_operation_status_message;
         int access;
         long int size;
       } IFILE;
\endcode

\par
       The parameters in this structure are defined as follows:

       - *ifile_id*:      the HDF5 object-id for this dataset.

       - *file_position*: This is the next byte to read-from or write-to,
                      where file_position=0 means to read/write the first
                      byte in the file.

       - *readPastEOF*:   "TRUE"=1 or "FALSE"=0. Whether the previous operation
                      tried to read past the last byte in the file.

       - *last_operation_status*: "SUCCESS"=1 or "FAILURE"=0
                      Each operation on the IFile sets this.

       - *last_operation_status_message*: a string written by each of the 
                      IFile functions, whether they succeed or fail.

       - *access*:        A copy of the dataset-metadata entry.

       - *size*:          The current file size in bytes.
                      This is obtained when opened using HDF5 function calls.
                      Thereafter it is updated by the the functions as
                      needed.

\par
       Functions that deal with these metadata items are given below.
       Generally the user should never call any of these, except for:
          - IFileSetWriteability() for setting the writeability of an IFile
                          when it is closed.
          - IFileError()    for determining error status
          - IFilePerror()   for printing the latest error
          - IFileEOF()      for determining the EOF status of last read
          - IFileSeek()     for moving the file position
          - IFileTell()     for determining the file position
          - IFileRewind()   for setting the file position to 0
       
       - GS_ObjectIsIFile -- must be a dataset with dataset_type="1"
       - GS_DatasetGetType-- returns the dataset_type
       - GS_DatasetSetType-- sets the dataset_type

       - IFileClearError  -- sets last_operation_status="SUCCESS" and
                           sets last_operation_status_message
       - IFileError       -- returns TRUE if last_operation_status="FAILURE"
       - IFilePerror      -- prints last_operation_status_message
       - IFileReadStatus  -- reads last_operation_status, and 
                           last_operation_status_message
       - IFileWriteStatus -- writes last_operation_status, and 
                           last_operation_status_message
       - IFileWriteStatusMessage -- writes last_operation_status_message

       - IFileEOF         -- returns TRUE if readPastEOF="TRUE"
       - IFileSetEOF      -- writes to readPastEOF

       - IFileGetWrite    -- returns TRUE If writeable="TRUE"
       - IFileSetWrite    -- sets writeability (developer function)
       - IFileSetWriteability  -- sets writeability (user function)

       - IFileReadAccess  -- reads access
       - IFileWriteAccess -- writes access

       - IFileSeek        -- sets file_position
       - IFileTell        -- reads file_position
       - IFileRewind      -- sets file_position to "0"


\par
       Functions to deal with the file size directly:

       - IFileTruncate    -- sets size of file (can shrink or grow)
       - IFileAllocate    -- expands file size if needed
       - IFileSize        -- gets the current file size from the struct
       - IFileSizeHDF     -- gets the current file size from HDF
  

\par
       Other functions that the user can use include:
       
       - IFileOpen        -- open an IFile
       - IFileClose       -- close an IFile
       - IFileFlush       -- flush any changes to the IFile to disk

       - IFileGetc        -- read a single character
       - IFileGets        -- read a newline-terminated string
       - IFileRead        -- read a set number of bytes
       - IFileScanf       -- formatted-read from next "line"

       - IFilePutc        -- write a single character
       - IFilePuts        -- write a newline-terminated string
       - IFileWrite       -- write a set number of bytes
       - IFilePrintf      -- write a formatted string 
 

\par
       Note that there is only ONE COPY of the file state, so there can 
       only be one process that is using it at one time.
       This is on purpose so that there are no parallel-read-write issues.
\endparblock
*/

//#define DEBUG

/*******************************************************************************/
IFILE *IFileOpen( hid_t file_id, const char *ifilename, const char *access )
{
  hid_t ifile_id;
  char *dir;
  char *b;
  int i;
  int has_r, has_w, has_a, has_x, has_plus;
  int access_code;
  hid_t dataset_id;
  IFILE *ifilep;
  bstring ctmp;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");
#ifdef DEBUG
  printf("IFileOpen: 1\n");
#endif

/* -------------------------------------------------------------------- */
/* Check valid file_id                                                  */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsFile(file_id)){
    bassignformat(error_string,"IFileOpen: invalid file id.");
    return NULL;
  }// endif
#ifdef DEBUG
  printf("IFileOpen: 2\n");
#endif

/* -------------------------------------------------------------------- */
/* Is it writeable?                                                     */
/* -------------------------------------------------------------------- */
  ifile_id = file_id;
  if(!GS_FileIsWriteable(ifile_id)){
    ctmp = bstrcpy(GS_GetFilename(file_id));
    bassignformat(error_string,"IFileOpen: File cannot be written to: %s\n",ctmp);
    bdestroy(ctmp);
    return NULL;
  }// endif
#ifdef DEBUG
  printf("IFileOpen: 3\n");
#endif

/* -------------------------------------------------------------------- */
/* Ifilename must start with a "/"                                      */
/* -------------------------------------------------------------------- */
  if(ifilename[0] != '/'){
    bassignformat(error_string,"IFileOpen: invalid ifilename: %s: must start with '/'.",
            ifilename);
    return NULL;
  }// endif
#ifdef DEBUG
  printf("IFileOpen: 4\n");
#endif

/* -------------------------------------------------------------------- */
/* Ifilename must not end with a "/"                                    */
/* -------------------------------------------------------------------- */
  if(ifilename[strlen(ifilename)-1] == '/'){
    bassignformat(error_string,"IFileOpen: invalid ifilename: %s: must not end with '/'.",
            ifilename);
    return NULL;
  }// endif

#ifdef DEBUG
  printf("IFileOpen: 5\n");
#endif

/* -------------------------------------------------------------------- */
/* Does path to this object already exist?                              */
/* -------------------------------------------------------------------- */
  dir = GS_Dirname(ifilename);
  if(!dir){
    bassignformat(error_string,"IFileOpen: invalid groupname: %s.",dir);
    return NULL;
  }// endif
  if(strlen(dir)==1 && dir[0]=='/') {
    // dir exists....
  } else {
    if(GS_CheckUniqueName(ifile_id,dir)){
      bassignformat(error_string,"IFileOpen: path: %s, does not exist.",dir);
      GFree(dir);
      return NULL;
    }// endif
  }// endif
 GFree(dir);

#ifdef DEBUG
  printf("IFileOpen: 6\n");
#endif


/* -------------------------------------------------------------------- */
/* Check validity of access string                                      */
/* -------------------------------------------------------------------- */
  // initialize which characters it has to none:
  has_r=FALSE;
  has_w=FALSE;
  has_a=FALSE;
  has_x=FALSE;
  has_plus=FALSE;
  for(i=0;i<strlen(access);i++){
    switch(access[i]){
    case 'r': has_r=TRUE; break;
    case 'w': has_w=TRUE; break;
    case 'a': has_a=TRUE; break;
    case 'x': has_x=TRUE; break;
    case '+': has_plus=TRUE; break;
    case 'b': break;
    case ' ': break;
    default:
      bassignformat(error_string,"IFileOpen: invalid character in access: %c.",access[i]);
      return NULL;
    }// end-switch
  }// endfor

#ifdef DEBUG
  printf("IFileOpen: 7\n");
#endif


/* -------------------------------------------------------------------- */
/* Determine which valid combination is requested                       */
/* -------------------------------------------------------------------- */
  access_code = 0;
  if(has_r & !has_w & !has_a & !has_x &!has_plus){
    access_code = IFILE_R;
  } else if (has_r & !has_w & !has_a & !has_x & has_plus){
    access_code = IFILE_RP;
  } else if (!has_r & has_w & !has_a & !has_x &!has_plus){
    access_code = IFILE_W;
  } else if (!has_r & has_w & !has_a & !has_x & has_plus){
    access_code = IFILE_WP;
  } else if (!has_r & !has_w & has_a & !has_x &!has_plus){
    access_code = IFILE_A;
  } else if (!has_r & !has_w & has_a & !has_x & has_plus){
    access_code = IFILE_AP;
  } else if (!has_r & has_w & !has_a & has_x &!has_plus){
    access_code = IFILE_WX;
  } else if (!has_r & has_w & !has_a & has_x & has_plus){
    access_code = IFILE_WXP;
  } else {
      bassignformat(error_string,"IFileOpen: invalid access code: %s.",access);
      return NULL;
  }// endif

#ifdef DEBUG
  printf("IFileOpen: 8\n");
#endif


/* -------------------------------------------------------------------- */
/* Take appropriate actions                                             */
/* -------------------------------------------------------------------- */
  switch(access_code){
  case IFILE_R:
  case IFILE_RP:
    ifilep = ifileopen_setup_read(ifile_id,ifilename,access_code);
    if( !ifilep ){
      return NULL;
    }// endif
    break;
  case IFILE_W:
  case IFILE_WP:
  case IFILE_WX:
  case IFILE_WXP:
    ifilep = ifileopen_setup_write(ifile_id,ifilename,access_code);
    if( !ifilep ){
      return NULL;
    }// endif
    break;
  case IFILE_A:
  case IFILE_AP:
    ifilep = ifileopen_setup_append(ifile_id,ifilename,access_code);
    if( !ifilep ){
      return NULL;
    }// endif
    break;
  default:
    bassignformat(error_string,"IFileOpen: invalid access code: %s.",access);
    return NULL;
  }// endswitch

#ifdef DEBUG
  printf("IFileOpen: 9\n");
#endif


/* -------------------------------------------------------------------- */
/* Set status metadata                                                  */
/* -------------------------------------------------------------------- */
  ifilep->last_operation_status = IFILE_SUCCESS;
  ifilep->last_operation_status_message = bfromcstr("");
  bassigncstr(ifilep->last_operation_status_message,
         "Successfully opened internal file");


#ifdef DEBUG
  printf("IFileOpen: 10 (success)\n");
#endif

/* -------------------------------------------------------------------- */
/* Return ifilep                                                        */
/* -------------------------------------------------------------------- */
  return ifilep;

}



/****************************************************************/
IFILE *ifileopen_setup_read(hid_t ifile_id,const char *ifilename, int access)
{
  hid_t dataset_id;
  int internal_access_code;
  IFILE *ifilep;
  bstring bfilename = bfromcstr("");

  /* -------------------------------------------------------------- */
  /* For read, ifilename must exist                                 */
  /* -------------------------------------------------------------- */
  if(GS_CheckUniqueName(ifile_id,ifilename)){
    bassignformat(error_string,"IFileOpen: ifilename: %s, does not exist.",ifilename);
    bdestroy(bfilename);
    return NULL;
  }// endif

  bfilename = bfromcstr(ifilename);
  dataset_id = GS_DatasetOpen(ifile_id,bfilename);
  bdestroy(bfilename);

  /* -------------------------------------------------------------- */
  /* Make sure dataset is the right type                            */
  /* -------------------------------------------------------------- */
  if(!GS_ObjectIsIFileByID(dataset_id)){
    GS_DatasetClose(dataset_id);
    bassignformat(error_string,"IFileOpen: dataset is not an internal file: %s.",ifilename);
    return NULL;
  }// endif

  /* -------------------------------------------------------------- */
  /* Read internal file access code                                 */
  /* -------------------------------------------------------------- */
  internal_access_code=IFileReadAccessHDF(dataset_id);
  if(internal_access_code<0){
    GS_DatasetClose(dataset_id);
    bassignformat(error_string,"IFileOpen: Can't read the access metadata for: %s.",
            ifilename);
    return NULL;
  }// endif

  // internal access code must be 0 for file to be lock-free:
  if(internal_access_code != 0){
    GS_DatasetClose(dataset_id);
    bassignformat(error_string,"IFileOpen: Another process has the lock for the internal file: %s.",
            ifilename);
    return NULL;
  }// endif


  /* -------------------------------------------------------------- */
  /* Setup structure                                                */
  /* -------------------------------------------------------------- */
  ifilep = (IFILE *)GMalloc(sizeof(IFILE));
  if(!ifilep){
    bassignformat(error_string,"IFileOpen: ifileopen_setup_read() can't create the IFILE structure");
    return NULL;
  }//endif
  
  ifilep->ifile_id = dataset_id;
  ifilep->file_position = 0L;
  ifilep->readPastEOF   = FALSE;
  ifilep->last_operation_status = IFILE_SUCCESS;
  bassigncstr(ifilep->last_operation_status_message, "Opened");
  ifilep->access = access;
  ifilep->size = IFileSizeHDF(ifilep);

  /* -------------------------------------------------------------- */
  /* Write new internal file access code                            */
  /* -------------------------------------------------------------- */
  if(!IFileWriteAccess(ifilep,access)){
    GS_DatasetClose(dataset_id);
    GFree(ifilep);
    bassignformat(error_string,"IFileOpen: Can't write the access metadata for: %s.",
            ifilename);
    return NULL;
  }// endif

  return ifilep;
  
}// endfunction: ifileopen_setup_read

/****************************************************************/
IFILE *ifileopen_setup_write(hid_t ifile_id,const char *ifilename, int access)
{
  hid_t dataset_id;
  hid_t datatype, dataspace;
  hsize_t dims[1], maxdims[1];
  hid_t plist;
  int writeable;
  IFILE *ifilep;
  bstring bfilename = bfromcstr("");

#ifdef DEBUG
  printf("IFileOpen: setup_write: 1\n");
#endif

  bfilename = bfromcstr(ifilename);
  /* -------------------------------------------------------------- */
  /* For write, if file exists, delete or error                     */
  /* -------------------------------------------------------------- */
  if(!GS_CheckUniqueName(ifile_id,ifilename)){
    
    /* -------------------------------------------------- */
    /* Is it an internal file?                            */
    /* -------------------------------------------------- */
    dataset_id = GS_DatasetOpen(ifile_id,bfilename);
    if(!GS_ObjectIsIFileByID(dataset_id)){
      bassignformat(error_string,"IFileOpen: attempt to over-write object that is not an internal file.");
      bdestroy(bfilename);
      return NULL;
    }// endif
    // get writeability:
    writeable = IFileGetWriteDataset(dataset_id);
    GS_DatasetClose(dataset_id);


    // delete for w, w+, fail for wx, w+x:
    switch(access){
    case IFILE_WX:
    case IFILE_WXP:
      /* -------------------------------------------------- */
      /* Don't delete: return error                         */
      /* -------------------------------------------------- */
        bassignformat(error_string,"IFileOpen: Won't delete pre-existing internal file: %s.",
                ifilename);
        bdestroy(bfilename);
        return NULL;

    case IFILE_W:
    case IFILE_WP:
      if(writeable==TRUE) {
        /* -------------------------------------------------- */
        /* Delete it                                          */
        /* -------------------------------------------------- */
        if(!GS_DatasetDelete(ifile_id,bfilename)){
          bassignformat(error_string,"IFileOpen: Can't delete pre-existing internal file: %s.",
                  ifilename);
          bdestroy(bfilename);
          return NULL;
        }// endif
      } else if (writeable==FALSE) {
        bassignformat(error_string,"IFileOpen: Trying to delete read-only pre-existing internal file: %s.",
                ifilename);
        bdestroy(bfilename);
        return NULL;
      } else {
        bassignformat(error_string,"IFileOpen: Cannot determine writeability of pre-existing internal file: %s.",
                ifilename);
        bdestroy(bfilename);
        return NULL;
      }// endif

    default:
      bassignformat(error_string,"IFileOpen: invalid (write) access code: %d.",access);
      bdestroy(bfilename);
      return NULL;
    } //end switch

  }// endif
  bdestroy(bfilename);

#ifdef DEBUG
  printf("IFileOpen: setup_write: 2\n");
#endif


  /* -------------------------------------------------------------- */
  /* Simplify access code                                           */
  /* -------------------------------------------------------------- */
  switch(access){
    case IFILE_WX:
      access = IFILE_W; break;
    case IFILE_WXP:
      access = IFILE_WP; break;
  }// end switch


#ifdef DEBUG
  printf("IFileOpen: setup_write: 2.5\n");
#endif

  /* -------------------------------------------------------------- */
  /* Create new internal file                                       */
  /* -------------------------------------------------------------- */
  ifilep = IFileCreate(ifile_id, ifilename);
  if(!ifilep ){
    bassignformat(error_string,"IFileOpen: internal file-creation failed: %s.",
            ifilename);
    return NULL;
  }//endif

#ifdef DEBUG
  printf("IFileOpen: setup_write: 3\n");
#endif


  /* -------------------------------------------------------------- */
  /* Write internal file access code                                */
  /* -------------------------------------------------------------- */
  ifilep->access = access;

#ifdef DEBUG
  printf("IFileOpen: setup_write: 4 (success)\n");
#endif



  return ifilep;
  
}// endfunction: ifileopen_setup_write

/****************************************************************/
IFILE *ifileopen_setup_append(hid_t ifile_id,const char *ifilename, int access)
{
  hid_t dataset_id;
  hid_t datatype, dataspace;
  hsize_t dims[1], maxdims[1];
  hid_t plist;

  long int size;
  int writeable;
  IFILE *ifilep;
  bstring bfilename = bfromcstr("");


  bfilename = bfromcstr(ifilename);
  /* -------------------------------------------------------------- */
  /* For append, if file exists, use it                             */
  /* if doesn't exist create it                                     */
  /* -------------------------------------------------------------- */
  dataset_id = 0;
  if(!GS_CheckUniqueName(ifile_id,ifilename)){
    // open existing
    dataset_id = GS_DatasetOpen(ifile_id,bfilename);
    if(dataset_id < 0) {
        bassignformat(error_string,"IFileOpen: Can't open existing internal file: %s.",
                ifilename);
        bdestroy(bfilename);
        return NULL;
    }// endif

    /* ------------------------------------------------------------ */
    /* Is it an internal file?                                      */
    /* ------------------------------------------------------------ */
    if(!GS_ObjectIsIFileByID(dataset_id)){
      GS_DatasetClose(dataset_id);
      bassignformat(error_string,"IFileOpen: attempt to append to non-internal-file object.");
      bdestroy(bfilename);
      return NULL;
    }// endif

    // get writeability:
    writeable = IFileGetWriteDataset(dataset_id);

    // cannot append if not writeable:
    if(writeable != TRUE){
      GS_DatasetClose(dataset_id);
      bassignformat(error_string,"IFileOpen: attempt to append to read-only internal-file object.");
      bdestroy(bfilename);
      return NULL;
    }// endif

  }// endif
  bdestroy(bfilename);




  /* -------------------------------------------------------------- */
  /* Create new internal file                                       */
  /* -------------------------------------------------------------- */
  if(dataset_id == 0) {
    // create new one:
    ifilep = IFileCreate(ifile_id, ifilename);
    if(!ifilep){
      bassignformat(error_string,"IFileOpen: internal file-creation failed: %s.",
              ifilename);
      return NULL;
    }//endif
    dataset_id = ifilep->ifile_id;


  } else {
    // create the necessary IFILE data structure for the existing file:
    ifilep = (IFILE *)GMalloc(sizeof(IFILE));
    if(!ifilep){
      bassignformat(error_string,"IFileOpen: ifileopen_setup_append() can't create the IFILE structure");
      return NULL;
    }//endif
  
    ifilep->ifile_id = dataset_id;
    ifilep->readPastEOF   = FALSE;
    ifilep->last_operation_status = IFILE_SUCCESS;
    bassigncstr(ifilep->last_operation_status_message, "Opened");
    ifilep->access = access;
    ifilep->size = IFileSizeHDF(ifilep);
    ifilep->file_position = ifilep->size;
    
  }// end: create-new file.


  /* -------------------------------------------------------------- */
  /* Write internal file access code                                */
  /* -------------------------------------------------------------- */
  if(!IFileWriteAccess(ifilep, access)){
    GS_DatasetClose(dataset_id);
    GFree(ifilep);
    bassignformat(error_string,"IFileOpen: Can't write the access metadata for: %s.",
            ifilename);
    return NULL;
  }// endif


  return ifilep;
  
}// endfunction: ifileopen_setup_append

/**********************************************************************/
/* Encapsulates code used to create a new internal file.              */
/* No checking because the code above that calls this does            */
/*    all that already.                                               */
/**********************************************************************/
IFILE *IFileCreate(hid_t ifile_id, const char *ifilename)
{
  hid_t dataset_id;
  hid_t datatype, dataspace;
  hsize_t dims[1], maxdims[1];
  hid_t plist;
  IFILE *ifile_data;


#ifdef DEBUG
 printf("IFileCreate: 1\n");
#endif

  /* ------------------------------- */
  /* 1. dataspace                    */
  /* ------------------------------- */
  dims[0] = 0;
  maxdims[0]= H5S_UNLIMITED;
  dataspace = H5Screate_simple(1, dims, maxdims);
  if(dataspace < 0 ){
    bassignformat(error_string,"IFileOpen: internal file-creation failed (dataspace): %s.",
            ifilename);
    return NULL;
  }//endif

#ifdef DEBUG
 printf("IFileCreate: 2\n");
#endif

  /* ------------------------------- */
  /* 2. chunking-size                */
  /* ------------------------------- */
  plist = H5Pcreate( H5P_DATASET_CREATE );
  dims[0] = 1024;
  if(H5Pset_chunk(plist, 1, dims) < 0){
    H5Sclose(dataspace);
    bassignformat(error_string,"IFileOpen: internal file-creation failed (chunking): %s.",
            ifilename);
    return NULL;
  }// endif

#ifdef DEBUG
 printf("IFileCreate: 3\n");
#endif

  /* ------------------------------- */
  /* 3. datatype                     */
  /* ------------------------------- */
  datatype = GS_GetHDF5Type(GS_DATATYPE_UI8);

#ifdef DEBUG
 printf("IFileCreate: 4\n");
#endif

  /* ------------------------------- */
  /* 4. creation                     */
  /* ------------------------------- */
  dataset_id = H5Dcreate(ifile_id, ifilename, datatype, dataspace, H5P_DEFAULT,
                      plist, H5P_DEFAULT);
  if(dataset_id < 0) {
    H5Sclose(dataspace);
    H5Pclose(plist);
    bassignformat(error_string,"IFileOpen: internal file-creation failed: %s.",
            ifilename);
    return NULL;
  }// endif

#ifdef DEBUG
 printf("IFileCreate: 5\n");
#endif

  /* ------------------------------- */
  /* 5. clean up                     */
  /* ------------------------------- */
  H5Sclose(dataspace);
  H5Pclose(plist);

#ifdef DEBUG
 printf("IFileCreate: 6\n");
#endif

  /* ------------------------------- */
  /* 6. init the metadata            */
  /* ------------------------------- */

  if(!GS_DatasetSetType(dataset_id,GS_OBJECT_TYPE_IFILE)){
    GFree(ifile_data);
    bassignformat(error_string,"IFileOpen: Can't write the dataset_type metadata");
    return NULL;
  }//endif

#ifdef DEBUG
 printf("IFileCreate: 7\n");
#endif

 bstring writeable = bfromcstr("writeable");
 bstring btrue = bfromcstr("TRUE");
 if(!GS_UpdateMetadata(dataset_id, writeable, btrue )){
    bassignformat(error_string,"IFileOpen: Can't write the writeability metadata");
    bdestroy(writeable);
    bdestroy(btrue);
    return NULL;
  }//endif
 bdestroy(writeable);
 bdestroy(btrue);

#ifdef DEBUG
 printf("IFileCreate: 8\n");
#endif


  ifile_data = (IFILE *)GMalloc(sizeof(IFILE));
  if(!ifile_data){
    bassignformat(error_string,"IFileOpen: IFileCreate can't create the IFILE structure");
    return NULL;
  }//endif
  
  ifile_data->ifile_id = dataset_id;
  ifile_data->file_position = 0L;
  ifile_data->readPastEOF   = FALSE;
  ifile_data->last_operation_status = IFILE_SUCCESS;
  ifile_data->last_operation_status_message = bfromcstr("");
  bassigncstr(ifile_data->last_operation_status_message, "Creation");
  ifile_data->access = IFILE_R;
  ifile_data->size = 0L;

#ifdef DEBUG
 printf("IFileCreate: 9\n");
#endif

 bstring access = bfromcstr("access");
 bstring one = bfromcstr("1");
 if(!GS_UpdateMetadata(dataset_id, access, one )){
    GFree(ifile_data);
    bassignformat(error_string,"IFileOpen: Can't write the last_operation_status_message metadata");
    bdestroy(access);
    bdestroy(one);
    return NULL;
  }//endif
 bdestroy(access);
 bdestroy(one);


#ifdef DEBUG
 printf("IFileCreate: 10\n");
#endif

  /* -------------------------------- */
  /* 7. return the new dataset struct */
  /* -------------------------------- */

  return ifile_data;

}// endfunction: IFileCreate()
