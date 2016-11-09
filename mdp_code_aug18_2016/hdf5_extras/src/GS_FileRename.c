/* GS_FileRename.c

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





/** \brief GS_FileRename renames an existing GeoSci datafile


\see GS_FileCreateEmpty(), GS_FileCopy()

\param[in] oldname
                   Name of existing file to rename.
\param[in] newname
                   New name to be assigned to file 'oldname'.

\returns
\c TRUE is returned on success, \c FALSE on failure.

\par Example
Rename a file named "testimage.hd5" to "sirc_april.hd5".
\code
bstring oldname = bfromcstr("testimage.hd5");
bstring newname = bfromcstr("sirc_april.hd5");
if(!GS_FileRename( oldname, newname )){
    printf("GS_FileRename failure.\n");
}
bdestroy(oldname);
bdestroy(newname);
\endcode

\par Details
GS_FileRename() will return an error if the file is open, or
if a file with the same newname already exists.

*/


int GS_FileRename(const_bstring oldname, const_bstring newname)
{
  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

  /* ------------------------------------------------------------- */
  /* Check if oldname is already open                              */
  /* ------------------------------------------------------------- */
  if(GS_FileIsOpen(oldname)){
    bassignformat(error_string,"GS_FileRename: file: cannot rename opened files: %s",bdata(oldname));
    return FALSE;
  }// endif

  if( GS_CheckFileExistence( newname ) ) {
    bassignformat(error_string,"GS_FileRename: newname already exists: %s.", bdata(newname));
    return FALSE;
  }
  
  if( !GS_CheckFileExistence( oldname ) ) {
    bassignformat(error_string,"GS_FileRename: oldname does not exist: %s.", bdata(oldname));
    return FALSE;
  }
  
  if( rename( bdata(oldname), bdata(newname) ) != 0 )
    {
      bassignformat(error_string,"GS_FileRename: rename from %s to %s failed.", bdata(oldname), bdata(newname));
      return FALSE;
    }
  
  return TRUE;
}
