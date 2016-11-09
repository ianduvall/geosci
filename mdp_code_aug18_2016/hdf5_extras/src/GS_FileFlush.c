/* GS_FileFlush.c

   by Leland Pierce, Jan 4, 2014

   Aug 3, 2014
   Modified for bstrings, doxygen and GS_

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"



/** \brief GS_FileFlush forces the datafile to be updated

GS_FileFlush() forces all changes to a file to be flushed to the
hard drive, forcing them to occur rather than waiting in a "buffered" 
state.

\see GS_FileOpen(), GS_FileClose()

\param[in] object_id
                   ID of a file, an image, or any other valid object
                   in a GeoSci  datafile. 

\returns
Returns \c TRUE on success, \c FALSE otherwise.

\par Example
After writing to an image, make sure all changes are written to disk.
\code
hid_t image_id;
bstring image_name=bfromcstr("Image3");
image_id=GS_ImageOpen(file_id,image_name);
bdestroy(image_name);
     ... modify the image ....
GS_FileFlush(image_id);
\endcode
Note that we could have used file_id as well.
*/

/**********************************************************************/

int GS_FileFlush(hid_t object_id)
{

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* See if the object-Id is valid                                        */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID(object_id)){
    bassigncstr(error_string,"GS_FileFlush: invalid id.");
    return FALSE;
  }// endif


/* -------------------------------------------------------------------- */
/*      Flush any pending updates                                       */
/* -------------------------------------------------------------------- */
  if( H5Fflush(object_id, H5F_SCOPE_GLOBAL) < 0){
    bassigncstr(error_string,"GS_FileFlush: H5Fflush() failure.");
    return FALSE;
  } else {
    return TRUE;
  }// endif

}
