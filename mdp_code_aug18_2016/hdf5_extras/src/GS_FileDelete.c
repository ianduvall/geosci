/* GS_FileDelete.c

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





/** \brief GS_FileDelete deletes an existing GeoSci datafile


\see GS_FileCreate(), GS_FileRename()

\param[in] filename
                   Name of existing file to delete.

\returns
\c TRUE is returned on success, \c FALSE on failure.

\par Example
Delete a file named "testimage.hd5".
\code
bstring filename = bfromcstr("testimage.hd5");
if(!GS_FileDelete( filename )){
    printf("GS_FileDelete failure.\n");
} else {
    printf("GS_FileDelete succeeded.\n");
}
bdestroy(filename);
\endcode

\par Details
GS_FileDelete() will return an error if the file is open.

*/


int GS_FileDelete(const_bstring filename)
{
  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

  /* ------------------------------------------------------------- */
  /* Check if filename is already open                             */
  /* ------------------------------------------------------------- */
  if(GS_FileIsOpen(filename)){
    bassignformat(error_string,"GS_FileDelete: file: cannot delete opened files: %s",
                  bdata(filename));
    return FALSE;
  }// endif


  if( !GS_CheckFileExistence(filename) ) {
    bassignformat(error_string,"GS_FileDelete: file does not exist: %s.", bdata(filename));
    return FALSE;
  }
  
  if( unlink(bdata(filename)) != 0 ) {
    bassignformat(error_string,"GS_FileDelete: file deletion failed: %s.", bdata(filename));
    return FALSE;
  }
  
  return TRUE;
  
}
