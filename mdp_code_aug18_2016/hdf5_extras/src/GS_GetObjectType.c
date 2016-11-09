/* GS_GetObjectType.c

   by Leland Pierce, Feb 26, 2014.
  
*/


/** \brief GS_GetObjectType returns the Object Type
\see GS_ImageCreate(), GS_VectorCreate()
\param[in] object_id
                 ID of GeoSci object.
                   Can be a file, an image, raster, etc.
\returns
On success, returns one of the following:
OBJECT_TYPE_METADATA_GROUP 1\n
OBJECT_TYPE_IMAGE    2\n
OBJECT_TYPE_VECTOR   3\n
OBJECT_TYPE_VECTOR2D 4\n
OBJECT_TYPE_VECTOR3D 5\n
OBJECT_TYPE_TIN      6\n
OBJECT_TYPE_MESH2D   7\n
OBJECT_TYPE_MESH3D   8\n
OBJECT_TYPE_RASTER   9\n
OBJECT_TYPE_IFILE   10\n
OBJECT_TYPE_FILE    11\n
OBJECT_TYPE_METADATA_DATASET    12\n
OBJECT_TYPE_METADATA_IFILE      13\n
Returns FALSE on failure.
\par Example
Get the object type of an previously-created image:
\code
hid_t object_id;
int   objtype;
if(objtype = GS_GetObjectType(object_id)){
   printf("The object is type: %d\n",objtype);
}
\endcode
*/

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"

//#define DEBUG
//#define DEBUG22

int GS_GetObjectType(hid_t object_id)
{
  int object_type;
  hid_t ifile_id;
  bstring object_name = bfromcstr("");

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Is it a file-id?                                                     */
/* -------------------------------------------------------------------- */
  if(GS_ObjectIsFile(object_id)){
    return GS_OBJECT_TYPE_FILE;
  }// endif

/* -------------------------------------------------------------------- */
/* Valid object id?                                                     */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID(object_id)) {
    bassignformat(error_string,"GetObjectType: invalid object-id: %d",object_id);
    return FALSE;
  }// endif

#ifdef DEBUG22
        printf("UpdateMetadata: DD1: \n");
        report_num_objects_open();
#endif

/* -------------------------------------------------------------------- */
/* Get a file_id                                                        */
/* -------------------------------------------------------------------- */
  ifile_id = GS_GetValidFileID(object_id);
  if(!ifile_id){
    bassignformat(error_string,"GetObjectType: Unable to get file_id.");
    return FALSE;
  }// endif

#ifdef DEBUG22
        printf("UpdateMetadata: DD2: \n");
        report_num_objects_open();
#endif

//----------------------------------------------------------------------
// Get object name
//----------------------------------------------------------------------
  object_name = GS_GetIDName(object_id);
  if(!object_name){
    GS_FileClose(ifile_id);
    bassignformat(error_string,"GetObjectType: could not get objectname.");
    return FALSE;
  }// endif

#ifdef DEBUG22
        printf("UpdateMetadata: DD3: \n");
        report_num_objects_open();
#endif


#ifdef DEBUG
        printf("GetObjectTYpe: object_name=|%s|\n",bdata(object_name));
#endif

/* -------------------------------------------------------------------- */
/* Determine object type                                                */
/* -------------------------------------------------------------------- */
  object_type=0;
  if(GS_ObjectIsGroup(ifile_id,object_name)){
    object_type = GS_GetGroupTypeByID(object_id);
#ifdef DEBUG
  printf("GetObjectTYpe: a1 object_type=%d\n",object_type);
#endif

#ifdef DEBUG22
        printf("UpdateMetadata: DD4: \n");
        report_num_objects_open();
#endif


    if(!object_type){
      bassignformat(error_string,"GetObjectType: invalid object_type");
      GS_FileClose(ifile_id);
      bdestroy(object_name);
      return FALSE;
    }// endif
  } else if(GS_ObjectIsDatasetByID(object_id)){
#ifdef DEBUG22
        printf("UpdateMetadata: DD5: \n");
        report_num_objects_open();
#endif

#ifdef DEBUG
        printf("GetObjectTYpe: a2.1 object_type is dataset\n");
#endif


    if(GS_ObjectIsRaster(ifile_id,object_name)){
      object_type = GS_OBJECT_TYPE_RASTER;
    }// endif

#ifdef DEBUG
    printf("GetObjectTYpe: a2.2 object_type =%d\n",object_type);
#endif


#ifdef DEBUG22
        printf("UpdateMetadata: DD6: \n");
        report_num_objects_open();
#endif


        // need to check for this first and only
        // check for other 2 if fails:
    if(GS_ObjectIsMetadataIFile(ifile_id,object_name)){
      object_type = GS_OBJECT_TYPE_METADATA_IFILE;
    } else {

      if(GS_ObjectIsIFile(ifile_id,object_name)){
        object_type = GS_OBJECT_TYPE_IFILE;
      }// endif
      
      
      if(GS_ObjectIsMetadataDataset(ifile_id,object_name)){
        object_type = GS_OBJECT_TYPE_METADATA_DATASET;
      }// endif

    }// endif



#ifdef DEBUG
    printf("GetObjectTYpe: a2.3 object_type =%d\n",object_type);
#endif


#ifdef DEBUG22
        printf("UpdateMetadata: DD7: \n");
        report_num_objects_open();
#endif


  }// endif

#ifdef DEBUG22
        printf("UpdateMetadata: DD8: \n");
        report_num_objects_open();
#endif

  bdestroy(object_name);
  GS_FileClose(ifile_id);

#ifdef DEBUG22
        printf("UpdateMetadata: DD9: \n");
        report_num_objects_open();
#endif


#ifdef DEBUG
  printf("GetObjectTYpe: object_type=%d\n",object_type);
#endif

  return object_type;
}
