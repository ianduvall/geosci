/* GS_GroupSetType.c

   by Leland Pierce, Aug 12, 2014

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"



/** \brief GS_GroupSetType sets the type of a group in a GeoSci datafile

\see GS_GroupOpen()

\param[in] group_id
                   A handle for the group.
\param[in] typecode
                   An integer type code specifying the type
                   of dataset. One of:\n
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
\c                GS_OBJECT_TYPE_METADATA_IFILE   13

\returns
\c TRUE on sucess, \c FALSE on failure.

\par Example
Set the type of an image:
\code
hid_t image_id;
if(!GS_GroupSetType(image_id, GS_OBJECT_TYPE_IMAGE) ) {
  printf("Failed to set the type of the image.\n");
}
\endcode
*/

int GS_GroupSetType(hid_t group_id, int typecode)
{
  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Check if its a dataset id                                            */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsGroupByID( group_id )){
    bassigncstr(error_string,"GS_GroupSetType: Not a group ID.");
    return FALSE;
  }// endif


/* -------------------------------------------------------------------- */
/* Check that code is valid                                             */
/* -------------------------------------------------------------------- */
  switch(typecode){
  case GS_OBJECT_TYPE_METADATA_GROUP:
  case GS_OBJECT_TYPE_IMAGE:
  case GS_OBJECT_TYPE_VECTOR:
  case GS_OBJECT_TYPE_VECTOR2D:
  case GS_OBJECT_TYPE_VECTOR3D:
  case GS_OBJECT_TYPE_TIN:
  case GS_OBJECT_TYPE_MESH2D:
  case GS_OBJECT_TYPE_MESH3D:
    break;
  default:
    bassignformat(error_string,"GS_GroupSetType: Not a known group type: %d.",
                  typecode);
    return FALSE;
  }// endswitch


  bstring grouptype = bfromcstr("grouptype");
  bstring metadata = bfromcstr("Metadata");
  bstring image = bfromcstr("Image");
  bstring vector = bfromcstr("Vector");
  bstring vector2d = bfromcstr("Vector2D");
  bstring vector3d = bfromcstr("Vector3D");
  bstring tin = bfromcstr("TIN");
  bstring mesh2d = bfromcstr("Mesh2D");
  bstring mesh3d = bfromcstr("Mesh3D");


/* -------------------------------------------------------------------- */
/* Write the typecode                                                   */
/* -------------------------------------------------------------------- */
  switch(typecode){

  case GS_OBJECT_TYPE_METADATA_GROUP:
    if(!GS_UpdateMetadata(group_id,grouptype,metadata)){
      bassigncstr(error_string,"GS_GroupSetType: Can't write group type.");
      bdestroy(grouptype);
      bdestroy(metadata);
      bdestroy(image);
      bdestroy(vector);
      bdestroy(vector2d);
      bdestroy(vector3d);
      bdestroy(tin);
      bdestroy(mesh2d);
      bdestroy(mesh3d);
      return FALSE;
    }// endif
    break;

  case GS_OBJECT_TYPE_IMAGE:
    if(!GS_UpdateMetadata(group_id,grouptype,image)){
      bassigncstr(error_string,"GS_GroupSetType: Can't write group type.");
      bdestroy(image);
      bdestroy(grouptype);
      bdestroy(metadata);
      bdestroy(image);
      bdestroy(vector);
      bdestroy(vector2d);
      bdestroy(vector3d);
      bdestroy(tin);
      bdestroy(mesh2d);
      bdestroy(mesh3d);
      return FALSE;
    }// endif
    break;

  case GS_OBJECT_TYPE_VECTOR:
    if(!GS_UpdateMetadata(group_id,grouptype,vector)){
      bassigncstr(error_string,"GS_GroupSetType: Can't write group type.");
      bdestroy(grouptype);
      bdestroy(metadata);
      bdestroy(image);
      bdestroy(vector);
      bdestroy(vector2d);
      bdestroy(vector3d);
      bdestroy(tin);
      bdestroy(mesh2d);
      bdestroy(mesh3d);
      return FALSE;
    }// endif
    break;

  case GS_OBJECT_TYPE_VECTOR2D:
    if(!GS_UpdateMetadata(group_id,grouptype,vector2d)){
      bassigncstr(error_string,"GS_GroupSetType: Can't write group type.");
      bdestroy(grouptype);
      bdestroy(metadata);
      bdestroy(image);
      bdestroy(vector);
      bdestroy(vector2d);
      bdestroy(vector3d);
      bdestroy(tin);
      bdestroy(mesh2d);
      bdestroy(mesh3d);
      return FALSE;
    }// endif
    break;

  case GS_OBJECT_TYPE_VECTOR3D:
    if(!GS_UpdateMetadata(group_id,grouptype,vector3d)){
      bassigncstr(error_string,"GS_GroupSetType: Can't write group type.");
      bdestroy(grouptype);
      bdestroy(metadata);
      bdestroy(image);
      bdestroy(vector);
      bdestroy(vector2d);
      bdestroy(vector3d);
      bdestroy(tin);
      bdestroy(mesh2d);
      bdestroy(mesh3d);
      return FALSE;
    }// endif
    break;

  case GS_OBJECT_TYPE_TIN:
    if(!GS_UpdateMetadata(group_id,grouptype,tin)){
      bassigncstr(error_string,"GS_GroupSetType: Can't write group type.");
      bdestroy(grouptype);
      bdestroy(metadata);
      bdestroy(image);
      bdestroy(vector);
      bdestroy(vector2d);
      bdestroy(vector3d);
      bdestroy(tin);
      bdestroy(mesh2d);
      bdestroy(mesh3d);
      return FALSE;
    }// endif
    break;

  case GS_OBJECT_TYPE_MESH2D:
    if(!GS_UpdateMetadata(group_id,grouptype,mesh2d)){
      bassigncstr(error_string,"GS_GroupSetType: Can't write group type.");
      bdestroy(grouptype);
      bdestroy(metadata);
      bdestroy(image);
      bdestroy(vector);
      bdestroy(vector2d);
      bdestroy(vector3d);
      bdestroy(tin);
      bdestroy(mesh2d);
      bdestroy(mesh3d);
      return FALSE;
    }// endif
    break;

  case GS_OBJECT_TYPE_MESH3D:
    if(!GS_UpdateMetadata(group_id,grouptype,mesh3d)){
      bassigncstr(error_string,"GS_GroupSetType: Can't write group type.");
      bdestroy(grouptype);
      bdestroy(metadata);
      bdestroy(image);
      bdestroy(vector);
      bdestroy(vector2d);
      bdestroy(vector3d);
      bdestroy(tin);
      bdestroy(mesh2d);
      bdestroy(mesh3d);
      return FALSE;
    }// endif
    break;


  }// endswitch
  bdestroy(grouptype);
  bdestroy(metadata);
  bdestroy(image);
  bdestroy(vector);
  bdestroy(vector2d);
  bdestroy(vector3d);
  bdestroy(tin);
  bdestroy(mesh2d);
  bdestroy(mesh3d);


  return TRUE;

}
