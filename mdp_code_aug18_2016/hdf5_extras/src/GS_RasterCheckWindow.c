/* GS_RasterCheckWindow.c

   by Leland Pierce, Nov 17, 2013

   modified June 19,2016, LEP

*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


/** \brief GS_RasterCheckWindow checks whether a requested window selection
is within the bounds of an image.  It will also default a window specification to be the whole
image if XSize and/or YSize are zero.

\see GS_RasterCheckWindowByID()

\param[in] image_id
     Image handle that has the raster in it.
\param[in] raster_name   
     Name of raster to check.
\param[in,out] window   
Vector containing desired window specification:\n
window[0] = Xoffset\n
window[1] = Yoffset\n
window[2] = Xsize\n
window[3] = Ysize\n
\n
If window[2] is 0, window[2] will be set to
image X size - window[0].
\n
If window[3] is 0, window[3] will be set to
image Y size - window[1].
\returns
 TRUE is returned on success, FALSE on failure.
\par Example
In this example a GeoSci raster is opened and a user specified
window is checked for validity in one of its images.  
\code
              ...
hid_t      image_id;
int        window[4];
              ...
window[0]=0;
window[1]=0;
window[2]=512;
window[3]=1024;
if(!GS_RasterCheckWindow( image_id, bsstatic("r1"), window)) {
     printf("GS_RasterCheckWindow failure.\n");
}
             ...
\endcode
*/

/*****************************************************************************/
int GS_RasterCheckWindow( hid_t image_id, const_bstring raster_name, int *window)
{
  hid_t raster_id;
  int npixels, nlines;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");


/* -------------------------------------------------------------------- */
/* Check valid image_id                                                 */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsImageByID(image_id)){
    bassigncstr(error_string,"GS_RasterCheckWindow: Invalid image_id.");
    return FALSE;
  }// endif

 
/* -------------------------------------------------------------------- */
/*	Open the raster.	                      			*/
/* -------------------------------------------------------------------- */
  raster_id = GS_RasterOpen(image_id, raster_name);
  if(raster_id < 0){
    bassignformat(error_string,"GS_RasterCheckWindow: Can't open the raster: %s.",raster_name);
    return FALSE;
  }// endif


/* -------------------------------------------------------------------- */
/*	Get Raster size. 	                      			*/
/* -------------------------------------------------------------------- */
  if(!GS_RasterSizeInfo (raster_id, &npixels, &nlines )){
    GS_RasterClose(raster_id);
    bassignformat(error_string,"GS_RasterCheckWindow: Can't get raster size info: %s.",raster_name);
    return FALSE;
  }// endif
  GS_RasterClose(raster_id);


/* -------------------------------------------------------------------- */
/*	Assign window sizes, if window[2] == 0 and window[3] == 0.	*/
/* -------------------------------------------------------------------- */
  if( window[2] == 0 ) window[2] = npixels - window[0];
  if( window[3] == 0 ) window[3] = nlines  - window[1];

/*----------------------------------------------------------------------*/
/*	Check that the window is legal.					*/
/*----------------------------------------------------------------------*/
  if( window[0] < 0 || window[0] > npixels ) {
    bassignformat(error_string,"GS_RasterCheckWindow: Xoffset is out-of-bounds: %d",window[0]);
    return FALSE;
  }// endif

  if( window[1] < 0 || window[1] > nlines ) {
    bassignformat(error_string,"GS_RasterCheckWindow: Yoffset is out-of-bounds: %d",window[1]);
    return FALSE;
  } // endif

  if( window[2] < 0 || window[2]+window[0] > npixels ) {
    bassignformat(error_string,"CheckWindow: Xsize causes the window to be out-of-bounds: %d",window[2]);
    return FALSE;
  }// endif

  if( window[3] < 0 || window[3]+window[1] > nlines ) { 
    bassignformat(error_string,"CheckWindow: Ysize causes the window to be out-of-bounds: %d",window[3]);
    return FALSE;
  }// endif


  return TRUE;
	
}




/** \brief CheckWindowInside checks if one Window is contained by another.
\see GS_RasterCheckWindow()
\param[in] window   
 Vector containing desired window specification.\n
 window[0] = Xoffset\n
 window[1] = Yoffset\n
 window[2] = Xsize\n
 window[3] = Ysize\n
\n
 If window[2] is 0, window[2] will be set to
 image X size - window[0].
\n
 If window[3] is 0, window[3] will be set to
 image Y size - window[1].
\param[in] window_enclosing  
Vector containing enclosing window specification.
\returns
TRUE is returned is window is enclosed by window_enclosing,
False otherwise.
Note that "enclosing" includes the case where any border are
the same.
\par Example
In this example a  user specified
window is checked for validity within some other window.
\code
		...
int dbiw[4];
int dbiw_enclosing[4];
    	...
// Determine user requested window.
    	...
if( GS_CheckWindowInside( dbiw, dbiw_enclosing )) {
   printf("The window is enclosed.\n");
}
		...
\endcode
*/
/*****************************************************************/
int GS_CheckWindowInside( int *window_in, const int *window_enclosing )
{

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

  if(window_in[0] < window_enclosing[0] ||
     window_in[0] > window_enclosing[0]+window_enclosing[2]){
    bassignformat(error_string,"CheckWindowInside: Xoffset is out-of-bounds: %d",window_in[0]);
    return FALSE;
  }

  if(window_in[1] < window_enclosing[1] ||
     window_in[1] > window_enclosing[1]+window_enclosing[3]){
    bassignformat(error_string,"CheckWindowInside: Yoffset is out-of-bounds: %d",window_in[1]);
    return FALSE;
  }

  if(window_in[0]+window_in[2] < window_enclosing[0] ||
     window_in[0]+window_in[2] > window_enclosing[0]+window_enclosing[2]){
    bassignformat(error_string,"CheckWindowInside: Xsize causes the window to be out-of-bounds: %d",window_in[2]);
    return FALSE;
  }

  if(window_in[1]+window_in[3] < window_enclosing[1] ||
     window_in[1]+window_in[3] > window_enclosing[1]+window_enclosing[3]){
    bassignformat(error_string,"CheckWindowInside: Ysize causes the window to be out-of-bounds: %d",window_in[3]);
    return FALSE;
  }

  return TRUE;
}
