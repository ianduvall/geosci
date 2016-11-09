/* GS_DatasetIsWriteable.c

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


/** \brief GS_DatasetIsWriteable returns if dataset is writeable or not


\see GS_DatasetOpen(), GS_DatasetClose()

\param[in] id
                   A handle for the file.
\param[in] dataset_name
                   The name of the dataset in the file.
                   Should start with a "/".

\returns
\c TRUE if the dataset is writeable, \c FALSE otherwise.


\par Example
Determine if a image raster dataset named "/Image1/r1" is writeable or not:
\code
hid_t file_id;
bstring name = bfromcstr("/Image1/r1");
if(GS_DatasetIsWriteable(file_id, name)) {
  printf("It is writeable\n");
} else {
  printf("It is not writeable\n");
}
bdestroy(name);
\endcode
*/

/**********************************************************************/
int GS_DatasetIsWriteable(hid_t file_id, const_bstring dataset_name)
{
  hid_t ifile_id;
  int ret;
  hid_t data_id;
  bstring writestring;
  bstring tempstr;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Get a valid file_id                                                  */
/* -------------------------------------------------------------------- */
  ifile_id = GS_GetValidFileID(file_id);
  if(!ifile_id){
    bassigncstr(error_string,"GS_DatasetIsWriteable: invalid file id).");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Can we modify the file?                                              */
/* -------------------------------------------------------------------- */
  if(!GS_FileIsWriteable(ifile_id)){
    tempstr=GS_GetIDName(file_id);
    bassignformat(error_string,"GS_DatasetIsWriteable: File cannot be written to: %s\n",
                  bdata(tempstr));
    bdestroy(tempstr);
    GS_FileClose(ifile_id);
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* See if this name is a dataset                                        */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsDataset( ifile_id, dataset_name )){
    bassignformat(error_string,"GS_DatasetIsWriteable: Name: %s is not a dataset.",
                  bdata(dataset_name));
    GS_FileClose(ifile_id);
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Make sure can open the dataset                                       */
/* -------------------------------------------------------------------- */
  data_id = GS_DatasetOpen(ifile_id, dataset_name);
  if(data_id < 0){
    bassignformat(error_string,"GS_DatasetIsWriteable: Can't open '%s'.",bdata(dataset_name));
    GS_FileClose(ifile_id);
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/*	Get writeability for dataset.                			*/
/* -------------------------------------------------------------------- */
  writestring=bfromcstr("");
  bstring writeable = bfromcstr("writeable");
  if(!GS_GetStringAttribute(data_id,writeable,writestring)){
    bassignformat(error_string,"GS_DatasetIsWriteable: Can't get the writeability for '%s'.",
                  bdata(dataset_name));
    bdestroy(writestring);
    bdestroy(writeable);
    GS_DatasetClose(data_id);
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
/*      Close the dataset                                               */
/* -------------------------------------------------------------------- */
  if(!GS_DatasetClose(data_id)){
    bassignformat(error_string,"GS_DatasetIsWriteable: Can't close '%s'.",bdata(dataset_name));
    GS_FileClose(ifile_id);
    return FALSE;
  }// endif
 
/* -------------------------------------------------------------------- */
/*      Close the file                                                  */
/* -------------------------------------------------------------------- */
  if(!GS_FileClose(ifile_id)){
    bassignformat(error_string,"GS_DatasetIsWriteable: Can't close the file.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/*      Return                                                          */
/* -------------------------------------------------------------------- */
  return ret;

}
