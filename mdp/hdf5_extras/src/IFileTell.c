/* IFileTell.c

   by Leland Pierce, Jan 22, 2014.
  
*/

/** \brief IFileTell retrieves the current read/write position in an internal file
\sa IFileOpen, IFileClose
\param[in] ifilep The struct of the open internal file.
\returns
The position in the file is returned on success, and 
-1L on failure.
The \c error_string variable is also set to a system-specific 
error code on failure.
\par Example
Let's assume that one already has an GeoSci file, and one wants
to find the position of the file-pointer in an internal file 
named "/somename".
\code
hid_t file_id;
IFILE *ifilep;
long int position;

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifilep) {
   printf("IFileOpen failed on /somename\n");
}

position = IFileTell(ifilep);
if(position < 0){
   printf("tell failed\n");
}
\endcode
\par Details
This function is meant to emulate as close as possible the standard
C \c ftell() function.
*/

#include "ifile.h"


/*******************************************************************************/
long int IFileTell( IFILE *ifilep )
{
  return ifilep->file_position;
}
