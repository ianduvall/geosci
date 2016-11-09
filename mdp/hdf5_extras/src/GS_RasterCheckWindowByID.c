/* GS_RasterCheckWindowByID.c

   by Leland Pierce, april 12, 2014
 
   modified June 19, 2016, LEP

*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"

/** \brief GS_RasterCheckWindowByID checks whether a requested window selection
is within the bounds of a raster.  It will also default a window specification to be the whole
raster if XSize and/or YSize are zero.

\see GS_RasterCheckWindow
\param[in] raster_id
  Handle of raster to check.
\param[in,out] window  
Vector containing desired window specification:\n
window[0] = Xoffset\n
window[1] = Yoffset\n
window[2] = Xsize\n
window[3] = Ysize\n
\n
If window[2] is 0, on return window[2] will be set to
image X size - window[0].
\n
If window[3] is 0, on return window[3] will be set to
image Y size - window[1].
\returns
TRUE is returned on success, FALSE on failure.
\par Example
In this example a GeoSci file and raster is opened and a
user specified window is checked for validity in one of its rasters.  
\code
              ...
hid_t      file_id, image_id, raster_id;
int        window[4];
              ...
bstring file = bfromcstr("blah.h5");
file_id = GS_FileOpen( file, bsstatic("r+") );
if(!file_id) {
   printf("GS_FileOpen failed.\n");
   exit(-1);
}

image_id = GS_ImageOpen( file_id, "/image1" );
if(!image_id) {
   printf("GS_ImageOpen failed.\n");
   exit(-1);
}

raster_id = GS_RasterOpen( file_id, "r5" );
if(!raster_id) {
   printf("GS_RasterOpen failed.\n");
   exit(-1);
}

window[0]=0;
window[1]=0;
window[2]=512;
window[3]=1024;
if(!GS_RasterCheckWindowByID( raster_id, window)) {
     printf("GS_RasterCheckWindowByID failure.\n");
}
             ...
\endcode

*/


//#define DEBUG

/*************************************************************/
int GS_RasterCheckWindowByID( hid_t raster_id,  int *window)
{
  int npixels, nlines, nchannels;
  hid_t raster_space;
  int ndims;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("GS_RasterCheckWindowByID: start\n");
#endif

/* -------------------------------------------------------------------- */
/* Make sure raster_id is valid, get dimensions                         */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsRasterByID(raster_id)){
    bassigncstr(error_string,"GS_RasterCheckWindowByID: invalid raster id.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_RasterCheckWindowByID: 1\n");
#endif

/* -------------------------------------------------------------------- */
/* Get dimensions                                                       */
/* -------------------------------------------------------------------- */
  if(!GS_RasterSizeInfo (raster_id, &npixels, &nlines )){
    bassigncstr(error_string,"GS_RasterCheckWindowByID: Can't get raster size info.");
    return FALSE;
  }// endif 
  
#ifdef DEBUG
 printf("GS_RasterCheckWindowByID: 2, raster dims: %d X %d\n",npixels, nlines);
#endif

/* -------------------------------------------------------------------- */
/*	Assign window sizes, if window[2] == 0 and window[3] == 0.	*/
/* -------------------------------------------------------------------- */
  if( window[2] == 0 ) window[2] = npixels - window[0];
  if( window[3] == 0 ) window[3] = nlines  - window[1];

/*----------------------------------------------------------------------*/
/*	Check that the window is legal.					*/
/*----------------------------------------------------------------------*/
  if( window[0] < 0 || window[0] > npixels ) {
    bassignformat(error_string,"CheckWindowByID: Xoffset is out-of-bounds: %d",window[0]);
    return FALSE;
  }// endif

  if( window[1] < 0 || window[1] > nlines ) {
    bassignformat(error_string,"CheckWindowByID: Yoffset is out-of-bounds: %d",window[1]);
    return FALSE;
  } // endif

  if( window[2] < 0 || window[2]+window[0] > npixels ) {
    bassignformat(error_string,"CheckWindowByID: Xsize causes the window to be out-of-bounds: %d",window[2]);
    return FALSE;
  }// endif

  if( window[3] < 0 || window[3]+window[1] > nlines ) {
     bassignformat(error_string,"CheckWindowByID: Ysize causes the window to be out-of-bounds: %d",window[3]);
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Return success                                                       */
/* -------------------------------------------------------------------- */
  return TRUE;
	
}

