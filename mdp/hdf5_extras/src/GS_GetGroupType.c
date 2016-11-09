/* GS_GetGroupType.c

   by Leland Pierce, Nov 20, 2013

*/
#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"

//#define DEBUG

/** \brief GS_GetGroupType return the group type
\see GS_ImageOpen(), GS_FileOpen(), GS_FileCreate()
\param[in] file_id
                   File handle of selected GeoSci file.
\param[in] group_name  
                   Name of group to query.
\returns
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
hid_t    file_id;
int      grouptype;

file_id = GS_FileOpen(bsstatic("test123.h5"),bsstatic("r+"));
if(file_id < 0) {
   printf("Could not open file.\n");
   return;
}

grouptype = GS_GetGroupType(file_id,bsstatic("/Channel_1"));
if(grouptype){
   printf("group type is %d\n",grouptype);
}

          ...
\endcode
*/


int GS_GetGroupType(hid_t file_id, const_bstring group_name)
{
  hid_t group_id;
  bstring grouptypestring = bfromcstr("");
  hid_t ifile_id;
  int objtype;
  char *group_name2;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");


/* -------------------------------------------------------------------- */
/* Get a file_id                                                        */
/* -------------------------------------------------------------------- */
  ifile_id = GS_GetValidFileID(file_id);
  if(!ifile_id){
    bassignformat(error_string,"GetGroupType: Unable to get file_id.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* See if this name exists in the file                                  */
/* -------------------------------------------------------------------- */
  group_name2 = (char *)GMalloc(sizeof(char)*(bstrlen(group_name)+10));
  if(!group_name2){
    bassignformat(error_string,"GetGroupType: GMalloc error");
    return FALSE;
  }
  strcpy(group_name2,bdata(group_name));
  if(H5Lexists( ifile_id, group_name2, H5P_DEFAULT )==TRUE){
  } else {
    GFree(group_name2);
    GS_FileClose(ifile_id);
    bassignformat(error_string,"GetGroupType: Name: %s does not exist in file.",bdata(group_name));
    return FALSE;
  }// endif
  GFree(group_name2);

/* -------------------------------------------------------------------- */
/* Is it a group?                                                       */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsGroup(ifile_id,group_name)){
    GS_FileClose(ifile_id);
    bassignformat(error_string,"GetGroupType: Object is not a group: %s.",bdata(group_name));
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Open the group                                                       */
/* -------------------------------------------------------------------- */
  group_id = GS_GroupOpen(ifile_id, group_name);
  if(group_id < 0){
    GS_FileClose(ifile_id);
    bassignformat(error_string,"GetGroupType: Can't open the group: %s.",bdata(group_name));
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Get the typestring of group                                          */
/* -------------------------------------------------------------------- */
  bstring grouptype = bfromcstr("grouptype");
  if(!GS_GetStringAttribute(group_id,grouptype,grouptypestring)){
    GS_GroupClose(group_id);
    GS_FileClose(ifile_id);
    bassignformat(error_string,"GetGroupType: HDF5 group has no grouptype, group_name: %s.",
                  bdata(group_name));
    bdestroy(grouptype);
    return FALSE;
  }// endif
  bdestroy(grouptype);

#ifdef DEBUG
  printf("GetGroupType: grouptypestring=|%s|\n",grouptypestring);
#endif

//----------------------------------------------------------------------
// Get the type int
//----------------------------------------------------------------------
  objtype = GS_GroupTypeString2Int(grouptypestring);
#ifdef DEBUG
  printf("GetGroupType: objtype=%d\n",objtype);
#endif


/* -------------------------------------------------------------------- */
/* Clean up                                                             */
/* -------------------------------------------------------------------- */
  bdestroy(grouptypestring);
  GS_GroupClose(group_id);

/* -------------------------------------------------------------------- */
/* Close the file                                                       */
/* -------------------------------------------------------------------- */
  GS_FileClose(ifile_id);


  return objtype;
}
