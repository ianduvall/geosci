/* GS_ImageClose.c

   by Leland Pierce, Nov 16, 2013

   converted June 9, 2016, LEP

*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


/** \brief GS_ImageClose closes an Open Image}
\see GS_ImageOpen(), GS_FileOpen(), GS_ImageCreate()
\param[in] image_id
            Image handle of an image in a GEOSCI file.

\returns
Returns \c TRUE on success, \c FALSE on failure.
\par Example
Close an image.
\code
           ...
hid_t    image_id;

if(!GS_ImageClose(image_id)){
   printf("GS_ImageClose failed.\n");
   return;
}
          ...
\endcode
*/


//#define DEBUG

/***********************************************************************/
int GS_ImageClose( hid_t image_id)
{

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* See if the image-Id is a valid image                                 */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsImageByID(image_id)){
    bassigncstr(error_string,"GS_ImageClose: Invalid image ID.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_ImageClose 1: image_id=%d\n",(int)image_id);
#endif



/* -------------------------------------------------------------------- */
/* Close it                                                             */
/* -------------------------------------------------------------------- */
  if( GS_GroupClose(image_id) < 0){
#ifdef DEBUG
    printf("ImageClose 5: failure\n");
#endif
    return FALSE;
  } else {
#ifdef DEBUG
    printf("ImageClose 5: success\n");
#endif
    return TRUE;
  }

}
