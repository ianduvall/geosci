/* GS_DatasetOpen.c

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



#define ERROR -1


/** \brief GS_DatasetOpen opens a dataset in a GeoSci datafile

\see GS_DatasetClose()

\param[in] source
                   A handle for a file or other container that
                   has a dataset in it.
\param[in] name
                   The name of the dataset to open.

\returns
The handle of the opened dataset is returned.
If it is less than zero, the open failed.

\par Example
Open an image raster dataset:
\code
hid_t image_id, raster_id;
bstring name = bfromcstr("r1");
raster_id = GS_DatasetOpen(image_id, name);
if(raster_id < 0) {
  printf("Failed to open dataset.\n");
}
bdestroy(name);
\endcode
*/


hid_t GS_DatasetOpen(hid_t source, const_bstring name)
{
  bassigncstr(status_string,"");
  bassigncstr(error_string,"");


/* -------------------------------------------------------------------- */
/* See if this name is a dataset                                        */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsDataset( source, name )){
    bassignformat(error_string,"GS_DatasetOpen: Name: %s is not a dataset.",bdata(name));
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Open the dataset                                                     */
/* -------------------------------------------------------------------- */
  return H5Dopen(source, bdata(name), H5P_DEFAULT);
}
