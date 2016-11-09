/* GS_SetStringAttribute.c

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

//#define DEBUG



/** \brief GS_SetStringAttribute sets the value of a string attribute for a given object.

\see GS_GetStringAttribute()

\param[in] group_id
                   The opened object where the attribute is stored.

\param[in] name
                   The name of the attribute variable.

\param[in] value
                   The value of the variable to set.

\returns
TRUE on success, FALSE on failure.

\par Example:
Set the group type attribute:
\code
bstring group_type=bfromcstr("");
hid_t object_id;
bassigncstr(group_type,"unknown type");
if(!GS_SetStringAttribute(object_id,bsstatic("grouptype"),group_type)){
   printf("Unable to set group-type string\n");
} else {
   printf("Group-type successfully set.\n");
}// endif
... use group_type ...
bdestroy(group_type);
\endcode
*/





int GS_SetStringAttribute( hid_t object_id, const_bstring name, 
                        const_bstring value)
{
  int cset = H5T_CSET_ASCII;
  bstring object_name;
  int L;
  int st;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("GS_SetStringAttribute: 1: name=|%s|, value=|%s|\n",
         bdata(name),bdata(value) );
#endif

/* -------------------------------------------------------------------- */
/* See if the object-Id is valid                                         */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID(object_id)){
    bassignformat(error_string,"GS_SetStringAttribute: Invalid ID.");
    return FALSE;
  }// endif 

#ifdef DEBUG
  printf("GS_SetStringAttribute: attribute name=|%s|\n",bdata(name));
#endif

/* -------------------------------------------------------------------- */
/* Set the attribute                                                    */
/* -------------------------------------------------------------------- */
  st = H5ATTRset_attribute_string( object_id, name, value, 
                                   bstrlen(value), cset);
#ifdef DEBUG
  printf("GS_SetStringAttribute: func return value=%d\n",st);
#endif
  if( st == ERROR) {
    object_name = GS_GetIDName(object_id);
    bassignformat(error_string,"GS_SetStringAttribute: Can't set attribute '%s' in node '/%s'.",
                  bdata(name),bdata(object_name));
    bdestroy(object_name);
    return FALSE;
  }
  
#ifdef DEBUG
  printf("GS_SetStringAttribute: returning TRUE, st=%d, value=|%s|\n",st, bdata(value));
#endif

  return TRUE;
}
