/* GS_DatasetDelete.c

   by Leland Pierce, Jan 21, 2014

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


/** \brief GS_DatasetDelete deletes a a dataset in a GeoSci datafile

\see GS_DatasetOpen(), GS_DatasetClose()

\param[in] id
                   A handle for the file.
\param[in] dataset_name
                   The name of the dataset in the file.
                   Should start with a "/".

\returns
\c TRUE on sucess, \c FALSE on failure.

\par Example
Delete an image raster dataset named "/Image1/r1":
\code
hid_t file_id;
bstring name = bfromcstr("/Image1/r1");
if(!GS_DatasetDelete(file_id, name)) {
  printf("Failed to delete dataset.\n");
}
bdestroy(name);
\endcode
*/


int GS_DatasetDelete(hid_t file_id, const_bstring dataset_name)
{
  int ret;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Determine if its a writeable dataset                                 */
/* -------------------------------------------------------------------- */
  if(!GS_DatasetIsWriteable(file_id,dataset_name)){
    bassigncstr(error_string,"GS_DatasetDelete: not a writeable dataset.");
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Delete it, return status                                             */
/* -------------------------------------------------------------------- */
  if(H5Ldelete( file_id, bdata(dataset_name), H5P_DEFAULT) < 0 ){
    return FALSE;
  } else {
    return TRUE;
  }// endif

}
