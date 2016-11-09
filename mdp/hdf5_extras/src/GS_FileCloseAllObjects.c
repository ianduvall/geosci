/* GS_FileCloseAllObjects.c

   by Leland Pierce, Aug 6, 2014
   including: bstrings, doxygen and GS_

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"




/** \brief GS_FileCloseAllObjects closes all open objects in a file

\see GS_FileClose(), GS_FileOpen()

\param[in] id
    The handle of the open file, or any object in the file.

\returns
\c TRUE is returned on success, \c FALSE otherwise.

\par Example
Close all objects in an already-open file:
\code
hid_t file_id;
if(GS_FileCloseAllObjects( file_id )){
    printf("success.\n");
} else {
    printf("failure.\n");
}
\endcode

*/

//#define DEBUG

int GS_FileCloseAllObjects(hid_t id)
{
  hid_t file_id;

  int i, the_id;
  int num_objs;
  hid_t *obj_id_list;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("GS_FileCloseAllObjects: 1\n");
#endif

/* -------------------------------------------------------------------- */
/*       Get a valid file                                               */
/* -------------------------------------------------------------------- */
  file_id = GS_GetValidFileID(id);
  if(!file_id){
    bassigncstr(error_string,"GS_FileCloseAllObjects: Can't get file_id of the given handle.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_FileCloseAllObjects: 1\n");
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
    bassigncstr(error_string,"GS_FileCloseAllObjects: GMalloc error.");
    H5Fclose(file_id);
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_FileCloseAllObjects: 3\n");
#endif

/* -------------------------------------------------------------------- */
/*       1.2 Get ids of all open datasets in this file                  */
/* -------------------------------------------------------------------- */
  num_objs = H5Fget_obj_ids( file_id, H5F_OBJ_DATASET, num_objs, obj_id_list);

#ifdef DEBUG
  printf("GS_FileCloseAllObjects: 4\n");
#endif

/* -------------------------------------------------------------------- */
/*       1.3 Close each Dataset                                         */
/* -------------------------------------------------------------------- */
  for(i=0;i<num_objs;i++){
    the_id=obj_id_list[i];
    if(GS_ValidID(the_id)) {
      GS_DatasetClose(the_id);
    }// endif: valid ID?
  }// endfor

#ifdef DEBUG
  printf("GS_FileCloseAllObjects: 5\n");
#endif

/* -------------------------------------------------------------------- */
/*       1.4 Clean up                                                   */
/* -------------------------------------------------------------------- */
  GFree(obj_id_list);

#ifdef DEBUG
  printf("GS_FileCloseAllObjects: 6\n");
#endif




/* -------------------------------------------------------------------- */
/*       2.1 Get number of open datatypes in this file                  */
/* -------------------------------------------------------------------- */
  num_objs = H5Fget_obj_count( file_id, H5F_OBJ_DATATYPE);
  obj_id_list = (hid_t *)GMalloc(num_objs*sizeof(hid_t));
  if(!obj_id_list){
    bassigncstr(error_string,"GS_FileCloseAllObjects: GMalloc error.");
    H5Fclose(file_id);
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_FileCloseAllObjects: 7\n");
#endif

/* -------------------------------------------------------------------- */
/*       2.2 Get ids of all open datatypes in this file                 */
/* -------------------------------------------------------------------- */
  num_objs = H5Fget_obj_ids( file_id, H5F_OBJ_DATATYPE, num_objs, obj_id_list);

#ifdef DEBUG
  printf("GS_FileCloseAllObjects: 8\n");
#endif

/* -------------------------------------------------------------------- */
/*       2.3 Close each Datatype                                        */
/* -------------------------------------------------------------------- */
  for(i=0;i<num_objs;i++){
    the_id=obj_id_list[i];
    if(GS_ValidID(the_id)) {
      H5Tclose(the_id);
    }// endif: valid ID?
  }// endfor

#ifdef DEBUG
  printf("GS_FileCloseAllObjects: 9\n");
#endif

/* -------------------------------------------------------------------- */
/*       2.4 Clean up                                                   */
/* -------------------------------------------------------------------- */
  GFree(obj_id_list);

#ifdef DEBUG
  printf("GS_FileCloseAllObjects: 10\n");
#endif





/* -------------------------------------------------------------------- */
/*       3.1 Get number of open attributes in this file                 */
/* -------------------------------------------------------------------- */
  num_objs = H5Fget_obj_count( file_id, H5F_OBJ_ATTR);
  obj_id_list = (hid_t *)GMalloc(num_objs*sizeof(hid_t));
  if(!obj_id_list){
    bassigncstr(error_string,"GS_FileCloseAllObjects: GMalloc error.");
    H5Fclose(file_id);
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_FileCloseAllObjects: 11\n");
#endif

/* -------------------------------------------------------------------- */
/*       3.2 Get ids of all open attributes in this file                */
/* -------------------------------------------------------------------- */
  num_objs = H5Fget_obj_ids( file_id, H5F_OBJ_ATTR, num_objs, obj_id_list);

#ifdef DEBUG
  printf("GS_FileCloseAllObjects: 12\n");
#endif

/* -------------------------------------------------------------------- */
/*       3.3 Close each attribute                                       */
/* -------------------------------------------------------------------- */
  for(i=0;i<num_objs;i++){
    the_id=obj_id_list[i];
    if(GS_ValidID(the_id)) {
      H5Aclose(the_id);
    }// endif: valid ID?
  }// endfor

#ifdef DEBUG
  printf("GS_FileCloseAllObjects: 13\n");
#endif

/* -------------------------------------------------------------------- */
/*       3.4 Clean up                                                   */
/* -------------------------------------------------------------------- */
  GFree(obj_id_list);
#ifdef DEBUG
  printf("GS_FileCloseAllObjects: 14\n");
#endif








/* -------------------------------------------------------------------- */
/*       4.1 Get number of open groups in this file                     */
/*           note: since this is last, there should be no open          */
/*                 objects inside the groups, so they should            */
/*                 close cleanly.                                       */
/* -------------------------------------------------------------------- */
  num_objs = H5Fget_obj_count( file_id, H5F_OBJ_GROUP);
  obj_id_list = (hid_t *)GMalloc(num_objs*sizeof(hid_t));
  if(!obj_id_list){
    bassigncstr(error_string,"GS_FileCloseAllObjects: GMalloc error.");
    H5Fclose(file_id);
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_FileCloseAllObjects: 15\n");
#endif

/* -------------------------------------------------------------------- */
/*       4.2 Get ids of all open groups in this file                    */
/* -------------------------------------------------------------------- */
  num_objs = H5Fget_obj_ids( file_id, H5F_OBJ_GROUP, num_objs, obj_id_list);

#ifdef DEBUG
  printf("GS_FileCloseAllObjects: 16\n");
#endif

/* -------------------------------------------------------------------- */
/*       4.3 Close each group                                           */
/* -------------------------------------------------------------------- */
  for(i=0;i<num_objs;i++){
    the_id=obj_id_list[i];
    if(GS_ValidID(the_id)) {
      H5Gclose(the_id);
    }// endif: valid ID?
  }// endfor

#ifdef DEBUG
  printf("GS_FileCloseAllObjects: 17\n");
#endif

/* -------------------------------------------------------------------- */
/*       4.4 Clean up                                                   */
/* -------------------------------------------------------------------- */
  GFree(obj_id_list);

#ifdef DEBUG
  printf("GS_FileCloseAllObjects: 18\n");
#endif






/* -------------------------------------------------------------------- */
/*       6. Verify no open objects remain open                          */
/*          If they passed me a file id, then there should be 2 objects */
/*          still open.                                                 */
/*          If they passed in something else, there should only be      */
/*          one object open, the temporary file handle.                 */
/* -------------------------------------------------------------------- */
  num_objs = H5Fget_obj_count( file_id, H5F_OBJ_ALL);
  if(num_objs > 2 ) {
#ifdef DEBUG
  printf("GS_FileCloseAllObjects: 19\n");
#endif
    bassigncstr(error_string,"GS_FileCloseAllObjects: some objects remain open.");
    H5Fclose(file_id); // close temporary handle
    return FALSE;
  } else {
#ifdef DEBUG
  printf("GS_FileCloseAllObjects: 20\n");
#endif
    H5Fclose(file_id); // close temporary handle
    return TRUE;
  }//endif


}
