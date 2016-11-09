/* GS_ObjectIsGroup.c

   by Leland Pierce, Nov 19, 2013
  
   July 8, 2014:
   modified for multi-level library
   modified to use bstrings

   Aug 2, 2014
   Modified for doxygen and GS_

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"




/** \brief GS_ObjectIsGroup determines if a named object is a group

\see GS_FileOpen(), GS_FileCreate()

\param[in] id
                   object handle of a file or another group
                   in the selected GeoSci datafile.
\param[in] object_name
                   Name of object to query. 
                   This name is relative to the given object.

\returns
TRUE is returned if the object is a group, FALSE otherwise.

\par Example:
Query about an image named "Channel_1"  in a file named "test123.h5".
\code
hid_t    file_id;
bstring filename = bfromcstr("test123.h5");
bstring access = bfromcstr("r+");
file_id = GS_FileOpen(filename,access);
if(file_id < 0) {
   printf("Could not open file.\n");
   bdestroy(filename);
   bdestroy(access);
   return;
}
bstring channel = bfromcstr("Channel_1");
if(GS_ObjectIsGroup(file_id,channel)){
   ... open the object as a group and use it ....
} else {
   printf("Object is not a group.\n");
} // endif
bdestroy(channel);
\endcode



*/




int GS_ObjectIsGroup(hid_t id, const_bstring name)
{
  H5O_info_t objinfo;
  hid_t ifile_id;
  char *name2;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");


/* -------------------------------------------------------------------- */
/* Special case of "/"                                                  */
/* -------------------------------------------------------------------- */
  bstring root=bfromcstr("/");
  if(EQUAL(name,root)) {
    bdestroy(root);
    return TRUE;
  }
  bdestroy(root);

/* -------------------------------------------------------------------- */
/* See if the id is valid                                               */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID(id)){
    bassigncstr(error_string,"GS_ObjectIsGroup: Invalid ID.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Does the specified name exist?                                       */
/* -------------------------------------------------------------------- */
  name2 = (char *)GMalloc(sizeof(char)*(bstrlen(name)+10) );
  if(!name2){
    bassigncstr(error_string,"GS_ObjectIsGroup: GMalloc error.");
    return FALSE;
  }
  strcpy(name2,bdata(name));
  if(H5Lexists( id, name2, H5P_DEFAULT ) != TRUE){
    bassignformat(error_string,"GS_ObjectIsGroup: name: %s does not exist.",
                bdata(name));
    GFree(name2);
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Get the HDF5 group type for the name                                 */
/* -------------------------------------------------------------------- */
  if(H5Oget_info_by_name( id, name2, &objinfo, H5P_DEFAULT) <0){
    bassigncstr(error_string,"GS_ObjectIsGroup: Failed to get group info.");
    GFree(name2);
    return FALSE;
  }// endif
  GFree(name2);


/* -------------------------------------------------------------------- */
/* See if the object-type is GROUP or not                               */
/* -------------------------------------------------------------------- */
  if( objinfo.type != H5O_TYPE_GROUP) {
    return FALSE;
  }// endif

  return TRUE;
}




