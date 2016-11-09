/* GS_GroupIsWriteable.c

   by Leland Pierce, Aug 7, 2014
   including: bstrings, doxygen, GS_

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


/** \brief GS_GroupIsWriteable returns if the group is writeable or not


\see GS_GroupOpen(), GS_GroupClose()

\param[in] id
                   A handle for the file.
\param[in] group_name
                   The name of the group in the file.
                   Should start with a "/".

\returns
\c TRUE if the dataset is writeable, \c FALSE otherwise.


\par Example
Determine if a image dataset named "Image1" is writeable or not:
\code
hid_t file_id;
bstring name = bfromcstr("/Image1");
if(GS_GroupIsWriteable(file_id, name)) {
  printf("It is writeable\n");
} else {
  printf("It is not writeable\n");
}
bdestroy(name);
\endcode
*/

/*********************************************************************/
int GS_GroupIsWriteable(hid_t file_id, const_bstring group_name)
{
  hid_t ifile_id;
  int ret;
  hid_t group_id;
  bstring writestring;
  bstring tempstr;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Get a valid file_id                                                  */
/* -------------------------------------------------------------------- */
  ifile_id = GS_GetValidFileID(file_id);
  if(!ifile_id){
    bassigncstr(error_string,"GS_GroupIsWriteable: invalid file id).");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Can we modify the file?                                              */
/* -------------------------------------------------------------------- */
  if(!GS_FileIsWriteable(ifile_id)){
    tempstr=GS_GetIDName(file_id);
    bassignformat(error_string,"GS_GroupIsWriteable: File cannot be written to: %s\n",
                  bdata(tempstr));
    bdestroy(tempstr);
    GS_FileClose(ifile_id);
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* See if this name is a group                                          */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsGroup( ifile_id, group_name )){
    bassignformat(error_string,"GS_GroupIsWriteable: Name: %s is not a group.",
                  bdata(group_name));
    GS_FileClose(ifile_id);
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Make sure can open the group                                         */
/* -------------------------------------------------------------------- */
  group_id = GS_GroupOpen(ifile_id, group_name);
  if(group_id < 0){
    bassignformat(error_string,"GS_GroupIsWriteable: Can't open '%s'.",bdata(group_name));
    GS_FileClose(ifile_id);
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/*	Get writeability for dataset.                			*/
/* -------------------------------------------------------------------- */
  writestring=bfromcstr("");
  bstring writeable = bfromcstr("writeable");
  if(!GS_GetStringAttribute(group_id,writeable,writestring)){
    bassignformat(error_string,"GS_GroupIsWriteable: Can't get writeability for '%s'.",
                  bdata(group_name));
    bdestroy(writestring);
    bdestroy(writeable);
    GS_GroupClose(group_id);
    GS_FileClose(ifile_id);
    return FALSE;
  }// endif
  bdestroy(writeable);

  bstring btrue = bfromcstr("TRUE");
  if(!EQUAL(writestring,btrue)){
    ret = TRUE;
  } else {
    ret = FALSE;
  }// endif
  bdestroy(writestring);
  bdestroy(btrue);

/* -------------------------------------------------------------------- */
/*      Close the dataset                                               */
/* -------------------------------------------------------------------- */
  if(!GS_DatasetClose(group_id)){
    bassignformat(error_string,"GS_GroupIsWriteable: Can't close '%s'.",bdata(group_name));
    GS_FileClose(ifile_id);
    return FALSE;
  }// endif
 
/* -------------------------------------------------------------------- */
/*      Close the file                                                  */
/* -------------------------------------------------------------------- */
  if(!GS_FileClose(ifile_id)){
    bassignformat(error_string,"GS_GroupIsWriteable: Can't close the file.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/*      Return                                                          */
/* -------------------------------------------------------------------- */
  return ret;

}
