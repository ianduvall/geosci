/* IFileClearError.c

   by Leland Pierce, Jan 20, 2014.
  
*/


/** \brief IFileClearError clears the error condition on an internal file

\param[in] ifilep
The struct of the open internal file.

\returns
 TRUE is returned on SUCCESS, FALSE otherwise.
The error_string variable is also set to a system-specific 
error code on failure.

\par Example
Let's assume that one already has an GeoSci file, and one wants
to clear any error related to the previous read from an internal file 
named "/somename".
\code
hid_t file_id;
IFILE *ifilep;
char c;

ifilep = IFileOpen(file_id,"/somename","w");
....
c = IFileGets(ifilep);

IFileClearError(ifilep);
\endcode

\par Details
This function is meant to emulate as close as possible the standard
C clearerr() function.

*/

#include "ifile.h"


/*******************************************************************************/
int IFileClearError( IFILE *ifilep )
{

/* -------------------------------------------------------------------- */
/* Clear the last operation status metadata                             */
/* -------------------------------------------------------------------- */
  ifilep->last_operation_status = IFILE_SUCCESS;
  bassigncstr(ifilep->last_operation_status_message,
         "Successfully cleared errors in internal file");

/* -------------------------------------------------------------------- */
/* Clear the readPastEOF metadata                                       */
/* -------------------------------------------------------------------- */
  ifilep->readPastEOF=FALSE;

  return TRUE;
 
}
