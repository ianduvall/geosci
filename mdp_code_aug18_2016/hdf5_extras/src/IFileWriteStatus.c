/* IFileWriteStatus.c

   by Leland Pierce, Jan 24, 2014.
  
*/

/** \brief IFileWriteStatus writes the status properties of an internal file
\sa IFileOpen, IFileClose
\param[in] ifilep The struct of the open internal file.
\param[in] status  The value to set the status property to:\n
            \c IFILE_SUCCESS -- the last IFile operation succeeded \n
            \c IFILE_FAILURE -- the last IFile operation failed \n
\param[in] message Message associated with the status.
\returns
\c TRUE is returned on success, \c FALSE otherwise.
The \c error_string variable is also set to a system-specific 
error code on failure.
\par Example
Let's assume that one already has an GeoSci file, and one has
created an internal file is named "/somename" and the last write
operation failed (using IFilePutc()).
\code
hid_t file_id;
IFILE *ifilep;

ifilep = IFileOpen(file_id,"/somename","w");

if(!IFileWriteStatus(ifilep,IFILE_FAILURE,"IFilePutc() failed")){
   printf("IFileWriteStatus failed\n");
}
\endcode
\par Details.
This function encapsulates writing the IFile status metadata. 
This function would not be used by the ordinary user.
*/

#include "ifile.h"


/*******************************************************************************/
int IFileWriteStatus( IFILE *ifilep, int status, const char *message)
{

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Set last_operation_status metadata                                   */
/* -------------------------------------------------------------------- */
  switch(status){

  case IFILE_SUCCESS:
    ifilep->last_operation_status = IFILE_SUCCESS;
    break;

  case IFILE_FAILURE:
    ifilep->last_operation_status = IFILE_FAILURE;
    break;
    
  default:
    bassignformat(error_string,"IFileWriteStatus: Invalid status: %d.",status);
    return FALSE;
    
  }// end-switch

/* -------------------------------------------------------------------- */
/* Set last_operation_status_message metadata                           */
/* -------------------------------------------------------------------- */
  if(!message){
    // deal with NULL Message
    bassigncstr(ifilep->last_operation_status_message,"");

  } else {
    bassigncstr(ifilep->last_operation_status_message,message);

  }//endif

  return TRUE;

}
