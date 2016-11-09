/* IFileAllocate.c

   by Leland Pierce, Jan 22, 2014.
  
*/

/** \brief IFileAllocate is used to allocate more space to an internal file.


\param[in] ifilep
                   The struct of the open internal file.
\param[in] offset
                   The offset, in bytes, from the start of the file
                   where the newly-allocates bytes must start.
                   Should be >= 0.
\param[in] length 
                   The length, in bytes, of the region of the
                   newly-allocated bytes.
                   Should be >0.

\returns
  TRUE is returned on success, FALSE otherwise.
       The error_string variable is also set to a system-specific 
       error code on failure.

\par Example
Let's assume that one already has an GeoSci file, and one wants
 to add 100 bytes to the end of an internal file named "/somename".
\code
 hid_t file_id;
 IFILE *ifilep;
 long int istart, icount;

 ifilep = IFileOpen(file_id,"/somename","w");
 if(!ifilep) {
    printf("IFileOpen failed on /somename\n");
 }

 istart=IFileSize(ifilep);

 icount = 100L;
 if(!IFileAllocate(ifilep, istart, icount)){
    printf("allocate failed\n");
 }
\endcode

\par Details
If the size of the file is less than offset+len, then the file 
is increased to this size; otherwise the file size is left unchanged. 
This function is meant to emulate as close as possible the standard
posix_fallocate() function.

*/

#include "ifile.h"

//#define DEBUG

/*******************************************************************************/
int IFileAllocate( IFILE *ifilep, long int offset, long int length )
{
  long int end_byte;
  hsize_t dims[1];

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("IFileAllocate: offset=%ld, length=%ld\n",offset, length);
#endif

/* -------------------------------------------------------------------- */
/* Error-check length                                                   */
/* -------------------------------------------------------------------- */
  if(length < 0L ){
    bassignformat(error_string,"IFileAllocate: length must be positive, length=%ld",length);
    return FALSE;
  }//endif

  // nothing to do:
  if(length == 0L) {
    return TRUE;
  }// endif

#ifdef DEBUG
  printf("IFileAllocate: at 1\n");
  printf("IFileAllocate: ifilep->access=%d\n",ifilep->access);
#endif


/* -------------------------------------------------------------------- */
/* Is the file writeable?                                               */
/* -------------------------------------------------------------------- */
  if(ifilep->access == IFILE_R){
    bassignformat(error_string,"IFileAllocate: internal file is not writeable.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("IFileAllocate: at 2\n");
#endif


/* -------------------------------------------------------------------- */
/* Determine if file size needs to be increased                         */
/* If so, increase it                                                   */
/* -------------------------------------------------------------------- */
  end_byte = offset+length;

#ifdef DEBUG
  printf("IFileAllocate: at 3\n");
#endif


  if(end_byte > ifilep->size) {
    dims[0] = end_byte;
    if(H5Dset_extent(ifilep->ifile_id, dims) <0){
      bassignformat(error_string,"IFileAllocate: Error extending dataset to %ld bytes.",end_byte);
      return FALSE;
    }// endif

  } else {
    // no need to increase file size:
    return TRUE;
  }// endif

#ifdef DEBUG
  printf("IFileAllocate: at 4\n");
#endif

  
  ifilep->size = end_byte;

#ifdef DEBUG
  printf("IFileAllocate: at 5\n");
  printf("               IFileSizeHDF says length is %ld\n",IFileSizeHDF(ifilep));
#endif



/* -------------------------------------------------------------------- */
/* Clear the last operation status metadata                             */
/* -------------------------------------------------------------------- */
  ifilep->last_operation_status = IFILE_SUCCESS;
  bassigncstr(ifilep->last_operation_status_message,
         "IFileAllocate() ran successfully");

#ifdef DEBUG
  printf("IFileAllocate: at 6\n");
#endif


/* -------------------------------------------------------------------- */
/* Clear the readPastEOF metadata                                       */
/* -------------------------------------------------------------------- */
  ifilep->readPastEOF = FALSE;

#ifdef DEBUG
  printf("IFileAllocate: at 7\n");
#endif


  return TRUE;
 
}
