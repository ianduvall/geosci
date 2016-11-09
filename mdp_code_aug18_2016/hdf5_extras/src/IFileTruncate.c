/* IFileTruncate.c

   by Leland Pierce, Jan 22, 2014.
  
*/

/** \brief IFileTruncate changes the size of an internal file
\sa IFileOpen, IFileClose
\param[in] ifilep The struct of the open internal file.
\param[in] length The desired length, in bytes, of the file.
                   Must be >= 0.
\returns
\c TRUE is returned on success, \c FALSE otherwise.
The \c error_string variable is also set to a system-specific 
error code on failure.
\par Example
Let's assume that one already has an GeoSci file, and one wants
to make the new size be 1000 bytes for an internal file named "/somename".
\code
hid_t file_id;
IFILE *ifilep;
long int icount;

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifilep) {
   printf("IFileOpen failed on /somename\n");
}

icount = 1000L;
if(!IFileTruncate(ifilep, icount)){
   printf("truncate failed\n");
}
\endcode
\par Details
If the size of the file is less than \c length, then the file 
is increased to this size.
If the size of the file is greater than \c length, then the file
is decreased to this size.
This function is meant to emulate as close as possible the standard
C \c ftruncate() function.
*/

#include "ifile.h"


/*******************************************************************************/
int IFileTruncate( IFILE *ifilep, long int length )
{
  long int size;
  hsize_t dims[1];
  int writeable;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Error-check length                                                   */
/* -------------------------------------------------------------------- */
  if(length < 0 ){
    bassignformat(error_string,"IFileTruncate: length must be >=0, length=%ld",length);
    return FALSE;
  }//endif

/* -------------------------------------------------------------------- */
/* Nothing to do                                                        */
/* -------------------------------------------------------------------- */
  if(length == ifilep->size) {
    return TRUE;
  }// endif

/* -------------------------------------------------------------------- */
/* Need to increase or decrease file size                               */
/* -------------------------------------------------------------------- */
  dims[0] = length;
  if(H5Dset_extent(ifilep->ifile_id,dims) <0){
    bassignformat(error_string,"IFileTruncate: Error changing dataset size to %ld bytes.",length);
    return FALSE;
  }// endif
  ifilep->size = length;

/* -------------------------------------------------------------------- */
/* Clear the last operation status metadata                             */
/* -------------------------------------------------------------------- */
  ifilep->last_operation_status = IFILE_SUCCESS;
  bassigncstr(ifilep->last_operation_status_message,
         "IFileTruncate() ran successfully");

/* -------------------------------------------------------------------- */
/* Clear the readPastEOF metadata                                       */
/* -------------------------------------------------------------------- */
  ifilep->readPastEOF = FALSE;

  return TRUE;
 
}
