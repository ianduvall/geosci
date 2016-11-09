/* GS_DatasetClose.c

   by Leland Pierce, Nov 16, 2013

   Aug 4, 2014:
   modify for bstrings, doxygen, GS_

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


/** \brief GS_DatasetClose closes a dataset in a GeoSci datafile

\see GS_DatasetOpen()

\param[in] id
                   A handle for the dataset. Perhaps from GS_DatasetOpen().

\returns
\c TRUE on sucess, \c FALSE on failure.

\par Example
Close an image raster dataset:
\code
hid_t raster_id;
if(!GS_DatasetClose(raster_id)) {
  printf("Failed to close dataset.\n");
}
\endcode
*/


hid_t GS_DatasetClose(hid_t id)
{

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Check if its a dataset id                                            */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsDatasetByID(id)){
    bassigncstr(error_string,"GS_DatasetClose: Not a dataset ID.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Close it                                                             */
/* -------------------------------------------------------------------- */
  if(H5Dclose(id)<0){
    return FALSE;
  } else {
    return TRUE;
  }// endif
}
