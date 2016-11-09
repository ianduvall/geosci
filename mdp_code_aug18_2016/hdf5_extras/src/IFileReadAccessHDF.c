/* IFileReadAccessHDF.c

   by Leland Pierce, Jan 22, 2014.
  
*/
/** \brief  IFileReadAccessHDF reads access code of an internal file

\sa IFileOpen, IFileClose
\param[in] datset_id The hdf-id of the open internal file dataset-object.

\returns
On success, one of:\n
\c IFILE_CLOSED 0 \n
\c IFILE_R   1 \n 
\c IFILE_W   2 \n
\c IFILE_A   3 \n
\c IFILE_RP  4 \n
\c IFILE_WP  5 \n
\c IFILE_AP  6 \n
On failure, \c ERROR is returned.

\par Example
Let's assume that one already has an GeoSci file, and one wants
to determine the access code for an internal file named "/somename".
\code
hid_t file_id;
his_t dataset_id;
int access_code;

dataset_id = GS_DatasetOpen(file_id,"/somename");
if(dataset_id < 0) {
   printf("IFileOpen failed on /somename\n");
}

access_code = IFileReadAccessHDF(dataset_id);
if(access_code<0){
   printf("could not obtain access code for internal file\n");
}
\endcode

\par Details
This function is used by the IFile* functions to determine how
to implement each function for a specific file access code.
*/

#include "ifile.h"


/*******************************************************************************/
int IFileReadAccessHDF( hid_t dataset_id )
{

  int internal_access_code;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Make sure dataset is the right type                                  */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsIFileByID(dataset_id)){
    bassignformat(error_string,"IFileReadAccessHDF: dataset is not an internal file.");
    return FALSE;
  }// endif

  /* -------------------------------------------------------------- */
  /* Read internal file access code                                 */
  /* -------------------------------------------------------------- */
  bstring internal_access = bfromcstr("");
  bstring access = bfromcstr("access");
  if(!GS_GetStringAttribute(dataset_id,access,internal_access)){
    bassignformat(error_string,"IFileReadAccessHDF: Can't read the access metadata");
    bdestroy(internal_access);
    bdestroy(access);
    return ERROR;
  }// endif
  bdestroy(access);
  if(sscanf(bdata(internal_access),"%d",&internal_access_code) != 1){
    bdestroy(internal_access);
    bassignformat(error_string,"IFileReadAccessHDF: Can't parse the access metadata");
    return ERROR;
  }// endif
  bdestroy(internal_access);

  /* -------------------------------------------------------------- */
  /* Valid code?                                                    */
  /* -------------------------------------------------------------- */
  if(internal_access_code <0 ||
     internal_access_code >6 ){
    bassignformat(error_string,"IFileReadAccessHDF: invalid access code.");
    return ERROR;
  }// endif
  
  /* -------------------------------------------------------------- */
  /* Return the file access code                                    */
  /* -------------------------------------------------------------- */
  return internal_access_code;

}
