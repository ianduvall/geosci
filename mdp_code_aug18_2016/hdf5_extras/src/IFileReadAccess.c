/* IFileReadAccess.c

   by Leland Pierce, Jan 22, 2014.
  
*/

/** \brief IFileReadAccess reads access code of an internal file
\sa IFileOpen, IFileClose
\param[in] ifilep The struct of the open internal file.
\returns
On success, one of: \n
\c IFILE_CLOSED 0 \n
\c IFILE_R   1 \n
\c IFILE_W   2 \n 
\c IFILE_A   3 \n 
\c IFILE_RP  4 \n
\c IFILE_WP  5 \n
\c IFILE_AP  6 \n 
On failure, ERROR is returned.

\par Example
Let's assume that one already has an GeoSci file, and one wants
to determine the access code for an internal file named "/somename".
\code
hid_t file_id;
IFILE *ifilep;
int access_code;

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifilep) {
   printf("IFileOpen failed on /somename\n");
}

access_code = IFileReadAccess(ifilep);
if(access_code<0){
   printf("could not obtain access code for internal file\n");
}
\endcode

\par Details
This function is used by the IFile* functions to determine how
to implement each function for a specific file access code.

*/

#include "ifile.h"


/*******************************************************************************/
int IFileReadAccess( IFILE *ifilep )
{
  /* -------------------------------------------------------------- */
  /* Return the file access code                                    */
  /* -------------------------------------------------------------- */
  return ifilep->access;

}
