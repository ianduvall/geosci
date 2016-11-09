/* GS_ValidMetadataName.c

   by Leland Pierce, Feb 26, 2014.
  
   July 8, 2014:
   modified for multi-level library
   modified to use bstrings

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"



/** \brief GS_ValidMetadataName check if the name is a valid metadata item for the given object type.

\anchor GS_ValidMetadataName
\addindex GS_ValidMetadataName


\see GS_FileCreate()

\param[in] object_type
                An integer giving the type of object this
                metadata variable refers to. 
                Valid object types are: \n
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
\param[in] name
                   A character string giving the name of the 
                   metadata variable to query.

\returns
\c TRUE is returned if the name is a valid metadata variable,
\c FALSE otherwise.

\par Example:
Make sure that \c descriptor is a valid metadata item for an 
image before updating its value:
\code
bstring name = bfromcstr("descriptor");
if(!GS_ValidMetadataName(GS_OBJECT_TYPE_IMAGE, name)){
   printf("not a valid image metadata variable.\n");
}
bdestroy(name);
\endcode

\par Details:
Because the metadata updating functions will not only update, but also create 
new metadata items,  this function is used to 
prevent mis-spellings of valid metadata items, 
and also to prevent accidental creation of new metadata items.

*/


//#define DEBUG

/*************************************************************************/
int GS_ValidMetadataName(int object_type, const_bstring name)
{


  switch(object_type){


  case GS_OBJECT_TYPE_METADATA_GROUP: // file-metadata
    if(EQUAL(name,bsstatic("grouptype"))){
    } else if(EQUAL(name,bsstatic("filetype"))){
    } else if(EQUAL(name,bsstatic("software_version"))){
    } else if(EQUAL(name,bsstatic("filelib"))){
    } else if(EQUAL(name,bsstatic("descriptor"))){
    } else if(EQUAL(name,bsstatic("writeable"))){
    } else if(EQUAL(name,bsstatic("creation_datetime"))){
    } else if(EQUAL(name,bsstatic("last_update_datetime"))){
    } else if(EQUAL(name,bsstatic("history"))){
    } else {
      return FALSE;
    }
    return TRUE;

  case GS_OBJECT_TYPE_IMAGE:
    if(EQUAL(name,bsstatic("grouptype"))){
    } else if(EQUAL(name,bsstatic("descriptor"))){
    } else if(EQUAL(name,bsstatic("creation_datetime"))){
    } else if(EQUAL(name,bsstatic("last_update_datetime"))){
    } else if(EQUAL(name,bsstatic("history"))){
    } else if(EQUAL(name,bsstatic("filetype"))){
    } else if(EQUAL(name,bsstatic("filelib"))){
      //    } else if(EQUAL(name,bsstatic("pixel_size_units"))){
      //    } else if(EQUAL(name,bsstatic("pixel_size_x"))){
      //    } else if(EQUAL(name,bsstatic("pixel_size_y"))){
      //    } else if(EQUAL(name,bsstatic("type"))){
      //    } else if(EQUAL(name,bsstatic("nchannels"))){
      //    } else if(EQUAL(name,bsstatic("npixels"))){
      //    } else if(EQUAL(name,bsstatic("nlines"))){
    } else if(EQUAL(name,bsstatic("writeable"))){
      //    } else if(EQUAL(name,bsstatic("spatialref"))){
      //    } else if(EQUAL(name,bsstatic("coordinates"))){
    } else {
      return FALSE;
    }
    return TRUE;


  case GS_OBJECT_TYPE_RASTER:
    if(EQUAL(name,bsstatic("dataset_type"))){
    } else if(EQUAL(name,bsstatic("descriptor"))){
    } else if(EQUAL(name,bsstatic("writeable"))){
    } else if(EQUAL(name,bsstatic("last_update_datetime"))){
      // need to add these for my new rasters...yet...june 9, 2016
    } else if(EQUAL(name,bsstatic("creation_datetime"))){
    } else if(EQUAL(name,bsstatic("history"))){
    } else if(EQUAL(name,bsstatic("pixel_size_units"))){
    } else if(EQUAL(name,bsstatic("pixel_size_x"))){
    } else if(EQUAL(name,bsstatic("pixel_size_y"))){
    } else if(EQUAL(name,bsstatic("type"))){
    } else if(EQUAL(name,bsstatic("nchannels"))){
    } else if(EQUAL(name,bsstatic("npixels"))){
    } else if(EQUAL(name,bsstatic("nlines"))){
    } else if(EQUAL(name,bsstatic("spatialref"))){
    } else if(EQUAL(name,bsstatic("coordinates"))){
    } else if(EQUAL(name,bsstatic("filetype"))){
    } else if(EQUAL(name,bsstatic("filelib"))){
    } else {
      return FALSE;
    }
    return TRUE;


  case GS_OBJECT_TYPE_IFILE:
    if(EQUAL(name,bsstatic("dataset_type"))){
    } else if(EQUAL(name,bsstatic("descriptor"))){
    } else if(EQUAL(name,bsstatic("access"))){
    } else if(EQUAL(name,bsstatic("writeable"))){
    } else {
      return FALSE;
    }
    return TRUE;

  case GS_OBJECT_TYPE_FILE:
    if(EQUAL(name,bsstatic("descriptor"))){
    } else if(EQUAL(name,bsstatic("filetype"))){
    } else if(EQUAL(name,bsstatic("software_version"))){
    } else if(EQUAL(name,bsstatic("creation_datetime"))){
    } else if(EQUAL(name,bsstatic("last_update_datetime"))){
    } else if(EQUAL(name,bsstatic("history"))){
    } else if(EQUAL(name,bsstatic("writeable"))){
    } else {
      return FALSE;
    }
    return TRUE;


  case GS_OBJECT_TYPE_METADATA_DATASET:
    if(EQUAL(name,bsstatic("dataset_type"))){
    } else if(EQUAL(name,bsstatic("descriptor"))){
    } else if(EQUAL(name,bsstatic("writeable"))){
    } else if(EQUAL(name,bsstatic("last_update_datetime"))){
    } else {
      return FALSE;
    }
    return TRUE;

  case GS_OBJECT_TYPE_METADATA_IFILE:
    if(EQUAL(name,bsstatic("dataset_type"))){
    } else if(EQUAL(name,bsstatic("descriptor"))){
    } else if(EQUAL(name,bsstatic("writeable"))){
    } else if(EQUAL(name,bsstatic("access"))){
    } else if(EQUAL(name,bsstatic("last_update_datetime"))){
    } else {
      return FALSE;
    }
    return TRUE;




  case GS_OBJECT_TYPE_VECTOR:
    if(EQUAL(name,bsstatic("grouptype"))){
    } else if(EQUAL(name,bsstatic("descriptor"))){
    } else if(EQUAL(name,bsstatic("creation_datetime"))){
    } else if(EQUAL(name,bsstatic("last_update_datetime"))){
    } else if(EQUAL(name,bsstatic("history"))){
    } else if(EQUAL(name,bsstatic("writeable"))){
    } else if(EQUAL(name,bsstatic("spatialref"))){
    } else {
      return FALSE;
    }
    return TRUE;

  case GS_OBJECT_TYPE_UNDER_CONSTRUCTION: 
    return TRUE;

  default:
    return FALSE;
  }// end-switch

}

