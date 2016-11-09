/* GS_GroupCreate.c

   by Leland Pierce, Jan 20, 2014.
  
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


/** \brief GS_GroupCreate creates a new group in a GeoSci Datafile


\see GS_FileOpen(), GS_GroupDelete()

\param[in] file_id
                   A handle for an already-open GeoSci datafile.
\param[in] groupname                               
                  The name of the group in an existing GeoSci
                  datafile to be created.
                  Use Unix/Web filenaming conventions, giving the full 
                  pathname, starting with "/".
                  All but the last component of this name must already
                  exist.

\returns

A valid handle to the new group is returned on success.
A negative value is returned on failure.

\par Example
Create a group named "/group3" in an already-open GeoSci file.
\code
hid_t file_id, group_id;
bstring groupname = bfromcstr("/group3");
group_id = GS_GroupCreate(file_id,groupname);
bdestroy(groupname);
if(group_id < 0 ){
  printf("GS_GroupCreate failure\n");
  return;
}
.... use it here ....
GS_GroupClose(group_id);
\endcode

*/

//#define DEBUG

hid_t GS_GroupCreate(hid_t file_id, const_bstring groupname)
{
  hid_t ifile_id;
  hid_t group_id;
  bstring dir=bfromcstr("");
  bstring tempstr=bfromcstr("");

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("GS_GroupCreate: 1, groupname=|%s|\n",bdata(groupname));
#endif
  
/* -------------------------------------------------------------------- */
/* Check that file_id is a file                                         */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsFile(file_id)){
    bassigncstr(error_string,"GS_GroupCreate: invalid file id).");
    bdestroy(dir);
    bdestroy(tempstr);
    return ERROR;
  }// endif


#ifdef DEBUG
  printf("GS_GroupCreate: 2\n");
#endif

/* -------------------------------------------------------------------- */
/* Check that file_id is writeable                                      */
/* -------------------------------------------------------------------- */
  if(!GS_FileIsWriteable(file_id)){
    tempstr = GS_GetIDName(file_id);
    bassignformat(error_string,"GS_GroupCreate: File cannot be written to: %s\n",bdata(tempstr));
    bdestroy(tempstr);
    bdestroy(dir);
    return ERROR;
  }// endif

#ifdef DEBUG
  printf("GS_GroupCreate: 3\n");
#endif

/* -------------------------------------------------------------------- */
/* Groupname must start with a "/"                                      */
/* -------------------------------------------------------------------- */
  if(bchar(groupname, 0) != '/'){
    bassignformat(error_string,"GS_GroupCreate: invalid groupname: %s: must start with '/'.",
                  bdata(groupname));
    bdestroy(dir);
    return ERROR;
  }// endif

#ifdef DEBUG
  printf("GS_GroupCreate: 4\n");
#endif

/* -------------------------------------------------------------------- */
/* Does path to this object already exist?                              */
/* -------------------------------------------------------------------- */
  dir = GS_PathnameGetDir(groupname);
#ifdef DEBUG
  printf("GS_GroupCreate: 4.1, dir=|%s|\n",bdata(dir));
#endif
  if(bstrlen(dir)>0){
    if(!GS_CheckInternalName(file_id,dir)){
      bassignformat(error_string,"GS_GroupCreate: group-path: %s, does not exist.",bdata(dir));
      bdestroy(dir);
      return ERROR;
    }// endif
  }//endif
  bdestroy(dir);

#ifdef DEBUG
  printf("GS_GroupCreate: 5\n");
#endif


/* -------------------------------------------------------------------- */
/* Does name already exist?                                             */
/* -------------------------------------------------------------------- */
  if(GS_CheckInternalName(file_id,groupname)){
    bassignformat(error_string,"GS_GroupCreate: groupname: %s, already exists.",bdata(groupname));
    return ERROR;
  }// endif

#ifdef DEBUG
  printf("GS_GroupCreate: 6\n");
#endif

/* -------------------------------------------------------------------- */
/* Create the group                                                     */
/* -------------------------------------------------------------------- */
  group_id = H5Gcreate(file_id, bdata(groupname), 
                       H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  if(group_id < 0){
    bassignformat(error_string,"GS_GroupCreate: Can't create the group %s.",bdata(groupname));
    return ERROR;
  }// endif

#ifdef DEBUG
  printf("GS_GroupCreate: 7 (success)\n");
#endif

  return group_id;
}
