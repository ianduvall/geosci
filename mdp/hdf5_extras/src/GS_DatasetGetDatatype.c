/* GS_DatasetGetDatatype.c

   by Leland Pierce, Aug 9, 2014
   with doxygen, bstrings, GS_

*/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"



/** \brief GS_DatasetGetDatatype returns the GeoSci datatype for data in the Dataset 


\see GS_DatasetOpen(), GS_DatasetCreate()

\param[in] id
                   File handle of selected GeoSci file, 
                   or object handle of the container for the Dataset.
\param[in] object_name
                   Name of object to query.

\returns
The GeoSci datatype code is returned, and is 0 if there is an error.
       Valid datatypes are:<br>
\c GS_DATATYPE_UI1 (Unsigned Integer, 1-bit)\n
\c GS_DATATYPE_UI8 (Unsigned Integer, 8-bit)\n
\c GS_DATATYPE_SI8 (Signed Integer, 8-bit)\n
\c GS_DATATYPE_CI8 (Complex Integer, 8-bit (not supported))\n
\c GS_DATATYPE_UI16 (Unsigned Integer, 16-bit)\n
\c GS_DATATYPE_SI16 (Signed Integer, 16-bit)\n
\c GS_DATATYPE_CI16 (Complex Integer, 16-bit)\n
\c GS_DATATYPE_UI32 (Unsigned Integer, 32-bit)\n
\c GS_DATATYPE_SI32 (Signed Integer, 32-bit)\n
\c GS_DATATYPE_CI32 (Complex Integer, 32-bit)\n
\c GS_DATATYPE_CI64 (Complex Integer, 64-bit)\n
\c GS_DATATYPE_R32 (Real, 32-bit)\n
\c GS_DATATYPE_R64 (Real, 64-bit)\n
\c GS_DATATYPE_C64 (Complex, 64-bit)\n
\c GS_DATATYPE_C128 (Complex, 128-bit)\n
\c GS_DATATYPE_UI64 (Unsigned Integer, 64-bit)\n
\c GS_DATATYPE_SI64 (Signed Integer, 64-bit)\n


\par Example
Get the GeoSci type code for an image raster Dataset.
\code
hid_t image_id;
int gs_type;
bstring name = bfromcstr("r1");
gs_type = GS_DatasetGetDatatype(image_id,name);
bdestroy(name);
if(!gs_type) {
   printf("GS_DatasetGetDatatype failure\n");
}
\endcode

*/

int GS_DatasetGetDatatype(hid_t id, const_bstring name)
{
  hid_t dataset_id;
  hid_t hdf_datatype;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Check for valid Dataset                                              */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsDataset(id, name)){
    bassigncstr(error_string,"GS_DatasetGetDatatype: is is not a Dataset.\n");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Open the Dataset                                                     */
/* -------------------------------------------------------------------- */
  dataset_id = GS_DatasetOpen(id, name);
  if(dataset_id < 0){
    bassigncstr(error_string,"GS_DatasetGetDatatype: can't open the Dataset.\n");
    return FALSE;
  }// endif

/* ------------------------------------------------------------ */
/* Get the type of the Dataset                                  */
/* ------------------------------------------------------------ */
  hdf_datatype = H5Dget_type(dataset_id);
  if(hdf_datatype<0){
    bassigncstr(error_string,"GS_DatasetGetDatatype: Can't get datatype of Dataset.");
    GS_DatasetClose(dataset_id);   
    return FALSE;
  }// endif

/* ------------------------------------------------------------ */
/* Close the Dataset                                            */
/* ------------------------------------------------------------ */
  if(GS_DatasetClose(dataset_id)){
    bassigncstr(error_string,"GS_DatasetGetDatatype: Can't close the Dataset.");
    GS_DatasetClose(dataset_id);// try again....   
    return FALSE;
  }// endif

/* ------------------------------------------------------------ */
/* Convert to GeoSci Datatype, return it                        */
/* ------------------------------------------------------------ */
  return GS_ConvertFromHDFDatatype(hdf_datatype);

}
