/* GS_DatasetSetType.c

   by Leland Pierce, Nov 16, 2013
   modified May 22, 2016, LEP

*/


#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"
#include "hdf5mine.h"

//#define DEBUG22


/** \brief GS_DatasetSetType set the type of a dataset in a GeoSci file.
\param[in] dataset_id
                  A handle for the dataset.
\param[in] typecode
An integer type code specifying the type
of dataset. One of:\n

GS_OBJECT_TYPE_IFILE           10\n
GS_OBJECT_TYPE_RASTER           9\n
GS_OBJECT_TYPE_METADATA_DATASET 12\n
GS_OBJECT_TYPE_METADATA_IFILE  13\n
\returns
TRUE on sucess, FALSE on failure.
\par Example
Set the type of an image raster dataset:
\code
hid_t raster_id;
if(!GS_DatasetSetType(raster_id, GS_OBJECT_TYPE_RASTER) ) {
  printf("Failed to set the type of the dataset.\n");
  return;
}
\endcode
*/


int GS_DatasetSetType(hid_t dataset_id, int typecode)
{
  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* See if the Id is valid                                               */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID(dataset_id)){
    bassignformat(error_string,"DatasetSetType: Invalid ID.");
    return FALSE;
  }// endif

#ifdef DEBUG22
        printf("DatasetSetType: BB1: \n");
        report_num_objects_open();
#endif


/* -------------------------------------------------------------------- */
/* Check if its a dataset id                                            */
/* -------------------------------------------------------------------- */
  if(H5Iget_type( dataset_id ) != H5I_DATASET){
    bassignformat(error_string,"DatasetSetType: Not a dataset ID.");
    return FALSE;
  }// endif

#ifdef DEBUG22
        printf("DatasetSetType: BB2: \n");
        report_num_objects_open();
#endif


/* -------------------------------------------------------------------- */
/* Check that code is valid                                             */
/* -------------------------------------------------------------------- */
  switch(typecode){
  case GS_OBJECT_TYPE_IFILE:
  case GS_OBJECT_TYPE_RASTER:
  case GS_OBJECT_TYPE_METADATA_DATASET:
  case GS_OBJECT_TYPE_METADATA_IFILE:
    break;
  default:
    bassignformat(error_string,"DatasetSetType: Not a known type: %d.",typecode);
    return FALSE;
  }// endswitch

#ifdef DEBUG22
        printf("DatasetSetType: BB3: \n");
        report_num_objects_open();
#endif


/* -------------------------------------------------------------------- */
/* Write the typecode                                                   */
/* -------------------------------------------------------------------- */
  bstring dataset_type = bfromcstr("dataset_type");
  bstring one = bfromcstr("10");
  bstring two = bfromcstr("9");
  bstring three = bfromcstr("12");
  bstring four = bfromcstr("13");

  switch(typecode){

  case GS_OBJECT_TYPE_IFILE:
    if(!GS_UpdateMetadata(dataset_id,dataset_type,one)){
      bassignformat(error_string,"DatasetSetType: Can't write dataset type.");
      bdestroy(dataset_type);
      bdestroy(one);
      bdestroy(two);
      bdestroy(three);
      bdestroy(four);
      return FALSE;
    }// endif
    break;

  case GS_OBJECT_TYPE_RASTER:
    if(!GS_UpdateMetadata(dataset_id,dataset_type,two)){
      bassignformat(error_string,"DatasetSetType: Can't write dataset type.");
      bdestroy(dataset_type);
      bdestroy(one);
      bdestroy(two);
      bdestroy(three);
      bdestroy(four);
      return FALSE;
    }// endif
    break;

  case GS_OBJECT_TYPE_METADATA_DATASET:
    if(!GS_UpdateMetadata(dataset_id,dataset_type,three)){
      bassignformat(error_string,"DatasetSetType: Can't write dataset type.");
      bdestroy(dataset_type);
      bdestroy(one);
      bdestroy(two);
      bdestroy(three);
      bdestroy(four);
      return FALSE;
    }// endif
    break;

  case GS_OBJECT_TYPE_METADATA_IFILE:
    if(!GS_UpdateMetadata(dataset_id,dataset_type,four)){
      bassignformat(error_string,"DatasetSetType: Can't write dataset type.");
      bdestroy(dataset_type);
      bdestroy(one);
      bdestroy(two);
      bdestroy(three);
      bdestroy(four);
      return FALSE;
    }// endif
    break;
  }// endcase

  bdestroy(dataset_type);
  bdestroy(one);
  bdestroy(two);
  bdestroy(three);
  bdestroy(four);

#ifdef DEBUG22
        printf("DatasetSetType: BB4: \n");
        report_num_objects_open();
#endif


  return TRUE;

}
