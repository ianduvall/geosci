/* GS_ObjectGetH5Children.c

   by Leland Pierce, May 22, 2016

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"



#define ERROR -1

herr_t getcount_it( hid_t o_id, const char *name, 
                    const H5O_info_t *object_info,void *op_data);

herr_t getnames_it( hid_t o_id, const char *name, 
                    const H5O_info_t *object_info,void *op_data);

int gcount;

int *hdftypes;


/** \brief GS_ObjectGetH5Children returns list of all children objects


\see GS_ObjectGetGSChildren()

\param[in]       object_id
                   Handle of an open object.
                   The names of the children of this object, if any, 
                   are returned.

\param[out]       nobjs;
                   The number of objects (children) returned.
                   
\param[out]       objnames;
                   The returned list of object names.
                   bdestroy() each item it when done, and
                   GFree() entire list when done.
                   
\param[out]       objtypes;
                   The returned list of object types.
                   GFree it when done.
                   Valid values are:\n
                   H5O_TYPE_GROUP\n
                   H5O_TYPE_DATASET\n
                   H5O_TYPE_NAMED_DATATYPE\n

\returns
\c TRUE on success \c FALSE on failure.


\par Example
Obtain list of objects in a file named "testimage.hd5".

\code
    ...
bstring *objnames;
int objhdftypes;
int nobjs;
int i;
bstring filename = bfromcstr("testimage.hd5");
bstring access = bfromcstr("r+");
file_id = GS_FileOpen(filename,access);
bdestroy(filename);
bdestroy(access);
if(file_id<0){
   printf("GS_FileOpen failed\n");
   printf("%s\n",error_string);
   exit(-1);
}//endif

if(!GS_ObjectGetH5Children(file_id, &nobjs, &objnames,&objhdftypes)){
   printf("GS_ObjectGetH5Children failed\n");
   printf("%s\n",error_string);
   exit(-1);
}

// print results:
for(i=0;i<nobjs;i++){
   printf("obj# %d: %s (%s)\n",i,objnames[i],
           GS_H5ObjectTypeAsString(objhdftypes[i]));
}

// clean up:
for(i=0;i<nobjs;i++){
   bdestroy(objnames[i]);
}
GFree(objnames);
GFree(objhdftypes);
\endcode
*/



//#define DEBUG

/**********************************************************************/
int GS_ObjectGetH5Children( hid_t object_id, int *nobjs, 
                 bstring **objnames, int **objtypes)
{
  int i;
  int count;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* See if the object-id is valid                                        */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID(object_id)){
    bassignformat(error_string,"GS_ObjectGetH5Children: Invalid Object ID");
    return FALSE;    
  }// endif

/* -------------------------------------------------------------------- */
/* Recursively visit all the children                                   */
/* count them.                                                          */
/* -------------------------------------------------------------------- */
  count = 0;
  if(H5Ovisit( object_id, H5_INDEX_NAME, H5_ITER_INC,
               getcount_it, &count ) <0){
      bassignformat(error_string,"GS_ObjectGetH5Children: H5Ovisit error(1)");
      return FALSE;    
  }// endif

  
/* -------------------------------------------------------------------- */
/* GMalloc space for the names                                          */
/* -------------------------------------------------------------------- */
  *nobjs = count;
  *objnames = (bstring *)GMalloc(count*sizeof(bstring));
  if(!(*objnames)){
    bassignformat(error_string,"GS_ObjectGetH5Children: GMalloc error(1)");
    return FALSE;    
  }// endif

/* -------------------------------------------------------------------- */
/* GMalloc space for the types                                          */
/* -------------------------------------------------------------------- */
  hdftypes = (int *)GMalloc(count*sizeof(int));
  if(!hdftypes){
    bassignformat(error_string,"GS_ObjectGetH5Children: GMalloc error(3)");
    return FALSE;    
  }// endif

/* -------------------------------------------------------------------- */
/* Recursively visit all the children                                   */
/* get the names                                                        */
/* -------------------------------------------------------------------- */
  gcount=0;
  if(H5Ovisit( object_id, H5_INDEX_NAME, H5_ITER_INC,
               getnames_it, *objnames) <0) {
      bassignformat(error_string,"GS_ObjectGetH5Children: H5Ovisit error(2)");
      return FALSE;    
  }// endif


/* -------------------------------------------------------------------- */
/* GMalloc space for the types                                          */
/* -------------------------------------------------------------------- */
  *objtypes = (int *)GMalloc(count*sizeof(int));
  if(!(*objtypes)){
    bassignformat(error_string,"GS_ObjectGetH5Children: HMalloc error(4)");
    return FALSE;    
  }// endif

/* -------------------------------------------------------------------- */
/* Copy results for types                                               */
/* -------------------------------------------------------------------- */
  for(i=0;i<count;i++){
    (*objtypes)[i] = hdftypes[i];
  }// endfor
  GFree(hdftypes);
  
  return TRUE;

}// endfunction: GS_ObjectGetH5Children


/************************************************************************/
herr_t getcount_it( hid_t o_id, const char *name, 
                    const H5O_info_t *object_info,void *op_data)
{
#ifdef DEBUG
  printf("GetChildren:getcount_it: name=|%s|\n",name);
#endif
  (*(int *)op_data)++;
  return 0; //success

}// endfunction: getcount_it
/************************************************************************/
herr_t getnames_it( hid_t o_id, const char *name, 
                    const H5O_info_t *object_info,void *op_data)
{

#ifdef DEBUG
  printf("GS_ObjectGetH5Children:getnames_it: name=|%s|, type=%d\n",name,object_info->type);
#endif


  // gcount is file-global
  bstring *names;
  names = (bstring *)op_data;
  names[gcount] = bfromcstr(name);

  //hdftypes is global
  hdftypes[gcount++] = object_info->type;

  return 0; //success
}// endfunction: getnames_it
