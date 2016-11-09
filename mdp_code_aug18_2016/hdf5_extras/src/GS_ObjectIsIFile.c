/* ObjectIsIFile.c

      by Leland Pierce, Jan 22, 2014.
  
*/


#include "ifile.h"
#include "bstrlib.h"


//#define DEBUG22


/** \brief GS_ObjectIsIFile determines if the object is an internal file


\param[in] id
                   The handle for the already-open group.

\param[in] name
                   The name of the ifile within the group.


\returns
\c TRUE is returned if the object is an IFile, \c FALSE otherwise.

\par Example
Open an internal file and query if it's valid IFile handle:
\code
hid_t file_id;
hid_t group_id;

group_id = GS_GroupOpen(file_id,"/somename");
if(group_id < 0) {
   printf("GS_GroupOpen failed on /somename\n");
}
bstring internal_file_name=bfromcstr("internal_file_name");
if(!GS_ObjectIsIFile(group_id, internal_file_name )){
  printf("Object is not an internal file.\n");
}
bdestroy(internal_file_name);
\endcode
*/



/*******************************************************************************/
int GS_ObjectIsIFile( hid_t id, const_bstring name )
{
  hid_t ifile_id;
  hid_t dataset_id;
  int thetype;
  bstring ctmp;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Is it a dataset?                                                     */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsDataset(id,name)){
    bassignformat(error_string,"ObjectIsIFile: not a dataset).");
    return FALSE;
  }// endif


#ifdef DEBUG22
        printf("EE1: \n");
        report_num_objects_open();
#endif


/* -------------------------------------------------------------------- */
/* Open the dataset                                                     */
/* -------------------------------------------------------------------- */
  dataset_id = GS_DatasetOpen(id,name);
  if(!dataset_id){
    bassignformat(error_string,"ObjectIsIFile: can't open dataset");
    return FALSE;
  }// endif

#ifdef DEBUG22
        printf("EE2: \n");
        report_num_objects_open();
#endif

/* -------------------------------------------------------------------- */
/* Determine the id of the GeosSciPy file                               */
/* -------------------------------------------------------------------- */
  ifile_id = GS_GetValidFileID(dataset_id);
  if(!ifile_id){
    GS_DatasetClose(dataset_id);
    bassignformat(error_string,"ObjectIsIFile: File-id cannot be determined");
    return FALSE;
  }// endif

#ifdef DEBUG22
        printf("EE3: \n");
        report_num_objects_open();
#endif


/* -------------------------------------------------------------------- */
/* File must be writeable                                               */
/* -------------------------------------------------------------------- */
  if(!GS_FileIsWriteable(ifile_id)){
    GS_FileClose(ifile_id);
    GS_DatasetClose(dataset_id);
    ctmp = bstrcpy(GS_GetFilename(ifile_id));
    bassignformat(error_string,"ObjectIsIFile: File cannot be written to: %s.",ctmp);
    bdestroy(ctmp);
    return FALSE;
  }// endif

#ifdef DEBUG22
        printf("EE4: \n");
        report_num_objects_open();
#endif


/* -------------------------------------------------------------------- */
/* Get the dataset_type metadata                                        */
/* -------------------------------------------------------------------- */
  thetype = GS_DatasetGetType(dataset_id);
  if(thetype != GS_OBJECT_TYPE_IFILE &&
     thetype != GS_OBJECT_TYPE_METADATA_IFILE ){
    GS_FileClose(ifile_id);
    GS_DatasetClose(dataset_id);
    return FALSE;
  }// endif

#ifdef DEBUG22
        printf("EE5: \n");
        report_num_objects_open();
#endif

/* -------------------------------------------------------------------- */
/* Close the dataset                                                    */
/* -------------------------------------------------------------------- */
  if(!GS_DatasetClose(dataset_id)){
    bassignformat(error_string,"ObjectIsIFile: dataset cannot be closed.");
    GS_FileClose(ifile_id);
    return FALSE;
  }//endif

#ifdef DEBUG22
        printf("EE6: \n");
        report_num_objects_open();
#endif

  GS_FileClose(ifile_id);
  return TRUE;
}
/**************************************************************************/
/** \brief GS_ObjectIsIFileByID determines if the objectID is an internal file

\param[in] id
                   The handle of the open internal file.


\returns
\c TRUE is returned if the object is an IFile, \c FALSE otherwise.

\par Example
Open an internal file and query if it's valid IFile handle:
\code
hid_t file_id;
hid_t internal_file_id;

internal_file_id = GS_IFileOpen(file_id,"/somename","w");
if(internal_file_id < 0) {
   printf("GS_IFileOpen failed on /somename\n");
}

if(!GS_ObjectIsIFileByID(internal_file_id)){
  printf("Object is not an internal file.\n");
}
\endcode
*/
/*******************************************************************************/
int GS_ObjectIsIFileByID( hid_t dataset_id )
{
  hid_t ifile_id;
  int thetype;
  char *ctmp;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Is it a dataset?                                                     */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsDatasetByID(dataset_id)){
    bassignformat(error_string,"ObjectIsIFileByID: invalid object id).");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Determine the id of the GeosSciPy file                               */
/* -------------------------------------------------------------------- */
  ifile_id = GS_GetValidFileID(dataset_id);
  if(!ifile_id){
    bassignformat(error_string,"ObjectIsIFileByID: File-id cannot be determined");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* File must be writeable                                               */
/* -------------------------------------------------------------------- */
  if(!GS_FileIsWriteable(ifile_id)){
    ctmp = bstrcpy(GS_GetFilename(ifile_id));
    bassignformat(error_string,"ObjectIsIFileByID: File cannot be written to: %s.",ctmp);
    bdestroy(ctmp);
    GS_FileClose(ifile_id);
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Get the dataset_type metadata                                        */
/* -------------------------------------------------------------------- */
  thetype = GS_DatasetGetType(dataset_id);
  if(thetype != GS_OBJECT_TYPE_IFILE &&
     thetype != GS_OBJECT_TYPE_METADATA_IFILE ){
    GS_FileClose(ifile_id);
    return FALSE;
  }// endif


  GS_FileClose(ifile_id);
  return TRUE;
}
