/* GS_ObjectIsImage.c

      by Leland Pierce, Feb 13, 2014.
   
   July 10, 2014:
   modified for multi-level library
   modified to use bstrings
 
*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


/** \brief GS_ObjectIsImage determines if the object is an image

\anchor GS_ObjectIsImage
\addindex GS_ObjectIsImage



\see GS_ImageCreate(), GS_ImageOpen()

\param[in] id
                   The handle of an object in a GeoSci file.
\param[in] object_name;  
                   Name of object to query. 
                   This name is relative to the given object.
\returns TRUE on success, FALSE on failure.

\par Example:
Query if an object is a valid image.
\code
bstring filename = bfromcstr("/somename");
if(!GS_ObjectIsImage(file_id,filename);
  printf("Object is not an image.\n");
}
bdestroy(filename);
\endcode
*/



/*********************************************************************/
int GS_ObjectIsImage(hid_t file_id, const_bstring image_name)
{

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* See if the file-Id is valid                                          */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID(file_id)){
    bassigncstr(error_string,"GS_ObjectIsImage: Invalid ID");
    return FALSE;    
  }// endif

/* -------------------------------------------------------------------- */
/* Is it a group?                                                       */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsGroup(file_id,image_name)){
    bassignformat(error_string,"GS_ObjectIsImage: Object is not an image: %s.",bdata(image_name));
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* If not an Image, return false                                        */
/* -------------------------------------------------------------------- */
  if(GS_GetGroupType(file_id, image_name) != GS_OBJECT_TYPE_IMAGE){
    return FALSE;
  }// endif

  return TRUE;
  

}
