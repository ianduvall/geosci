/* GS_RasterCreateMetadata.c

   by Leland Pierce, June 10, 2016. 
   copied from ImageCreateMetadata, and edited

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


/** \brief GS_RasterCreateMetadata fills the standard raster metadata attributes
       with the values that are passed in.


\see ImageAdd()

\param[in]  image_id
                   The ID of the image that contains this raster. 
\param[in]  raster_name
                   A character string giving the name of the existing raster in the image.
\param[in]  raster_type
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
\param[in] descriptor
                   A character string describing the image contents.


\returns \c TRUE on success, \c FALSE on failure.

\par Example:

After creating a new raster named "r3" in an image named "datatake 34" 
in a file named "av1.h5",
we must set the metadata for the raster before the raster object can be used.
The descriptor is "SIRC-SLC data from SRL-1 over Greenland".
The raster datatype is complex-integer, each 32 bits, 
with 1024 pixels along the x-direction, and 2048 lines along the y-dimension.
 

\code
hid_t file_id;
hid_t image_id;
bstring filename=bfromcstr("av1.h5");
bstring access=bfromcstr("r+");
file_id = GS_FileOpen(filename,access);
bdestroy(filename);
bdestroy(access);

if(file_id > 0) {
   bstring image_name =bfromcstr("datatake 34");
   bstring image_descriptor = bfromcstr("sirc data");
   image_id = GS_ImageCreate(file_id,image_name,image_descriptor);
   bdestroy(image_name);
   bdestroy(image_descriptor);
   
   if(image_id>0){
      ... create a new dataset that represents the Raster...

      bstring name = bfromcstr("r3");
      bstring descriptor = bfromcstr("SIRC-SLC data from SRL-1 over Greenland");
      if(GS_RasterCreateMetadata(image_id,raster_name,GS_DATATYPE_CI32,
                                 1024,2048,descriptor){
         printf("success\n");
      }
      bdestroy(name);
      bdestroy(descriptor);
   }
   GS_ImageClose(image_id);
}
GS_FileClose(file_id);
\endcode

\par Details:
\parblock
The raster metadata is attached to the Dataset that holds the raster data.
The raster metadata items are: \n
\c dataset_type (= \c "Raster"),\n
\c pixel_size_units (default is \c "meter"), \n
\c pixel_size_x,\n
\c pixel_size_y,\n
\c type, \n
\c npixels, \n
\c nlines, \n
\c descriptor,\n
\c creation_datetime,\n 
\c last_update_datetime,\n 
\c history, \n
 and \c writeable. 

Note that until the raster metadata is set up correctly the raster is not 
recognized as a valid GeoSci Raster object, and so most functions in this library
will not operate on it successfully. Because of this, 
the raster creation functions,
such as \ref GS_RasterCreate,  call this function as 
part of the creation process.
\endparblock

*/

//#define DEBUG

/**************************************************************************/
int GS_RasterCreateMetadata(hid_t image_id, const_bstring raster_name, 
                            int raster_type, int npixels, int nlines,
                            const_bstring descriptor)
{
  hid_t raster_id;
  hid_t ifile_id;
  int i, n;


  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("GS_RasterCreateMetadata: start\n");
#endif

/* -------------------------------------------------------------------- */
/* See if the image-Id is a valid image                                 */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsImageByID(image_id)){
    bassigncstr(error_string,"GS_RasterCreateMetadata: Invalid image ID.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_RasterCreateMetadata: 1\n");
#endif


/* -------------------------------------------------------------------- */
/* Make sure can open the raster to store the raster metadata in        */
/* must use dataset open because the metadata is not there yet.         */
/* -------------------------------------------------------------------- */
  raster_id = GS_DatasetOpen(image_id, raster_name);
  if(raster_id < 0){
    bassignformat(error_string,"GS_RasterCreateMetadata: Can't open the raster %s.",
            bdata(raster_name));
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_RasterCreateMetadata: 2\n");
#endif

/* -------------------------------------------------------------------- */
/*  Store each Raster metadata item                                     */
/* -------------------------------------------------------------------- */

  // write the dataset type:
  if(!GS_DatasetSetType(raster_id,GS_OBJECT_TYPE_RASTER)){
    bassignformat(error_string,"GS_RasterCreate: Can't set the dataset type for raster:%s.",
                  bdata(raster_name));
    GS_DatasetClose(raster_id);
    return ERROR;
  }// endif
 
#ifdef DEBUG
  printf("GS_RasterCreateMetadata: 3\n");
#endif
 

  bstring name = bfromcstr("");
  bstring string_value = bfromcstr("");
  bassigncstr(name,"pixel_size_units");
  bassigncstr(string_value,"meter");
  if(!GS_UpdateMetadata(raster_id,name,string_value)){
    bassignformat(error_string,"GS_RasterCreate: Error writing pixel_size_units.");
    GS_DatasetClose(raster_id);
    bdestroy(name);
    bdestroy(string_value);
    return FALSE;
  }

#ifdef DEBUG
  printf("GS_RasterCreateMetadata: 4\n");
#endif


  bassigncstr(name,"pixel_size_x");
  bassigncstr(string_value,"1.0");
  if(!GS_UpdateMetadata(raster_id,name,string_value)){
    bassignformat(error_string,"GS_RasterCreate: Error writing pixel_size_x.");
    GS_DatasetClose(raster_id);
    bdestroy(name);
    bdestroy(string_value);
    return FALSE;
  }
  bassigncstr(name,"pixel_size_y");
  bassigncstr(string_value,"1.0");
  if(!GS_UpdateMetadata(raster_id,name,string_value)){
    bassignformat(error_string,"GS_RasterCreate: Error writing pixel_size_y.");
    GS_DatasetClose(raster_id);
    bdestroy(name);
    bdestroy(string_value);
    return FALSE;
  }

#ifdef DEBUG
  printf("GS_RasterCreateMetadata: 5\n");
#endif


  bassigncstr(name,"type");
  string_value = bstrcpy(GS_GetRasterTypeAsString(raster_type));
  if(!GS_UpdateMetadata(raster_id,name,string_value)){
    bassignformat(error_string,"GS_RasterCreate: Error writing type.");
    GS_DatasetClose(raster_id);
    bdestroy(name);
    bdestroy(string_value);
    return FALSE;
  }
  bassigncstr(name,"npixels");
  bassignformat(string_value,"%d",npixels);
  if(!GS_UpdateMetadata(raster_id,name,string_value)){
    bassignformat(error_string,"GS_RasterCreate: Error writing npixels.");
    GS_DatasetClose(raster_id);
    bdestroy(name);
    bdestroy(string_value);
    return FALSE;
  }
  bassigncstr(name,"nlines");
  bassignformat(string_value,"%d",nlines);
  if(!GS_UpdateMetadata(raster_id,name,string_value)){
    bassignformat(error_string,"GS_RasterCreate: Error writing nlines.");
    GS_DatasetClose(raster_id);
    bdestroy(name);
    bdestroy(string_value);
    return FALSE;
  }
  bdestroy(string_value);

#ifdef DEBUG
  printf("GS_RasterCreateMetadata: 6\n");
#endif


  // create metadata for the raster channel:
  bassigncstr(name,"descriptor");
  bstring desc = bstrcpy(descriptor);
  if(!GS_UpdateMetadata(raster_id,name, desc)){
    bassignformat(error_string,"GS_RasterCreate: Can't create the raster metadata for raster:%s.",
                  bdata(raster_name));
    GS_DatasetClose(raster_id);
    bdestroy(name);
    bdestroy(desc);
    return ERROR;
  }//endif
  bdestroy(desc);

#ifdef DEBUG
  printf("GS_RasterCreateMetadata: 7\n");
#endif


  bstring datetime = bfromcstr("");
  GS_Time(datetime,7);
  bassigncstr(name,"last_update_datetime");
  if( !GS_UpdateMetadata( raster_id, name, datetime)){
    bassignformat(error_string,"GS_RasterCreate: Can't alter last_update_time for raster.");
    GS_DatasetClose(raster_id);
    bdestroy(datetime);
    bdestroy(name);
    return ERROR;
  }// endif
  bdestroy(datetime);

#ifdef DEBUG
  printf("GS_RasterCreateMetadata: 8\n");
#endif


  string_value=bfromcstr("");
  GS_Time(datetime,7);
  string_value = bstrcpy(datetime);
  bstring ctmp = bfromcstr(": Creation;\n");
  bconcat(string_value,ctmp);
  //bassignformat(string_value,"%s: Creation;\n",bdata(datetime));
#ifdef DEBUG
  printf("GS_RasterCreateMetadata: 8.5: string_value=|%s|\n",bdata(string_value));
#endif

  bassigncstr(name,"history");
  if(!GS_UpdateMetadata(raster_id,name,string_value)){
    bassignformat(error_string,"GS_RasterCreateMetadata: Error writing history.");
    bdestroy(string_value);
    bdestroy(name);
    bdestroy(ctmp);
    GS_DatasetClose(raster_id);
    return FALSE;
  }
  bdestroy(datetime);
  bdestroy(ctmp);

#ifdef DEBUG
  printf("GS_RasterCreateMetadata: 9\n");
#endif


  bassigncstr(name,"writeable");
  bassigncstr(string_value,"TRUE");
  if(!GS_UpdateMetadata(raster_id,name,string_value)){
    bassignformat(error_string,"GS_RasterCreateMetadata: Error writing writeable.");
    GS_RasterClose(raster_id);
    bdestroy(name);
    bdestroy(string_value);
    return FALSE;
  }


  bdestroy(name);
  bdestroy(string_value);

  GS_DatasetClose(raster_id);



#ifdef DEBUG
  printf("GS_RasterCreateMetadata: 10\n");
#endif


/* -------------------------------------------------------------------- */
/*      Flush any pending updates                                       */
/* -------------------------------------------------------------------- */
  if(!GS_FileFlush(image_id)){
    bassignformat(error_string,"GS_RasterCreateMetadata: Can't flush file.");
    return FALSE;
  }// endif


#ifdef DEBUG
  printf("GS_RasterCreateMetadata: 11\n");
#endif


/* -------------------------------------------------------------------- */
/* Return success                                                       */
/* -------------------------------------------------------------------- */
  return TRUE;
}
