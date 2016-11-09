/* IFileWriteStatusMessage.c

   by Leland Pierce, Jan 24, 2014.
  
*/

/** \brief IFileWriteStatusMessage writes the status message of an internal file.
\sa IFileOpen, FileClose, IFileWriteStatus
\param[in] ifilep The struct of the open internal file.
\param[in] message Message associated with the status.
\returns
\c TRUE is returned on success, \c FALSE otherwise.
The \c error_string variable is also set to a system-specific 
error code on failure.
\par Example
Let's assume that one already has an GeoSci file, and one has
created an internal file is named "/somename" and one wishes to
write a status message that the last operation caused the file
to hit the end-of-file.
\code
hid_t file_id;
IFILE *ifilep;

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifilep) {
   printf("IFileOpen failed on /somename\n");
}

if(!IFileWriteStatusMessage(ifilep,"IFilePutc() hit EOF")){
   printf("IFileWriteStatusMessage failed\n");
}
\endcode
\par Details
This function encapsulates writing the IFile status metadata.
This function would not normally be called by a user.
*/

#include "ifile.h"


/*******************************************************************************/
int IFileWriteStatusMessage( IFILE *ifilep, const char *message)
{

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");


/* -------------------------------------------------------------------- */
/* Set last_operation_status_message metadata                           */
/* -------------------------------------------------------------------- */
  if(!message){
    // deal with NULL Message: make it a single blank:
    bassigncstr(ifilep->last_operation_status_message,"");

  } else {
    bassigncstr(ifilep->last_operation_status_message,message);

  }//endif

  return TRUE;

}
