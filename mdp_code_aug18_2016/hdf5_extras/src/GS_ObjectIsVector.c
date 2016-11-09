/* GS_ObjectIsVector.c

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



/** \brief GS_ObjectIsVector determines if the object is a vector.

\anchor GS_ObjectIsVector
\addindex GS_ObjectIsVector

\see GS_ObjectIsVectorByID(), IFileOpen(), GS_FileClose()

\param[in] id
                   object handle of a file or another group
                   in the selected GeoSci file.
\param[in] object_name   
                   Name of object to query. 
                   This name is relative to the given object.

\returns TRUE on success, FALSE on failure.

\par Example:
Query if an object is a valid vector.
\code
bstring name = bfromcstr("/somename");
if(!GS_ObjectIsVector(file_id,name)){
  printf("Object is not a vector.\n");
}
\endcode
*/



/*******************************************************************************/
int GS_ObjectIsVector( hid_t id, const_bstring name )
{
  int objtype;
  hid_t group_id;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");


/* -------------------------------------------------------------------- */
/* Is it a valid id?                                                    */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID(id)){
    bassignformat(error_string,"GS_ObjectIsVector: invalid object id.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Does the specified name exist?                                       */
/* -------------------------------------------------------------------- */
  bstring bname = bstrcpy(name);
  if(!GS_CheckInternalName(id, bname)){
    bassignformat(error_string,"GS_ObjectIsVector: name: %s does not exist.",name);
    bdestroy(bname);
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Is it a group?                                                     */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsGroup(id,bname)){
    bassignformat(error_string,"GS_ObjectIsVectorByID: object is not a group.");
    bdestroy(bname);
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Open it                                                              */
/* -------------------------------------------------------------------- */
  group_id=GS_GroupOpen(id,bname);
  bdestroy(bname);
  if(group_id<0){
    bassignformat(error_string,"GS_ObjectIsVectorByID: can't open object.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Get the type of group                                                */
/* -------------------------------------------------------------------- */
  objtype = GS_GetObjectType(group_id);

/* -------------------------------------------------------------------- */
/* Close the vector group                                               */
/* -------------------------------------------------------------------- */
  if(!GS_GroupClose(group_id)){
    bassignformat(error_string,"GS_ObjectIsVectorByID: can't close object.");
    return FALSE;
  }// endif

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

