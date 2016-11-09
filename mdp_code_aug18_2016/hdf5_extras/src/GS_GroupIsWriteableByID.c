/* GS_GroupIsWriteableByID.c

   by Leland Pierce, Aug 12, 2014
   including: bstrings, doxygen, GS_

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


/** \brief GS_GroupIsWriteableByID returns if the group is writeable or not


\see GS_GroupOpen(), GS_GroupClose()

\param[in] id
                   A handle for the group.

\returns
\c TRUE if the dataset is writeable, \c FALSE otherwise.


\par Example
Determine if a image  is writeable or not:
\code
hid_t image_id;
if(GS_GroupIsWriteableByID(image_id)) {
  printf("It is writeable\n");
} else {
  printf("It is not writeable\n");
}
\endcode
*/

//#define DEBUG

/******************************************************************/
int GS_GroupIsWriteableByID(hid_t group_id)
{
  hid_t ifile_id;
  int ret;
  bstring writestring;
  bstring tempstr;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("GS_GroupIsWriteableByID: start\n");
#endif

/* -------------------------------------------------------------------- */
/* Check if valid ID                                                    */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID(group_id)){
    bassigncstr(error_string,"GS_GroupIsWriteableByID: invalid id).");
    return FALSE;
  }// endif
#ifdef DEBUG
  printf("GS_GroupIsWriteableByID: 1\n");
#endif

/* -------------------------------------------------------------------- */
/* Get a valid file_id                                                  */
/* -------------------------------------------------------------------- */
  ifile_id = GS_GetValidFileID(group_id);
  if(!ifile_id){
    bassigncstr(error_string,"GS_GroupIsWriteableByID: invalid file id).");
    return FALSE;
  }// endif
#ifdef DEBUG
  printf("GS_GroupIsWriteableByID: 2\n");
#endif

/* -------------------------------------------------------------------- */
/* Can we modify the file?                                              */
/* -------------------------------------------------------------------- */
  if(!GS_FileIsWriteable(ifile_id)){
    tempstr = GS_GetIDName(ifile_id);
    bassignformat(error_string,"GS_GroupIsWriteableByID: File cannot be written to: %s\n",
                  bdata(tempstr));
    bdestroy(tempstr);
    GS_FileClose(ifile_id);
    return FALSE;
  }// endif
#ifdef DEBUG
  printf("GS_GroupIsWriteableByID: 3\n");
#endif

/* -------------------------------------------------------------------- */
/*	Get writeability for group.                			*/
/* -------------------------------------------------------------------- */
  writestring=bfromcstr("");
  bstring writeable = bfromcstr("writeable");
  if(!GS_GetStringAttribute(group_id,writeable,writestring)){
    tempstr = GS_GetIDName(group_id);
    bassignformat(error_string,"GS_GroupIsWriteableByID: Can't get writeability for '%s'.",
                  bdata(tempstr));
    bdestroy(tempstr);
    bdestroy(writeable);
    bdestroy(writestring);
    GS_FileClose(ifile_id);
    return FALSE;
  }// endif
  bdestroy(writeable);

#ifdef DEBUG
  printf("GS_GroupIsWriteableByID: 3.5, writestring=|%s|\n",bdata(writestring));
#endif


  bstring btrue = bfromcstr("TRUE");
  if(EQUAL(writestring,btrue)){
    ret = TRUE;
  } else {
    ret = FALSE;
  }// endif
  bdestroy(btrue);
  bdestroy(writestring);
#ifdef DEBUG
  printf("GS_GroupIsWriteableByID: 4\n");
#endif

/* -------------------------------------------------------------------- */
/*      Close the file                                                  */
/* -------------------------------------------------------------------- */
  if(!GS_FileClose(ifile_id)){
    bassignformat(error_string,"GS_GroupIsWriteableByID: Can't close the file.");
    return FALSE;
  }// endif
#ifdef DEBUG
  printf("GS_GroupIsWriteableByID: 5\n");
#endif

/* -------------------------------------------------------------------- */
/*      Return                                                          */
/* -------------------------------------------------------------------- */
  return ret;

}
