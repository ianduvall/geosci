/* GS_ObjectIsRaster.c

      by Leland Pierce, Feb 13, 2014.
  

   July 10, 2014:
   modified for multi-level library
   modified to use bstrings
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"

/** \brief GS_ObjectIsRaster determines if the object is a raster image

\anchor GS_ObjectIsRaster
\addindex GS_ObjectIsRaster



\see GS_ObjectIsRasterByID(), IFileOpen(), GS_FileClose()


\param[in]  id
                 The handle of the open image.
\param[in]  name
                   Name of the raster within the image.

\returns TRUE on success, FALSE on failure.

\par Example:
Open an image and query if the dataset named "r1" is a valid raster image.
\code
hid_t file_id;
hid_t image1;
bstring name = bfromcstr("/somename");
image1 = GS_ImageOpen(file_id,name);
if(image1 < 0) {
   printf("GS_ImageOpen failed on /somename\n");
}
bdestroy(name);
if(!GS_ObjectIsRaster(image1,r1)){
  printf("Object is not a raster image.\n");
}
\endcode
*/




//#define DEBUG

/*******************************************************************************/
int GS_ObjectIsRaster( hid_t id, const_bstring name )
{
  hid_t dataset_id;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("GS_ObjectIsRaster: 0\n");
#endif

/* -------------------------------------------------------------------- */
/* Is it a dataset?                                                     */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsDataset(id,name)){
    bassigncstr(error_string,"GS_ObjectIsRaster: not a dataset).");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_ObjectIsRaster: 1\n");
#endif

/* -------------------------------------------------------------------- */
/* Open the dataset                                                     */
/* -------------------------------------------------------------------- */
  dataset_id = GS_DatasetOpen(id,name);
  if(!dataset_id){
    bassigncstr(error_string,"GS_ObjectIsRaster: can't open dataset");
    return FALSE;
  }// endif


#ifdef DEBUG
  printf("GS_ObjectIsRaster: 2: type=%d, (RASTER=%d)\n",
         GS_DatasetGetType(dataset_id),GS_OBJECT_TYPE_RASTER);
#endif

/* -------------------------------------------------------------------- */
/* Get the dataset_type metadata                                        */
/* -------------------------------------------------------------------- */
  if(GS_DatasetGetType(dataset_id) != GS_OBJECT_TYPE_RASTER){
    GS_DatasetClose(dataset_id);
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_ObjectIsRaster: 3\n");
#endif


/* -------------------------------------------------------------------- */
/* Close the dataset                                                    */
/* -------------------------------------------------------------------- */
  if(!GS_DatasetClose(dataset_id)){
#ifdef DEBUG
  printf("GS_ObjectIsRaster: cant close dataset*****************\n");
#endif
    bassigncstr(error_string,"GS_ObjectIsRaster: can't close dataset");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_ObjectIsRaster: 4\n");
#endif


  return TRUE;
}
