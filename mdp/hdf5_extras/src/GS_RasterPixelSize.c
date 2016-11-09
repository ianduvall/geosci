/* GS_RasterPixelSize.c

   by Leland Pierce, Nov 16, 2013

   modified June 9, 2016, LEP
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"

/** \brief  GS_RasterPixelSize reads/writes Raster Pixel Size
\see GS_RasterCreate
\param[in] raster_id      
          Handle to already-opened Raster in a GeoSci file.
\param[in] func
          Flag indicating whether to write the passed pixel size
          to the image metadata (GS_WRITE), or return the pixel size
          currently stored for the image (GS_READ).
\param[in,out] x_pixelsize      
          Pixel ground X-size in `pixel_units' units.
\param[in,out] y_pixelsize
          Pixel ground Y-size in `pixel_units' units.
\param[in,out] pixel_units      
          Pixel Size Units string.
\returns
\c TRUE is returned on success, \c FALSE otherwise.
\par Example
The following code prints out the current pixel size information, 
and then sets the pixel size to 1 meter by 1 meter.
\code
hid_t      raster_id;
double     xsize, ysize;
bstring    pixel_units=bfromcstr("");
...
if(GS_RasterPixelSize( raster_id, GS_READ, &xsize, &ysize, pixel_units )){
   printf( "Pixel Size was %g x %g %s\n", 
         xsize, ysize, bdata(pixel_units) );
}

xsize = 1.0;
ysize = 1.0;
bassigncstr(pixel_units,"meter");
if(GS_RasterPixelSize( raster_id, GS_WRITE, &xsize, &ysize, pixel_units )){
   printf( "Pixel size reset to 1 x 1 Meters.\n" );
}
bdestroy(pixel_units);
...
\endcode
\par Details
This is useful for rasters that have no projection, or whose projection
and spatial system are only loosely known.
In other cases, using the projection information and the coordinates information
in other metadata collections is preferred.
*/


/********************************************************************/
int GS_RasterPixelSize(hid_t raster_id,int func, double *x_pixelsize,
                   double *y_pixelsize, bstring pixel_units)
{
  double xpix_save, ypix_save;


  bassigncstr(status_string,"");
  bassigncstr(error_string,"");


  //----------------------------------------------------------------------
  // check that raster_id is valid
  //----------------------------------------------------------------------
  if( !GS_ObjectIsRasterByID( raster_id ) ){
    bassigncstr(error_string,"GS_RasterPixelSize: handle is not valid raster.\n");
    return FALSE;
  }// endif 

  //----------------------------------------------------------------------
  // remember inputs:
  //----------------------------------------------------------------------
  xpix_save = *x_pixelsize;
  ypix_save = *y_pixelsize;
  bstring units_save = bfromcstr("");
  bassign(units_save, pixel_units);

  //----------------------------------------------------------------------
  // error check:
  //----------------------------------------------------------------------
  if( func == GS_READ) {
  } else if (func == GS_WRITE) {
  } else {
    bassignformat(error_string,"GS_RasterPixelSize: invalid read/write flag: %d. (Must be 0 or 1)\n",func);
    bdestroy(units_save);
    return FALSE;
  } // endif 


  //----------------------------------------------------------------------
  // Read values
  //----------------------------------------------------------------------
  bstring name = bfromcstr("pixel_size_units");
  bstring val = bfromcstr("");
  if(!GS_GetStringAttribute(raster_id,name,val)){
    bassigncstr(error_string,"GS_RasterPixelSize: error reading pixel_size_units from image\n");
    bdestroy(units_save);
    bdestroy(val);
    bdestroy(name);
    return FALSE;
  }// endif
  bassign(pixel_units,val);
  
  bassigncstr(name,"pixel_size_x");
  if(!GS_GetStringAttribute(raster_id,name,val)){
    bassigncstr(error_string,"GS_RasterPixelSize: error reading pixel_size_x from image\n");
    bdestroy(units_save);
    bdestroy(val);
    bdestroy(name);
    return FALSE;
  }// endif
  if(sscanf(bdata(val),"%lf",x_pixelsize) != 1) {
    bassigncstr(error_string,"GS_RasterPixelSize: error decoding pixel_size_x from image\n");
    bdestroy(units_save);
    bdestroy(val);
    bdestroy(name);
    return FALSE;
  }// endif

  bassigncstr(name,"pixel_size_y");
  if(!GS_GetStringAttribute(raster_id,name,val)){
    bassigncstr(error_string,"GS_RasterPixelSize: error reading pixel_size_y from image\n");
    bdestroy(units_save);
    bdestroy(val);
    bdestroy(name);
    return FALSE;
  }// endif
  if(sscanf(bdata(val),"%lf",y_pixelsize) != 1) {
    bassigncstr(error_string,"GS_RasterPixelSize: error decoding pixel_size_y from image\n");
    bdestroy(units_save);
    bdestroy(val);
    bdestroy(name);
    return FALSE;
  }// endif
  bdestroy(val);

  if( func == GS_READ) return TRUE;




/* -------------------------------------------------------------------- */
/*	Get writeability for file.                			*/
/* -------------------------------------------------------------------- */
  if( GS_FileIsWriteable(raster_id) <= 0 ) {
    // file not writeable
    bassigncstr(error_string,"GS_RasterPixelSize: file not writeable\n");
    bdestroy(units_save);
    bdestroy(name);
    bdestroy(val);
    return FALSE;
  }// endif


  //----------------------------------------------------------------------
  // Write values
  // The complication here is that we try to "roll back" any changes
  // if any of the 3 writes fails. This way we preserve the previous
  // set of 3 values instead of partially messing them up.
  //----------------------------------------------------------------------
  bstring xstr=bfromcstr("");
  bstring ystr=bfromcstr("");
  bassignformat(xstr,"%lg",xpix_save);
  bassignformat(ystr,"%lg",ypix_save);

  bassigncstr(name,"pixel_size_units");
  if(!GS_UpdateMetadata(raster_id,name,units_save)){
    // put back the as-called values:
    *x_pixelsize = xpix_save;
    *y_pixelsize = ypix_save;
    
    bassigncstr(error_string,"GS_RasterPixelSize: error updating pixel_size_units for image.\n");
    bdestroy(units_save);
    bdestroy(name);
    bdestroy(val);
    bdestroy(xstr);
    bdestroy(ystr);
    return FALSE;
  }//endif

  bassigncstr(name,"pixel_size_x");
  if(!GS_UpdateMetadata(raster_id,name,xstr)){
    // try to put back old size units:
    bassigncstr(name,"pixel_size_units");
    GS_UpdateMetadata(raster_id,name,units_save);
    // put back the as-called values:
    *x_pixelsize = xpix_save;
    *y_pixelsize = ypix_save;

    bassigncstr(error_string,"GS_RasterPixelSize: error updating pixel_size_x for image.\n");
    bdestroy(units_save);
    bdestroy(name);
    bdestroy(val);
    bdestroy(xstr);
    bdestroy(ystr);
    return FALSE;
  }// endif
   
  bassigncstr(name,"pixel_size_y");
  if(!GS_UpdateMetadata(raster_id,name,ystr)){
    // try to put back old size units, etc:
    bassigncstr(name,"pixel_size_units");
    GS_UpdateMetadata(raster_id,name,units_save);
    bassignformat(xstr,"%lg",*x_pixelsize);
    bassigncstr(name,"pixel_size_x");
    GS_UpdateMetadata(raster_id,name,xstr);
    // put back the as-called values:
    *x_pixelsize = xpix_save;
    *y_pixelsize = ypix_save;

    bassigncstr(error_string,"GS_RasterPixelSize: error updating pixel_size_y for image.\n");
    bdestroy(units_save);
    bdestroy(name);
    bdestroy(val);
    bdestroy(xstr);
    bdestroy(ystr);
    return FALSE;
  }// endif
  
  // put back the as-called values:
  *x_pixelsize = xpix_save;
  *y_pixelsize = ypix_save;

/* -------------------------------------------------------------------- */
/*      Flush any pending updates                                       */
/* -------------------------------------------------------------------- */
  if(!GS_FileFlush(raster_id)){
    bassigncstr(error_string,"GS_RasterPixelSize: Can't flush file.");
    bdestroy(units_save);
    bdestroy(name);
    bdestroy(val);
    bdestroy(xstr);
    bdestroy(ystr);
    return FALSE;
  }// endif

  bdestroy(units_save);
  bdestroy(name);
  bdestroy(val);
  bdestroy(xstr);
  bdestroy(ystr);

  return TRUE;
}
