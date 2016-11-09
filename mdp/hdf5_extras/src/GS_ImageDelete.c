/* GS_ImageDelete.c

   by Leland Pierce, Nov 19, 2013

   modified June 9, 2016, LEP

*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"

/** \brief GS_ImageDelete deletes an existing image
\see GS_ImageOpen(), GS_FileOpen(), GS_FileCreate()
\param[in] file_id;
            File handle of selected GeoSci file
\param[in] image_name   
            Name of image to delete.

\returns
\c TRUE is returned on success, \c FALSE on failure.
\par Example
Delete an image named "/Channel_1"  in a file named "test123.h5".
\code
           ...
hid_t    file_id;
bstring name = bfromcstr("test123.h5");
bstring access = bfromcstr("r+");
file_id = GS_FileOpen(name,access);
bdestroy(name);
bdestroy(access);
if(file_id < 0) {
   printf("Could not open file.\n");
   return;
}
bstring image_name = bfromcstr("/Channel_1");
if(!GS_ImageDelete(file_id,image_name)){
   printf("image deletion failed.\n");
   bdestroy(image_name);
   return;
}
bdestroy(image_name);
          ...
\endcode
*/


//#define DEBUG

//************************************************************************8
int GS_ImageDelete( hid_t file_id, const_bstring image_name)
{

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* See if the file-Id/image is valid                                    */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsImage(file_id,image_name)){
    bassigncstr(error_string,"GS_ImageDelete: Invalid Image");
    return FALSE;    
  }// endif

/* -------------------------------------------------------------------- */
/* Delete the image group                                               */
/* -------------------------------------------------------------------- */
  return GS_GroupDelete(file_id,image_name);

}
