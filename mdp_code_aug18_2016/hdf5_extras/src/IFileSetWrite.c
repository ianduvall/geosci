/* IFileSetWrite.c

   by Leland Pierce, Jan 20, 2014.
  
*/

/** \brief IFileSetWrite sets the read/write property of an internal file
\sa IFileOpen, IFileClose
\param[in] ifilep    The struct of the open internal file.
\param[in] property  The value to set the read/write property to:\n
                   GS_READ  -- internal file set to read-only mode\n
                   GS_WRITE -- internal file set to read and write mode
\returns
\c TRUE is returned on success, \c FALSE otherwise.
The \c error_string variable is also set to a system-specific 
error code on failure.
\par Example
Let's assume that one already has an GeoSci file, and one wants
to create an internal file is named "/somename" and make it writeable.
This is used by the internal IFileOpen routine when creating a new
internal file.
\code
hid_t file_id;
IFILE *ifilep;

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifilep) {
   printf("IFileOpen failed on /somename\n");
}

if(!IFileSetWrite(ifilep,GS_WRITE)){
   printf("IFileSetWrite to read-write failed\n");
}
\endcode
\par Details
This function is meant to enable IFileOpen to make an internal file
have read and write rights on creation.
*/

#include "ifile.h"


/*******************************************************************************/
int IFileSetWrite( IFILE *ifilep, int property )
{
  char *status_str;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Check property                                                       */
/* -------------------------------------------------------------------- */
  switch(property){
  case GS_READ:
  case GS_WRITE:
    break;
  default:
    bassignformat(error_string,"IFileSetWrite: invalid property: %d",property);
    return FALSE;
  }// endswitch

/* -------------------------------------------------------------------- */
/* Update the writeability metadata                                     */
/* -------------------------------------------------------------------- */
  bstring writeable = bfromcstr("writeable");
  bstring btrue = bfromcstr("TRUE");
  bstring bfalse = bfromcstr("FALSE");
  switch(property){
  case GS_WRITE:
    if(!GS_UpdateMetadata(ifilep->ifile_id, writeable, btrue )){
      bassignformat(error_string,"IFileSetWrite: Can't write the writeability metadata for internal file");
      bdestroy(writeable);
      bdestroy(btrue);
      bdestroy(bfalse);
      return FALSE;
    }//endif
    break;

  case GS_READ:
    if(!GS_UpdateMetadata(ifilep->ifile_id, writeable, bfalse )){
      bassignformat(error_string,"IFileSetWrite: Can't write the writeability metadata for internal file");
      bdestroy(writeable);
      bdestroy(btrue);
      bdestroy(bfalse);
      return FALSE;
    }//endif
    break;
  }// endswitch

  bdestroy(writeable);
  bdestroy(btrue);
  bdestroy(bfalse);
  

  return TRUE;
}
