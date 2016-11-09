/* GS_ConvertFromHDFDatatype.c

   by Leland Pierce, Nov 18, 2013


   Aug 9, 2014
   convert to use bstrings, doxygen, GS_

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//#include "../base/base.h"
//#include "hdf.h"
//#include "../obj/obj_datatypes.h"
#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"

#define ERROR -1




/** \brief GS_ConvertFromHDFDatatype returns the GeoSci datatype for the given HDF datatype

\sa GS_ConvertToHDFDatatype
\param[in]  datatype
                   A handle to an HDF5 datatype.

\returns
       The GeoSci typecode is returned, and is 0 if invalid.
       Valid datatypes are:\n
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
\c GS_DATATYPE_SI64 (Signed Integer, 64-bit)

\par Example
Get the GeoSci equivalent of the HDF5 type H5T_NATIVE_INT8:
\code
int gs_type;
gs_type = GS_ConvertFromHDFDatatype(H5T_NATIVE_INT8);
if(!gs_type) {
   printf("GS_ConvertFromHDFDatatype failure\n");
}
\endcode

\par Details
Currently this checks against the HDF5 "NATIVE" datatypes.
I suspect this will not work if the data is in a different byte order.
Need to fix this. (LEP: Aug 9, 2014).
*/


int GS_ConvertFromHDFDatatype(hid_t datatype)
{
  int i;
  
// complex datatypes:
  typedef struct {
    double re; /*real part */
    double im; /*imaginary part */
  } cc128_t;

  typedef struct {
    float re; /*real part */
    float im; /*imaginary part */
  } cc64_t;


  typedef struct {
    int8_t re; /*real part */
    int8_t im; /*imaginary part */
  } cci16_t;

  typedef struct {
    int16_t re; /*real part */
    int16_t im; /*imaginary part */
  } cci32_t;

  typedef struct {
    int32_t re; /*real part */
    int32_t im; /*imaginary part */
  } cci64_t;


  cc128_t c128; /*used only to compute offsets */
  hid_t c128_id;

  cc64_t c64; /*used only to compute offsets */
  hid_t c64_id;

  cci16_t ci16; /*used only to compute offsets */
  hid_t ci16_id;

  cci32_t ci32; /*used only to compute offsets */
  hid_t ci32_id;

  cci64_t ci64; /*used only to compute offsets */
  hid_t ci64_id;

    ci16_id = H5Tcreate (H5T_COMPOUND, sizeof(cci16_t));
    if(ci16_id<0) return ERROR;
    H5Tinsert (ci16_id, "real",      HOFFSET(cci16_t,re),H5T_NATIVE_INT8);
    H5Tinsert (ci16_id, "imaginary", HOFFSET(cci16_t,im),H5T_NATIVE_INT8);

    ci32_id = H5Tcreate (H5T_COMPOUND, sizeof(cci32_t));
    if(ci32_id<0) return ERROR;
    H5Tinsert (ci32_id, "real",      HOFFSET(cci32_t,re),H5T_NATIVE_INT16);
    H5Tinsert (ci32_id, "imaginary", HOFFSET(cci32_t,im),H5T_NATIVE_INT16);


    ci64_id = H5Tcreate (H5T_COMPOUND, sizeof(cci64_t));
    if(ci64_id<0) return ERROR;
    H5Tinsert (ci64_id, "real",      HOFFSET(cci64_t,re),H5T_NATIVE_INT32);
    H5Tinsert (ci64_id, "imaginary", HOFFSET(cci64_t,im),H5T_NATIVE_INT32);

    c64_id = H5Tcreate (H5T_COMPOUND, sizeof(cc64_t));
    if(c64_id<0) return ERROR;
    H5Tinsert (c64_id, "real",      HOFFSET(cc64_t,re),H5T_NATIVE_FLOAT);
    H5Tinsert (c64_id, "imaginary", HOFFSET(cc64_t,im),H5T_NATIVE_FLOAT);

    c128_id = H5Tcreate (H5T_COMPOUND, sizeof(cc128_t));
    if(c128_id<0) return ERROR;
    H5Tinsert (c128_id, "real",      HOFFSET(cc128_t,re),H5T_NATIVE_DOUBLE);
    H5Tinsert (c128_id, "imaginary", HOFFSET(cc128_t,im),H5T_NATIVE_DOUBLE);


  if(H5Tequal(datatype,H5T_NATIVE_UINT8)){
    i = GS_DATATYPE_UI8;
  } else if(H5Tequal(datatype,H5T_NATIVE_INT8)){
    i = GS_DATATYPE_SI8;
  } else if(H5Tequal(datatype,H5T_NATIVE_UINT16)){
    i = GS_DATATYPE_UI16;
  } else if(H5Tequal(datatype,H5T_NATIVE_INT16)){
    i = GS_DATATYPE_SI16;
  } else if(H5Tequal(datatype,ci16_id)){
    i = GS_DATATYPE_CI16;
  } else if(H5Tequal(datatype,H5T_NATIVE_UINT32)){
    i = GS_DATATYPE_UI32;
  } else if(H5Tequal(datatype,H5T_NATIVE_INT32)){
    i = GS_DATATYPE_SI32;
  } else if(H5Tequal(datatype,ci32_id)){
    i = GS_DATATYPE_CI32;
  } else if(H5Tequal(datatype,ci64_id)){
    i = GS_DATATYPE_CI64;
  } else if(H5Tequal(datatype,H5T_NATIVE_FLOAT)){
    i = GS_DATATYPE_R32;
  } else if(H5Tequal(datatype,H5T_NATIVE_DOUBLE)){
    i = GS_DATATYPE_R64;
  } else if(H5Tequal(datatype,c64_id)){
    i = GS_DATATYPE_C64;
  } else if(H5Tequal(datatype,c128_id)){
    i = GS_DATATYPE_C128;
  } else if(H5Tequal(datatype,H5T_NATIVE_UINT64)){
    i = GS_DATATYPE_UI64;
  } else if(H5Tequal(datatype,H5T_NATIVE_INT64)){
    i = GS_DATATYPE_SI64;
  } else {
    i = FALSE;
  }

  H5Tclose(ci16_id);
  H5Tclose(ci32_id);
  H5Tclose(ci64_id);
  H5Tclose(c64_id);
  H5Tclose(c128_id);
  
  return i;

}
