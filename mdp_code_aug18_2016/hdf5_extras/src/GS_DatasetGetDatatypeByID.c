/* GS_DatasetGetDatatypeByID.c

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




/** \brief GS_DatasetGetDatatypeByID returns the GeoSci datatype for the data in the Dataset 


\see GS_DatasetOpen(), GS_DatasetCreate()

\param[in] id
                   Datset handle

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
Get the GeoSci type code for the data in an image raster Dataset.
\code
hid_t dataset_id;
int gs_type;
gs_type = GS_DatasetGetDatatypeByID(dataset_id);
if(!gs_type) {
   printf("GS_DatasetGetDatatypeByID failure\n");
}
\endcode

*/

int GS_DatasetGetDatatypeByID(hid_t id)
{
  hid_t hdf_datatype;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Check for valid Dataset                                              */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsDatasetByID(id)){
    bassigncstr(error_string,"GS_DatasetGetDatatypeByID: is is not a Dataset.\n");
    return FALSE;
  }// endif

/* ------------------------------------------------------------ */
/* Get the type of the Dataset                                  */
/* ------------------------------------------------------------ */
  hdf_datatype = H5Dget_type(id);
  if(hdf_datatype<0){
    bassigncstr(error_string,"GS_DatasetGetDatatypeByID: Can't get datatype of Dataset.");
    return FALSE;
  }// endif

/* ------------------------------------------------------------ */
/* Convert to GeoSci Datatype, return it                        */
/* ------------------------------------------------------------ */
  return GS_ConvertFromHDFDatatype(hdf_datatype);

}
