/* GS_ImageCreateMetadata.c

   by Leland Pierce, Nov 12, 2013.
  
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


/** \brief GS_ImageCreateMetadata fills the standard image metadata attributes
       with the values that are passed in.


\see GS_ImageCreate()

\param[in]   file_id
                   The file ID 
\param[in]  image_name
                   A character string giving the name of the image.
\param[in] descriptor
                   A character string describing the image contents.

\returns TRUE on success, FALSE on failure.

\par Example:

After creating a new image named "datatake 34" in a file named "av1.h5",
we must set the metadata for the image before the image object can be used.
The descriptor is "Apr 3, 2012, 2:18 PM".

\code
hid_t file_id;
bstring filename=bfromcstr("av1.h5");
bstring access=bfromcstr("r+");
file_id = GS_FileOpen(filename,access);
bdestroy(filename);
bdestroy(access);
if(file_id > 0) {
   ... create a group with datasets in it that represent the image...
   bstring name = bfromcstr("datatake 34");
   bstring descriptor = bfromcstr("Apr 3, 2012, 2:18 PM");
   if(GS_ImageCreateMetadata(file_id,name,descriptor){
      printf("success\n");
   }
   bdestroy(name);
   bdestroy(descriptor);
}
\endcode

\par Details:
\parblock
The image metadata is attached to the Group within which
all the Datasets are stored.
The image metadata items are: \c grouptype (= \c "Image"), 
 \c descriptor,
\c creation_datetime, \c last_update_datetime, \c history, 
 and \c writeable. 

Note that until the image metadata is set up correctly the image is not 
recognized as a valid GeoSci Image object, and so most functions in this library
will not operate on it successfully. Because of this, 
the image creation functions,
such as \ref GS_ImageCreate,  call this function as 
part of the creation process.
\endparblock

*/



/**************************************************************************/
int GS_ImageCreateMetadata(hid_t file_id, const_bstring image_name, 
                        const_bstring descriptor)
{
  hid_t image_header_id;
  bstring datetime;
  bstring string_value;
  hid_t ifile_id;
  int i, n;


  bassigncstr(status_string,"");
  bassigncstr(error_string,"");


/* -------------------------------------------------------------------- */
/* Make sure I have a file_id, not some other id                        */
/* -------------------------------------------------------------------- */
  ifile_id = GS_GetValidFileID(file_id);
  if(!ifile_id){
    bassignformat(error_string,"GS_ImageCreateMetadata: Can't get file_id.");
    return FALSE;
  }// endif


/* -------------------------------------------------------------------- */
/* Make sure can open the image to store the image metadata in          */
/* must use group open because the metadata is not there yet.           */
/* -------------------------------------------------------------------- */
  image_header_id = GS_GroupOpen(ifile_id, image_name);
  if(image_header_id < 0){
    GS_FileClose(ifile_id);
    bassignformat(error_string,"GS_ImageCreateMetadata: Can't open the image %s.",
            bdata(image_name));
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/*  Store each IMAGE metadata item                                      */
/* -------------------------------------------------------------------- */
  if(!GS_GroupSetType(image_header_id,GS_OBJECT_TYPE_IMAGE)){
    bassignformat(error_string,"GS_ImageCreateMetadata: Error writing grouptype.");
    GS_GroupClose(image_header_id);
    GS_FileClose(ifile_id);
    return FALSE;
  }
  bstring bdescriptor = bfromcstr("descriptor");
  string_value = bstrcpy(descriptor);
  if(!GS_UpdateMetadata(image_header_id,bdescriptor,string_value)){
    bassignformat(error_string,"GS_ImageCreateMetadata: Error writing descriptor.");
    GS_GroupClose(image_header_id);
    GS_FileClose(ifile_id);
    bdestroy(bdescriptor);
    bdestroy(string_value);
    return FALSE;
  }
  bdestroy(bdescriptor);

  bstring name=bfromcstr("creation_datetime");
  datetime=bfromcstr("");
  GS_Time(datetime,7);
  if(!GS_UpdateMetadata(image_header_id,name,datetime)){
    bassignformat(error_string,"GS_ImageCreateMetadata: Error writing creation_datetime.");
    bdestroy(datetime);
    bdestroy(name);
    bdestroy(string_value);
    GS_GroupClose(image_header_id);
    GS_FileClose(ifile_id);
    return FALSE;
  }
  bassigncstr(name,"last_update_datetime");
  if(!GS_UpdateMetadata(image_header_id,name,datetime)){
    bassignformat(error_string,"GS_ImageCreateMetadata: Error writing last_update_datetime.");
    bdestroy(datetime);
    bdestroy(name);
    bdestroy(string_value);
    GS_GroupClose(image_header_id);
    GS_FileClose(ifile_id);
    return FALSE;
  }

  string_value=bfromcstr("");
  bassignformat(string_value,"%s: Creation;\n",bdata(datetime));
  bdestroy(datetime);
  bassigncstr(name,"history");
  if(!GS_UpdateMetadata(image_header_id,name,string_value)){
    bassignformat(error_string,"GS_ImageCreateMetadata: Error writing history.");
    bdestroy(string_value);
    bdestroy(name);
    GS_GroupClose(image_header_id);
    GS_FileClose(ifile_id);
    return FALSE;
  }

  bassigncstr(name,"writeable");
  bassigncstr(string_value,"TRUE");
  if(!GS_UpdateMetadata(image_header_id,name,string_value)){
    bassignformat(error_string,"GS_ImageCreateMetadata: Error writing writeable.");
    GS_GroupClose(image_header_id);
    GS_FileClose(ifile_id);
    bdestroy(name);
    bdestroy(string_value);
    return FALSE;
  }


  bdestroy(name);
  bdestroy(string_value);

  GS_GroupClose(image_header_id);

/* -------------------------------------------------------------------- */
/*      Flush any pending updates                                       */
/* -------------------------------------------------------------------- */
  if(!GS_FileFlush(ifile_id)){
    bassignformat(error_string,"GS_ImageCreateMetadata: Can't flush file.");
    GS_FileClose(ifile_id);
    return FALSE;
  }// endif


/* -------------------------------------------------------------------- */
/* Close the file                                                       */
/* -------------------------------------------------------------------- */
  if(!GS_FileClose(ifile_id)){
    bassignformat(error_string,"GS_ImageCreateMetadata: Can't close file.");
    return FALSE;
  }//endif


/* -------------------------------------------------------------------- */
/* Return success                                                       */
/* -------------------------------------------------------------------- */
  return TRUE;
}
