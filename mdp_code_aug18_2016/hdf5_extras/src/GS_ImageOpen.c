/* GS_ImageOpen.c

   by Leland Pierce, Nov 16, 2013

   revised June 9, 2016, LEP

*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"

/** \brief GS_ImageOpen opens an existing image
\see  GS_FileOpen(), GS_ImageCreate(), GS_ImageClose()
\param[in] file_id
            File handle of selected GeoSci file.
\param[in] image_name  
            Name of image to open.
\returns
A handle to the image is returned on success, while a value
less than 0 is returned on failure.
\par Example
Open an image named "Channel_1"  in a file named "test123.h5".
\code
           ...
hid_t    file_id, image_id;
bstring name = bfromcstr("test123.h5");
bstring access = bfromcstr("r+");
file_id = GS_FileOpen(name, access);
bdestroy(name);
bdestroy(access);
if(file_id < 0) {
   printf("Could not open file.\n");
   return;
}
bstring image_name = bfromcstr("/Channel_1");
image_id = GS_ImageOpen(file_id,image_name);
bdestroy(image_name);
if(image_id < 0) {
   printf("Could not open image.\n");
   return;
}
          ...
\endcode
*/

/*******************************************************************/
hid_t	GS_ImageOpen( hid_t file_id, const_bstring image_name)
{
  hid_t image_id;
  int objtype;
  hid_t ifile_id;
  char *cname;
  bstring imname = bstrcpy(image_name);

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");


  //printf("ImageOpen: 2\n");

/* -------------------------------------------------------------------- */
/* Check that object is a file                                          */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsFile(file_id)){
    bassigncstr(error_string,"GS_ImageOpen: Invalid file_id.");
    bdestroy(imname);
    return FALSE;
  }// endif

  //printf("ImageOpen: 3\n");


/* -------------------------------------------------------------------- */
/* See if this name exists in the file                                  */
/* -------------------------------------------------------------------- */
  cname = (char *)GMalloc(sizeof(char)*bstrlen(imname));
  if(!cname) {
    bassignformat(error_string,"GS_ImageOpen: GMalloc error on name:%s.",bdata(imname));
    bdestroy(imname);
     return ERROR;
  }
  strcpy(cname,bdata(imname));
  if(H5Lexists( file_id, cname, H5P_DEFAULT )==TRUE){
  } else {
    bassignformat(error_string,"GS_ImageOpen: Name: %s does not exist in file.",bdata(imname));
    bdestroy(imname);
    GFree(cname);
     return ERROR;
  }// endif
  GFree(cname);

  //printf("ImageOpen: 4\n");

/* -------------------------------------------------------------------- */
/* Is it a group?                                                       */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsGroup(file_id,imname)){
    bassignformat(error_string,"GS_ImageOpen: Object is not an image: %s.",bdata(imname));
    bdestroy(imname);
     return ERROR;
  }// endif

  //printf("ImageOpen: 5\n");

/* -------------------------------------------------------------------- */
/* Open the group                                                       */
/* -------------------------------------------------------------------- */
  image_id = GS_GroupOpen(file_id, image_name);
  if(image_id < 0){
    bassignformat(error_string,"ImageOpen: Can't open the image: %s.",bdata(imname));
    bdestroy(imname);
     return ERROR;
  }// endif

  //printf("ImageOpen: 6\n");

/* -------------------------------------------------------------------- */
/* Get the type of group                                                */
/* -------------------------------------------------------------------- */
  objtype = GS_GetObjectType(image_id);

  //printf("ImageOpen: 7\n");

/* -------------------------------------------------------------------- */
/* If it's an Image, return it.                                         */
/* -------------------------------------------------------------------- */
  if(objtype == GS_OBJECT_TYPE_IMAGE){
    bdestroy(imname);
     return image_id;

  } else {
    GS_GroupClose(image_id);
    bassignformat(error_string,"ImageOpen: Not an image: imagename: %s.",bdata(imname));
    //printf("ImageOpen: 10\n");
    bdestroy(imname);
     return ERROR;
  }

  //printf("ImageOpen: 11\n");

}
