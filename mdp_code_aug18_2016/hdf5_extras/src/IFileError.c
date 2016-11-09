/* IFileError.c

   by Leland Pierce, Jan 20, 2014.
  
*/
/** \brief IFileError returns error condition on an internal file

\sa IFileOpen, IFileClose

\param[in] ifilep
The struct of the open internal file.

\returns
FALSE is returned if "last_operation_status" metadata for the
internal file is "SUCCESS", TRUE otherwise.
ERROR is returned on any kind of error.
The error_string variable is also set to a system-specific 
error code on failure.

\par Example
Let's assume that one already has an GeoSci file, and one wants
to determine if the previous read from an internal file 
named "/somename" succeeded or not.
\code
hid_t file_id;
IFILE *ifilep;

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifilep) {
   printf("IFileOpen failed on /somename\n");
}

if(!IFileError(ifilep)){
   printf("previous read DID NOT cause an error\n");
} else {
   printf("previous read DID cause an error\n");
}
\endcode

\par Details
This function is meant to emulate as close as possible the standard
C ferror() function.
*/

#include "ifile.h"


/*******************************************************************************/
int IFileError( IFILE *ifilep )
{
  if(ifilep->last_operation_status == IFILE_SUCCESS) {
    return FALSE;
  } else {
    return TRUE;
  }// endif
}
