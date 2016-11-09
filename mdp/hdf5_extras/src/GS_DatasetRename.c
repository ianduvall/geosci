/* GS_DatasetRename.c

   by Leland Pierce, April 3, 2014

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




/** \brief GS_DatasetRename renames a dataset in a GeoSci datafile

\param[in] group_id
                   A handle for the container of the dataset.
                   This should be a group.
\param[in] oldname
                   Current name of dataset to rename.
\param[in] newname
                   Desired new name of the dataset.

\return
\c TRUE on sucess, \c FALSE on failure.

\par Example
Rename a dataset from "r1" to "r4".
\code
hid_t group_id;
bstring oldname = bfromcstr("r1");
bstring newname = bfromcstr("r2");
if(!GS_GS_DatasetRename(group_id, oldname, newname) ) {
  printf("Failed to rename the dataset.\n");
}
bdestroy(oldname);
bdestroy(newname);
\endcode
*/

int GS_DatasetRename(hid_t group_id, const_bstring oldname, const_bstring newname)
{
  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Check that oldname is valid                                          */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsDataset( group_id, oldname )){
    bassignformat(error_string,"GS_DatasetRename: Name: %s is not a dataset.",
                  bdata(oldname));
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Create new name                                                      */
/* -------------------------------------------------------------------- */
  if(H5Lcreate_hard(group_id, bdata(oldname), group_id, bdata(newname), 
                     H5P_DEFAULT, H5P_DEFAULT)<0){
    bassignformat(error_string,"GS_DatasetRename: Could not create new name.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Delete old name                                                      */
/* -------------------------------------------------------------------- */
  if(H5Ldelete(group_id, bdata(oldname), H5P_DEFAULT)<0){
    bassignformat(error_string,"GS_DatasetRename: Could not delete old name.");
    return FALSE;
  }// endif

  return TRUE;

}
