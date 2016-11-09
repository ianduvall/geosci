/* GS_UpdateMetadata.c

   by Leland Pierce, Jan 4, 2014.
  
   July 10, 2014:
   modified for multi-level library
   modified to use bstrings
*/

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"
#include "string_funcs.h"


//#define DEBUG
//#define DEBUG22


/** \brief GS_UpdateMetadata update a metadata item for the given object.

\anchor GS_UpdateMetadata
\addindex GS_UpdateMetadata



\see GS_AppendMetadata

\param[in] object_id
                  ID of GeoSci object to modify.
                  Can be a file, an image, etc.
\param[in] name
                  The name of the metadata variable to modify.
\param[in] value
                   The new value for the metadata variable.

\returns TRUE on success, FALSE on failure.

\par Example:
Change the \c descriptor metadata for an image to "Angkor Wat, Cambodia".
\code
hid_t image_id;
bstring name = bfromcstr("/image45");
image_id = GS_ImageOpen(file_id,name);
bdestroy(name);
if(!image_id) {
  printf("Failed to open the image\n");
}
bstring descriptor = bfromcstr("descriptor");
bstring descriptor_value = bfromcstr("Angkor Wat, Cambodia");
if(!GS_UpdateMetadata(image_id,descriptor,descriptor_value))){
  printf("Failed to modify image descriptor\n");
}
bdestroy(descriptor);
bdestroy(descriptor_value);
\endcode


*/


/***********************************************************************/
int GS_UpdateMetadata( hid_t object_id, const_bstring name, bstring value)
{
  hid_t ifile_id;
  int object_type;


  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("GS_UpdateMetadata: START objid=%d, name=|%s|, value=|%s|\n",
         (int)object_id,bdata(name), bdata(value));
#endif



/* -------------------------------------------------------------------- */
/* Get a file_id                                                        */
/* -------------------------------------------------------------------- */
  ifile_id = GS_GetValidFileID(object_id);
  if(!ifile_id){
    bassigncstr(error_string,"GS_UpdateMetadata: Unable to get file_id.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_UpdateMetadata: a1, ifile_id=%d\n",ifile_id);
#endif

#ifdef DEBUG22
  printf("GS_UpdateMetadata: CC1: \n");
  report_num_objects_open();
#endif



/* -------------------------------------------------------------------- */
/*	Get writeability for file.                			*/
/* -------------------------------------------------------------------- */
  //ignore writeability if we are updating the writeable metadata
  if(!EQUAL(name, bsstatic("writeable"))){
  if( GS_FileIsWriteable(ifile_id) <= 0 ) {
    // file not writeable
    GS_FileClose(ifile_id);
    bassigncstr(error_string,"GS_UpdateMetadata: file not writeable\n");
    return FALSE;
  }// endif
    }

#ifdef DEBUG
  printf("GS_UpdateMetadata: a2\n");
#endif

#ifdef DEBUG22
        printf("GS_UpdateMetadata: CC2: \n");
        report_num_objects_open();
#endif














/* -------------------------------------------------------------------- */
/*	Get object type.                                                */
/* -------------------------------------------------------------------- */
  object_type = GS_GetObjectType(object_id);
#ifdef DEBUG
  printf("GS_UpdateMetadata: a3, object_type=%d\n",object_type);
#endif

#ifdef DEBUG22
  printf("GS_UpdateMetadata: CC3: \n");
  report_num_objects_open();
#endif

  if(!object_type) {
    bstring dataset_type = bfromcstr("dataset_type");
    bstring grouptype = bfromcstr("grouptype");
    if(EQUAL(name,dataset_type)){
      object_type = GS_OBJECT_TYPE_UNDER_CONSTRUCTION;
    } else if(EQUAL(name,grouptype)){
      object_type = GS_OBJECT_TYPE_UNDER_CONSTRUCTION;
    }// endif
    bdestroy(dataset_type);
    bdestroy(grouptype);
  }// endif
#ifdef DEBUG
  printf("GS_UpdateMetadata: a4, object_type=%d\n",object_type);
#endif

  if(!object_type) {
    GS_FileClose(ifile_id);
    bassignformat(error_string,"GS_UpdateMetadata: metadata object-type invalid: %d\n",object_type);
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_UpdateMetadata: a5, object_type=%d\n",object_type);
#endif


#ifdef DEBUG22
        printf("GS_UpdateMetadata: CC4: \n");
        report_num_objects_open();
#endif


/* -------------------------------------------------------------------- */
/*	Make sure a valid metadata item-name                            */
/* -------------------------------------------------------------------- */
  if(!GS_ValidMetadataName(object_type,name)){
    GS_FileClose(ifile_id);
    bassignformat(error_string,"GS_UpdateMetadata: metadata item-name invalid: %s\n",bdata(name));
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_UpdateMetadata: a6\n");
#endif

#ifdef DEBUG22
        printf("GS_UpdateMetadata: CC5: \n");
        report_num_objects_open();
#endif



#ifdef DEBUG22
  printf("GS_UpdateMetadata: before call to H5ATTRset_attribute_string:\n");
  report_num_objects_open();
#endif









/* -------------------------------------------------------------------- */
/*	Set the new value for the variable.                             */
/* -------------------------------------------------------------------- */
  if( GS_SetStringAttribute(object_id,name,value) < 0 ) {
    GS_FileClose(ifile_id);
    bassignformat(error_string,"GS_UpdateMetadata: Can't set attribute '%s'.",bdata(name));
    return FALSE;
  }// endif








#ifdef DEBUG22
  printf("GS_UpdateMetadata: after call to H5ATTRset_attribute_string:\n");
  report_num_objects_open();
#endif


 
#ifdef DEBUG
  printf("GS_UpdateMetadata: a7\n");
#endif

/* -------------------------------------------------------------------- */
/*      Flush any pending updates                                       */
/* -------------------------------------------------------------------- */
  if(!GS_FileFlush(ifile_id)){
    GS_FileClose(ifile_id);
    bassigncstr(error_string,"GS_UpdateMetadata: Can't flush file.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_UpdateMetadata: a8\n");
#endif

#ifdef DEBUG22
        printf("GS_UpdateMetadata: CC6: \n");
        report_num_objects_open();
#endif


/* -------------------------------------------------------------------- */
/*      Close the file                                                  */
/* -------------------------------------------------------------------- */
    GS_FileClose(ifile_id);


  return TRUE;
}
