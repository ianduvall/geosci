/* GS_GroupRename.c

   by Leland Pierce, Aug 12, 2014

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"




/** \brief GS_GroupRename renames a group in a GeoSci datafile

\param[in] id
                   A handle for the file, or other container of the group.
\param[in] oldname
                   Current name of group to rename.
\param[in] newname
                   Desired new name of the group.

\return
\c TRUE on sucess, \c FALSE on failure.

\par Example
Rename an image from "/SIRC-1" to "/SIRC-1a".
\code
hid_t file_id;
bstring oldname = bfromcstr("/SIRC-1");
bstring newname = bfromcstr("/SIRC-1a");
if(!GS_GS_GroupRename(file_id, oldname, newname) ) {
  printf("Failed to rename the group.\n");
}
bdestroy(oldname);
bdestroy(newname);
\endcode
*/

int GS_GroupRename(hid_t id, const_bstring oldname, const_bstring newname)
{
  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Check that oldname is valid                                          */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsGroup(id, oldname )){
    bassignformat(error_string,"GS_GroupRename: Name: %s is not a group.",
                  bdata(oldname));
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Create new name                                                      */
/* -------------------------------------------------------------------- */
  if(H5Lcreate_hard(id, bdata(oldname), id, bdata(newname), 
                     H5P_DEFAULT, H5P_DEFAULT)<0){
    bassignformat(error_string,"GS_GroupRename: Could not create new name.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Delete old name                                                      */
/* -------------------------------------------------------------------- */
  if(H5Ldelete(id, bdata(oldname), H5P_DEFAULT)<0){
    bassignformat(error_string,"GS_GroupRename: Could not delete old name.");
    return FALSE;
  }// endif

  return TRUE;

}
