/* GS_RasterClose.c

   by Leland Pierce, Nov 22, 2013

   modified June 9, 2016, LEP

*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"

/** \brief GS_RasterClose closes an open raster dataset
\see GS_RasterOpen, GS_FileOpen, GS_RasterCreate
\param[in] raster_id
            Raster handle in a GeoSci file.
\returns
\c TRUE is returned on success, \c FALSE otherwise.
\par Example
Close a raster whose handle was obtaiend earlier.
\code
hid_t  raster_id;
if(!GS_RasterClose(raster_id)) {
   printf("Could not close raster.\n");
   return;
}
\endcode
*/


int GS_RasterClose( hid_t raster_id)
{

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Is it a raster?                                                      */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsRasterByID(raster_id)){
    bassigncstr(error_string,"RasterClose: Object is not a raster.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Close the raster                                                     */
/* -------------------------------------------------------------------- */
  if(!GS_DatasetClose(raster_id)){
    bassigncstr(error_string,"RasterClose: Can't close the raster.");
    return FALSE;
  }// endif

  return TRUE;

}
