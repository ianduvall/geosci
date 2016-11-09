/* IFileSeek.c

   by Leland Pierce, Jan 22, 2014.
  
*/

/** \brief  IFileSeek moves to a position in an internal file

\sa IFileOpen, IFileClose
\param[in] ifilep The struct of the open internal file.
\param[in] offset Offset, in bytes, from a reference position
where the new file pointer is desired to go.
\parma[in] origin Position used as reference for the offset. 
            Specify one of the following constants :
            Constant  |    Reference Position
            --------- | -------------------------------
            SEEK_SET  |    Start of file
            SEEK_CUR  |    Current position in the file
            SEEK_END  |    End of file

\returns
\parblock
\c TRUE is returned on success, \c FALSE otherwise.

For situations where the new position would be before the first byte,
the file pointer position is not changed, and \c FALSE is returned.

For situations where the new position is beyond the last byte,
this is allowed, but a subsequent read will fail. A subsequent
write will "fill in" \c NULL bytes to get to this point before writing.

The error_string variable is also set to a system-specific 
error code on failure.
\endparblock

\par Example
Let's assume that one already has an GeoSci file, and one wants
to skip to byte 35 in an internal file named "/somename".
\code
hid_t file_id;
IFILE *ifilep;

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifilep) {
   printf("IFileOpen failed on /somename\n");
}

if(!IFileSeek(ifilep, 35L, SEEK_SET)){
   printf("seek failed\n");
}
\endcode

\par Details
This function is meant to emulate as close as possible the standard
C \c fseek() function.
*/

#include "ifile.h"
#include "unistd.h"

//#define DEBUG

/*******************************************************************************/
int IFileSeek( IFILE *ifilep, long int offset, int origin )
{
  long int iorigin;
  long int new_position;
  char *file_position_string;
  char filepos_string[100];

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("IFileSeek: 1\n");
#endif



/* -------------------------------------------------------------------- */
/* Is the value of origin valid?                                        */
/* -------------------------------------------------------------------- */
  switch(origin){
  case SEEK_SET:
  case SEEK_CUR:
  case SEEK_END:
    break;
  default:
    bassignformat(error_string,"IFileSeek: invalid origin.");
    return FALSE;    
  }// end switch

#ifdef DEBUG
  printf("IFileSeek: 3\n");
#endif


/* -------------------------------------------------------------------- */
/* Determine origin byte position                                       */
/* -------------------------------------------------------------------- */
  switch(origin){
  case SEEK_SET:
    iorigin = 0L; break;
  case SEEK_CUR:
    iorigin = ifilep->file_position; break;
  case SEEK_END:
    iorigin = ifilep->size; break;
  }// endswitch

/* -------------------------------------------------------------------- */
/* Calculate new position                                               */
/* -------------------------------------------------------------------- */
  new_position = iorigin + offset;
  if(new_position < 0) {
    bassignformat(error_string,"IFileSeek: Can't seek before 1st byte.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Write new file position                                              */
/* -------------------------------------------------------------------- */
  ifilep->file_position = new_position;

/* -------------------------------------------------------------------- */
/* Update the last operation status metadata                            */
/* -------------------------------------------------------------------- */
  ifilep->last_operation_status = IFILE_SUCCESS;
  bassigncstr(ifilep->last_operation_status_message,
         "IFileSeek() ran successfully");

/* -------------------------------------------------------------------- */
/* Clear the readPastEOF metadata                                       */
/* -------------------------------------------------------------------- */
  ifilep->readPastEOF = FALSE;

  return TRUE;
 
}
