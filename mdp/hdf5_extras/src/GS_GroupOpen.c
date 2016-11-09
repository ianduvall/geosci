/* GS_GroupOpen.c

   by Leland Pierce, Nov 16, 2013
  
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


/** \brief GS_GroupOpen opens an existing group in a GeoSci datafile

\param[in] source
                   A handle for a file or other container that
                   has a group in it.
\param[in] name
                   The name of the group to open.

\returns
The handle of the opened group is returned.
If less than zero, the open failed.

\par Example
Open the file metadata group:
\code
hid_t file_id, meta_id;
meta_id = GS_GroupOpen(file_id, bsstatic("_Header"));
if(meta_id < 0) {
  printf("Failed to open group.\n");
}
\endcode
*/


hid_t GS_GroupOpen(hid_t source, const_bstring name)
{

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Is it a group?                                                       */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsGroup(source,name)){
    bassignformat(error_string,"GS_GroupOpen: Object is not a group: %s.",bdata(name));
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Open it                                                              */
/* -------------------------------------------------------------------- */
  return H5Gopen(source,bdata(name), H5P_DEFAULT);
}
