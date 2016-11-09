/* GS_ObjectUnlock.c

   by Leland Pierce, April 5, 2014

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


/** \brief GS_ObjectUnlock changes and object to have read-write status.

\anchor GS_ObjectUnlock
\addindex GS_ObjectUnlock



\see GS_ObjectLock()

\param[in]   id
		    The handle of the file, or any object in the file.

\returns
Returns TRUE if the object is now (or was already) read-write.
Returns FALSE if the object is still read-only, or on any error.

\par Example:
Make an image named "testimage" be read-write.
\code
hid_t object_id;
bstring name = bfromcstr("testimage");
object_id = GS_ImageOpen(file_id,name);
if(object_id < 0){
  printf("Could not open the image.\n");
  bdestroy(name);
  return;
}
bdestroy(name);
if(!GS_ObjectUnlock(object_id)){
  printf("Could not make image read-write.\n");
  return;
}
\endcode
*/

//#define DEBUG

/**********************************************************************/
int GS_ObjectUnlock(hid_t id)
{
  hid_t ifile_id;
  int objtype;
  bstring writestring;
  int npixels, nlines, nchans;
  char w1[100];

  int *writeability;
  int n;
  int done;
  int channum;
  hid_t image_id;
  int i;


  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("GS_ObjectUnlock: start\n");
#endif

/* -------------------------------------------------------------------- */
/*       Get the object type                                            */
/* -------------------------------------------------------------------- */
  objtype = GS_GetObjectType(id);
  if(!objtype){
    bassigncstr(error_string,"GS_ObjectUnlock: Can't get object-type of the given handle\n");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_ObjectUnlock: 1\n");
#endif

/* -------------------------------------------------------------------- */
/* Get a valid file-Id                                                  */
/* -------------------------------------------------------------------- */
  ifile_id = GS_GetValidFileID(id);
  if(!ifile_id){
    bassigncstr(error_string,"GS_ObjectUnlock: invalid id.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_ObjectUnlock: 2\n");
#endif

/* -------------------------------------------------------------------- */
/*	Get writeability for file.                			*/
/* -------------------------------------------------------------------- */
  if(objtype == GS_OBJECT_TYPE_FILE){
    // changing writeability for a file:
    // only care if opened rw or not, ignore writeability metadata:
#ifdef DEBUG
  printf("GS_ObjectUnlock: 2.1\n");
#endif
    unsigned int intent;
    if(H5Fget_intent(ifile_id, &intent) < 0 ){
      bassigncstr(error_string,"GS_FileIsWriteable: Can't get file read/write status.\n");
      GS_FileClose(ifile_id);
#ifdef DEBUG
  printf("GS_ObjectUnlock: 2.2\n");
#endif
      return FALSE;
    }// endif
    if (intent != H5F_ACC_RDWR) {
      bassigncstr(error_string,"GS_FileIsWriteable: File is read-only.\n");
      GS_FileClose(ifile_id);
#ifdef DEBUG
  printf("GS_ObjectUnlock: 2.3\n");
#endif
      return FALSE;
    }
  } else {
    if( GS_FileIsWriteable(ifile_id) <= 0 ) {
      bassigncstr(status_string,"GS_ObjectUnlock: file is read-only.");
      GS_FileClose(ifile_id); 
#ifdef DEBUG
  printf("GS_ObjectUnlock: 2.4\n");
#endif
      return FALSE;
    }// endif
  }

#ifdef DEBUG
  printf("GS_ObjectUnlock: 3\n");
#endif

/* -------------------------------------------------------------------- */
/*       Try to change "writeability" to "TRUE"                         */
/* -------------------------------------------------------------------- */
  bstring writeable = bfromcstr("writeable");
  bstring btrue = bfromcstr("TRUE");
  switch(objtype){
  case GS_OBJECT_TYPE_METADATA_GROUP:
  case GS_OBJECT_TYPE_METADATA_DATASET:
  case GS_OBJECT_TYPE_METADATA_IFILE:
  case GS_OBJECT_TYPE_VECTOR:
  case GS_OBJECT_TYPE_VECTOR2D:
  case GS_OBJECT_TYPE_VECTOR3D:
  case GS_OBJECT_TYPE_TIN:
  case GS_OBJECT_TYPE_MESH2D:
  case GS_OBJECT_TYPE_MESH3D:
  case GS_OBJECT_TYPE_IFILE:
  case GS_OBJECT_TYPE_FILE:
  case GS_OBJECT_TYPE_IMAGE:
  case GS_OBJECT_TYPE_RASTER:

    // each has a metadata item named "writeable"
    if(!GS_UpdateMetadata(id, writeable, btrue)){
      bassigncstr(error_string,"GS_ObjectUnlock: Can't write the writeability metadata.");
      GS_FileClose(ifile_id); 
      bdestroy(writeable);
      bdestroy(btrue);
      return FALSE;
    }//endif
    break;


  default:
    bassigncstr(error_string,"GS_ObjectUnlock: unknown object type");
    GS_FileClose(ifile_id); 
      bdestroy(writeable);
      bdestroy(btrue);
    return FALSE;
  }// endswitch


#ifdef DEBUG
  printf("GS_ObjectUnlock: 4\n");
#endif


/* -------------------------------------------------------------------- */
/*       Verify that object is read-only                                */
/* -------------------------------------------------------------------- */
  switch(objtype){
  case GS_OBJECT_TYPE_METADATA_GROUP:
  case GS_OBJECT_TYPE_METADATA_DATASET:
  case GS_OBJECT_TYPE_METADATA_IFILE:
  case GS_OBJECT_TYPE_VECTOR:
  case GS_OBJECT_TYPE_VECTOR2D:
  case GS_OBJECT_TYPE_VECTOR3D:
  case GS_OBJECT_TYPE_TIN:
  case GS_OBJECT_TYPE_MESH2D:
  case GS_OBJECT_TYPE_MESH3D:
  case GS_OBJECT_TYPE_FILE:
  case GS_OBJECT_TYPE_IFILE:
  case GS_OBJECT_TYPE_IMAGE:
  case GS_OBJECT_TYPE_RASTER:

    // each has a metadata item named "writeable"
    writestring = bfromcstr("");
    if(!GS_GetStringAttribute(id,writeable,writestring)){
      bassigncstr(error_string,"GS_ObjectUnlock: Can't get the writeability metadata.");
      GS_FileClose(ifile_id); 
      bdestroy(writestring);
      bdestroy(writeable);
      bdestroy(btrue);

#ifdef DEBUG
  printf("GS_ObjectUnlock: 5\n");
#endif
      return FALSE;
    }// endif
    if(EQUAL(writestring,bsstatic("TRUE"))){
      bdestroy(writestring);
      bdestroy(writeable);
      bdestroy(btrue);
      GS_FileClose(ifile_id); 

#ifdef DEBUG
  printf("GS_ObjectUnlock: 6\n");
#endif
      return TRUE;
    } else {
      bdestroy(writestring);
      bdestroy(writeable);
      bdestroy(btrue);
      GS_FileClose(ifile_id); 

#ifdef DEBUG
  printf("GS_ObjectUnlock: 7\n");
#endif
      return FALSE;
    }// endif



  default:
    // should never be here.
    bassigncstr(error_string,"GS_ObjectUnlock: unknown object type");
    GS_FileClose(ifile_id); 
      bdestroy(writestring);
      bdestroy(writeable);
      bdestroy(btrue);
    return FALSE;
  }// endswitch


#ifdef DEBUG
  printf("GS_ObjectUnlock: 8\n");
#endif

      bdestroy(writestring);
      bdestroy(writeable);
      bdestroy(btrue);


}
