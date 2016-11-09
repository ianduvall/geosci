/* IFileWriteAccess.c

   by Leland Pierce, Jan 22, 2014.
  
*/


/** \brief IFileWriteAccess writes a new access code for an internal file
\sa IFileOpen, IFileClose
\param[in] ifilep The struct of the open internal file.
\param[in] code The access code one wishes to write to the file, one of:\n
            \c IFILE_CLOSED 0 \n
            \c IFILE_R   1 \n
            \c IFILE_W   2 \n
            \c IFILE_A   3 \n
            \c IFILE_RP  4 \n
            \c IFILE_WP  5 \n
            \c IFILE_AP  6 \n
\returns
\c TRUE is returned on success, \c FALSE on failure.
\par Example
Let's assume that one already has an GeoSci file, and one wants
to set the access code to read-only for an internal file 
named "/somename".
\code
hid_t file_id;
IFILE *ifilep;
int access_code;

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifilep) {
   printf("IFileOpen failed on /somename\n");
}

if(!IFileWriteAccess(ifilep, IFILE_R)){
   printf("could not set access code for internal file\n");
}
\endcode
\par Details
This function is used by the IFile* functions to set the
internal-file access policy. They can be read, write, append,
read-and-update, write-and-update, append-and-update, as well
as closed. This routine is for use by the IFile* routines, and
not normally for use by ordinary users.
*/

#include "ifile.h"

//#define DEBUG

/*******************************************************************************/
int IFileWriteAccess(  IFILE *ifilep, int code )
{
  bstring internal_access = bfromcstr("");

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
 printf("IFileWriteAccess: start\n");
#endif

  /* -------------------------------------------------------------- */
  /* Check that code is valid                                       */
  /* -------------------------------------------------------------- */
  if(code < 0 || code > 6 ){
    bassignformat(error_string,"IFileWriteAccess: Invalid access code: %d.",code);
    bdestroy(internal_access);
    return FALSE;
  }//endif

#ifdef DEBUG
 printf("IFileWriteAccess: 1\n");
#endif


  /* -------------------------------------------------------------- */
  /* Write internal file access code to structure                   */
  /* -------------------------------------------------------------- */
  ifilep->access = code;


#ifdef DEBUG
 printf("IFileWriteAccess: 2\n");
#endif

  /* -------------------------------------------------------------- */
  /* Write internal file access code to HDF5 metadata               */
  /* -------------------------------------------------------------- */
  bassignformat(internal_access,"%d",code);
  bstring access = bfromcstr("access");
  if(!GS_UpdateMetadata(ifilep->ifile_id, access, internal_access)){
    bdestroy(internal_access);
    bdestroy(access);
    bassignformat(error_string,"IFileWriteAccess: Can't create the access metadata.");
    return FALSE;
  }//endif
  bdestroy(internal_access);
  bdestroy(access);

#ifdef DEBUG
 printf("IFileWriteAccess: 4\n");
#endif

  
  return TRUE;

}
