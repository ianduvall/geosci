/* GS_ObjectIsVectorByID.c

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



/** \brief GS_ObjectIsVectorByID determines if the object-id refers to a vector.

\anchor GS_ObjectIsVectorByID
\addindex GS_ObjectIsVectorByID


\see GS_ObjectIsVector(), IFileOpen(), GS_FileClose()

\param[in] id
                   The handle of the open vector.

\returns TRUE on success, FALSE on failure.

\par Example:
Open a vector and query if it is a valid vector.
\code
hid_t file_id;
hid_t vector1;
bstring filename = bfromcstr("/somename");
vector1 = GS_VectorOpen(file_id,filename);
bdestroy(filename);
if(vector1 < 0) {
   printf("GS_VectorOpen failed on /somename\n");
}
if(!GS_ObjectIsVectorByID(vector1)){
  printf("Object is not a vector.\n");
}
\endcode
*/


/*******************************************************************************/
int GS_ObjectIsVectorByID( hid_t id )
{
  int objtype;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");


/* -------------------------------------------------------------------- */
/* Is it a group?                                                     */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsGroupByID(id)){
    bassignformat(error_string,"GS_ObjectIsVectorByID: invalid object id).");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Get the type of group                                                */
/* -------------------------------------------------------------------- */
  objtype = GS_GetObjectType(id);

/* -------------------------------------------------------------------- */
/* If it's a Vector, return it.                                         */
/* -------------------------------------------------------------------- */
  if( objtype == GS_OBJECT_TYPE_VECTOR ||
      objtype == GS_OBJECT_TYPE_VECTOR2D ||
      objtype == GS_OBJECT_TYPE_VECTOR3D   ) {
    return TRUE;
  } else {
    return FALSE;
  }

}
