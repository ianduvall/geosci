/* GS_ObjectIsDataset.c

   by Leland Pierce, Nov 19, 2013

  
   July 8, 2014:
   modified for multi-level library
   modified to use bstrings

   Aug 2, 2014
   modified for doxygen, GS_

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"




/** \brief GS_ObjectIsDatasetByID determines if an object-id refers to a dataset

\see GS_FileOpen(), GS_FileCreate()

\param[in] id
                   object handle in selected GeoSci file.

\returns
\c TRUE is returned if the object is a dataset, \c FALSE otherwise.

\par Example:
Query about a raster that was already opened:
\code
hid_t    raster_id;
if(GS_ObjectIsDatasetByID(raster_id)){
   ... the object is a dataset: use it...
} else {
   printf("Object is not a dataset.\n");
} // endif
\endcode
*/






/************************************************************************/
int GS_ObjectIsDatasetByID(hid_t id)
{
  H5O_info_t objinfo;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");


/* -------------------------------------------------------------------- */
/* Check for valid id                                                   */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID( id )){
    bassigncstr(error_string,"GS_ObjectIsDatasetByID: invalid handle\n");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Get object HDF5 type                                                 */
/* -------------------------------------------------------------------- */
  if(H5Oget_info( id, &objinfo) <0){
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Check if object type is DATASET or not                               */
/* -------------------------------------------------------------------- */
  if( objinfo.type != H5O_TYPE_DATASET) return FALSE;

  return TRUE;

}
