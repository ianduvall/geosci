/* GS_ImageCreate.c

   by Leland Pierce, June 9, 2016
  
*/



#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


//#define DEBUG


#define USER_BLOCK_SIZE  0
//"""Sets the user block size of a file.
//The default user block size is 0; it may be set to any power of 2 equal
//to 512 or greater (512, 1024, 2048, etc.).


#define METADATA_CACHE_SIZE  (1 * 1024* 1024)
//"""Size (in bytes) of the HDF5 metadata cache."""
// 1 MB is the default for HDF5



/** \brief GS_ImageCreate creates a new image in a GeoSci file and opens it.
\see GS_FileCreate, GS_RasterCreate
\param[in] file_id
\param[in] imagename
Name of the image to create. Must not be the same
as any other existing object in the file or the
creation will fail.
\param[in] image_descriptor
A string describing the contents of the image.
This describes all the channels, in a general sense.
\returns 
Returns an image handle on success, or a negative integer on failure.
\par Example
Create an image named "sirc-102.41-spring" 
with an image descriptor of "raw slc data".
\code
    ...
hid_t       file_id;
bstring name = bfromcstr("sirc-102.41-spring");
bstring descriptor = bfromcstr("raw slc data");

image_id = GS_ImageCreate(file_id, name, descriptor)
if(image_id < 0) {
   printf("image creation failed.\n");
}// endif
bdestroy(name);
bdestroy(descriptor);
    ...
\endcode
*/



//***********************************************************************
hid_t GS_ImageCreate(hid_t file_id, const_bstring image_name, 
               const_bstring image_descriptor)
{
  hid_t group_id=-1;
  bstring name = bstrcpy(image_name);
  bstring descriptor = bstrcpy(image_descriptor);

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("GS_ImageCreate: start\n");
#endif



/* -------------------------------------------------------------------- */
/*   0. Add "/" to front of name if needed                              */
/* -------------------------------------------------------------------- */
  if(bchar(name,0) != '/') {
    bassigncstr(name,"/");
    bconcat(name,image_name);
  }

/* -------------------------------------------------------------------- */
/*   1. Create the empty Group                                          */
/* -------------------------------------------------------------------- */
  group_id = GS_GroupCreate(file_id,name);
  if(group_id < 0){
    bassigncstr(error_string,"GS_ImageCreate: invalid file id).");
    bdestroy(name);
    bdestroy(descriptor);
    return ERROR;
  }// endif


#ifdef DEBUG
  printf("GS_ImageCreate: 1\n");
  fflush(stdout);
#endif

/* -------------------------------------------------------------------- */
/*   2. Create the image's metadata                                     */
/* -------------------------------------------------------------------- */
  if(!GS_ImageCreateMetadata(file_id,name,descriptor) ){
    bassignformat(error_string,"GS_ImageCreate: Can't create image metadata for image: %s.",bdata(name));
    bdestroy(name);
    bdestroy(descriptor);
    return ERROR;
  }// endif
   


#ifdef DEBUG
  printf("GS_ImageCreate: 2\n");
  fflush(stdout);
#endif

/* -------------------------------------------------------------------- */
/*   3. Alter fileupdate time, and file history                         */
/* -------------------------------------------------------------------- */
  bstring string_value = bfromcstr("");
  bassignformat(string_value,"GS_ImageCreate: Added image %s",bdata(name));
  GS_AppendHistory(file_id,string_value); // ignore any errors.

  bdestroy(name);
  bdestroy(descriptor);


#ifdef DEBUG
  printf("GS_ImageCreate: 3\n");
  fflush(stdout);
#endif


/* -------------------------------------------------------------------- */
/*  4. Return id for new image                                         */
/* -------------------------------------------------------------------- */
  return group_id;

}
