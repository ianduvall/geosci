/* GS_CheckInternalName.c

   by Leland Pierce, Nov 17, 2013

   Aug 3, 2014:
   modify for bstrings, doxygen, GS_

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"



/** \brief GS_CheckInternalName checks for existence of an object name in the file.


\see GS_RasterCheckWindow()

\param[in] object_id
                   Handle of an already-open GeoSci object.
\param[in] name
                   Name of the object one wishes to test.
                   Must be a relative pathname, relative to the
                   object in "object_id". If object_id is a file,
                   then this can be an absolute name.
                   For example: /groupa/group1/datasetw.

\returns 
\c TRUE if the name exists, \c FALSE if not.

\par Example:
Test if the name "Channel_3" already exists or not:
\code
hid_t       file_id;
bstring name = bfromcstr("Channel_3");
if(GS_CheckInternalName(file_id,name) ){
   printf("Name: \"Channel_3\" is already in use.\n");
}// endif
bdestroy(name);
\endcode
*/

//#define DEBUG


/**********************************************************************/
int GS_CheckInternalName(hid_t object_id, const_bstring thename){
  char *name2;
  bassigncstr(status_string,"");
  bassigncstr(error_string,"");
#ifdef DEBUG
  printf("GS_CheckInternalName: 1, thename=|%s|\n",bdata(thename));
#endif
/* -------------------------------------------------------------------- */
/* Special case of '/'                                                  */
/* -------------------------------------------------------------------- */
  bstring root = bfromcstr("/");
  if(EQUAL(thename,root)) {
    bdestroy(root);
    return TRUE;
  }// endif
  bdestroy(root);

#ifdef DEBUG
  printf("GS_CheckInternalName: 2\n");
#endif

/* -------------------------------------------------------------------- */
/* Check if object_id is valid                                          */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID(object_id)){
    bassigncstr(error_string,"GS_CheckInternalName: Invalid object_id.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_CheckInternalName: 3\n");
#endif

#ifdef DEBUG
  printf("GS_CheckInternalName: 3.1, thename=|%s|\n",bdata(thename) );
#endif

/* -------------------------------------------------------------------- */
/* See if this name exists in the file                                  */
/* -------------------------------------------------------------------- */
  name2= (char *)GMalloc(sizeof(char)*(bstrlen(thename)+10));
  if(!name2) {
    bassigncstr(error_string,"GS_CheckInternalName: GMalloc error.");
    return FALSE;
  }
  strcpy(name2,bdata(thename));
#ifdef DEBUG
  printf("GS_CheckInternalName: 3.2, name2=|%s|\n",name2);
#endif
  if((int)H5Lexists( object_id, name2, H5P_DEFAULT )==TRUE){
#ifdef DEBUG
  printf("GS_CheckInternalName: 4\n");
#endif
    GFree(name2);
    return TRUE;
  } else {
#ifdef DEBUG
  printf("GS_CheckInternalName: 5\n");
#endif
    GFree(name2);
    return FALSE;
  }// endif

}
