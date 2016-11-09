/* IFileGetWriteDataset.c

   by Leland Pierce, Jan 20, 2014.
  
*/

/** \brief IFileGetWriteDataset gets the read/write property of an internal file.

\sa IFileOpen, IFileClose

\param[in] id;
The handle of the open dataset.

\returns
\parblock
\c TRUE is returned if the dataset is writeable,
\c FALSE if not writeable.

ERROR is returned on any kind of error.
The \c error_string variable is also set to a system-specific 
error code on failure.
\endparblock

\par Example
Let's assume that one already has an GeoSci file, and one wants
to determine if the  internal file named "/somename" is writeable.
\code
hid_t file_id;
hid_t dataset_id;

dataset_id = GS_DatasetOpen(file_id,"/somename");
if(datset_id < 0) {
   printf("GS_DatasetOpen failed on /somename\n");
}

status = IFileGetWriteDataset(ifilep);
if( status ==FALSE){
   printf("Cannot write to internal file\n");
} else if(status == TRUE) {
   printf("Can write to internal file\n");
} else {
   printf("error getting write-status of internal file\n");
}
\endcode

\par Detials
This function is meant to enable a user to make an internal file
have read and write rights like in the standard file system.
*/

#include "ifile.h"


//#define DEBUG

/*******************************************************************************/
int IFileGetWriteDataset( hid_t dataset_id )
{ 
  bstring w_string=bfromcstr("");

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("IFileGetWriteDataset: 1\n");
#endif

/* -------------------------------------------------------------------- */
/* Read the writeable metadata                                          */
/* -------------------------------------------------------------------- */
  bstring writeable = bfromcstr("writeable");
  if(!GS_GetStringAttribute(dataset_id, writeable,w_string)){
    bassignformat(error_string,"IFileGetWriteDataset: Can't get the eof metadata.");
    bdestroy(writeable);
    bdestroy(w_string);
    return ERROR;
  }// endif
  bdestroy(writeable);

#ifdef DEBUG
  printf("IFileGetWriteDataset: 3, w_string=|%s|\n",bdata(w_string));
#endif


/* -------------------------------------------------------------------- */
/* Return writeability status                                           */
/* -------------------------------------------------------------------- */
  bstring btrue = bfromcstr("TRUE");
  if(EQUAL(w_string,btrue)){
    bdestroy(w_string);
    bdestroy(btrue);
    return TRUE;
  } else {
    bdestroy(w_string);
    bdestroy(btrue);
    return FALSE;
  }// endif
 
}
