/* GS_GroupCloseAllObjects.c

   by Leland Pierce, Aug 11, 2014
   including: bstrings, doxygen and GS_
   (modifed FileCloseAllObjects)

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"




/** \brief GS_GroupCloseAllObjects closes all open objects in a group

\see GS_GroupClose(), GS_FileOpen()

\param[in] id
    The handle of the open group.

\returns
\c TRUE is returned on success, \c FALSE otherwise.

\par Example
Close all objects in an already-open group:
\code
hid_t group_id;
if(GS_GroupCloseAllObjects( group_id )){
    printf("success.\n");
} else {
    printf("failure.\n");
}
\endcode

*/

//#define DEBUG

int GS_GroupCloseAllObjects(hid_t id)
{
  hid_t file_id;

  int i, the_id;
  int num_objs;
  hid_t *obj_id_list;
  bstring group_name;
  bstring obj_name;
  int ret;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("GS_GroupCloseAllObjects: START, id=%d\n",id);
#endif
/* -------------------------------------------------------------------- */
/*       Make sure the id refers to a Group                             */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsGroupByID(id)){
    bassigncstr(error_string,"GS_GroupCloseAllObjects: Object ID is not a group.");
    return FALSE;
  }// endif
#ifdef DEBUG
  printf("GS_GroupCloseAllObjects: 1\n");
#endif

/* -------------------------------------------------------------------- */
/*       Get a valid file                                               */
/* -------------------------------------------------------------------- */
  file_id = GS_GetValidFileID(id);
  if(!file_id){
    bassigncstr(error_string,"GS_GroupCloseAllObjects: Can't get file_id of the given handle.");
    return FALSE;
  }// endif

  // yet: need to fix this code to close the file_id on any error.
  // its refcounted, so its OK.
#ifdef DEBUG
  printf("GS_GroupCloseAllObjects: 2\n");
#endif


/* -------------------------------------------------------------------- */
/*       Get name of this Group                                         */
/* -------------------------------------------------------------------- */
  group_name = GS_GetIDName(id);
#ifdef DEBUG
  printf("GS_GroupCloseAllObjects: 2.1, group_name=|%s|, length=%d\n",
         bdata(group_name), bstrlen(group_name) );
#endif
  if(bstrlen(group_name)==0) {
    bassigncstr(error_string,"GS_GroupCloseAllObjects: Can't get group name.");
    bdestroy(group_name);
    return FALSE;
  }// endif
#ifdef DEBUG
  printf("GS_GroupCloseAllObjects: 3\n");
#endif

/* -------------------------------------------------------------------- */
/*       Close leaf objects first, and then groups                      */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/*       1.1 Get number of open datasets in this file                   */
/* -------------------------------------------------------------------- */
  num_objs = H5Fget_obj_count( file_id, H5F_OBJ_DATASET);
  obj_id_list = (hid_t *)GMalloc(num_objs*sizeof(hid_t));
  if(!obj_id_list){
    bassigncstr(error_string,"GS_GroupCloseAllObjects: GMalloc error.");
    bdestroy(group_name);
    return FALSE;
  }// endif
#ifdef DEBUG
  printf("GS_GroupCloseAllObjects: 4\n");
#endif


/* -------------------------------------------------------------------- */
/*       1.2 Get ids of all open datasets in this file                  */
/* -------------------------------------------------------------------- */
  num_objs = H5Fget_obj_ids( file_id, H5F_OBJ_DATASET, num_objs, obj_id_list);
#ifdef DEBUG
  printf("GS_GroupCloseAllObjects: 5\n");
#endif

/* -------------------------------------------------------------------- */
/*       1.3 Close only if dataset is in this Group                     */
/* -------------------------------------------------------------------- */
  for(i=0;i<num_objs;i++){
    the_id=obj_id_list[i];
    if(GS_ValidID(the_id)) {
      obj_name =  GS_GetIDName(the_id);
      // check if it's contained in the group:
      if(binstr(obj_name,0,group_name)){
        GS_DatasetClose(the_id);
      }//endif
      bdestroy(obj_name);
    }// endif: valid ID?
  }// endfor
#ifdef DEBUG
  printf("GS_GroupCloseAllObjects: 6\n");
#endif

/* -------------------------------------------------------------------- */
/*       1.4 Clean up                                                   */
/* -------------------------------------------------------------------- */
  GFree(obj_id_list);





/* -------------------------------------------------------------------- */
/*       2.1 Get number of open datatypes in this file                  */
/* -------------------------------------------------------------------- */
  num_objs = H5Fget_obj_count( file_id, H5F_OBJ_DATATYPE);
  obj_id_list = (hid_t *)GMalloc(num_objs*sizeof(hid_t));
  if(!obj_id_list){
    bassigncstr(error_string,"GS_GroupCloseAllObjects: GMalloc error.");
    return FALSE;
  }// endif
#ifdef DEBUG
  printf("GS_GroupCloseAllObjects: 7\n");
#endif

/* -------------------------------------------------------------------- */
/*       2.2 Get ids of all open datatypes in this file                 */
/* -------------------------------------------------------------------- */
  num_objs = H5Fget_obj_ids( file_id, H5F_OBJ_DATATYPE, num_objs, obj_id_list);
#ifdef DEBUG
  printf("GS_GroupCloseAllObjects: 8\n");
#endif

/* -------------------------------------------------------------------- */
/*       2.3 Close each Datatype in this Group                          */
/* -------------------------------------------------------------------- */
  for(i=0;i<num_objs;i++){
    the_id=obj_id_list[i];
    if(GS_ValidID(the_id)) {
      obj_name =  GS_GetIDName(the_id);
      // check if it's contained in the group:
      if(binstr(obj_name,0,group_name)){
        H5Tclose(the_id);
      }//endif
      bdestroy(obj_name);
    }// endif: valid ID?
  }// endfor
#ifdef DEBUG
  printf("GS_GroupCloseAllObjects: 9\n");
#endif

/* -------------------------------------------------------------------- */
/*       2.4 Clean up                                                   */
/* -------------------------------------------------------------------- */
  GFree(obj_id_list);






/* -------------------------------------------------------------------- */
/*       3.1 Get number of open attributes in this file                 */
/* -------------------------------------------------------------------- */
  num_objs = H5Fget_obj_count( file_id, H5F_OBJ_ATTR);
  obj_id_list = (hid_t *)GMalloc(num_objs*sizeof(hid_t));
  if(!obj_id_list){
    bassigncstr(error_string,"GS_GroupCloseAllObjects: GMalloc error.");
    return FALSE;
  }// endif
#ifdef DEBUG
  printf("GS_GroupCloseAllObjects: 10\n");
#endif

/* -------------------------------------------------------------------- */
/*       3.2 Get ids of all open attributes in this file                */
/* -------------------------------------------------------------------- */
  num_objs = H5Fget_obj_ids( file_id, H5F_OBJ_ATTR, num_objs, obj_id_list);
#ifdef DEBUG
  printf("GS_GroupCloseAllObjects: 11\n");
#endif

/* -------------------------------------------------------------------- */
/*       3.3 Close each attribute                                       */
/* -------------------------------------------------------------------- */
  for(i=0;i<num_objs;i++){
    the_id=obj_id_list[i];
    if(GS_ValidID(the_id)) {
      obj_name =  GS_GetIDName(the_id);
      // check if it's contained in the group:
      if(binstr(obj_name,0,group_name)){
        H5Aclose(the_id);
      }//endif
      bdestroy(obj_name);
    }// endif: valid ID?
  }// endfor
#ifdef DEBUG
  printf("GS_GroupCloseAllObjects: 12\n");
#endif

/* -------------------------------------------------------------------- */
/*       3.4 Clean up                                                   */
/* -------------------------------------------------------------------- */
  GFree(obj_id_list);








/* -------------------------------------------------------------------- */
/*       4.1 Get number of open groups in this file                     */
/*           note: since this is last, there should be no open          */
/*                 objects inside the groups, so they should            */
/*                 close cleanly.                                       */
/* -------------------------------------------------------------------- */
  num_objs = H5Fget_obj_count( file_id, H5F_OBJ_GROUP);
  obj_id_list = (hid_t *)GMalloc(num_objs*sizeof(hid_t));
  if(!obj_id_list){
    bassigncstr(error_string,"GS_GroupCloseAllObjects: GMalloc error.");
    return FALSE;
  }// endif
#ifdef DEBUG
  printf("GS_GroupCloseAllObjects: 13\n");
#endif

/* -------------------------------------------------------------------- */
/*       4.2 Get ids of all open groups in this file                    */
/* -------------------------------------------------------------------- */
  num_objs = H5Fget_obj_ids( file_id, H5F_OBJ_GROUP, num_objs, obj_id_list);
#ifdef DEBUG
  printf("GS_GroupCloseAllObjects: 14\n");
#endif

/* -------------------------------------------------------------------- */
/*       4.3 Close each group in this Group                             */
/* -------------------------------------------------------------------- */
  for(i=0;i<num_objs;i++){
    the_id=obj_id_list[i];
    if(GS_ValidID(the_id)) {
      obj_name =  GS_GetIDName(the_id);
      // check if it's contained in the group:
      if(binstr(obj_name,0,group_name)){
        // don't close the containing group
        if(!EQUAL(obj_name,group_name)){
          H5Gclose(the_id);
        }//endif
      }//endif
      bdestroy(obj_name);
    }// endif: valid ID?
  }// endfor
#ifdef DEBUG
  printf("GS_GroupCloseAllObjects: 15\n");
#endif

/* -------------------------------------------------------------------- */
/*       4.4 Clean up                                                   */
/* -------------------------------------------------------------------- */
  GFree(obj_id_list);







/* -------------------------------------------------------------------- */
/*       6. Verify no open objects remain open in this Group            */
/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
/*       6.1 Get # of open objects                                      */
/* -------------------------------------------------------------------- */
  num_objs = H5Fget_obj_count( file_id, H5F_OBJ_ALL);
  obj_id_list = (hid_t *)GMalloc(num_objs*sizeof(hid_t));
  if(!obj_id_list){
    bassigncstr(error_string,"GS_GroupCloseAllObjects: GMalloc error.");
    bdestroy(group_name);
    return FALSE;
  }// endif
#ifdef DEBUG
  printf("GS_GroupCloseAllObjects: 16\n");
#endif

/* -------------------------------------------------------------------- */
/*       6.2 Get ids of all open objects in this file                   */
/* -------------------------------------------------------------------- */
  num_objs = H5Fget_obj_ids( file_id, H5F_OBJ_GROUP, num_objs, obj_id_list);
#ifdef DEBUG
  printf("GS_GroupCloseAllObjects: 17\n");
#endif

/* -------------------------------------------------------------------- */
/*       6.3 Check each object that is in this Group                    */
/* -------------------------------------------------------------------- */
  ret = TRUE;
  for(i=0;i<num_objs;i++){
    the_id=obj_id_list[i];
    if(GS_ValidID(the_id)) {
      obj_name =  GS_GetIDName(the_id);
      // check if it's contained in the group:
      if(binstr(obj_name,0,group_name)){
        // don't count the containing group
        if(!EQUAL(obj_name,group_name)){
          ret = FALSE;
        }//endif
      }//endif
      bdestroy(obj_name);
    }// endif: valid ID?
  }// endfor
#ifdef DEBUG
  printf("GS_GroupCloseAllObjects: 18\n");
#endif

/* -------------------------------------------------------------------- */
/*       6.4 Clean up                                                   */
/* -------------------------------------------------------------------- */
  GFree(obj_id_list);
  bdestroy(group_name);

/* -------------------------------------------------------------------- */
/*       7. Return status                                               */
/* -------------------------------------------------------------------- */
  if(ret) {
    return TRUE;
  } else {
    bassigncstr(error_string,"GS_GroupCloseAllObjects: some objects remain open.");
    return FALSE;
  }//endif


}
