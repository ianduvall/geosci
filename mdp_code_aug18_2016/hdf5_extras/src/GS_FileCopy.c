/* GS_FileCopy.c

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

int copy(const_bstring oldname, const_bstring newname);


/** \brief GS_FileCopy copies a GeoSci datafile to a new file


\see GS_FileRename()

\param[in] oldname
                   Name of existing file to copy.
\param[in] newname
                   Name of new file to create and copy into.

\returns
\c TRUE is returned on success, \c FALSE on failure.

\par Example
Copy a file named "testimage.hd5" to "sirc_april.hd5".
\code
bstring filename1 = bfromcstr("testimage.hd5");
bstring filename2 = bfromcstr(sirc_april.hdf5");
if(!GS_FileCopy( filename1, filename2 )){
    printf("GS_FileCopy failure.\n");
}
bdestroy(filename1);
bdestroy(filename2);
\endcode

\par Details
GS_FileCopy() will return an error if the file is open, or
if a file with the same destination name already exists.
*/


int GS_FileCopy(bstring oldname, bstring newname)
{
  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

  /* ------------------------------------------------------------- */
  /* Check if either file is already open                          */
  /* ------------------------------------------------------------- */
  if(GS_FileIsOpen(oldname)){
    bassignformat(error_string,"GS_FileCopy: cannot copy opened files: %s",bdata(oldname));
    return FALSE;
  }// endif

  if(GS_FileIsOpen(newname)){
    bassignformat(error_string,"GS_FileCopy: cannot copy opened files: %s",bdata(newname));
    return FALSE;
  }// endif


  /* ------------------------------------------------------------- */
  /* Make sure oldname exists                                      */
  /* ------------------------------------------------------------- */
  if(!GS_CheckFileExistence(oldname)){
    bassignformat(error_string,"GS_FileCopy: cannot copy non-existent file: %s",bdata(oldname));
    return FALSE;
  }

  /* ------------------------------------------------------------- */
  /* Make sure newname does not exist                              */
  /* ------------------------------------------------------------- */
  if(GS_CheckFileExistence(oldname)){
    bassignformat(error_string,"GS_FileCopy: will not overwrite existing file: %s",bdata(newname));
    return FALSE;
  }


  return copy(oldname, newname);


}
/************************************************************************/
int copy(const_bstring oldname, const_bstring newname)
{
  FILE *ifp;
  FILE *ofp;
  int c;

  // returns TRUE on success

  ifp=fopen (bdata(oldname),"r");
  if(!ifp) return FALSE;

  ofp=fopen (bdata(newname),"w+");
  if(!ofp) {
    fclose(ifp);
    return FALSE;
  }

  while( (c=fgetc(ifp)) != EOF) {
    if(fputc(c,ofp) == EOF || ferror(ofp) || ferror(ifp) ) {
      fclose(ifp);
      fclose(ofp);
      return FALSE;
    }// endif
  }// endwhile

  // success
  fclose(ifp);
  fclose(ofp);
  return TRUE;

}// endfunction: copy()
