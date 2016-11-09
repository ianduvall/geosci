/* GS_ObjectIsGroupByID.c

   by Leland Pierce, Nov 19, 2013
  
   July 8, 2014:
   modified for multi-level library
   modified to use bstrings

   Aug 2, 2014
   Modified for doxygen and GS_

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


// for herr_t
#define ERROR -1
#define SUCCESS 0



/** \brief GS_ObjectIsGroupByID determines if an object-id refers to a group


\see GS_FileOpen(), GS_FileCreate()

\param[in] object_id
                  object handle in a GeoSci file.

\returns
TRUE is returned if the object is a group, FALSE otherwise.

\par Example:
Query about an image named "Channel_1"  in a file named "test123.h5".
\code
hid_t    file_id, image_id;
bstring filename = bfromcstr("test123.h5");
bstring access = bfromcstr("r+");
file_id = GS_FileOpen(filename,access);
if(file_id < 0) {
   printf("Could not open file.\n");
   bdestroy(filename);
   bdestroy(access);
   return;
}
bdestroy(filename);
bdestroy(access);
bstring channel = bfromcstr("Channel_1");
image_id = GS_ImageOpen(file_id,channel);
if(image_id < 0) {
   printf("Could not open image.\n");
   bdestroy(channel);
   return;
}
bdestroy(channel);
if(GS_ObjectIsGroupByID(image_id)){
   ... open the object as a group and use it ....
} else {
   printf("Object is not a group.\n");
} // endif
\endcode
*/





int GS_ObjectIsGroupByID(hid_t id)
{
  H5O_info_t objinfo;
  hid_t ifile_id;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* See if the Id is valid                                               */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID(id)) return FALSE;

/* -------------------------------------------------------------------- */
/* Is it a group?                                                       */
/* -------------------------------------------------------------------- */
  if(H5Iget_type(id) != H5I_GROUP){
    return FALSE;
  }// endif

  return TRUE;

}
