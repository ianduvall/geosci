/* GS_ObjectIsImageByID.c

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


/** \brief GS_ObjectIsImageByID determines if the object is an image

\anchor GS_ObjectIsImageByID
\addindex GS_ObjectIsImageByID

\see GS_ImageCreate(), GS_ImageOpen()


\param[in] id;
        The handle of the open objbect.

\returns TRUE on success, FALSE on failure.

\par Example:
Query if an object is a valid image.
\code
hid_t file_id;
hid_t image1;
bstring filename = bfromcstr("/somename");
image1 = GS_ImageOpen(file_id,filename);
bdestroy(filename);
if(image1 < 0) {
   printf("GS_ImageOpen failed on /somename\n");
}
if(!GS_ObjectIsImageByID(image1)){
  printf("Object is not an image.\n");
}
\endcode
*/



/*******************************************************************************/
int GS_ObjectIsImageByID( hid_t id )
{
  int objtype;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Is it a group?                                                     */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsGroupByID(id)){
    bassigncstr(error_string,"GS_ObjectIsImageByID: invalid object id.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Get the type of group                                                */
/* -------------------------------------------------------------------- */
  objtype = GS_GetObjectType(id);

/* -------------------------------------------------------------------- */
/* If it's an Image, return it.                                         */
/* -------------------------------------------------------------------- */
  if(objtype == GS_OBJECT_TYPE_IMAGE){
    return TRUE;
  } else {
    return FALSE;
  }// endif

}
