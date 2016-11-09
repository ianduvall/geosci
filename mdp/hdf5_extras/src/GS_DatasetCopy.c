/* GS_DatasetCopy.c

   by Leland Pierce, Aug 9, 2014

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


#define ERROR -1


/** \brief GS_DatasetCopy copies a dataset to an existing Group:
this can be in the same group, a different group, or a different file.

\see GS_DatasetClose(), GS_DatasetOpen()

\param[in] source
                   A handle for a dataset.
\param[in] destination_group_id
                   The handle of the destination group.
\param[in] destination_dataset_name
                   The name of the new dataset to create.

\returns
The handle of the new dataset is returned.
If it is less than zero, the copy failed.

\par Example
Copy an image raster dataset to a new group, with a new name ("r3").
We have already opened the destination group.
\code
hid_t source_raster_id;
hid_t destination_group_id;
hid_t raster_id;
bstring name = bfromcstr("r3");
raster_id = GS_DatasetCopy(source_raster_id, destination_group_id, name);
bdestroy(name);
if(raster_id < 0) {
  printf("Failed to copy dataset.\n");
}
\endcode
*/


hid_t GS_DatasetCopy(hid_t source, hid_t destination_group_id, 
                     const_bstring destination_dataset_name)
{
  hid_t file_id;
  bstring source_name;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");


/* -------------------------------------------------------------------- */
/* See if the source is a dataset                                       */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsDatasetByID(source)){
    bassignformat(error_string,"GS_DatasetCopy: ID is not a Dataset.");
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* See if the destination is a Group                                    */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsGroupByID(destination_group_id)){
    bassignformat(error_string,"GS_DatasetCopy: ID is not a Group.");
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* See if the destination name is already in use                        */
/* -------------------------------------------------------------------- */
  if(GS_CheckInternalName(destination_group_id, destination_dataset_name)){
    bassignformat(error_string,"GS_DatasetCopy: destination name already exists.");
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Get the file_id of the source                                        */
/* -------------------------------------------------------------------- */
  file_id = GS_GetValidFileID(source);
  if(file_id < 0 ) {
    bassignformat(error_string,"GS_DatasetCopy: Could not get source file-id.");
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Get the name of the source                                           */
/* -------------------------------------------------------------------- */
  source_name = GS_GetIDName(source);
  if(bstrlen(source_name)==0){
    bassignformat(error_string,"GS_DatasetCopy: Could not get source-name.");
    bdestroy(source_name);
    GS_FileClose(file_id);
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Do the copy                                                          */
/* -------------------------------------------------------------------- */
  if( H5Ocopy(file_id, bdata(source_name), destination_group_id, 
              bdata(destination_dataset_name), 
              H5P_DEFAULT, H5P_DEFAULT ) < 0 ) {
    bassignformat(error_string,"GS_DatasetCopy: Copy failed.");
    bdestroy(source_name);
    GS_FileClose(file_id);
    return ERROR;
  }// endif
  bdestroy(source_name);
  
/* -------------------------------------------------------------------- */
/* Close the source file-id                                             */
/* -------------------------------------------------------------------- */
  if(!GS_FileClose(file_id)){
    bassignformat(error_string,"GS_DatasetCopy: error closing source-file.");
    return ERROR;
  }// endif


/* -------------------------------------------------------------------- */
/* Open the copied Dataset and return handle to it                      */
/* -------------------------------------------------------------------- */
  return GS_DatasetOpen(destination_group_id, destination_dataset_name);

}
