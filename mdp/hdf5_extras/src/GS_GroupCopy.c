/* GS_GroupCopy.c

   by Leland Pierce, Aug 14, 2014

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


#define ERROR -1


/** \brief GS_GroupCopy copies a group to another location

\see GS_GroupClose(), GS_GroupOpen()

\param[in] source
                   A handle for a group.
\param[in] destination_object_id
                   The handle of the destination group or file.
\param[in] destination_group_name
                   The name of the new dataset to create.

\returns
The handle of the new group is returned.
If it is less than zero, the copy failed.

\par Example
Copy an image to a new file, call the new image "/Boston_day3".
We have already opened the destination file.
\code
hid_t source_image_id;
hid_t destination_file_id;
hid_t group_id;
bstring newname = bfromcstr("/Boston_day3");
group_id = GS_GroupCopy(source_image_id, destination_file_id, newname);
if(group_id < 0) {
  printf("Failed to copy group.\n");
}
bdestroy(newname);
\endcode
*/


hid_t GS_GroupCopy(hid_t source, hid_t destination_object_id, 
                   const_bstring destination_group_name)
{
  hid_t file_id;
  bstring source_name;
  int datatype;
  const_bstring type_string;
  hid_t hdf_datatype;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");


/* -------------------------------------------------------------------- */
/* See if the source is a group                                         */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsGroupByID(source)){
    bassignformat(error_string,"GS_GroupCopy: ID is not a Group.");
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* See if the destination is a Group or a File                          */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsGroupByID(destination_object_id) &&
     !GS_ObjectIsFIle(destination_object_id)){
    bassignformat(error_string,"GS_GroupCopy: ID is not a Group or a File.");
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* See if the destination name is already in use                        */
/* -------------------------------------------------------------------- */
  if(GS_CheckInternalName(destination_object_id, destination_group_name)){
    bassignformat(error_string,"GS_GroupCopy: destination name already exists.");
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Get the file_id of the source                                        */
/* -------------------------------------------------------------------- */
  file_id = GS_GetValidFileID(source);
  if(file_id < 0 ) {
    bassignformat(error_string,"GS_GroupCopy: Could not get source file-id.");
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Get the name of the source                                           */
/* -------------------------------------------------------------------- */
  source_name = GS_GetIDName(source);
  if(bstrlen(source_name)==0){
    bassignformat(error_string,"GS_GroupCopy: Could not get source-name.");
    bdestroy(source_name);
    GS_FileClose(file_id);
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Do the copy                                                          */
/* -------------------------------------------------------------------- */
  if( H5Ocopy(file_id              , bdata(source_name), 
              destination_object_id, bdata(destination_group_name), 
              H5P_DEFAULT, H5P_DEFAULT ) < 0 ) {
    bassignformat(error_string,"GS_GroupCopy: Copy failed.");
    bdestroy(source_name);
    GS_FileClose(file_id);
    return ERROR;
  }// endif
  bdestroy(source_name);
  
/* -------------------------------------------------------------------- */
/* Close the source file-id                                             */
/* -------------------------------------------------------------------- */
  if(!GS_FileClose(file_id)){
    bassignformat(error_string,"GS_GroupCopy: error closing source-file.");
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Open the copied Group and return handle to it                      */
/* -------------------------------------------------------------------- */
  return GS_GroupOpen(destination_object_id, destination_group_name);

}
