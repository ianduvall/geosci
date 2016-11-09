/* IFileSize.c

   by Leland Pierce, Jan 22, 2014.
  
*/

/** \brief IFileSize gets the #bytes in an internal file
\sa IFileOpen, IFileClose
\param[in] ifilep The struct of the open internal file.
\returns
On success, the number of characters in the internal file is returned.
-1 is returned on error.
\par Example
Let's assume that one already has an GeoSci file, and one wants
to know the size of an internal file named "/somename".
\code
hid_t file_id;
IFILE *ifilep;
long int numbytes;

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifilep) {
   printf("IFileOpen failed on /somename\n");
}

numbytes = IFileSize(ifilep);
if(numbytes < 0) {
   printf("failed to get file size\n");
}
\endcode
\par Details
There is no equivalent standard C function to get the size of an open file.
*/

#include "ifile.h"

long int IFileSize( IFILE *ifilep )
{
  return ifilep->size;
}
