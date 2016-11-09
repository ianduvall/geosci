/* GS_GetValidFileID.c

   by Leland Pierce, Jan 13, 2014

   Aug 6, 2014
   Modified for bstrings, doxygen and GS_

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


#define ERROR -1


/*! \brief GS_GetValidFileID returns a valid FileID that contains the object

\param[in] id
An id for an object in a GeoSci file.

\returns
    A valid FileID is returned or a value less than 0 on error.

\par Example
Assume one has opened a file and an image, passes the image_id
into a function, and needs the file_id asociated with the image_id.
\code
hid_t image_id, file_id;
file_id = GS_GetValidFileID(image_id);
if(file_id < 0) {
   printf("Could not get valid file_id\n");
}
\endcode

\par Details
\parblock
This function either returns an \c id that is the same as the passed-in
\c id, but with an incremented reference-count, or
it returns a different \c id, also with an incremented reference-count.
The user should always \ref GS_FileClose() this \c id when done with it.
The associated file will only be closed when the reference count
reaches zero.

A typical usage of this is to make sure a passed-in \c id is a \c file_id.
In such a case, this will increment the reference count by 1, and
the user should then \ref GS_FileClose() it when done, which will NOT close
the file that the passed-in \c file_id refers to.

In another scenario, where the code needs to return a newly-opened
object in the passed-in \c file_id, one needs to be more careful.
Do not use this routine. 
Instead call \ref GS_ObjectIsFile(id) to make sure the \c id refers to a file,
and return an error if it doesn't.
\endparblock
*/

//#define DEBUG

hid_t GS_GetValidFileID(hid_t id)
{
  hid_t file_id;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("GS_GetValidFIleID: START id=%d, isvalid=%d\n",
         (int)id,GS_ValidID(id));
#endif


/* -------------------------------------------------------------------- */
/*       Is the id valid?                                               */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID(id)){
    bassigncstr(error_string,"GS_GetValidFileID: invalid id.");
    return ERROR;
  }// endif

#ifdef DEBUG
  printf("GS_GetValidFIleID: 1: id=%d, isvalid=%d\n",
         (int)id,GS_ValidID(id));
#endif


  if(H5Iget_type( id ) != H5I_FILE){
/* -------------------------------------------------------------------- */
/*       if object type for the id is NOT FILE                          */
/*       get a new file_id                                              */
/* -------------------------------------------------------------------- */
    file_id = H5Iget_file_id( id );
    if( file_id < 0) {
      bassigncstr(error_string,"GS_GetValidFileID: Can't get file_id from id.");
      return ERROR;
    }// endif

  } else {
/* -------------------------------------------------------------------- */
/*       if object type for the id is FILE                              */
/*       increment reference count for this id                          */
/* -------------------------------------------------------------------- */
    file_id = id;
    if(H5Iinc_ref(file_id) , 0){
      bassigncstr(error_string,"GS_GetValidFileID: Can't increment refcount.");
      return ERROR;
    }// endif
  }// endif

#ifdef DEBUG
  printf("GS_GetValidFIleID: 2: id=%d, isvalid=%d\n",
         (int)id,GS_ValidID(id));
#endif

  return file_id;

}
