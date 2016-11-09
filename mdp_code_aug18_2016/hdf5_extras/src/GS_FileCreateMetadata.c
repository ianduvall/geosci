/* GS_FileCreateMetadata.c

   by Leland Pierce, Nov 11, 2013.
  
   July 8, 2014:
   modified for multi-level library
   modified to use bstrings

   July 23, 2014:
   modified to use doxygen documentation.

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"




/** \brief  GS_FileCreateMetadata Creates metadata in a GeoSci datafile


GS_FileCreateMetadata() creates a new metadata group to store per-file 
metadata in, it then fills them in with default values, and the file
descriptor string that is passed in.
 

\see FileCreate()

\param[in] file_id
                   The file ID
\param[in] descriptor
                   A character string describing the file contents.

\returns TRUE on success, FALSE on failure.

\par Example:
Create a new file named "av1.h5" and set it's file metadata, 
in particular make the file descriptor string be 
"Aviris over Boston, MA".
\code
hid_t file_id;
bstring filename = bfromcstr("av1.h5");
file_id = GS_FileCreateEmpty(filename);
bdestroy(filename);
if(file_id > 0) {
   bstring description = bfromcstr("Aviris over Boston, MA");
   if(GS_FileCreateMetadata(file_id,description){
      printf("success\n");
   }
   bdestroy(description);
}
\endcode

\par Details:
\parblock
The file metadata is stored in a group named "/_Header".
The file metadata items are: \c grouptype (= \c "Metadata"), 
\c filetype (= \c "GEOSCI"), \c software_version (= \c "V0.0.1"),
\c creation_datetime, \c last_update_datetime, \c history, and \c descriptor, 
the last one being set by a call to this function.

Note that until the file metadata is set up correctly the file is not 
recognized as a valid GeoSci file, and so most functions in this library
will not operate on it successfully. Because of this, the file creation function, 
\ref GS_FileCreate, calls this function as 
part of the file creation process.
\endparblock
*/



//#define DEBUG

/*************************************************************************/
int GS_FileCreateMetadata(hid_t file_id, const_bstring descriptor)
{
  hid_t file_header_id;
  hid_t ifile_id;

  bstring bdescriptor=bfromcstr(bdata(descriptor));

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("GS_FileCreateMetadata: 1, bdescriptor=|%s|\n",bdata(bdescriptor));
#endif 

/* -------------------------------------------------------------------- */
/* Make sure I have a FILE id, not something else                       */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsFile(file_id)){
    bassignformat(error_string,"GS_FileCreateMetadata: Invalid file_id.");
    return FALSE;
  }// endif
  ifile_id = file_id;

#ifdef DEBUG
        printf("GS_FileCreateMetadata: 2\n");
#endif 
#ifdef DEBUG22
        printf("GS_FileCreateMetadata: BB1: \n");
        report_num_objects_open();
#endif
       


/* -------------------------------------------------------------------- */
/* Create the group to store the file metadata in                       */
/* -------------------------------------------------------------------- */
  file_header_id = GS_GroupCreate(ifile_id, file_metadata_name);
#ifdef DEBUG
  printf("GS_FileCreateMetadata: 3: used GroupCreate: file_header_id=%d\n",(int)file_header_id);
#endif
  if(file_header_id < 0){
    bassignformat(error_string,"GS_FileCreateMetadata: Can't create the file metadata group %s.",
            bdata(file_metadata_name));
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_FileCreateMetadata: 4\n");
#endif 
#ifdef DEBUG22
  printf("GS_FileCreateMetadata: BB2: \n");
  report_num_objects_open();
#endif
       


/* -------------------------------------------------------------------- */
/* Store each metadata item                                             */
/* -------------------------------------------------------------------- */
  if(!GS_GroupSetType(file_header_id,GS_OBJECT_TYPE_METADATA_GROUP)){
    bassignformat(error_string,"GS_FileCreateMetadata: Error writing grouptype.");
    GS_GroupClose(file_header_id);
    return FALSE;
  }

  bstring name = bfromcstr("filetype");
  bstring value = bfromcstr("GEOSCI");
  if(!GS_UpdateMetadata(file_header_id,name,value)){
    bassignformat(error_string,"GS_FileCreateMetadata: Error writing filetype.");
    GS_GroupClose(file_header_id);
    bdestroy(name);
    bdestroy(value);
    return FALSE;
  }

  bassigncstr(name, "software_version");
  bassigncstr(value, "V0.0.1");
  if(!GS_UpdateMetadata(file_header_id,name,value)){
    bassignformat(error_string,"GS_FileCreateMetadata: Error writing software_version.");
    GS_GroupClose(file_header_id);
    bdestroy(name);
    bdestroy(value);
    return FALSE;
  }

  bassigncstr(name, "descriptor");
  if(!GS_UpdateMetadata(file_header_id,name,bdescriptor)){
    bassignformat(error_string,"GS_FileCreateMetadata: Error writing descriptor.");
    GS_GroupClose(file_header_id);
    bdestroy(name);
    bdestroy(value);
    return FALSE;
  }

  bassigncstr(name, "writeable");
  bassigncstr(value, "TRUE");
  if(!GS_UpdateMetadata(file_header_id,name,value )){
    bassignformat(error_string,"GS_FileCreateMetadata: Error writing writeable.");
    GS_GroupClose(file_header_id);
    bdestroy(name);
    bdestroy(value);
    return FALSE;
  }

  bassigncstr(name, "creation_datetime");
  bstring datetime=bfromcstr("");
  GS_Time(datetime,7);
  if(!GS_UpdateMetadata(file_header_id,name,datetime)){
    bassignformat(error_string,"GS_FileCreateMetadata: Error writing creation_datetime.");
    bdestroy(datetime);
    GS_GroupClose(file_header_id);
    bdestroy(name);
    bdestroy(value);
    return FALSE;
  }
  bdestroy(datetime);

  bassigncstr(name, "last_update_datetime");
  bstring datetime2=bfromcstr("");
  GS_Time(datetime2,7);
  if(!GS_UpdateMetadata(file_header_id,name,datetime2)){
    bassignformat(error_string,"GS_FileCreateMetadata: Error writing last_update_datetime.");
    bdestroy(datetime);
    GS_GroupClose(file_header_id);
    bdestroy(name);
    bdestroy(value);
    return FALSE;
  }
  bdestroy(datetime2);
  
  bassigncstr(name, "history");
  bstring datetime3=bfromcstr("");
  GS_Time(datetime3,7);
  //printf("datetime3=|%s|\n",bdata(datetime3));
  bassign(value,datetime3);
  //printf("value=|%s|\n",bdata(value));
  bcatcstr(value,": Creation;\n");
  //does not work...//bassignformat(value,"%s: Creation;\n",datetime3);
  //printf("GS_FileCreateMetadata: history-value=|%s|\n",bdata(value));
  bdestroy(datetime3);
  if(!GS_UpdateMetadata(file_header_id,name,value)) {
    bassignformat(error_string,"GS_FileCreateMetadata: Error writing history.");
    GS_GroupClose(file_header_id);
    bdestroy(name);
    bdestroy(value);
    return FALSE;
  }
  bdestroy(name);
  bdestroy(value);

#ifdef DEBUG
  printf("GS_FileCreateMetadata: 5\n");
    printf("GS_FileCreateMetadata: 5.1: file_header_id=%d\n",file_header_id);
#endif 

/* -------------------------------------------------------------------- */
/*      Close the file-metadata group                                   */
/* -------------------------------------------------------------------- */
  if(!GS_GroupClose(file_header_id)){
    bassignformat(error_string,"GS_FileCreateMetadata: Error closing metadata group.");
#ifdef DEBUG
    printf("GS_FileCreateMetadata: 5.2: file_header_id=%d\n",file_header_id);
#endif 
    return FALSE;
  }// endif
#ifdef DEBUG
  printf("GS_FileCreateMetadata: 6\n");
#endif 

/* -------------------------------------------------------------------- */
/*      Flush any pending updates                                       */
/* -------------------------------------------------------------------- */
  if(!GS_FileFlush(ifile_id)){
    bassignformat(error_string,"GS_FileCreateMetadata: Can't flush file.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_FileCreateMetadata: 7\n");
#endif 

/* -------------------------------------------------------------------- */
/* Return Success                                                       */
/* -------------------------------------------------------------------- */
  return TRUE;
}
