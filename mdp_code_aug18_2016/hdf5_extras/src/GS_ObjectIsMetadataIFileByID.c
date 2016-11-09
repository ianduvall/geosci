/* GS_ObjectIsMetadataIFileByID.c

      by Leland Pierce, May 21, 2014.
  
   July 10, 2014:
   modified for multi-level library
   modified to use bstrings

*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


/** \brief GS_ObjectIsMetadataIFileByID determines if the object-id refers to a metadata internal-file

\anchor GS_ObjectIsMetadataIFileByID
\addindex GS_ObjectIsMetadataIFileByID

\see GS_ObjectIsMetadataIFile(), IFileOpen(), GS_FileClose()

\param[in] id
                   The handle of the open object.

\returns \c TRUE on success, \c FALSE on failure.

\par Example:
Open the Location metadata for an image and query 
if it's valid Metadata IFile:
\code
hid_t file_id;
hid_t objid;
bstring name = bfromcstr("someimage/location");
objid = GS_DatasetOpen(file_id,name);
if(objid < 0) {
   printf("GS_DatasetOpen failed on /someimage/location\n");
}
bdestroy(name);
if(!GS_ObjectIsMetadataIFileByID(objid)){
  printf("Object is not a metadata internal file.\n");
}
\endcode

\par Details:
Some Metadata is stored as metadata items assocated with Groups, and other metadata is
large enough that we create internal files (IFiles) to store the data in.
These files are like ordinary files, except that they are \a internal to the
GeoSci file.
They can be binary, or text, and the arrangement of the data within them is 
specific to the Metadata that needs to be stored.
The names of these IFiles are used to determine the format.
We have coders and ecoders for each format so that the actual format used does 
not need to be known by users of the system.
*/



/*******************************************************************************/
int GS_ObjectIsMetadataIFileByID( hid_t dataset_id )
{
  hid_t ifile_id;


  bassigncstr(status_string,"");
  bassigncstr(error_string,"");


/* -------------------------------------------------------------------- */
/* Is it a dataset?                                                     */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsDatasetByID(dataset_id)){
    bassigncstr(error_string,"GS_ObjectIsMetadataIFileByID: invalid object id).");
    return FALSE;
  }// endif


/* -------------------------------------------------------------------- */
/* Get the dataset_type metadata                                        */
/* -------------------------------------------------------------------- */
  if(GS_DatasetGetType(dataset_id) !=  GS_OBJECT_TYPE_METADATA_IFILE ){
    FileClose(ifile_id);
    return FALSE;
  }// endif


  GS_FileClose(ifile_id);
  return TRUE;
}
