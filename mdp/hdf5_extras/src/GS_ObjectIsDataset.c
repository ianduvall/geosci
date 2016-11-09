/* GS_ObjectIsDataset.c

   by Leland Pierce, Nov 19, 2013

  
   July 8, 2014:
   modified for multi-level library
   modified to use bstrings

   Aug 2, 2014
   modified for doxygen, GS_

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


//#define DEBUG


/** \brief GS_ObjectIsDataset determines if an object is a dataset


\see GS_FileOpen(), GS_FileCreate()

\param[in] id
                   File handle or Image handle of selected GeoSci file.
\param[in] object_name;   
                   Name of object to query.

\returns
       TRUE is returned if the object is a dataset, FALSE otherwise.

\par Example:
Query about a raster named "r1" in an image named "Channel_1",
in a file named "test123.h5".
\code
hid_t    file_id, image_id;
bstring filename = bfromcstr("test123.h5");
bstring access = bfromcstr("r+");
file_id = GS_FileOpen(filename,access);
if(file_id < 0) {
   printf("Could not open file.\n");
   bdestroy(filename);
   bdestroy(access);
   return;
}
bdestroy(filename);
bdestroy(access);
bstring channel=bfromcstr("Channel_1");
image_id = GS_ImageOpen(file_id,channel);
if(image_id < 0) {
   printf("Could not open image.\n");
   bdestroy(channel);
   return;
}
bdestroy(channel);
bstring dataset_name = bfromcstr("r1");
if(GS_ObjectIsDataset(image_id,dataset_name)){
   ... open the object as a dataset and do stuff ....
} else {
   printf("Object is not a dataset.\n");
} // endif
bdestroy(dataset_name);
\endcode
*/




//#define DEBUG


int GS_ObjectIsDataset(hid_t id, const_bstring name)
{
  H5O_info_t objinfo;
  hid_t ifile_id;
  char *name2;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");


#ifdef DEBUG
  printf("GS_ObjectIsDataset: top\n");
#endif
/* -------------------------------------------------------------------- */
/* Check for valid id                                                   */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID( id )){
    bassigncstr(error_string,"GS_ObjectIsDataset: invalid handle\n");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_ObjectIsDataset: 1\n");
#endif


/* -------------------------------------------------------------------- */
/* Check if name exists                                                 */
/* -------------------------------------------------------------------- */
  name2 = (char *)GMalloc(sizeof(char)*(bstrlen(name)+1) );
  if(!name2){
    bassigncstr(error_string,"GS_ObjectIsDataset: GMalloc error\n");
    return FALSE;
  }
  strcpy(name2, bdata(name));
  if(H5Lexists( id, name2, H5P_DEFAULT ) != TRUE){
    GFree(name2);
    return FALSE;
  }// endif


#ifdef DEBUG
  printf("GS_ObjectIsDataset: 2\n");
#endif

/* -------------------------------------------------------------------- */
/* Get object HDF5 type                                                 */
/* -------------------------------------------------------------------- */
  if(H5Oget_info_by_name( id, name2, &objinfo, H5P_DEFAULT) <0){
    GFree(name2);
    return FALSE;
  }// endif
  GFree(name2);

#ifdef DEBUG
  printf("GS_ObjectIsDataset: 3\n");
#endif


/* -------------------------------------------------------------------- */
/* Check if object type is DATASET or not                               */
/* -------------------------------------------------------------------- */
  if( objinfo.type != H5O_TYPE_DATASET) return FALSE;

#ifdef DEBUG
  printf("GS_ObjectIsDataset: 4\n");
#endif


  return TRUE;

}
