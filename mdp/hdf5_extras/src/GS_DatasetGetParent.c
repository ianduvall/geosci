/* GS_DatasetGetParent.c

   by Leland Pierce, Aug 9, 2014

*/



#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


#define ERROR -1



/** \brief GS_DatasetGetParent returns the parent object-id for a Dataset


\param[in] id
		    The handle of the Dataset.

\returns
Returns the object-id  of the parent object (>=0), or ERROR (-1) on failure.

\par Example
Get the parent of a Dataset.
\code
hid_t dataset_id, object_id;
object_id = GS_DatasetGetParent(dataset_id);
if(object_id < 0){
  printf("Could not open the parent of the Dataset.\n");
}
\endcode
*/


hid_t GS_DatasetGetParent(hid_t id)
{
  bstring idname;
  int i;
  bstring parent_name;
  hid_t group_id, file_id;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/*       Validate the ID                                                */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsDatasetByID(id)){
    bassigncstr(error_string,"GS_DatasetGetParent: invalid id.");
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/*       Get the name for this Dataset                                  */
/* -------------------------------------------------------------------- */
  idname = GS_GetIDName(id);
  if(bstrlen(idname)==0){
    bassigncstr(error_string,"GS_DatasetGetParent: Can't get name for the id.");
    bdestroy(idname);
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/*       Keep the part before the last "/" (or all of it if no "/")     */
/* -------------------------------------------------------------------- */
  parent_name = GS_PathnameGetHDFDir(idname);
  if(bstrlen(parent_name)==0) {
    if(bstrchr(idname,'/') != BSTR_ERR){
      bassign(parent_name,idname);
    } else {
      bassigncstr(error_string,"GS_DatasetGetParent: parsing error.");
      bdestroy(idname);
      bdestroy(parent_name);
      return ERROR;
    }// endif
  }// endif
  bdestroy(idname);

/* -------------------------------------------------------------------- */
/*       Get a file_id                                                  */
/* -------------------------------------------------------------------- */
  file_id = GS_GetValidFileID(id);
  if(file_id < 0) {
    bassigncstr(error_string,"GS_DatasetGetParent: Could not get valid file_id.");
    bdestroy(parent_name);
    return ERROR;
  }// endif
  
/* -------------------------------------------------------------------- */
/*       Open the parent group                                          */
/* -------------------------------------------------------------------- */
  group_id = GS_GroupOpen(file_id,parent_name);
  if(!group_id) {
    bassigncstr(error_string,"GS_DatasetGetParent: Could not open parent.");
    bdestroy(parent_name);
    return ERROR;
  }// endif
  bdestroy(parent_name);

/* -------------------------------------------------------------------- */
/*       Clean up                                                       */
/* -------------------------------------------------------------------- */
  if(!GS_FileClose(file_id)){
    bassigncstr(error_string,"GS_DatasetGetParent: Error closing file.");
    return ERROR;
  }// endif

  return group_id;

}
