/* GS_RasterOpen.c

   by Leland Pierce, Nov 16, 2013

   modified JUne 9, 2016, LEP

*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"

/** \brief GS_RasterOpen opens existing raster dataset.
\see GS_FileOpen, GS_RasterCreate, GS_RasterClose
\param[in] image_id
            Image handle in a GEOSCI file
\param[in] raster_name  
            Name of raster dataset to open.
\returns
A handle to the raster dataset is returned on success, while a value
less than 0 is returned on failure.
\par Example
Open an image named "Channel_1"  in a file named "test123.h5",
and then open the raster, which is named "r1".
\code
           ...
hid_t    file_id, image_id, raster_id;
bstring name = bfromcstr("test123.h5");
bstring access = bfromcstr("r+");
file_id = GS_FileOpen(name, access);
bdestroy(access);
bdestroy(name);
if(file_id < 0) {
   printf("Could not open file.\n");
   return;
}

bstring image_name = bfromcstr("/Channel_1");
image_id = GS_ImageOpen(file_id,image_name);
bdestroy(image_name);
if(image_id < 0) {
   printf("Could not open image.\n");
   return;
}

bstring raster_name = bfromcstr("r1");
raster_id = GS_RasterOpen(image_id,raster_name);
bdestroy(raster_name);
if(raster_id < 0) {
   printf("Could not open raster.\n");
   return;
}
          ...
\endcode
*/



//#define DEBUG

hid_t	GS_RasterOpen( hid_t image_id, const_bstring raster_name)
{
  hid_t raster_id;
  int dataset_type;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");


/* -------------------------------------------------------------------- */
/* See if the image-Id is valid                                         */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsImageByID(image_id)){
    bassigncstr(error_string,"GS_RasterOpen: invalid image id.");
#ifdef DEBUG
    printf("GS_RasterOpen: invalid image id.\n");
#endif
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Is it a raster?                                                      */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsRaster(image_id,raster_name)){
    bassignformat(error_string,"GS_RasterOpen: Object is not an raster: %s.",bdata(raster_name));
#ifdef DEBUG
    printf("GS_RasterOpen: Object is not an raster: %s.\n",bdata(raster_name));
#endif
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Open the raster                                                      */
/* -------------------------------------------------------------------- */
  raster_id = GS_DatasetOpen(image_id, raster_name);
  if(raster_id < 0){
    bassignformat(error_string,"GS_RasterOpen: Can't open the dataset: %s.",bdata(raster_name));
#ifdef DEBUG
    printf("GS_RasterOpen: Can't open the dataset: %s.\n",bdata(raster_name));
#endif
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Get the dataset type                                                 */
/* -------------------------------------------------------------------- */
  dataset_type = GS_DatasetGetType(raster_id);
  if(!dataset_type){
    bassignformat(error_string,"GS_RasterOpen: Can't get the dataset type: %s.",bdata(raster_name));
#ifdef DEBUG
    printf("GS_RasterOpen: Can't get the dataset type: %s.\n",bdata(raster_name));
#endif
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Is it a raster dataset?                                              */
/* -------------------------------------------------------------------- */
  if(dataset_type == GS_OBJECT_TYPE_RASTER){
#ifdef DEBUG
    printf("GS_RasterOpen: Successful return\n");
#endif
    return raster_id;

  } else {
    GS_DatasetClose(raster_id);
    bassignformat(error_string,"GS_RasterOpen: Dataset is not a raster: %s.",bdata(raster_name));
#ifdef DEBUG
    printf("GS_RasterOpen: Dataset is not a raster: %s.\n",bdata(raster_name));
    printf("RasterOpen: dataset_type=%d\n",dataset_type);
#endif
    return ERROR;
  }// endif

}
