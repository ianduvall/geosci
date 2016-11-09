/* GS_GetStringAttribute.c

   by Leland Pierce, nov 13, 2013

   Aug 2, 2014
   Modified for bstrings, doxygen and GS_

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <hdf5.h>


#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


// for herr_t
#define ERROR -1
#define SUCCESS 0



/** \brief GS_GetStringAttribute reads a string attribute from an object and returns its value.

\param[in]   object_id
                   The opened object where the attribute is stored.
\param[in] name
                   The name of the attribute variable.
\param[in] value
The returned value of the variable. On input, this must be a valid bstring.

\returns
 TRUE on success, FALSE on failure

\par Example:
Read the group type attribute:
\code
bstring group_type=bfromcstr("");
bstring grouptype =bfromcstr("grouptype"):
hid_t object_id;
if(!GS_GetStringAttribute(object_id,grouptype,group_type)){
   bassigncstr(group_type,"...unknown...");
}// endif
bdestroy(grouptype);
...use group_type...
bdestroy(group_type);
\endcode

*/




//#define DEBUG

int GS_GetStringAttribute( hid_t object_id, const_bstring name, bstring value)
{
  int cset = H5T_CSET_ASCII;
  bstring object_name;
  int L;
  int st;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");
#ifdef DEBUG
  printf("GS_GetStringAttribute: 1: name=|%s|, value=|%s|\n",
         bdata(name),bdata(value));
#endif
/* -------------------------------------------------------------------- */
/* See if the object-Id is valid                                         */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID(object_id)){
    bassignformat(error_string,"GS_GetStringAttribute: Invalid ID.");
    return FALSE;
  }// endif 

#ifdef DEBUG
  printf("GS_GetStringAttribute: attribute name=|%s|\n",bdata(name));
#endif

/* -------------------------------------------------------------------- */
/* Get the attribute                                                    */
/* -------------------------------------------------------------------- */
  st = H5ATTRget_attribute_string( object_id, name, value, &cset);
  if( st == ERROR) {
    object_name = GS_GetIDName(object_id);
    bassignformat(error_string,"GS_GetStringAttribute: Can't get attribute '%s' in node '/%s'.",
                  bdata(name),bdata(object_name));
    bdestroy(object_name);
    return FALSE;
  }
  
#ifdef DEBUG
  printf("GS_GetStringAttribute: returning TRUE, st=%d, value=|%s|\n",st, bdata(value));
#endif

  return TRUE;
}
