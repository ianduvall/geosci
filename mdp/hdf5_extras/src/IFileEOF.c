/* IFileEOF.c

   by Leland Pierce, Jan 20, 2014.
  
*/

/** \brief IFileEOF returns the End-Of-File condition of the previous internal read

\sa IFileOpen, IFileClose

\param[in] ifilep
The struct of the open internal file.

\returns
 TRUE is returned if a previous read tried to read past  the
end-of-file, FALSE if not.
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
if(!IFileEOF(ifilep)){
   printf("previous read DID NOT read past end-of-file\n");
} else {
   printf("previous read DID read past end-of-file\n");
}
\endcode

\par Details 
This function is meant to emulate as close as possible the standard
C feof() function.
*/

#include "ifile.h"


/*******************************************************************************/
int IFileEOF( IFILE *ifilep)
{
  //printf("IFIleEOF: ifilep=%p, ifilep->readPastEOF=%d\n",
  //       ifilep,ifilep->readPastEOF);

  return ifilep->readPastEOF;
}
