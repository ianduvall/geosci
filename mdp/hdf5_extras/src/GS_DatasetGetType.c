/* GS_DatasetGetType.c

   by Leland Pierce, Nov 16, 2013

   converted to GS_, bstr, and doxygen on May 22, 2016, LEP
*/


/** \brief GS_DatasetGetType returns the type of a GeoSci Dataset 


\see GS_DatasetOpen(), GS_DatasetCreate()

\param[in] dataset_id
                   A handle for the dataset.

\returns
The dataset type code is returned on success:<br>
\c GS_OBJECT_TYPE_IFILE            (10)\n
\c GS_OBJECT_TYPE_RASTER           (9)\n
\c GS_OBJECT_TYPE_METADATA_DATASET (12)\n
\c GS_OBJECT_TYPE_METADATA_IFILE   (13)\n
<br>
On failure, the following type code is returned:<br>
\c GS_OBJECT_TYPE_UNKNOWN (0)

\par Example
       Get the type of an image raster dataset:
\code
hid_t raster_id;

if(GS_DatasetGetType(raster_id) != GS_OBJECT_TYPE_RASTER ) {
  printf("Dataset is not a raster.\n");
}
\endcode
*/

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


int GS_DatasetGetType(hid_t dataset_id)
{
  bstring type_string=bfromcstr("");
  int typecode;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* See if the Id is valid                                               */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID(dataset_id)){
    bassignformat(error_string,"GS_DatasetGetType: Invalid ID.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Check if its a dataset id                                            */
/* -------------------------------------------------------------------- */
  if(H5Iget_type( dataset_id ) != H5I_DATASET){
    bassignformat(error_string,"GS_DatasetGetType: Not a dataset ID.");
    return FALSE;
  }// endif


/* -------------------------------------------------------------------- */
/* Read the typecode                                                    */
/* -------------------------------------------------------------------- */
  bstring dataset_type = bfromcstr("dataset_type");
  if(!GS_GetStringAttribute(dataset_id,dataset_type,type_string)){
    bassignformat(error_string,"GS_DatasetGetType: Can't get the dataset type metadata.");
    bdestroy(dataset_type);
    return FALSE;
  }// endif
  bdestroy(dataset_type);

/* -------------------------------------------------------------------- */
/* Decode the typestring                                                */
/* -------------------------------------------------------------------- */
  if(sscanf(bdata(type_string),"%d",&typecode)!= 1) {
    bassignformat(error_string,"GS_DatasetGetType: Can't decode the dataset type metadata.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Return value                                                         */
/* -------------------------------------------------------------------- */  
  return typecode;

}
