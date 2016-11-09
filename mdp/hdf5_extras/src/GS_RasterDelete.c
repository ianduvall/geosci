/* RasterDelete.c

   by Leland Pierce, April 11, 2014

   modified June 9, 2016, LEP  

*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"

/** \brief GS_RasterDelete deletes an existing raster.
\see GS_ImageOpen, GS_RasterOpen, GS_RasterClose
\param[in] image_id
            Handle of containing image.
\param[in] raster_name  
            Name of raster to delete.
\returns
\c TRUE is returned on success, \c FALSE on failure.
\par Example
Delete a raster named "r3" in image named "/Channel_1"  
in a file named "test123.h5".
\code
           ...
hid_t    file_id;
bstring name = bfromcstr("test123.h5");
bstring access = bfromcstr("r+");
file_id = GS_FileOpen(name,access);
bdestroy(name);
bdstroy(access);
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

bstring raster_name = bfromcstr("r3");
if(!GS_RasterDelete(image_id,raster_name)){
   printf("raster deletion failed.\n");
   bdestroy(raster_name);
   return;
}
bdestroy(raster_name);
          ...
\endcode
*/


//#define DEBUG

//************************************************************************8
int GS_RasterDelete( hid_t image_id, const_bstring raster_name)
{

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* See if the file-Id/image is valid                                    */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsRaster(image_id,raster_name)){
    bassignformat(error_string,"GS_RasterDelete: Invalid raster name: %s",
                  bdata(raster_name));
    return FALSE;    
  }// endif

/* -------------------------------------------------------------------- */
/* Delete the raster dataset                                            */
/* -------------------------------------------------------------------- */
  char *cname;
  cname = (char *)GMalloc(sizeof(char)*bstrlen(raster_name));
  if(!cname){
    bassigncstr(error_string,"GS_RasterDelete: GMalloc error");
    return FALSE;    
  }
  strcpy(cname,bdata(raster_name));
  if(H5Ldelete( image_id, cname, H5P_DEFAULT)<0){
    GFree(cname);
    return FALSE;
  }//endif
  GFree(cname);

  return TRUE;

}
