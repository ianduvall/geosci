/* GS_DatasetIsWriteableByID.c

   by Leland Pierce, June 18, 2016

*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


/** \brief GS_DatasetIsWriteableByID returns if dataset is writeable or not


\see GS_DatasetOpen(), GS_DatasetClose()

\param[in] id
                   A handle for the dataset.

\returns
\c TRUE if the dataset is writeable, \c FALSE otherwise.


\par Example
Determine if a previously-opened image raster
 dataset named "/Image1/r1" is writeable or not:
\code
hid_t id;
if(GS_DatasetIsWriteableByID(id)) {
  printf("It is writeable\n");
} else {
  printf("It is not writeable\n");
}
\endcode
*/

/**********************************************************************/
int GS_DatasetIsWriteableByID(hid_t id)
{
  hid_t ifile_id;
  int ret;
  hid_t data_id;
  bstring writestring;
  bstring tempstr;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Check for valid Dataset                                              */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsDatasetByID(id)){
    bassigncstr(error_string,"GS_DatasetIsWriteableByID: is is not a Dataset.\n"
);
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Get a valid file_id                                                  */
/* -------------------------------------------------------------------- */
  ifile_id = GS_GetValidFileID(id);
  if(!ifile_id){
    bassigncstr(error_string,"GS_DatasetIsWriteableByID: invalid id).");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Can we modify the file?                                              */
/* -------------------------------------------------------------------- */
  if(!GS_FileIsWriteable(ifile_id)){
    tempstr=GS_GetIDName(ifile_id);
    bassignformat(error_string,"GS_DatasetIsWriteableByID: File cannot be written to: %s\n",
                  bdata(tempstr));
    bdestroy(tempstr);
    GS_FileClose(ifile_id);
    return FALSE;
  }// endif


/* -------------------------------------------------------------------- */
/*	Get writeability for dataset.                			*/
/* -------------------------------------------------------------------- */
  writestring=bfromcstr("");
  bstring writeable = bfromcstr("writeable");
  if(!GS_GetStringAttribute(id,writeable,writestring)){
    bassignformat(error_string,"GS_DatasetIsWriteableByID: Can't get the writeability for dataset.");
    bdestroy(writestring);
    bdestroy(writeable);
    GS_FileClose(ifile_id);
    return FALSE;
  }// endif
  bdestroy(writeable);

  bstring btrue = bfromcstr("TRUE");
  if(EQUAL(writestring,btrue)){
    ret = TRUE;
  } else {
    ret = FALSE;
  }// endif
  bdestroy(writestring);
  bdestroy(btrue);

/* -------------------------------------------------------------------- */
/*      Close the file                                                  */
/* -------------------------------------------------------------------- */
  if(!GS_FileClose(ifile_id)){
    bassignformat(error_string,"GS_DatasetIsWriteableByID: Can't close the file.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/*      Return                                                          */
/* -------------------------------------------------------------------- */
  return ret;

}
