/* GS_ObjectGetGSChildren.c

   by Leland Pierce, March 30, 2014

   modified JUne 20, 2016

*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"



herr_t GSgetcount_it( hid_t o_id, const char *name, 
                    const H5O_info_t *object_info,void *op_data);

herr_t GSgetnames_it( hid_t o_id, const char *name, 
                    const H5O_info_t *object_info,void *op_data);

int gcount;
hid_t gobj_id;
int *gsptypes;



/** \brief GS_ObjectGetGSChildren returns list of GeoSci children objects
\param[in] object_id
            handle of an open object.
            The names of the children of this object, if any, 
            are returned.
\param[out] nobjs
            The number of objects (children) found.
\param[out] objnames
            The returned list of object names.
            when done, bdestroy() each element, and GFree the container.
\param[out] objtypes
            The returned list of object types.\n
            GFree it when done.\n
            Valid values are:\n
             GS_OBJECT_TYPE_METADATA_GROUP 1\n
             GS_OBJECT_TYPE_IMAGE    2\n
             GS_OBJECT_TYPE_VECTOR   3\n
             GS_OBJECT_TYPE_VECTOR2D 4\n
             GS_OBJECT_TYPE_VECTOR3D 5\n
             GS_OBJECT_TYPE_TIN      6\n
             GS_OBJECT_TYPE_MESH2D   7\n
             GS_OBJECT_TYPE_MESH3D   8\n
\returns
The routine returns TRUE on success, FALSE on failure.
\par Example
Obtain list of objects in a file named "testimage.hd5".
\code
    ...
bstring *objnames;
int objtypes;
int nobjs;
int i;

file_id = GS_FileOpen(bsstatic("testimage.hd5"),bsstatic("r+"));
if(file_id<0){
   printf("GS_FileOpen failed\n");
   printf("%s\n",bdata(error_string));
   exit(-1);
}//endif

if(!GS_ObjectGetGSChildren(file_id, &nobjs, &objnames,&objtypes)){
   printf("GS_ObjectGetGSChildren failed\n");
   printf("%s\n",bdata(error_string));
   exit(-1);
}

// print results:
for(i=0;i<nobjs;i++){
   printf("obj# %d: %s (%s)\n",i,bdata(objnames[i]),
           bdata(GS_GSObjectTypeAsString(objtypes[i])) );
}

// clean up:
for(i=0;i<nobjs;i++){
   bdestroy(objnames[i]);
}
GFree(objnames);
GFree(objtypes);
\endcode

*/


//#define DEBUG

/**********************************************************************/
int GS_ObjectGetGSChildren( hid_t object_id, int *nobjs, 
                 bstring **objnames, int **objtypes)
{
  int i;
  int count;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

  gobj_id = object_id;
/* -------------------------------------------------------------------- */
/* See if the object-id is valid                                        */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID(object_id)){
    bassigncstr(error_string,"GS_ObjectGetGSChildren: Invalid Object ID");
    return FALSE;    
  }// endif

/* -------------------------------------------------------------------- */
/* Recursively visit all the children                                   */
/* count them.                                                          */
/* -------------------------------------------------------------------- */
  count = 0;
  if(H5Ovisit( object_id, H5_INDEX_NAME, H5_ITER_INC,
               GSgetcount_it, &count ) <0){
      bassigncstr(error_string,"GS_ObjectGetGSChildren: H5Ovisit error(1)");
      return FALSE;    
  }// endif

/* -------------------------------------------------------------------- */
/* GMalloc space for the names                                          */
/* -------------------------------------------------------------------- */
  *nobjs = count;
  *objnames = (bstring *)GMalloc(count*sizeof(bstring));
  if(!(*objnames)){
    bassignformat(error_string,"GS_ObjectGetGSChildren: GMalloc error(1)");
    return FALSE;    
  }// endif


/* -------------------------------------------------------------------- */
/* GMalloc space for the types                                          */
/* -------------------------------------------------------------------- */
  gsptypes = (int *)GMalloc(count*sizeof(int));
  if(!gsptypes){
    bassignformat(error_string,"GS_ObjectGetGSChildren: HMalloc error(3)");
    return FALSE;    
  }// endif

/* -------------------------------------------------------------------- */
/* Recursively visit all the children                                   */
/* get the names                                                        */
/* -------------------------------------------------------------------- */
  gcount=0;
  if(H5Ovisit( object_id, H5_INDEX_NAME, H5_ITER_INC,
               GSgetnames_it, *objnames) <0) {
      bassigncstr(error_string,"GS_ObjectGetGSChildren: H5Ovisit error(2)");
      return FALSE;    
  }// endif

/* -------------------------------------------------------------------- */
/* GMalloc space for the types                                          */
/* -------------------------------------------------------------------- */
  *objtypes = (int *)GMalloc(count*sizeof(int));
  if(!(*objtypes)){
    bassigncstr(error_string,"GS_ObjectGetGSChildren: GMalloc error(4)");
    return FALSE;    
  }// endif

/* -------------------------------------------------------------------- */
/* Copy results for types                                               */
/* -------------------------------------------------------------------- */
  for(i=0;i<count;i++){
    (*objtypes)[i] = gsptypes[i];
  }// endfor
  GFree(gsptypes);
  
  return TRUE;

}// endfunction: GetGSPChildren


/************************************************************************/
herr_t GSgetcount_it( hid_t o_id, const char *name, 
                    const H5O_info_t *object_info,void *op_data)
{
#ifdef DEBUG
  printf("GS_ObjectGetGSChildren:getcount_it: name=|%s|\n",name);
#endif
  if(object_info->type == H5O_TYPE_GROUP &&
     !EQUALC(name,".") ){
    
    (*(int *)op_data)++;
  }
  if(object_info->type == H5O_TYPE_DATASET ) {
    (*(int *)op_data)++;
  }
  return 0; //success

}// endfunction: getcount_it
/************************************************************************/
herr_t GSgetnames_it( hid_t o_id, const char *name, 
                    const H5O_info_t *object_info,void *op_data)
{

#ifdef DEBUG
  printf("GS_ObjectGetGSChildren:getnames_it: name=|%s|, type=%d\n",
         name,object_info->type);
#endif


  // gcount is file-global
  bstring *names;

  if(object_info->type == H5O_TYPE_GROUP &&
     !EQUALC(name,".") ){
  names = (bstring *)op_data;
  names[gcount] = bfromcstr(name);




  hid_t group_id;
  int objtype;

  bstring bname = bfromcstr(name);
/* -------------------------------------------------------------------- */
/* Open the group                                                       */
/* -------------------------------------------------------------------- */
  group_id = GS_GroupOpen(gobj_id, bname);
  bdestroy(bname);
  if(group_id < 0){
    bassignformat(error_string,"GS_ObjectGetGSChildren:getnames_it: Can't open the group: %s.",name);
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_ObjectGetGSChildren:getnames_it: o_id=%d, group_id=%d\n",o_id, group_id);
#endif

/* -------------------------------------------------------------------- */
/* Get the typestring of group                                          */
/* -------------------------------------------------------------------- */
  objtype = GS_GroupGetType(group_id);
#ifdef DEBUG
  printf("GetGSPChildren:getnames_it: objtype=%d\n",objtype);
#endif


/* -------------------------------------------------------------------- */
/* Clean up                                                             */
/* -------------------------------------------------------------------- */
  GS_GroupClose(group_id);

    //gsptypes, gobj_id are both global
    gsptypes[gcount++] = objtype;

  }// endif GROUP






  if(object_info->type == H5O_TYPE_DATASET){ 
  names = (bstring *)op_data;
  names[gcount] = bfromcstr(name);




  hid_t dataset_id;
  int objtype;

  bstring bname = bfromcstr(name);
/* -------------------------------------------------------------------- */
/* Open the group                                                       */
/* -------------------------------------------------------------------- */
  dataset_id = GS_DatasetOpen(gobj_id, bname);
  bdestroy(bname);
  if(dataset_id < 0){
    bassignformat(error_string,"GS_ObjectGetGSChildren:getnames_it: Can't open the group: %s.",name);
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_ObjectGetGSChildren:getnames_it: o_id=%d, dataset_id=%d\n",o_id, dataset_id);
#endif

/* -------------------------------------------------------------------- */
/* Get the typestring of dataset                                        */
/* -------------------------------------------------------------------- */
  objtype = GS_DatasetGetType(dataset_id);

#ifdef DEBUG
  printf("GetGSPChildren:getnames_it: objtype=%d\n",objtype);
#endif


/* -------------------------------------------------------------------- */
/* Clean up                                                             */
/* -------------------------------------------------------------------- */
  GS_DatasetClose(dataset_id);

    //gsptypes, gobj_id are both global
    gsptypes[gcount++] = objtype;

  }// endif DATASET









  return 0; //success
}// endfunction: getnames_it
