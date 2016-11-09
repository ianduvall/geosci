/* GS_FileIsOpen.c

   by Leland Pierce, April 5, 2014
  
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




/** \brief GS_FileIsOpen determines if a GeoSci datafile is already open in the current process.

\see GS_FileOpen(), GS_FileRename()

\param[in] filename
                   Name of existing file to query.

\returns
\c TRUE is returned if the file is open, \c FALSE otherwise.

\par Example
See if a file named "testimage.hd5" is open.
\code
bstring filename = bfromcstr("testimage.hd5");
if(GS_FileIsOpen( filename)){
    printf("testimage.hd5 is already open.\n");
} else {
    printf("testimage.hd5 is NOT open.\n");
}
bdestroy(filename);
\endcode

\par Details
GS_FileIsOpen() is not able to say if another process has opened
the file.
*/


int GS_FileIsOpen(const_bstring filename)
{
  int i;
  int num_objs;
  hid_t *obj_id_list;
  bstring fname;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/*       Get number of open files for this process                      */
/* -------------------------------------------------------------------- */
  num_objs = H5Fget_obj_count( H5F_OBJ_ALL, H5F_OBJ_FILE);
  obj_id_list = (hid_t *)GMalloc(num_objs*sizeof(hid_t));
  if(!obj_id_list){
    bassigncstr(error_string,"GS_FileIsOpen: GMalloc error.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/*       Get ids of all open files                                      */
/* -------------------------------------------------------------------- */
  num_objs = H5Fget_obj_ids( H5F_OBJ_ALL, H5F_OBJ_FILE, num_objs, obj_id_list);

/* -------------------------------------------------------------------- */
/*       See if any have the given filename                             */
/* -------------------------------------------------------------------- */
  for(i=0;i<num_objs;i++){
    fname = GS_GetIDName(obj_id_list[i]);
    if(EQUAL(filename,fname)) {
      bdestroy(fname);
      GFree(obj_id_list);
      return TRUE;
    }// endif
    bdestroy(fname);
  }// endfor

/* -------------------------------------------------------------------- */
/*       Clean up                                                       */
/* -------------------------------------------------------------------- */
  GFree(obj_id_list);
  return FALSE;
}
