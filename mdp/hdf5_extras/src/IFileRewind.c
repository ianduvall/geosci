/* IFileRewind.c

   by Leland Pierce, Jan 22, 2014.
  
*/

/** \brief IFileRewind moves to the start of an internal file

\see IFileOpen, IFileClose

\param[in] ifilep The struct of the open internal file.

\returns
\c TRUE is returned on success, \c FALSE otherwise.
The \c error_string variable is also set to a system-specific 
error code on failure.

\par Example
Let's assume that one already has an GeoSci file, and one wants
to rewind an internal file named "/somename".
\code
hid_t file_id;
IFILE *ifilep;

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifilep) {
   printf("IFileOpen failed on /somename\n");
}

if(!IFileRewind(ifilep)){
   printf("rewind failed\n");
}
\endcode

\par Details
This function is meant to emulate as close as possible the standard
C \c rewind() function.
*/

#include "ifile.h"

//#define DEBUG

/*******************************************************************************/
int IFileRewind( IFILE *ifilep )
{

#ifdef DEBUG
  printf("IFileRewind: ifilep=%p, ifilep->file_position=%ld\n",
         ifilep, ifilep->file_position);
#endif

/* -------------------------------------------------------------------- */
/* Write new file position                                              */
/* -------------------------------------------------------------------- */
  ifilep->file_position = 0L;

#ifdef DEBUG
  printf("IFileRewind: a: ifilep=%p, ifilep->file_position=%ld\n",
         ifilep, ifilep->file_position);
#endif


/* -------------------------------------------------------------------- */
/* Clear the last operation status metadata                             */
/* -------------------------------------------------------------------- */
  ifilep->last_operation_status = IFILE_SUCCESS;
  bassigncstr(ifilep->last_operation_status_message,
         "IFileRewind() ran successfully");

#ifdef DEBUG
  printf("IFileRewind: b: ifilep=%p, ifilep->file_position=%ld\n",
         ifilep, ifilep->file_position);
#endif


/* -------------------------------------------------------------------- */
/* Clear the readPastEOF metadata                                       */
/* -------------------------------------------------------------------- */
  ifilep->readPastEOF = FALSE;

#ifdef DEBUG
  printf("IFileRewind: c: ifilep=%p, ifilep->file_position=%ld\n",
         ifilep, ifilep->file_position);
#endif


  return TRUE;
 
}
