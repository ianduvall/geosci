/* GS_VectorCreateMetadata.c

   by Leland Pierce, Feb 26, 2014.
  
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



/** \brief GS_VectorCreateMetadata creates vector metadata.


\see GS_VectorCreate()

\param[in]  file_id
                   The file ID.
\param[in] vector_name
                   A character string giving the name of the image.
\param[in]  descriptor
                   A character string describing the vector contents.

\returns
TRUE on success, FALSE on failure.

\par Example:

After creating a new vector named "hydrology" in a file named "av1.h5",
we must set the metadata for the vector object before it can be used.
The descriptor is "water boundaries".

\code
hid_t file_id;
bstring name = bfromcstr("av1.h5");
bstring access = bfromcstr("r+");
file_id = GS_FileOpen(name,access);
bdestroy(name);
bdestroy(access);
if(file_id > 0) {
   ... create a group with a dataset in it that represent the vector information...
   bstring vname = bfromcstr("hydrology");
   bstring vdescriptor = bfromcstr("water boundaries");
   if(GS_VectorCreateMetadata(file_id,vname,vdescriptor){
      printf("success\n");
   }
   bdestroy(vname);
   bdestroy(vdescriptor);
}
\endcode



\par Details:
\parblock
The vector metadata is attached to the Group within which
 the vector Dataset is stored.
The vector metadata items are: \c grouptype (= \c "Image"), 
 \c descriptor,
\c creation_datetime, \c last_update_datetime, \c history, 
 \c writeable and \c spatialref.

Note that until the vector metadata is set up correctly the image is not 
recognized as a valid GeoSci Vector object, and so most functions in this library
will not operate on it successfully. Because of this, the vector creation function,
 \ref GS_VectorCreate  calls this function as part of the creation process.
\endparblock

*/




/**************************************************************************/
int GS_VectorCreateMetadata(hid_t file_id, const_bstring vector_name, 
                         const_bstring descriptor)
{
  hid_t vector_header_id;
  bstring datetime;
  bstring string_value;
  hid_t ifile_id;


  bassigncstr(status_string,"");
  bassigncstr(error_string,"");


/* -------------------------------------------------------------------- */
/* Make sure I have a file_id, not some other id                        */
/* -------------------------------------------------------------------- */
  ifile_id = GS_GetValidFileID(file_id);
  if(!ifile_id){
    bassigncstr(error_string,"GS_VectorCreateMetadata: Can't get file_id.");
    return FALSE;
  }// endif


/* -------------------------------------------------------------------- */
/* Open the vector to store the vector metadata in                      */
/* must use group open because the metadata is not there yet.           */
/* -------------------------------------------------------------------- */
  bstring name = bstrcpy(vector_name);
  vector_header_id = GS_GroupOpen(ifile_id, name);
  if(vector_header_id < 0){
    bassignformat(error_string,"GS_VectorCreateMetadata: Can't open the vector %s.",
              bdata(name));
    bdestroy(name);
    GS_FileClose(ifile_id);
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/*  Store each VECTOR metadata item                                     */
/* -------------------------------------------------------------------- */
  if(!GS_GroupSetType(vector_header_id,GS_OBJECT_TYPE_VECTOR)){
    bassigncstr(error_string,"GS_VectorCreateMetadata: Error writing grouptype.");
    GS_GroupClose(vector_header_id);
    GS_FileClose(ifile_id);
    bdestroy(name);
    return FALSE;
  }
  name = bfromcstr("descriptor");
  string_value = bstrcpy(descriptor);
  if(!GS_UpdateMetadata(vector_header_id,name,string_value)){
    bassigncstr(error_string,"GS_VectorCreateMetadata: Error writing descriptor.");
    GS_GroupClose(vector_header_id);
    GS_FileClose(ifile_id);
    bdestroy(name);
    bdestroy(string_value);
    return FALSE;
  }

  name = bfromcstr("creation_datetime");
  datetime=bfromcstr("");
  GS_Time(datetime,7);
  if(!GS_UpdateMetadata(vector_header_id,name,datetime)){
    bassigncstr(error_string,"GS_VectorCreateMetadata: Error writing creation_datetime.");
    bdestroy(datetime);
    bdestroy(name);
    GS_GroupClose(vector_header_id);
    GS_FileClose(ifile_id);
    return FALSE;
  }
  name = bfromcstr("last_update_datetime");
  if(!GS_UpdateMetadata(vector_header_id,name,datetime)){
    bassigncstr(error_string,"GS_VectorCreateMetadata: Error writing last_update_datetime.");
    bdestroy(datetime);
    GS_GroupClose(vector_header_id);
    GS_FileClose(ifile_id);
    return FALSE;
  }

  name = bfromcstr("history");
  bassignstr(string_value,"%s: Creation;\n",bdata(datetime));
  bdestroy(datetime);
  if(!GS_UpdateMetadata(vector_header_id,name,string_value)){
    bassigncstr(error_string,"GS_VectorCreateMetadata: Error writing history.");
    GS_GroupClose(vector_header_id);
    GS_FileClose(ifile_id);
    bdestroy(string_value);
    bdestroy(name);
    return FALSE;
  }
  bdestroy(string_value);

  name = bfromcstr("writeable");
  string_value = bfromcstr("TRUE");
  if(!GS_UpdateMetadata(vector_header_id,name,string_value)){
    bassigncstr(error_string,"GS_VectorCreateMetadata: Error writing writeable.");
    GS_GroupClose(vector_header_id);
    GS_FileClose(ifile_id);
    bdestroy(string_value);
    bdestroy(name);
    return FALSE;
  }

  name = bstrcpy(vector_name);
  bstring ref = bfromcstr("EPSG:-1");
  bstring units = bfromcstr("UNITS[\"Meters\",1.0]");
  string_value = bfromcstr("");
  if(!GS_SpatialrefCreate(ifile_id,name,ref,units,string_value)){
    bassigncstr(error_string,"GS_VectorCreateMetadata: Error writing spatialref.");
    GS_GroupClose(vector_header_id);
    GS_FileClose(ifile_id);
    bdestroy(string_value);
    bdestroy(name);
    bdestroy(ref);
    bdestroy(units);
    return FALSE;
  }
  bdestroy(string_value);
  bdestroy(name);
  bdestroy(ref);
  bdestroy(units);

/* -------------------------------------------------------------------- */
/*      Flush any pending updates                                       */
/* -------------------------------------------------------------------- */
  if(!GS_GroupClose(vector_header_id)){
    bassigncstr(error_string,"GS_VectorCreateMetadata: Can't close group.");
    GS_FileClose(ifile_id);
    return FALSE;
  }//endif

/* -------------------------------------------------------------------- */
/*      Flush any pending updates                                       */
/* -------------------------------------------------------------------- */
  if(!GS_FileFlush(ifile_id)){
    bassigncstr(error_string,"GS_VectorCreateMetadata: Can't flush file.");
    GS_FileClose(ifile_id);
    return FALSE;
  }// endif


/* -------------------------------------------------------------------- */
/* Close the file                                                       */
/* -------------------------------------------------------------------- */
  if(!GS_FileClose(ifile_id)){
    bassigncstr(error_string,"GS_VectorCreateMetadata: Can't close file.");
    return FALSE;
  }//endif


/* -------------------------------------------------------------------- */
/* Return success                                                       */
/* -------------------------------------------------------------------- */
  return TRUE;
}
