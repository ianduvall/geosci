/* GS_ObjectIsRasterbyID.c

      by Leland Pierce, Feb 13, 2014.
  

   July 10, 2014:
   modified for multi-level library
   modified to use bstrings
*/

#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


/** \brief GS_ObjectIsRasterByID determines if the object-id refers to a raster image

\anchor GS_ObjectIsRasterByID
\addindex GS_ObjectIsRasterByID


\see GS_ObjectIsRaster(), IFileOpen(), GS_FileClose()

\param[in] id
       The handle of the open raster.

\returns TRUE on success, FALSE on failure.

\par Example:
Open a raster image and query if it's valid raster image
\code
hid_t file_id;
hid_t raster1;
bstring name = bfromcstr("/somename/r1");
raster1 = GS_RasterOpen(file_id,name);
if(raster1 < 0) {
   printf("GS_RasterOpen failed on /somename/r1\n");
}
bdestroy(name);
if(!GS_ObjectIsRasterByID(raster1)){
  printf("Object is not a raster image.\n");
}
\endcode
*/



/*******************************************************************************/
int GS_ObjectIsRasterByID( hid_t dataset_id )
{
  hid_t ifile_id;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Is it a dataset?                                                     */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsDatasetByID(dataset_id)){
    bassigncstr(error_string,"GS_ObjectIsRasterByID: invalid object id).");
    return FALSE;
  }// endif


/* -------------------------------------------------------------------- */
/* Get the dataset_type metadata                                        */
/* -------------------------------------------------------------------- */
  if(GS_DatasetGetType(dataset_id) != GS_OBJECT_TYPE_RASTER){
    return FALSE;
  }// endif


  return TRUE;
}
