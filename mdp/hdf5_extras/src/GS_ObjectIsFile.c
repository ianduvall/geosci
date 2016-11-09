/* GS_ObjectIsFile.c

   by Leland Pierce, Jan 3, 2014

  
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




/** \brief GS_ObjectIsFile determines if an object-id refers to a file

\see GS_FileOpen(), GS_FileCreate()

\param[in] id
                   Object handle of selected GeoSci file.

\returns
TRUE is returned if the object is a file, FALSE otherwise.

\par Example:
Query about an image that was already opened:
\code
C       hid_t    image_id;
C       if(GS_ObjectIsFile(image_id)){
C          ... the object is a file: use it...
C       } else {
C          printf("Object is not a file.\n");
C       } // endif
\endcode
*/



int GS_ObjectIsFile(hid_t id)
{

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Check for valid id                                                   */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID( id )){
    bassigncstr(error_string,"GS_ObjectIsFile: invalid handle\n");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Check it's type                                                      */
/* -------------------------------------------------------------------- */
  if(H5Iget_type( id ) != H5I_FILE){
    return FALSE;
  }// endif

  return TRUE;
}
