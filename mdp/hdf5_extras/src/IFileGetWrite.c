/* IFileGetWrite.c

   by Leland Pierce, Jan 20, 2014.
  
*/

/** \brief IFileGetWrite gets the read/write property of an internal file

\sa IFileOpen, IFileClose

\param[in] ifilep
The struct of the open internal file.

\returns
\parblock
TRUE is returned if the internal file is writeable,
FALSE if not writeable.

ERROR is returned on any kind of error.
The error_string variable is also set to a system-specific 
error code on failure.
\endparblock

\par Example
Let's assume that one already has an GeoSci file, and one wants
to determine if the  internal file named "/somename" is writeable.
\code
hid_t file_id;
IFILE *ifilep;

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifilep) {
   printf("IFileOpen failed on /somename\n");
}

status = IFileGetWrite(ifilep);
if( status ==FALSE){
   printf("Cannot write to internal file\n");
} else if(status == TRUE) {
   printf("Can write to internal file\n");
} else {
   printf("error getting write-status of internal file\n");
}
\endcode

\par Details
This function is meant to enable a user to make an internal file
have read and write rights like in the standard file system.

*/

#include "ifile.h"

//#define DEBUG

/*******************************************************************************/
int IFileGetWrite( IFILE *ifilep )
{
  bstring w_string=bfromcstr("");
  
  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("IFileGetWrite: 1\n");
#endif

/* -------------------------------------------------------------------- */
/* Read the writeable metadata                                          */
/* -------------------------------------------------------------------- */
  bstring writeable = bfromcstr("writeable");
  if(!GS_GetStringAttribute(ifilep->ifile_id,writeable,w_string)){
    bassignformat(error_string,"IFileGetWrite: Can't get the writeability metadata.");
    bdestroy(writeable);
    bdestroy(w_string);
    return ERROR;
  }// endif
  bdestroy(writeable);

#ifdef DEBUG
  printf("IFileGetWrite: 3, w_string=|%s|\n",w_string);
#endif


/* -------------------------------------------------------------------- */
/* Return writeability status                                           */
/* -------------------------------------------------------------------- */
  bstring btrue = bfromcstr("TRUE");
  if(EQUAL(w_string,btrue)){
    bdestroy(w_string);
    bdestroy(btrue);
    return TRUE;
  } else {
    bdestroy(w_string);
    bdestroy(btrue);
    return FALSE;
  }// endif
 
}
