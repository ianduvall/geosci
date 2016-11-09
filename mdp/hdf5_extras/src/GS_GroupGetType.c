/* GS_GroupGetType.c

   by Leland Pierce, Aug 12, 2014

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


/** \brief GS_GroupGetType returns the type of a group in a GeoSci datafile


\see GS_GroupOpen()

\param[in] group_id
                   A handle for the group.

\returns
The dataset type code is returned on success:\n
\c                GS_OBJECT_TYPE_METADATA_GROUP 1\n
\c                GS_OBJECT_TYPE_IMAGE    2\n
\c                GS_OBJECT_TYPE_VECTOR   3\n
\c                GS_OBJECT_TYPE_VECTOR2D 4\n
\c                GS_OBJECT_TYPE_VECTOR3D 5\n
\c                GS_OBJECT_TYPE_TIN      6\n
\c                GS_OBJECT_TYPE_MESH2D   7\n
\c                GS_OBJECT_TYPE_MESH3D   8\n
\c                GS_OBJECT_TYPE_RASTER   9\n
\c                GS_OBJECT_TYPE_IFILE   10\n
\c                GS_OBJECT_TYPE_METADATA_DATASET 12\n
\c                GS_OBJECT_TYPE_METADATA_IFILE   13\n
On failure, the following type code is returned:
\li               GS_OBJECT_TYPE_UNKNOWN

\par Example
Get the type of an image:
\code
hid_t image_id;
if(GS_GroupGetType(image_id) != GS_OBJECT_TYPE_IMAGE ) {
  printf("Dataset is not an image, as expected.\n");
}
\endcode
*/

/********************************************************************/
int GS_GroupGetType(hid_t group_id)
{
  bstring typestring;
  int typecode;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Check if its a group id                                              */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsGroupByID( group_id )){
    bassigncstr(error_string,"GS_GroupGetType: Not a group ID.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Read the typecode                                                    */
/* -------------------------------------------------------------------- */
  typestring = bfromcstr("");
  bstring grouptype = bfromcstr("grouptype");
  if(!GS_GetStringAttribute(group_id,grouptype,typestring)){
    bassigncstr(error_string,"GS_GroupGetType: Can't get the group-type metadata.");
    bdestroy(typestring);
    bdestroy(grouptype);
    return FALSE;
  }// endif
  bdestroy(grouptype);

#ifdef OLD
/* -------------------------------------------------------------------- */
/* Decode the typestring                                                */
/* -------------------------------------------------------------------- */
  if(sscanf(bdata(typestring),"%d",&typecode)!= 1) {
    bassigncstr(error_string,"GS_GroupGetType: Can't decode the group-type metadata.");
    bdestroy(typestring);
    return FALSE;
  }// endif
#endif // OLD
  typecode = GS_GroupTypeString2Int(typestring);

/* -------------------------------------------------------------------- */
/* Clean up and return value                                            */
/* -------------------------------------------------------------------- */
  bdestroy(typestring);
  
  return typecode;

}
