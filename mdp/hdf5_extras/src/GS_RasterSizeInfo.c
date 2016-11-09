/* GS_RasterSizeInfo.c

   by Leland Pierce, June 19, 2016
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"

/** \brief GS_RasterSizeInfo returns the number of pixels and lines in a raster.
\see GS_RasterPixelSize()
\param[in] raster_id 
            Handle for opened raster in a Geosci  file.
\param[out]  npixels 
            Pointer to integer to be loaded with the number 
            of image pixels.
\param[out] nlines  
            Pointer to integer to be loaded with the number 
            of image lines.
\returns
On success, TRUE is returned, otherwise FALSE is returned, 
and -1 for the values of both npixels and nlines.

\par Example
Provide  the user  with  information  about a previously  opened
raster.  The program prints the number of
pixels  per line and lines per image.
\code
hid_t fileid, image_id, raster_id;
int npixels, nlines;

// open the file:
bstring file_name = bfromcstr("blah.h5");
file_id = GS_FileOpen( file_name, bsstatic("r") ); 
if(file_id < 0){
   printf("Can't open the file  %s.\n",bdata(file_name));
   return FALSE;
}// endif

// open the image
bstring image_name = bfromcstr("/Image1");
image_id = GS_ImageOpen(file_id, image_name);
if(image_id < 0){
   printf("Can't open the image %s.\n",bdata(image_name));
   return FALSE;
}// endif

//open the raster:
bstring raster_name = bfromcstr("raster4");
raster_id = GS_RasterOpen(image_id,raster_name);
if(raster_id < 0){
   printf("Can't open the raster %s.\n",bdata(raster_name));
   return FALSE;
}// endif

if(GS_RasterSizeInfo( raster_id, &npixels, &nlines )){
   printf( " Pixels: %d\n Lines: %d\n",
           npixels, nlines );
} else {
   printf("Can't get Raster size information.\n");
}
GS_RasterClose(raster_id);
GS_ImageClose(image_id);
GS_FileClose(file_id);
\endcode

*/



/************************************************************************/
int GS_RasterSizeInfo(hid_t raster_id, int *npixels, int *nlines)
{

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

  *npixels = -1;
  *nlines = -1;
/* -------------------------------------------------------------------- */
/* See if the image-Id is valid                                         */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsRasterByID(raster_id)){
    bassigncstr(error_string,"GS_RasterSizeInfo: Invalid raster ID.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/*      Grab raster-header info and return that.                        */
/* -------------------------------------------------------------------- */
  bstring name = bfromcstr("npixels");
  bstring val = bfromcstr("");
    if(!GS_GetStringAttribute(raster_id,name,val)){
    bassigncstr(error_string,"GS_RasterSizeInfo: Can't read npixels string\n");
    bdestroy(name);
    bdestroy(val);
    return FALSE;
  }// endif
    if(sscanf(bdata(val),"%d",npixels) != 1){
    bassigncstr(error_string,"GS_RasterSizeInfo: Can't decode npixels string\n");
    bdestroy(name);
    bdestroy(val);
    return FALSE;
  }// endif

  bassigncstr(name,"nlines");
  bassigncstr(val,"");
  if(!GS_GetStringAttribute(raster_id,name,val)){
    bassigncstr(error_string,"GS_RasterSizeInfo: Can't read nlines string\n");
    bdestroy(name);
    bdestroy(val);
    return FALSE;
  }// endif
  if(sscanf(bdata(val),"%d",nlines) != 1){
    bassigncstr(error_string,"GS_RasterSizeInfo: Can't decode nlines string\n");
    bdestroy(name);
    bdestroy(val);
    return FALSE;
  }// endif
  bdestroy(name);
  bdestroy(val);

          
/* -------------------------------------------------------------------- */
/*      Success                                                         */
/* -------------------------------------------------------------------- */
  return TRUE;
}

