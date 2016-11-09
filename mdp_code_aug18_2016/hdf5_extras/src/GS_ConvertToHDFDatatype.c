/* GS_ConvertToHDFDatatype.c

   by Leland Pierce, Nov 18, 2013

   Aug 7, 2014
   convert to use bstrings, doxygen, GS_

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


#define ERROR -1

#define DEBUG



/** \brief GS_ConvertToHDFDatatype returns the HDF datatype for the given GeoSci datatype

\sa GS_ConvertFromHDFDatatype

\param[in]  datatype
               An integer representing a GeoSci datatype.
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


\returns
       An HDF5 datatype is returned, and is negative if invalid.
       Complex datatypes should be H5Tclose()'d when done with them.

\par Example
Get the HDF5 equivalent of the raster type complex-integer-16-bit.
\code
hid_t hdf_type;

hdf_type = GS_ConvertToHDFDatatype(GS_DATATYPE_CI16);
if(hdf_type<0) {
   printf("GS_ConvertToHDFDatatype failure\n");
}
\endcode
*/


hid_t GS_ConvertToHDFDatatype(int datatype)
{
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


  switch(datatype){
  case GS_DATATYPE_UI1:
    return H5T_NATIVE_UINT8;
  case GS_DATATYPE_UI8:
    return H5T_NATIVE_UINT8;
  case GS_DATATYPE_SI8:
    return H5T_NATIVE_INT8;
  case GS_DATATYPE_CI8: 
    return ERROR; // for now
  case GS_DATATYPE_UI16:
    return H5T_NATIVE_UINT16;
  case GS_DATATYPE_SI16:
    return H5T_NATIVE_INT16;
  case GS_DATATYPE_CI16:
    ci16_id = H5Tcreate (H5T_COMPOUND, sizeof(cci16_t));
    if(ci16_id<0) return ERROR;
    H5Tinsert (ci16_id, "real",      HOFFSET(cci16_t,re),H5T_NATIVE_INT8);
    H5Tinsert (ci16_id, "imaginary", HOFFSET(cci16_t,im),H5T_NATIVE_INT8);
    return ci16_id;
  case GS_DATATYPE_UI32:
    return H5T_NATIVE_UINT32;
  case GS_DATATYPE_SI32:
    return H5T_NATIVE_INT32;
  case GS_DATATYPE_CI32:
    ci32_id = H5Tcreate (H5T_COMPOUND, sizeof(cci32_t));
    if(ci32_id<0) return ERROR;
    H5Tinsert (ci32_id, "real",      HOFFSET(cci32_t,re),H5T_NATIVE_INT16);
    H5Tinsert (ci32_id, "imaginary", HOFFSET(cci32_t,im),H5T_NATIVE_INT16);
    return ci32_id;
  case GS_DATATYPE_CI64:
    ci64_id = H5Tcreate (H5T_COMPOUND, sizeof(cci64_t));
    if(ci64_id<0) return ERROR;
    H5Tinsert (ci64_id, "real",      HOFFSET(cci64_t,re),H5T_NATIVE_INT32);
    H5Tinsert (ci64_id, "imaginary", HOFFSET(cci64_t,im),H5T_NATIVE_INT32);
    return ci64_id;
  case GS_DATATYPE_R32: 
    return H5T_NATIVE_FLOAT;
  case GS_DATATYPE_R64: 
    return H5T_NATIVE_DOUBLE;
  case GS_DATATYPE_C64: 
    c64_id = H5Tcreate (H5T_COMPOUND, sizeof(cc64_t));
    if(c64_id<0) return ERROR;
    H5Tinsert (c64_id, "real",      HOFFSET(cc64_t,re),H5T_NATIVE_FLOAT);
    H5Tinsert (c64_id, "imaginary", HOFFSET(cc64_t,im),H5T_NATIVE_FLOAT);
    return c64_id;
  case GS_DATATYPE_C128: 
    c128_id = H5Tcreate (H5T_COMPOUND, sizeof(cc128_t));
    if(c128_id<0) return ERROR;
    H5Tinsert (c128_id, "real",      HOFFSET(cc128_t,re),H5T_NATIVE_DOUBLE);
    H5Tinsert (c128_id, "imaginary", HOFFSET(cc128_t,im),H5T_NATIVE_DOUBLE);
    return c128_id;

  case GS_DATATYPE_UI64:
    return H5T_NATIVE_UINT64;
  case GS_DATATYPE_SI64:
    return H5T_NATIVE_INT64;
  default:
    return ERROR;
  }// end-switch
}
