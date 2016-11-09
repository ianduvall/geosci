/* IFileSetEOF.c

   by Leland Pierce, Jan 20, 2014.
  
*/

/** \brief IFileSetEOF sets End-Of-File condition on an internal file

\sa IFileOpen, IFileClose

\param[in] filep The struct of the open internal file.
\param[in] value The value to set the \c EOF condition to, one of:\n
                    \c TRUE (if there was an \c EOF ), or:\n
                    \c FALSE (there was not an \c EOF ) \n
\returns
\c TRUE is returned when the condition is set successfully,
\c FALSE if not.
The \c error_string variable is also set to a system-specific 
error code on failure.

\par Example
Let's assume that one already has an GeoSci file, and one wants
to set that the previous read  hit an \c EOF on an internal file
named "/somename" succeeded or not.
\code
hid_t file_id;
IFILE *ifilep;

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifilep < 0) {
   printf("IFileOpen failed on /somename\n");
}
if(!IFileSetEOF(ifilep, TRUE)){
   printf("could not set EOF condition\n");
} else {
   printf("successfully set EOF condition to TRUE\n");
}
\endcode

\par Details
This function is used internally by the IFile* functions, and is not
meant for usage by the normal user.
*/

#include "ifile.h"


/*******************************************************************************/
int IFileSetEOF( IFILE *ifilep, int value )
{

/* -------------------------------------------------------------------- */
/* Write the EOF metadata                                               */
/* -------------------------------------------------------------------- */
  if(value){
    ifilep->readPastEOF = TRUE;
  } else {
    ifilep->readPastEOF = FALSE;
  }// endif

  return TRUE;
}
