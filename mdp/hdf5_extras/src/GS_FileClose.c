/* GS_FileClose.c

   by Leland Pierce, Nov 12, 2013.
  
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



/*! \brief GS_FileClose closes a GeoSci datafile


\see GS_FileOpen()

\param[in] file_id
               File handle for GeoSci file to be closed.

\returns
\c TRUE is returned on success, while \c FALSE is returned on failure.


\par Example
This example opens and then closes a standard GeoSci file:
\code
hid_t   file_id;
bstring filename = bfromcstr("testimage.hd5");
bstring access = bfromcstr("r+");
file_id = GS_FileOpen( filename, access );
bdestroy(filename);
bdestroy(access);
  ... use the file ...
GS_FileClose( file_id );
\endcode

*/

//#define DEBUG

int GS_FileClose(hid_t file_id)
{

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("GS_FileClose: 1\n");
#endif
/* -------------------------------------------------------------------- */
/*      Is the id valid?                                                */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID(file_id)){
    bassigncstr(error_string,"GS_FileClose: Invalid ID.");
    return FALSE;
  }// endif
#ifdef DEBUG
  printf("GS_FileClose: 2\n");
#endif

/* -------------------------------------------------------------------- */
/*      Is it a FILE ID?                                                */
/* -------------------------------------------------------------------- */
  if(H5Iget_type( file_id ) != H5I_FILE){
    bassigncstr(error_string,"GS_FileClose: Not a valid FILE ID.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_FileClose: 3\n");
#endif

/* -------------------------------------------------------------------- */
/*      Flush any pending updates                                       */
/* -------------------------------------------------------------------- */
  if(!GS_FileFlush(file_id)){
    bassigncstr(error_string,"GS_FileClose: Can't flush file.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_FileClose: 4\n");
#endif

/* -------------------------------------------------------------------- */
/*      Close all open objects in this file first                       */
/*      but only if we are really closing the file                      */
/*      if refcount >1, then we aren't yet closing this file            */
/* -------------------------------------------------------------------- */
  if(H5Iget_ref(file_id)==1) {
  if(!GS_FileCloseAllObjects(file_id)){
    bassigncstr(error_string,"GS_FileClose: Can't close all objects in file.");
    return FALSE;
  }// endif
  }//endif

#ifdef DEBUG
  printf("GS_FileClose: 5\n");
#endif

/* -------------------------------------------------------------------- */
/*      Close the file                                                  */
/* -------------------------------------------------------------------- */
  if(H5Fclose(file_id) < 0){
#ifdef DEBUG
  printf("GS_FileClose: 6\n");
#endif
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_FileClose: 7\n");
#endif

  return TRUE;
}
