/* GS_GSObjectTypeAsString.c

   by Leland Pierce, March 30, 2014

   modified JUne 20, 2016, LEP
*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"
#include "hdf5mine.h"

/** \brief GS_GSObjectTypeAsString returns printable version of the object type
\param[in] object_type
  Object type. One of:\n
   GS_OBJECT_TYPE_METADATA_GROUP 1\n
   GS_OBJECT_TYPE_IMAGE    2\n
   GS_OBJECT_TYPE_VECTOR   3\n
   GS_OBJECT_TYPE_VECTOR2D 4\n
   GS_OBJECT_TYPE_VECTOR3D 5\n
   GS_OBJECT_TYPE_TIN      6\n
   GS_OBJECT_TYPE_MESH2D   7\n
   GS_OBJECT_TYPE_MESH3D   8\n
   GS_OBJECT_TYPE_RASTER   9\n
   GS_OBJECT_TYPE_IFILE   10\n
   GS_OBJECT_TYPE_FILE    11\n
   GS_OBJECT_TYPE_METADATA_DATASET 12\n
   GS_OBJECT_TYPE_METADATA_IFILE   13
\returns
On success, the routine returns the string representation
of the \c object_type according to the following table:\n

   object type                  | returned string
--------------------------------|---------------
GS_OBJECT_TYPE_METADATA_GROUP   | Metadata
GS_OBJECT_TYPE_IMAGE            | Image
GS_OBJECT_TYPE_VECTOR           | Vector
GS_OBJECT_TYPE_VECTOR2D         | Vector2D
GS_OBJECT_TYPE_VECTOR3D         | Vector3D
GS_OBJECT_TYPE_TIN              | TIN
GS_OBJECT_TYPE_MESH2D           | Mesh2D
GS_OBJECT_TYPE_MESH3D           | Mesh3D
GS_OBJECT_TYPE_RASTER           | Raster
GS_OBJECT_TYPE_IFILE            | Internal File
GS_OBJECT_TYPE_FILE             | File
GS_OBJECT_TYPE_METADATA_DATASET | Metadata Dataset
GS_OBJECT_TYPE_METADATA_IFILE   | Metadata Internal File

On failure, the string "Unknown Object Type" is returned.

Note that the returned string should NOT be GFree'd or bdestroy'd when done.
\par Example
Obtain list of objects in a file named "testimage.hd5".
\code
    ...
bstring *objnames;
int objtypes;
int nobjs;
int i;

file_id = GS_FileOpen("testimage.hd5","r+");
if(file_id<0){
   printf("GS_FileOpen failed\n");
   printf("%s\n",bdata(error_string));
   exit(-1);
}//endif

if(!GS_ObjectGetGSChildren(file_id, &nobjs, &objnames,&objtypes)){
   printf("GS_ObjectGetGSChildren failed\n");
   printf("%s\n",bdata(error_string));
   exit(-1);
}

// print results:
for(i=0;i<nobjs;i++){
   printf("obj# %d: %s (%s)\n",i,bdata(objnames[i]),
           bdata(GS_GSObjectTypeAsString(objtypes[i])));
}

// clean up:
for(i=0;i<nobjs;i++){
   bdestroy(objnames[i]);
}
GFree(objnames);
GFree(objtypes);
\endcode

*/


/**********************************************************************/
bstring GS_GSObjectTypeAsString(int objtype)
{
  switch(objtype){
  case GS_OBJECT_TYPE_METADATA_GROUP:
    return bsstatic("Metadata");
  case GS_OBJECT_TYPE_IMAGE:
    return bsstatic("Image");
  case GS_OBJECT_TYPE_VECTOR:
    return bsstatic("Vector");
  case GS_OBJECT_TYPE_VECTOR2D:
    return bsstatic("Vector2D");
  case GS_OBJECT_TYPE_VECTOR3D:
    return bsstatic("Vector3D");
  case GS_OBJECT_TYPE_TIN:
    return bsstatic("TIN");
  case GS_OBJECT_TYPE_MESH2D:
    return bsstatic("Mesh2D");
  case GS_OBJECT_TYPE_MESH3D:
    return bsstatic("Mesh3D");
  case GS_OBJECT_TYPE_RASTER:
    return bsstatic("Raster");
  case GS_OBJECT_TYPE_IFILE:
    return bsstatic("Internal File");
  case GS_OBJECT_TYPE_FILE:
    return bsstatic("File");
  case GS_OBJECT_TYPE_METADATA_DATASET:
    return bsstatic("Metadata Dataset");
  case GS_OBJECT_TYPE_METADATA_IFILE:
    return bsstatic("Metadata Internal File");
  default:
    return bsstatic("Unknown Object Type");
  }// end-switch
}
