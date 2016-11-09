/* GS_RasterCreate.c

   by Leland Pierce, Jan 20, 2014.

   modified June 9, 2016, LEP  

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"

/** \brief GS_RasterCreate create new image raster for a GeoSci Image
\see  GS_RasterOpen, GS_RasterClose, GS_RasterDelete
\param[in] image_id;
             A handle for an already-open image.
\param[in] raster_name
             The name of the raster channel to create.
\param[in] raster_type
             The type for the raster data. 
             One of:\n
             DATATYPE_UI1  1   A single bit\n
             \c GS_DATATYPE_UI8  2   Unsigned 8-bit integer\n
             \c GS_DATATYPE_SI8  3   Signed 8-bit integer\n
             \c GS_DATATYPE_CI8  4   Complex 8-bit integer\n
             \c GS_DATATYPE_UI16 5   Unsigned 16-bit integer\n
             \c GS_DATATYPE_SI16 6   Signed 16-bit integer\n
             \c GS_DATATYPE_CI16 7   Complex 16-bit integer\n
             \c GS_DATATYPE_UI32 8   Unsigned 32-bit integer\n
             \c GS_DATATYPE_SI32 9   Signed 32-bit integer\n
             \c GS_DATATYPE_CI32 10  Complex 32-bit integer\n
             \c GS_DATATYPE_CI64 11  Complex 64-bit integer\n
             \c GS_DATATYPE_R32  12  32-bit Real number\n
             \c GS_DATATYPE_R64  13  64-bit Real number\n
             \c GS_DATATYPE_C64  14  Complex 64-bit floating-point number\n
             \c GS_DATATYPE_C128 15  Complex 128-bit floating point number\n
             \c GS_DATATYPE_SI64 16  Signed 64-bit integer\n
\param[in] npixels The x-dimension of the raster channel.
\param[in] nlines The y-dimension of the raster channel.
\param[in] descriptor;
             A string describing the raster contents.
\returns
The handle for the created raster is returned.
This handle is less than zero if there is any error.
\par Example
Create the raster named "r7" for an image named "/image3" in 
an already-open GeoSci file. The raster should be a 32-bit-real
image, with 1024 pixels X 512 lines.
\code
hid_t file_id, image_id, raster_id;
bstring image_name = bfromcstr("/image3");
image_id = GS_ImageOpen(file_id,image_name);
bdestroy(image_name);
if(image_id < 0 ){
  printf("GS_ImageOpen failure\n");
  exit(-1);
}
bstring descriptor = bfromcstr("Howland-IR");
bstring name = bfromcstr("r7");
raster_id = GS_RasterCreate(image_id,name,GS_DATATYPE_R32,1024,512,descriptor);
bdestroy(descriptor);
bdestroy(name);
if(raster_id<0){
  printf("GS_RasterCreate failure\n");
  exit(-1);
}

.... do stuff ....

GS_RasterClose(raster_id);
GS_ImageClose(image_id);
\endcode

\par Details
This function defaults the pixel size metadata: \c pixel_size__units="meter", 
\c pixels_size_x=1.0, \c pixel_size_y=1.0. These can be reset to valid values
using the function GS_RasterPixelSize.
*/


//#define DEBUG
/*****************************************************************/
hid_t GS_RasterCreate(hid_t image_id, const_bstring const_raster_name, 
                   int raster_type, int npixels, int nlines,
                   const_bstring descriptor)
{
  hsize_t space_dims[20];
  hid_t hdf5type;
  hid_t data_id;
  hid_t space_id;
  hid_t file_id;
  int rank;

  bstring raster_name=bfromcstr("");

  bassign(raster_name,const_raster_name);
  
  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("GS_RasterCreate: 1, npixels=%d, nlines=%d\n",npixels,nlines);
#endif


/* -------------------------------------------------------------------- */
/* See if the image-Id is a valid image                                 */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsImageByID(image_id)){
    bassigncstr(error_string,"GS_RasterCreate: Invalid image ID.");
    return ERROR;
  }// endif

#ifdef DEBUG
  printf("GS_RasterCreate: 2\n");
#endif


/* -------------------------------------------------------------------- */
/* Check if can write to the file                                       */
/* -------------------------------------------------------------------- */
  if(!GS_FileIsWriteable(image_id)){
    bassigncstr(error_string,"GS_RasterCreate: File cannot be written to.");
    return ERROR;
  }// endif

#ifdef DEBUG
  printf("GS_RasterCreate: 3\n");
#endif



/* -------------------------------------------------------------------- */
/*   Invalid if either dimension is <= 0                                */
/* -------------------------------------------------------------------- */
  if(npixels <= 0 || nlines <= 0){
    bassignformat(error_string,"GS_RasterCreate: invalid dimensions: %d X %d\n",npixels, nlines);
    return ERROR;
  }// endif

#ifdef DEBUG
  printf("GS_RasterCreate: 5\n");
#endif


/* -------------------------------------------------------------------- */
/* Check validity of raster_name                                        */
/* -------------------------------------------------------------------- */
  if(!GS_CheckUniqueName(image_id,bdata(raster_name))){
    bassignformat(error_string,"GS_RasterCreate: Duplicate raster name: %s.",raster_name);
    return ERROR;
  }// endif

#ifdef DEBUG
  printf("GS_RasterCreate: 6\n");
#endif


/* -------------------------------------------------------------------- */
/* Check validity of raster_type                                        */
/* -------------------------------------------------------------------- */
  hdf5type = GS_GetHDF5Type(raster_type);
  if(hdf5type<0){
    bassignformat(error_string,"GS_RasterCreate: Invalid raster type: %d.",raster_type);
    return ERROR;
  }// endif

#ifdef DEBUG
  printf("GS_RasterCreate: 7\n");
#endif


/* -------------------------------------------------------------------- */
/* Define the raster dimensions                                         */
/* -------------------------------------------------------------------- */
  rank =2;
  space_dims[0]=nlines;
  space_dims[1]=npixels;

#ifdef DEBUG
  printf("GS_RasterCreate: 7.1: space_dims=%d, %d\n",
         (int)space_dims[0], (int)space_dims[1]);
  printf("                      npixels=%d, nlines=%d\n", npixels, nlines);
#endif

  // create the image data dimensions:
  space_id = H5Screate_simple( rank, space_dims, NULL );
  if(space_id < 0 ){
    bassignformat(error_string,"GS_RasterCreate: Can't create the dimensions for raster %s",bdata(raster_name));
    if(GS_RasterTypeIsComplex(raster_type)==TRUE){
      H5Tclose(hdf5type); // need to close complex types
    }// endif
    return ERROR;
  }// endif

#ifdef DEBUG
  printf("GS_RasterCreate: 8\n");
#endif


/* -------------------------------------------------------------------- */
/* Create the raster                                                    */
/* -------------------------------------------------------------------- */
  char *cname;
  cname = (char *)GMalloc(sizeof(char)*bstrlen(raster_name));
  if(!cname){
    bassignformat(error_string,"GS_RasterCreate: Gmalloc error on raster name: %s.",raster_name);
    return ERROR;
  }
  strcpy(cname,raster_name->data);
  data_id = H5Dcreate(image_id,cname, hdf5type, space_id, H5P_DEFAULT, 
                        H5P_DEFAULT, H5P_DEFAULT);
  //lep GFree(cname);
  if(data_id < 0){
    bassignformat(error_string,"GS_RasterCreate: Can't create the raster image data %s.",
                  bdata(raster_name));
    if(GS_RasterTypeIsComplex(raster_type)==TRUE){
      H5Tclose(hdf5type); // need to close complex types
    }// endif
    H5Sclose(space_id);
    return ERROR;
  }// endif

#ifdef DEBUG
  printf("GS_RasterCreate: 9\n");
#endif


  // close the type if needed:
  if(GS_RasterTypeIsComplex(raster_type)==TRUE){
    H5Tclose(hdf5type); // need to close complex types
  }// endif

#ifdef DEBUG
  printf("GS_RasterCreate: 10\n");
#endif


  // close the space:
  H5Sclose(space_id);

#ifdef DEBUG
  printf("GS_RasterCreate: 11\n");
#endif

/* -------------------------------------------------------------------- */
/* Create the Raster's metadata                                        */
/* -------------------------------------------------------------------- */
  if(!GS_RasterCreateMetadata(image_id,raster_name,raster_type,
                              npixels,nlines,descriptor)){
    bassignformat(error_string,"GS_RasterCreate: Can't create the raster image data %s.",
                  bdata(raster_name));
    return ERROR;
  }


#ifdef DEBUG
  printf("GS_RasterCreate: 12\n");
#endif



/* -------------------------------------------------------------------- */
/* Update the image's metadata                                          */
/* -------------------------------------------------------------------- */
  bstring history = bfromcstr("GS_RasterCreate: name:");
  bconcat(history,raster_name);
  bcatcstr(history,", type:");
  bconcat(history,GS_GetRasterTypeAsString(raster_type));
  //bassignformat(history,"GS_RasterCreate: name:%s, type:%s",bdata(raster_name),
  //        GS_GetRasterTypeAsString(raster_type));
  if(!GS_AppendHistory(image_id,history)){
    bassignformat(error_string,"GS_RasterCreate: Can't append history metadata for raster: %s.",bdata(raster_name));
    GS_RasterClose(data_id);
    bdestroy(history);
    return ERROR;
  }// endif

#ifdef DEBUG
  printf("GS_RasterCreate: 13\n");
#endif



/* -------------------------------------------------------------------- */
/* Alter fileupdate time, and file history                              */
/* -------------------------------------------------------------------- */
  file_id = GS_GetValidFileID(image_id);
  if(file_id < 0) {
    bassigncstr(error_string,"GS_RasterCreate: Can't get file_id.");
    GS_RasterClose(data_id);
    bdestroy(history);
    return ERROR;
  }// endif
  bstring image_name = GS_GetIDName(image_id);
  if(bstrlen(image_name)<=0){
    bassignformat(history,"GS_RasterCreate: Added raster %s to unknown image",bdata(raster_name));
  } else {
    bassignformat(history,"GS_RasterCreate: Added raster %s to image %s",bdata(raster_name),bdata(image_name));
  }// endif
  bdestroy(image_name);
  GS_AppendHistory(file_id,history); // ignore any errors.
  bdestroy(history);
  GS_FileClose(file_id);

#ifdef DEBUG
  printf("GS_RasterCreate: 14\n");
#endif



  return data_id;
}
