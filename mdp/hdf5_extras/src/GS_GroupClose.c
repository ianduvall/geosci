/* GS_GroupClose.c

   by Leland Pierce, Nov 20, 2013

  
   Aug 11, 2014
   added doxygen, bstrings, GS_
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


#define ERROR -1

//#define DEBUG

/** \brief GS_GroupClose closes a group in a GeoSci Datafile

\see GS_GroupOpen()

\param[in] id  The handle for the already-open group.

\returns
\c TRUE is returned if the group is succesfully closed, \c FALSE otherwise.

\par Example
Close the file metadata group after opening it:
\code
hid_t file_id, meta_id;
bstring headername = bfromcstr("/_Header");
meta_id = GS_GroupOpen(file_id, headername);
bdestroy(headername);
if(meta_id < 0) {
  printf("Failed to open group.\n");
  return;
}
... use it here ...
GS_GroupClose(meta_id);
\endcode
*/

/*************************************************/
int GS_GroupClose(hid_t id )
{

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("GS_GroupClose: START, id=%d\n",id);
#endif
/* -------------------------------------------------------------------- */
/* Is it a group?                                                       */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsGroupByID(id)){
    bassigncstr(error_string,"GroupClose: Object is not a group.");
    return FALSE;
  }// endif
#ifdef DEBUG
  printf("GS_GroupClose: 1\n");
#endif

  // yet: do I need to worry about groups having more than 2 refcount?

/* -------------------------------------------------------------------- */
/* Close all contained objects                                          */
/* -------------------------------------------------------------------- */
  if(!GS_GroupCloseAllObjects(id)){
    bassigncstr(error_string,"GroupClose: Unable to close all contained objects.");
    return FALSE;
  }// endif
#ifdef DEBUG
  printf("GS_GroupClose: 2\n");
#endif

/* -------------------------------------------------------------------- */
/* Close it                                                             */
/* -------------------------------------------------------------------- */
  if (H5Gclose(id) < 0) return FALSE;
  return TRUE;
}
