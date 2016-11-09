/* IFileSetWriteability.c

   by Leland Pierce, Jan 20, 2014.
  
*/

/** \brief  IFileSetWriteability sets the read/write property of an internal file
\sa IFileOpen, IFileClose
\param[in] file_id The handle of the open geosci file.
\param[in] name Name of the internal file whose properties
need to be altered.
The internal file must not be open, otherwise
this operation will fail.
\param[in] property  The value to set the read/write property to:\n
                   \c GS_READ  -- internal file set to read-only mode\n
                   \c GS_WRITE -- internal file set to read and write mode
\returns
\c TRUE is returned on success, \c FALSE otherwise.
The \c error_string variable is also set to a system-specific 
error code on failure.

\par Example
Let's assume that one already has an GeoSci file, and one wants
to make sure the file is not writeable accidentally. The internal 
file is named "/somename".
\code
hid_t file_id;

if(!IFileSetWriteability(file_id,"/somename",GS_READ)){
   printf("IFileSetWriteability to read-only failed\n");
}
\endcode

\par Details
This function is meant to enable a user to make an internal file
have read and write rights like in the standard file system.
*/

#include "ifile.h"


/*******************************************************************************/
int IFileSetWriteability( hid_t file_id, const char *name, int property )
{
  hid_t ifile_id;
  hid_t dataset_id;
  bstring ctmp;
  bstring bstr_name=bfromcstr("");

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Get a valid file_id                                                  */
/* -------------------------------------------------------------------- */
  ifile_id = GS_GetValidFileID(file_id);
  if(!ifile_id){
    bassignformat(error_string,"IFileSetWritability: invalid file id).");
    bdestroy(bstr_name);
    return FALSE;
  }// endif
  if(!GS_FileIsWriteable(ifile_id)){
    ctmp = bstrcpy(GS_GetFilename(file_id));
    bassignformat(error_string,"IFileSetWritability: File cannot be written to: %s",ctmp);
    bdestroy(ctmp);
    bdestroy(bstr_name);
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Check property                                                       */
/* -------------------------------------------------------------------- */
  switch(property){
  case GS_READ:
  case GS_WRITE:
    break;
  default:
    bassignformat(error_string,"IFileSetWritability: invalid property: %d",property);
    bdestroy(bstr_name);
    return FALSE;
  }// endswitch

/* -------------------------------------------------------------------- */
/* Try opening name as a dataset                                        */
/* -------------------------------------------------------------------- */
  bstr_name = bfromcstr(name);
  dataset_id = GS_DatasetOpen(file_id,bstr_name);
  if(dataset_id < 0){
    bassignformat(error_string,"IFileSetWritability: IFile cannot be opened: %s\n",name);
    bdestroy(bstr_name);
    return FALSE;
  }// endif
  bdestroy(bstr_name);

/* -------------------------------------------------------------------- */
/* Is it an internal file?                                              */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsIFileByID(dataset_id)){
    GS_DatasetClose(dataset_id);
    bassignformat(error_string,"IFileSetWritability: not a valid internal file.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Update the writeability metadata                                     */
/* -------------------------------------------------------------------- */
  bstring writeable = bfromcstr("writeable");
  bstring btrue = bfromcstr("TRUE");
  bstring bfalse = bfromcstr("FALSE");
  switch(property){
  case GS_WRITE:
    if(!GS_UpdateMetadata(dataset_id, writeable, btrue )){
      GS_DatasetClose(dataset_id);
      bassignformat(error_string,"IFileSetWritability: Can't write the writeability metadata for internal file");
      bdestroy(writeable);
      bdestroy(btrue);
      bdestroy(bfalse);
      return FALSE;
    }//endif
    break;

  case GS_READ:
    if(!GS_UpdateMetadata(dataset_id, writeable, bfalse )){
      GS_DatasetClose(dataset_id);
      bassignformat(error_string,"IFileSetWritability: Can't write the writeability metadata for internal file");
      bdestroy(writeable);
      bdestroy(btrue);
      bdestroy(bfalse);
      return FALSE;
    }//endif
    break;
  }// endswitch

/* -------------------------------------------------------------------- */
/* Clean up                                                             */
/* -------------------------------------------------------------------- */
  bdestroy(writeable);
  bdestroy(btrue);
  bdestroy(bfalse);
  GS_DatasetClose(dataset_id);

  return TRUE;
}
