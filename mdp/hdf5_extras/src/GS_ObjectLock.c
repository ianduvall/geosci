/* GS_ObjectLock.c

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


/** \brief GS_ObjectLock makes an object read-only.

\anchor GS_ObjectLock
\addindex GS_ObjectLock


\see GS_ObjectUnlock()

\param[in]  id
		    The handle of the file, or any object in the file.

\returns
Returns TRUE if the object is now (or was already) read-only.
Returns FALSE if the object is still read/write, or on any error.

\par Example:
Make an image named "testimage" be read-only.
\code
hid_t object_id;
bstring name = bfromcstr("testimage");
object_id = GS_ImageOpen(file_id,name);
if(object_id < 0){
  printf("Could not open the image.\n");
  return;
}
bdestroy(name);
if(!GS_ObjectLock(object_id)){
  printf("Could not make image read-only.\n");
  return;
}
\endcode

\par Details:
Yet: fill in details of how different object-types are handled.

*/



//#define DEBUG

/*******************************************************************/
int GS_ObjectLock(hid_t id)
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
  printf("GS_ObjectLock: start\n");
#endif

/* -------------------------------------------------------------------- */
/*       Get the object type                                            */
/* -------------------------------------------------------------------- */
  objtype = GS_GetObjectType(id);
  if(!objtype){
    bassigncstr(error_string,"GS_ObjectLock: Can't get object-type of the given handle\n");
    return FALSE;
  }// endif
#ifdef DEBUG
  printf("GS_ObjectLock: 1\n");
#endif

/* -------------------------------------------------------------------- */
/* Get a valid file-Id                                                  */
/* -------------------------------------------------------------------- */
  ifile_id = GS_GetValidFileID(id);
  if(!ifile_id){
    bassigncstr(error_string,"GS_ObjectLock: invalid id.");
    return FALSE;
  }// endif
#ifdef DEBUG
  printf("GS_ObjectLock: 2\n");
#endif

/* -------------------------------------------------------------------- */
/*	Get writeability for file.                			*/
/* -------------------------------------------------------------------- */
  if( GS_FileIsWriteable(ifile_id) <= 0 ) {
    bassigncstr(status_string,"GS_ObjectLock: file is already read-only.");
    GS_FileClose(ifile_id); 
    return TRUE;
  }// endif
#ifdef DEBUG
  printf("GS_ObjectLock: 3, objtype=%d\n",objtype);
#endif

/* -------------------------------------------------------------------- */
/*       Try to change "writeable" to "FALSE"                           */
/* -------------------------------------------------------------------- */
  bstring writeable = bfromcstr("writeable");
  bstring bfalse = bfromcstr("FALSE");
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

#ifdef DEBUG
  printf("GS_ObjectLock: 3.5, objtype=%d\n",objtype);
#endif
    // each has a metadata item named "writeable"
    if(!GS_UpdateMetadata(id, writeable, bfalse)){
      bassigncstr(error_string,"GS_ObjectLock: Can't write the writeability metadata.");
      GS_FileClose(ifile_id); 
      bdestroy(writeable);
      bdestroy(bfalse);
      return FALSE;
    }//endif
#ifdef DEBUG
  printf("GS_ObjectLock: 4\n");
#endif
    break;
    

  default:
    bassigncstr(error_string,"GS_ObjectLock: unknown object type");
    GS_FileClose(ifile_id); 
      bdestroy(writeable);
      bdestroy(bfalse);
#ifdef DEBUG
  printf("GS_ObjectLock: 5\n");
#endif
    return FALSE;
  }// endswitch
  bdestroy(bfalse);



/* -------------------------------------------------------------------- */
/*       Verify that object is read-only                                */
/* -------------------------------------------------------------------- */
  bassigncstr(writeable, "writeable");
  bassigncstr(bfalse, "FALSE");

#ifdef DEBUG
  printf("GS_ObjectLock: 6\n");
#endif

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

#ifdef DEBUG
  printf("GS_ObjectLock: 7\n");
#endif

    // each has a metadata item named "writeable"
  writestring=bfromcstr("");
  if(!GS_GetStringAttribute(id,writeable,writestring)){
    bassigncstr(error_string,"GS_ObjectLock: Can't get the writeability metadata.");
    GS_FileClose(ifile_id); 
#ifdef DEBUG
  printf("GS_ObjectLock: 7.1\n");
#endif
    bdestroy(writestring);
    bdestroy(writeable);
    bdestroy(bfalse);
    return FALSE;
  }// endif
  if(EQUAL(writestring,bfalse)){
    bdestroy(writestring);
    bdestroy(writeable);
    bdestroy(bfalse);
    GS_FileClose(ifile_id); 
#ifdef DEBUG
  printf("GS_ObjectLock: 7.2\n");
#endif
    return TRUE;
  } else {
    bdestroy(writeable);
    bdestroy(writestring);
    bdestroy(bfalse);
    GS_FileClose(ifile_id); 
#ifdef DEBUG
  printf("GS_ObjectLock: 7.3\n");
#endif
    return FALSE;
  }// endif
 


  default:
    // should never be here.
#ifdef DEBUG
  printf("GS_ObjectLock: 8\n");
#endif

    bassigncstr(error_string,"GS_ObjectLock: unknown object type");
    GS_FileClose(ifile_id); 
    bdestroy(writestring);
    bdestroy(writeable);
    bdestroy(bfalse);
    return FALSE;
  }// endswitch



    bdestroy(writestring);
    bdestroy(writeable);
    bdestroy(bfalse);
#ifdef DEBUG
  printf("GS_ObjectLock: 9\n");
#endif

}
