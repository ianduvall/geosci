/* GS_ObjectIsMetadataDataset.c

      by Leland Pierce, Jan 22, 2014.
  
*/

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


/** \brief  ObjectIsMetadataDataset determines if the object is a metadata dataset
\see IFileOpen, IFileClose
\param[in] id   The handle of the open group.
\param[in] name  The name of the object within the group.
\returns
TRUE is returned if it is, FALSE otherwise.
\par Example
Open the Location metadata for an image and query 
if it's valid Metadata Dataset:
\code
hid_t file_id;
hid_t group_id;
bstring name =bfromcstr("/somename");
group_id = GS_GroupOpen(file_id,name);
bdestroy(name);
if(group_id < 0) {
   printf("GS_GroupOpen failed on /somename\n");
}
bstring dname = bfromcstr("location");
if(!GS_ObjectIsMetadataDataset(group_id, dname)){
  printf("Object is not a Metadata Dataset.\n");
}
bdestroy(dname);
\endcode

*/


//#define DEBUG

/*******************************************************************************/
int GS_ObjectIsMetadataDataset( hid_t id, const_bstring name )
{
  hid_t ifile_id;
  hid_t dataset_id;
  int thetype;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Is it a dataset?                                                     */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsDataset(id,name)){
    bassignformat(error_string,"ObjectIsMetadataDataset: not a dataset).");
    return FALSE;
  }// endif


/* -------------------------------------------------------------------- */
/* Open the dataset                                                     */
/* -------------------------------------------------------------------- */
  dataset_id = GS_DatasetOpen(id,name);
  if(!dataset_id){
    bassignformat(error_string,"ObjectIsMetadataDataset: can't open dataset");
    return FALSE;
  }// endif


/* -------------------------------------------------------------------- */
/* Get the dataset_type metadata                                        */
/* -------------------------------------------------------------------- */
  thetype = GS_DatasetGetType(dataset_id);
  if(thetype != GS_OBJECT_TYPE_METADATA_DATASET &&
     thetype != GS_OBJECT_TYPE_METADATA_IFILE ){
    GS_DatasetClose(dataset_id);
    return FALSE;
  }// endif


/* -------------------------------------------------------------------- */
/* Close the dataset                                                    */
/* -------------------------------------------------------------------- */
  if(!GS_DatasetClose(dataset_id)){
    bassignformat(error_string,"ObjectIsMetadataDataset: dataset cannot be closed.");
    return FALSE;
  }//endif


  return TRUE;
}

/** \brief ObjectIsMetadataDatasetByID determines if the objectid is a metadata dataset.
\see IFileOpen, IFileClose
\param[in] id  The handle of the open object.
\returns
TRUE is returned if it is, FALSE otherwise.
\par Example
Open the Location metadata for an image and query 
if it's a valid Metadata Dataset:
\code
hid_t file_id;
hid_t objid;
bstring name = bfromcstr("/someimage/location");
objid = GS_DatasetOpen(file_id,name);
if(objid < 0) {
   printf("GS_DatasetOpen failed on /someimage\n");
}
bdestroy(name);
if(!GS_ObjectIsMetadataDatasetByID(objid)){
  printf("Object is not a metadata dataset.\n");
}
\endcode
*/

/*******************************************************************************/
int GS_ObjectIsMetadataDatasetByID( hid_t dataset_id )
{
  hid_t ifile_id;
  int thetype;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Is it a dataset?                                                     */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsDatasetByID(dataset_id)){
    bassignformat(error_string,"ObjectIsMetadataDatasetByID: invalid object id).");
    return FALSE;
  }// endif


/* -------------------------------------------------------------------- */
/* Get the dataset_type metadata                                        */
/* -------------------------------------------------------------------- */
  thetype = GS_DatasetGetType(dataset_id);
  if(thetype != GS_OBJECT_TYPE_METADATA_DATASET &&
     thetype != GS_OBJECT_TYPE_METADATA_IFILE ){
    GS_FileClose(ifile_id);
    return FALSE;
  }// endif


  GS_FileClose(ifile_id);
  return TRUE;
}
