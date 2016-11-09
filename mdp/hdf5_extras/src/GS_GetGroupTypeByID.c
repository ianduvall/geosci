/* GS_GetGroupTypeByID.c

   by Leland Pierce, Nov 20, 2013

*/

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"

//#define DEBUG

/** \brief GS_GetGroupTypeByID returns the group type
\see GS_GetGroupType(), GS_ImageOpen()
\param[in] object_id
                  Object handle in a GeoSci file.
\return
An integer that represents the group type, or 0 if there is 
       any error.
Valid group types are:\n
OBJECT_TYPE_METADATA_GROUP 1\n
OBJECT_TYPE_IMAGE    2\n
OBJECT_TYPE_VECTOR   3\n
OBJECT_TYPE_VECTOR2D 4\n
OBJECT_TYPE_VECTOR3D 5\n
OBJECT_TYPE_TIN      6\n
OBJECT_TYPE_MESH2D   7\n
OBJECT_TYPE_MESH3D   8\n
\par Example
Get the group-type of an image:
\code
           ...
hid_t    image_id;
int      grouptype;
            ...       
grouptype = GS_GetGroupTypeByID(image_id);
if(grouptype){
   printf("group type is %d\n",grouptype);
}
\endcode
*/


int GS_GetGroupTypeByID(hid_t object_id)
{
  hid_t file_id;
  bstring object_name = bfromcstr("");
  int objtype;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

//----------------------------------------------------------------------
// Get file_id
//----------------------------------------------------------------------
  file_id = GS_GetValidFileID(object_id);
  if(!file_id){
    bassignformat(error_string,"GetGroupTypeByID: could not get file handle.");
    return FALSE;
  }// endif

//----------------------------------------------------------------------
// Get object name
//----------------------------------------------------------------------
  object_name = GS_GetIDName(object_id);
  if(!object_name){
    GS_FileClose(file_id);
    bassignformat(error_string,"GetGroupTypeByID: could not get objectname.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GetGroupTypeByID: object_name=|%s|\n",object_name);
#endif

//----------------------------------------------------------------------
// Get the type int
//----------------------------------------------------------------------
  objtype = GS_GetGroupType(file_id, object_name);

#ifdef DEBUG
  printf("GetGroupTypeByID: objtype=%d\n",objtype);
#endif

//----------------------------------------------------------------------
// Clean up
//----------------------------------------------------------------------
  bdestroy(object_name);
  GS_FileClose(file_id);

//----------------------------------------------------------------------
// Return the group type int
//----------------------------------------------------------------------
  return objtype;
}
