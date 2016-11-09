/* IFileReadStatus.c

   by Leland Pierce, Jan 24, 2014.
  
*/

/** \brief IFileReadStatus reads the status properties of an internal file
\sa IFileOpen, IFileClose

\param[in] ifilep  The struct of the open internal file.
\param[in] message  Returned Message associated with the status.
            No need to GFree it when done.

\returns
On success, the return value is one of:\n
            \c IFILE_SUCCESS -- the last IFile operation succeeded\n
            \c IFILE_FAILURE -- the last IFile operation failed\n
On failure, \c FALSE is returned.
The \c error_string variable is also set to a system-specific 
error code on failure.

\par Example
Let's assume that one already has an GeoSci file, and one has
created an internal file is named "/somename" and the last write
operation may have failed. Let's get the status.
\code
hid_t file_id;
IFILE *ifilep;
int status;
char *status_str;

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifilep) {
   printf("IFileOpen failed on /somename\n");
}

status = IFileReadStatus(ifilep,&status_str);
if(!status){
   printf("IFileReadStatus failed\n");
} else {
   if(status == IFILE_SUCCESS){
      printf("status of last operation is: SUCCESS\n");
   } else {
      printf("status of last operation is: FAILURE\n");
   } // endif 
   printf("status message: %s\n",status_str);
} // endif
\endcode

\par Details
This function encapsulates reading the IFile status metadata.
*/

#include "ifile.h"


/*******************************************************************************/
int IFileReadStatus( IFILE *ifilep, char **message)
{

  *message = bdata(ifilep->last_operation_status_message);

  return ifilep->last_operation_status;

}
