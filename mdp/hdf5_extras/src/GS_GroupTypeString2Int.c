/* GS_GroupTypeString2Int.c

   by Leland Pierce, Feb 26, 2014.
*/

/** \brief GS_GroupTypeString2Int return the group type as an integer
\see ImageOpen(), FileOpen(), FileCreate()

\param[in] group_type_string  
  A string specifying the group type.
\returns
 An integer that represents the group type, or 0 if there is 
any error.

Valid object types are:\n
OBJECT_TYPE_METADATA_GROUP 1\n
OBJECT_TYPE_IMAGE    2\n
OBJECT_TYPE_VECTOR   3\n
OBJECT_TYPE_VECTOR2D 4\n
OBJECT_TYPE_VECTOR3D 5\n
OBJECT_TYPE_TIN      6\n
OBJECT_TYPE_MESH2D   7\n
OBJECT_TYPE_MESH3D   8\n
OBJECT_TYPE_RASTER   9\n

\par Example
Get the group-type of an image:
\code
printf("group type integer for an image is: %d\n",
        GS_GroupTypeString2Int("Image"));
\endcode
*/


#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"

//#define DEBUG

int GS_GroupTypeString2Int(const_bstring grouptypestring)
{
  int objtype;

#ifdef DEBUG
  printf("GroupTypeString2Int: input=|%s|\n",grouptypestring);
#endif

//----------------------------------------------------------------------
// Get the type int
//----------------------------------------------------------------------
  bstring metadata = bfromcstr("Metadata");
  bstring image = bfromcstr("Image");
  bstring vector = bfromcstr("Vector");
  bstring vector2d = bfromcstr("Vector2D");
  bstring vector3d = bfromcstr("Vector3D");
  bstring tin = bfromcstr("TIN");
  bstring mesh2d = bfromcstr("Mesh2D");
  bstring mesh3d = bfromcstr("Mesh3D");
  bstring raster = bfromcstr("Raster");

  if(EQUAL(grouptypestring,metadata )){
    objtype = GS_OBJECT_TYPE_METADATA_GROUP;
#ifdef DEBUG
    printf("GroupTypeString2Int: objtype is metadata: %d\n",objtype);
#endif
  } else if(EQUAL(grouptypestring,image)){
    objtype = GS_OBJECT_TYPE_IMAGE;
  } else if(EQUAL(grouptypestring,vector)){
    objtype = GS_OBJECT_TYPE_VECTOR;
  } else if(EQUAL(grouptypestring,vector2d)){
    objtype = GS_OBJECT_TYPE_VECTOR2D;
  } else if(EQUAL(grouptypestring,vector3d)){
    objtype = GS_OBJECT_TYPE_VECTOR3D;
  } else if(EQUAL(grouptypestring,tin)){
    objtype = GS_OBJECT_TYPE_TIN;
  } else if(EQUAL(grouptypestring,mesh2d)){
    objtype = GS_OBJECT_TYPE_MESH2D;
  } else if(EQUAL(grouptypestring,mesh3d)){
    objtype = GS_OBJECT_TYPE_MESH3D;
  } else if(EQUAL(grouptypestring,raster)){
    objtype = GS_OBJECT_TYPE_RASTER;
  } else {
    objtype = 0;
  }// endif
  
  bdestroy(metadata);
  bdestroy(image);
  bdestroy(vector);
  bdestroy(vector2d);
  bdestroy(vector3d);
  bdestroy(tin);
  bdestroy(mesh2d);
  bdestroy(mesh3d);
  bdestroy(raster);

#ifdef DEBUG
    printf("GroupTypeString2Int: returning objtype: %d\n",objtype);
#endif

  return objtype;
}
