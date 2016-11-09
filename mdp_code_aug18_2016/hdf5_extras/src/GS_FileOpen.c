/* GS_FileOpen.c

   by Leland Pierce, Nov 12, 2013.
  
   Aug 3, 2014
   modify to add bstrings, doxygen, and GS_

*/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


#define ERROR -1


/*! \brief GS_FileOpen opens a GeoSci datafile


\see GS_FileCreate(), GS_FileClose()

\param[in] dbname                               
                  The name of the GeoSci datafile to be opened.
\param[in] access
                Read/Write flag:<br>
                  "r" :      Open file for Read only<br>
                  "r+":      Open file for Read/Write

\returns
Returns the \c hid_t file handle for the file opened.  
If the GeoSci file does
not exist, a negative file handle is returned, and the 
global \c error_string is set.

\par Example
Open a file named "testimage.hd5" for read/write.  Note that the
default extension ".hd5" is tried by \ref GS_FileOpen() if the file
"testimage" does not exist.
\code
hid_t      file_id;
bstring filename= bfromcstr("testimage");
bstring access = bfromcstr("r+");
file_id = GS_FileOpen ( filename, access);
bdestroy(filename);
bdestroy(access);
  ... use the file ...
GS_FileClose (file_id);
\endcode
*/

hid_t GS_FileOpen(const_bstring dbname, const_bstring access)
{
  unsigned int flags;
  hid_t access_plist;
  hid_t file_id;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/*       Check access param                                             */
/* -------------------------------------------------------------------- */
  bstring accessr=bfromcstr("r");
  bstring accessrp = bfromcstr("r+");
  if( !EQUAL(access,accessr) && !EQUAL(access,accessrp) ){
    bassignformat(error_string,"GS_FileOpen: Invalid access string: %s.",bdata(access));
    bdestroy(accessr);
    bdestroy(accessrp);
    return ERROR;
  }// endif
  bdestroy(accessr);
  bdestroy(accessrp);
 
/* -------------------------------------------------------------------- */
/*      Check on existence of the datafile, with an optional            */
/*      .hd5 extension                                                  */
/* -------------------------------------------------------------------- */
  bstring tempname=bfromcstr("");
  bstring ending = bfromcstr("hd5");
  bassign( tempname, dbname );
  if( !GS_CheckFileExistence(tempname) ) {
    bconcat( tempname, ending );
    if( !GS_CheckFileExistence(tempname) ) {
      bassignformat(error_string,"GS_FileOpen: Can't find file: %s.",bdata(dbname));
      bdestroy(tempname);
      bdestroy(ending);
      return ERROR;
    }// endif
  }// endif
  bdestroy(ending);

/* -------------------------------------------------------------------- */
/*      Check that it is an HDF5 file.                                  */
/* -------------------------------------------------------------------- */
  char *ctempname;
  ctempname = (char *)GMalloc(sizeof(char)*(bstrlen(tempname)+10));
  strcpy(ctempname,bdata(tempname));
  bdestroy(tempname);
  if(!ctempname){
    bassignformat(error_string,"GS_FileOpen: GMalloc error.\n");
    return ERROR;
  }
  if(!H5Fis_hdf5( ctempname )){
    bassignformat(error_string,"GS_FileOpen: File (%s) is not in HDF5 format.\n",ctempname);
    GFree(ctempname);
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/*      Set access list (read/write or read-only).                      */
/* -------------------------------------------------------------------- */
  accessrp=bfromcstr("r+");
  if( EQUAL(access,accessrp) ){
    flags = H5F_ACC_RDWR;
  } else {
    flags = H5F_ACC_RDONLY;
  }// endif
  bdestroy(accessrp);
  access_plist = GS_CreateAccessPlist();

/* -------------------------------------------------------------------- */
/*      Open the file.                                                  */
/* -------------------------------------------------------------------- */
  file_id = H5Fopen( ctempname,flags,access_plist);

/* -------------------------------------------------------------------- */
/*      Clean up.                                                       */
/* -------------------------------------------------------------------- */
  GFree(ctempname);
  H5Pclose(access_plist);

/* -------------------------------------------------------------------- */
/*      Return the file_id for the open file.                                                       */
/* -------------------------------------------------------------------- */
  return file_id;
}
