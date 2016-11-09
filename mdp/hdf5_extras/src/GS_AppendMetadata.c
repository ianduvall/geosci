/* GS_AppendMetadata.c

   by Leland Pierce, Jan 3, 2014

   July 4, 2014:
   modified for multi-level library
   modified to use GetStringAttribute and SetStringAttribute

   July 7, 2014:
   modified to use bstrings

   July 23, 2014:
   modified to use doxygen documentation.

*/

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"
#include "string_funcs.h"


//#define DEBUG

/** \brief GS_AppendMetadata appends the given string to the metadata item.

\anchor GS_AppendMetadata
\addindex GS_AppendMetadata

\see GS_UpdateMetadata()

\param[in] object_id 
                  ID of a file, and image, or any other valid object
                  in a GeoSci file. 
\param[in] metadata_name
                   The name of the metadata item to update.
\param[in] value
                   This string is appended to the metadata item
                   for the given object.
 
\returns \c TRUE on success, \c FALSE on failure.

\par Example:
After writing to an image, update it's history.
\code
hid_t image_id;
bstring name = bfromcstr("Image3");
image_id=GS_ImageOpen(file_id,name);
bdestroy(name);
     ... modify the image ....
bstring history=bfromcstr("hsitory");
bstring history_value = bfromcstr("Task_name(task_param1,task_param2)");
GS_AppendMetadata(image_id,history,history_value);
bdestroy(history);
bdestroy(history_value);
\endcode

\par Details:
A special case is when writing a "history" metadata item.
In this case, if the last line of the history contains "ABORTED",
we remove this last line, and append the new line.
In essence this ends up "over-writing" the "ABORTED" line.
The reason for this is to have an aborted message when there is a
failure, but no aborted message on success.

*/



/**********************************************************************/
int GS_AppendMetadata(hid_t object_id, const_bstring metadata_name, 
                      const_bstring value)
{
  bstring old_value    =bfromcstr("");
  bstring new_value    =bfromcstr("");
  bstring history_front=bfromcstr("");
  bstring history_back =bfromcstr("");
  bstring history_use  =bfromcstr("");
  int L;
  int object_type;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");


#ifdef DEBUG
  printf("GS_AppendMetadata: start: name=%s, value=%s\n",bdata(metadata_name), 
         bdata(value));
#endif

/* -------------------------------------------------------------------- */
/* See if the object-Id is valid                                        */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID(object_id)){
#ifdef DEBUG
  printf("GS_AppendMetadata: invalid object_id=%d\n",(int)object_id);
#endif
    bassignformat(error_string,"GS_AppendMetadata: invalid object_id.");
    bdestroy(old_value);
    bdestroy(new_value);
    bdestroy(history_front);
    bdestroy(history_back);
    bdestroy(history_use);
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_AppendMetadata: 0.5: object_id=%d\n",(int)object_id);
#endif


/* -------------------------------------------------------------------- */
/*	Get object type.                                                */
/* -------------------------------------------------------------------- */
  object_type = GS_GetObjectType(object_id);
  if(!object_type) {
    bstring dataset_type=bfromcstr("dataset_type");
    bstring grouptype = bfromcstr("grouptype");
    if(EQUAL(metadata_name,dataset_type)){
      object_type = GS_OBJECT_TYPE_UNDER_CONSTRUCTION;
    } else if(EQUAL(metadata_name,grouptype)){
      object_type = GS_OBJECT_TYPE_UNDER_CONSTRUCTION;
    }// endif
    bdestroy(dataset_type);
    bdestroy(grouptype);
  }// endif
  if(!object_type) {
    bassignformat(error_string,"GS_AppendMetadata: metadata object-type invalid\n");
    bdestroy(old_value);
    bdestroy(new_value);
    bdestroy(history_front);
    bdestroy(history_back);
    bdestroy(history_use);
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/*	Make sure a valid metadata item-name                            */
/* -------------------------------------------------------------------- */
  if(!GS_ValidMetadataName(object_type,metadata_name)){
    bassignformat(error_string,"GS_AppendMetadata: metadata item-name invalid: %s\n",
                  bdata(metadata_name));
    bdestroy(old_value);
    bdestroy(new_value);
    bdestroy(history_front);
    bdestroy(history_back);
    bdestroy(history_use);
    return FALSE;
  }// endif


/* -------------------------------------------------------------------- */
/*	Get the old value.                                              */
/* -------------------------------------------------------------------- */
  if( !GS_GetStringAttribute( object_id, metadata_name, old_value)) {
    bassignformat(error_string,"GS_AppendMetadata: Can't get attribute '%s'.",
                  bdata(metadata_name));
    bdestroy(old_value);
    bdestroy(new_value);
    bdestroy(history_front);
    bdestroy(history_back);
    bdestroy(history_use);
    return FALSE;
  }

#ifdef DEBUG
  printf("GS_AppendMetadata: 1, old_value=|%s|\n",bdata(old_value));
#endif


/* -------------------------------------------------------------------- */
/*	If metadata_name = history and                                  */
/*         last line has "ABORTED" in it, delete the last line          */
/* -------------------------------------------------------------------- */
  bstring history = bfromcstr("history");
  if(EQUALN(metadata_name,history,7)){
#ifdef DEBUG
 printf("GS_AppendMetadata: 1.1\n");
#endif
    if(!GS_SplitHistory(old_value, history_front, history_back)){
      bassignformat(error_string,"GS_AppendMetadata: SplitHistory failure");
      bdestroy(old_value);
      bdestroy(new_value);
      bdestroy(history_front);
      bdestroy(history_back);
      bdestroy(history_use);
      bdestroy(history);
      return FALSE;
    }// endif
#ifdef DEBUG
    printf("GS_AppendMetadata: 1.2: front=|%s|, back=|%s|\n",
           bdata(history_front),bdata(history_back));
#endif
    bstring aborted=bfromcstr("ABORTED");
    if(binstr(history_back,0,aborted) != BSTR_ERR){
#ifdef DEBUG
 printf("GS_AppendMetadata: 1.2.1: found ABORTED\n");
#endif

      bassign(history_use, history_front);
    } else {
#ifdef DEBUG
 printf("GS_AppendMetadata: 1.2.2\n");
#endif
       bassign(history_use, old_value);
#ifdef DEBUG
 printf("GS_AppendMetadata: 1.2.3\n");
#endif
    }// endif
    bdestroy(aborted);
#ifdef DEBUG
 printf("GS_AppendMetadata: 1.3\n");
#endif

  }// endif
  bdestroy(history);

  bdestroy(old_value);
  bdestroy(history_front);
  bdestroy(history_back);



#ifdef DEBUG
 printf("GS_AppendMetadata: 2\n");
 printf("                   history_use=|%s|\n",bdata(history_use));
 printf("                   value=|%s|\n",bdata(value));
#endif

/* -------------------------------------------------------------------- */
/*	Append the new value to the old value.                          */
/* -------------------------------------------------------------------- */
  bassign(new_value,history_use);
#ifdef DEBUG
  printf("                  1: new_value=|%s|\n",bdata(new_value));
#endif
  bconcat(new_value,value);
#ifdef DEBUG
  printf("                  2: new_value=|%s|\n",bdata(new_value));
#endif
  bdestroy(history_use);

#ifdef DEBUG
  printf("GS_AppendMetadata: 3, new_value=|%s|\n",bdata(new_value));
#endif

  
/* -------------------------------------------------------------------- */
/*	Write the new value.                                            */
/* -------------------------------------------------------------------- */
  if( !GS_SetStringAttribute( object_id, metadata_name, new_value) ) {
    bassignformat(error_string,"GS_AppendMetadata: Can't set attribute '%s'.",
                  bdata(metadata_name));
    bdestroy(new_value);
    return FALSE;
  }
  bdestroy(new_value);

#ifdef DEBUG
 printf("GS_AppendMetadata: 4\n");
#endif


  return TRUE;
}
