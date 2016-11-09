/* GS_GroupDelete.c

   by Leland Pierce, Jan 20, 2014

  
   Aug 11, 2014
   added doxygen, bstrings, GS_
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


#define ERROR -1


/** \brief GS_GroupDelete deletes a group in a GeoSci datafile


\param[in] file_id
                   A handle for the already-open GeoSci file.
\param[in] group_name
                   The name of the group, starting with a '/' and giving
                   the full path to the group.

\returns
\c TRUE is returned if the group and all objects it 
contains are successfully deleted, \c FALSE otherwise.

\par Example
Delete an image group named Image1 (should normally call GS_ImageDelete()):
\code
hid_t file_id;
bstring imagename=bfromcstr("/Image1");
if(!GS_GroupDelete(file_id, imagename)){
   printf("Failed to delete group\n");
}
bdestroy(imagename);
\endcode
\par Details
Deletes the group and 
all objects it contains.
*/


int GS_GroupDelete(hid_t file_id, const_bstring group_name )
{
  herr_t ret;
  hid_t ifile_id;
  int i;
  bstring *objnames;
  int nobjs;
  int *hdfobjtypes;
  hid_t group_id;
  char *group_name2;

  bstring tempstr;


  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Get a valid file_id                                                  */
/* -------------------------------------------------------------------- */
  ifile_id = GS_GetValidFileID(file_id);
  if(!ifile_id){
    bassigncstr(error_string,"GS_GroupDelete: Unable to get file_id.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Can we modify the file?                                              */
/* -------------------------------------------------------------------- */
  if(!GS_FileIsWriteable(ifile_id)){
    tempstr = GS_GetIDName(file_id);
    bassignformat(error_string,"GS_GroupDelete: File cannot be written to: %s\n",
                  bdata(tempstr) );
    GS_FileClose(ifile_id);
    bdestroy(tempstr);
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* See if this name exists in the file                                  */
/* -------------------------------------------------------------------- */
  group_name2 = (char *)GMalloc(sizeof(char)*(bstrlen(group_name)+10));
  if(!group_name2){
    bassigncstr(error_string,"GS_GroupDelete: GMalloc error.");
    return FALSE;
  }
  strcpy(group_name2,bdata(group_name));
  if(H5Lexists( ifile_id, group_name2, H5P_DEFAULT ) != TRUE){
    bassignformat(error_string,"GS_GroupDelete: Name: %s does not exist in file.",
                  bdata(group_name));
    GFree(group_name2);
    GS_FileClose(ifile_id);
    return FALSE;
  }// endif
  GFree(group_name2);

/* -------------------------------------------------------------------- */
/* Is it a group?                                                       */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsGroup(ifile_id,group_name)){
    bassignformat(error_string,"GS_GroupDelete: Object is not a group: %s.",
                  bdata(group_name) );
    GS_FileClose(ifile_id);
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Open the group                                                       */
/* -------------------------------------------------------------------- */
  group_id = GS_GroupOpen(file_id, group_name);
  if(group_id<0){
    bassignformat(error_string,"GS_GroupDelete: unable to open Group");
    GS_FileClose(ifile_id);
    return FALSE;    
  }// endif

/* -------------------------------------------------------------------- */
/* Find the children of this object                                     */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectGetH5Children(group_id, &nobjs, &objnames,&hdfobjtypes)){
    bassignformat(error_string,"GS_GroupDelete: unable to get children");
    GS_GroupClose(group_id);
    GS_FileClose(ifile_id);
    return FALSE;    
  }// endif

/* -------------------------------------------------------------------- */
/* Delete Children                                                      */
/* -------------------------------------------------------------------- */
  // pass1: delete non-groups:
  for(i=0;i<nobjs;i++){
    if(hdfobjtypes[i] != H5O_TYPE_GROUP){
      if(H5Ldelete( group_id, bdata(objnames[i]), H5P_DEFAULT) < 0){
        bassignformat(error_string,"GS_GroupDelete: unable to delete object: %s/%s",
                      bdata(group_name),bdata(objnames[i]));
        GS_GroupClose(group_id);
        GS_FileClose(ifile_id);
        return FALSE;    
      }// endif
    }// endif
  }// endfor

  // pass2: delete groups, but not the containing group:
  bstring dot = bfromcstr(".");
  for(i=0;i<nobjs;i++){
    if(hdfobjtypes[i] == H5O_TYPE_GROUP && !EQUAL(objnames[i],dot) ){
      if(H5Ldelete( group_id, bdata(objnames[i]), H5P_DEFAULT) < 0){
        bassignformat(error_string,"GS_GroupDelete: unable to delete object: %s/%s",
                bdata(group_name),bdata(objnames[i]));
        GS_GroupClose(group_id);
        GS_FileClose(ifile_id);
        bdestroy(dot);
        return FALSE;    
      }// endif: H5Ldelete
    }// endif: objtype is group
  }// endfor
  bdestroy(dot);

/* -------------------------------------------------------------------- */
/* Close the group                                                      */
/* -------------------------------------------------------------------- */
  if(!GS_GroupClose(group_id)){
    bassignformat(error_string,"GS_GroupDelete: unable to close Group");
    GS_FileClose(ifile_id);
    return FALSE;    
  }// endif

/* -------------------------------------------------------------------- */
/* Delete it                                                            */
/* -------------------------------------------------------------------- */
  ret =  H5Ldelete( ifile_id, bdata(group_name), H5P_DEFAULT);

/* -------------------------------------------------------------------- */
/* Flush any pending updates                                            */
/* -------------------------------------------------------------------- */
  if(!GS_FileFlush(ifile_id)){
    bassignformat(error_string,"GS_GroupDelete: Can't flush file.");
    GS_FileClose(ifile_id);
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Close the file                                                       */
/* -------------------------------------------------------------------- */
  if(!GS_FileClose(ifile_id)){
    bassignformat(error_string,"GS_GroupDelete: Can't close file.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Return status                                                        */
/* -------------------------------------------------------------------- */
  if(ret >=0 ) return TRUE;
  return FALSE;

}
