/* IFileFlush.c

   by Leland Pierce, Jan 20, 2014.
  
*/

/** \brief IFileFlush flushes unwritten buffers to an internal file in a GeoSci File

\sa IFileOpen, IFileClose

\param[in] ifilep
The struct of the open internal file.

\returns
TRUE is returned on success, FALSE on failure.
The error_string variable is also set to a system-specific 
error code on failure.

\par Example
Let's assume that one already has an GeoSci file, and one wants
to flush an internal file named "/somename":
\code
hid_t file_id;
IFILE *ifilep;

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifilep) {
   printf("IFileOpen failed on /somename\n");
}
if(!IFileFlush(ifilep)){
   printf("IFileFlush failed on /somename\n");
}
\endcode

\par Details
This function is meant to emulate as close as possible the standard
C fflush() function.

*/

#include "ifile.h"


/*******************************************************************************/
int IFileFlush( IFILE *ifilep)
{
  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/*      Flush any pending updates                                       */
/* -------------------------------------------------------------------- */
  if(!GS_FileFlush(ifilep->ifile_id)){
    bassignformat(error_string,"IFileFlush: Can't flush file.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Set last operation status metadata                                   */
/* -------------------------------------------------------------------- */
  ifilep->last_operation_status = IFILE_SUCCESS;
  bassigncstr(ifilep->last_operation_status_message,
         "Successfully flushed internal file");

  return TRUE;

}
