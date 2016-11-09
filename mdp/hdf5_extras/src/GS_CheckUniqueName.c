/* GS_CheckUniqueName.c

   by Leland Pierce, Nov 17, 2013

*/

/** \brief GS_CheckUniqueName check if name is unique
\param[in]object_id
                   Handle of an already-open GeoSci object.

\param[in] name
 Name of the object one wishes to test.
 Must be a relative pathname, relative to the
 object in "object_id". If object_id is a file,
 then this can be an absolute name.
 For example: /groupa/group1/datasetw
\returns
Returns TRUE if the name is unique, FALSE otherwise.
\par Example
 Test if the name "/Channel_3" already exists or not:
\code
hid_t       file_id;

if(!GS_CheckUniqueName(file_id,bsstatic("/Channel_3") ) ){
   printf("Name is already in use.\n");
}// endif
\endcode
*/

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"

/**********************************************************************/
int GS_CheckUniqueName(hid_t object_id, const char *thename)
{
  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Special case of '/'                                                  */
/* -------------------------------------------------------------------- */
  if(strcmp(thename,"/")==0) {
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Get a valid file_id                                                  */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID(object_id)){
    bassignformat(error_string,"CheckUniqueName: Invalid object_id.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* See if this name exists in the file                                  */
/* -------------------------------------------------------------------- */
  if((int)H5Lexists( object_id, thename, H5P_DEFAULT )==TRUE){
    //printf("checkUniqueName: 2.2\n");
    bassignformat(status_string,"CheckUniqueName: Found a matching name.");
    return FALSE;
  } else {
    //printf("checkUniqueName: 2.3\n");
    return TRUE;
  }// endif

}
