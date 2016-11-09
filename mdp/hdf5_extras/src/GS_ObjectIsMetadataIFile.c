/* GS_ObjectIsMetadataIFile.c

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


/** \brief GS_ObjectIsMetadataIFile determines if the object refers to a metadata internal-file

\anchor GS_ObjectIsMetadataIFile
\addindex GS_ObjectIsMetadataIFile

\see GS_ObjectIsMetadataIFileByID(), IFileOpen(), GS_FileClose()

\param[in] id
                   The handle of the open group.
\param[in]  name
                   The name of the object within the group.

\returns \c TRUE on success, \c FALSE on failure.

\par Example:
Open the Location metadata for an image and query 
if it's valid Metadata IFile:
\code
hid_t file_id;
hid_t group_id;
bstring name = bfromcstr("/somename");
group_id = GS_GroupOpen(file_id,name);
if(group_id < 0) {
   printf("GS_GroupOpen failed on /somename\n");
}
bdestroy(name);
bstring location = bfromcstr("location");
if(!GS_ObjectIsMetadataIFile(group_id, location)){
  printf("Object is not a Metadata IFile.\n");
}
bdestroy(location);
\endcode

\par Details:
Some Metadata is stored as metadata items assocated with Groups, and other metadata is
large enough that we create internal files (IFiles) to store the data in.
These files are like ordinary files, except that they are  internal to the
GeoSci file.
They can be binary, or text, and the arrangement of the data within them is 
specific to the Metadata that needs to be stored.
The names of these IFiles are used to determine the format.
We have coders and ecoders for each format so that the actual format used does 
not need to be known by users of the system.
*/


//#define DEBUG

/*******************************************************************************/
int GS_ObjectIsMetadataIFile( hid_t id, const_bstring name )
{
  hid_t ifile_id;
  hid_t dataset_id;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Is it a dataset?                                                     */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsDataset(id,name)){
    bassigncstr(error_string,"GS_ObjectIsMetadataIFile: not a dataset).");
    return FALSE;
  }// endif


/* -------------------------------------------------------------------- */
/* Open the dataset                                                     */
/* -------------------------------------------------------------------- */
  dataset_id = GS_DatasetOpen(id,name);
  if(!dataset_id){
    bassigncstr(error_string,"GS_ObjectIsMetadataIFile: can't open dataset");
    return FALSE;
  }// endif


/* -------------------------------------------------------------------- */
/* Get the dataset_type metadata                                        */
/* -------------------------------------------------------------------- */
  if(GS_DatasetGetType(dataset_id) !=  GS_OBJECT_TYPE_METADATA_IFILE ){
    GS_DatasetClose(dataset_id);
    return FALSE;
  }// endif


/* -------------------------------------------------------------------- */
/* Close the dataset                                                    */
/* -------------------------------------------------------------------- */
  if(!GS_DatasetClose(dataset_id)){
    bassigncstr(error_string,"GS_ObjectIsMetadataIFile: dataset cannot be closed.");
    return FALSE;
  }//endif


  return TRUE;
}
