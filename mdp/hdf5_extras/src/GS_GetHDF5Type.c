/* GS_GetHDF5Type.c

   by Leland Pierce, Nov 18, 2013
   
   modified May 22, 2016, LEP

*/



/** \brief GetHDF5Type returns the HDF type for the given GeoSci Type


\param[in] datatype
                 An integer representing a GeoSci datatype.
                 Valid datatypes are:<br>
                 \c GS_DATATYPE_UI1  1   A single bit\n
                 \c GS_DATATYPE_UI8  2   Unsigned 8-bit integer\n
                 \c GS_DATATYPE_SI8  3   Signed 8-bit integer\n
                 \c GS_DATATYPE_CI8  4   Complex 8-bit integer\n
                 \c GS_DATATYPE_UI16 5   Unsigned 16-bit integer\n
                 \c GS_DATATYPE_SI16 6   Signed 16-bit integer\n
                 \c GS_DATATYPE_CI16 7   Complex 16-bit integer\n
                 \c GS_DATATYPE_UI32 8   Unsigned 32-bit integer\n
                 \c GS_DATATYPE_SI32 9   Signed 32-bit integer\n
                 \c GS_DATATYPE_CI32 10  Complex 32-bit integer\n
                 \c GS_DATATYPE_CI64 11  Complex 64-bit integer\n
                 \c GS_DATATYPE_R32  12  32-bit Real number\n
                 \c GS_DATATYPE_R64  13  64-bit Real number\n
                 \c GS_DATATYPE_C64  14  Complex 64-bit floating-point number\n
                 \c GS_DATATYPE_C128 15  Complex 128-bit floating point number\n
                 \c GS_DATATYPE_SI64 16  Signed 64-bit integer\n


\returns
       The type is returned, and is negative if invalid.

\par Example
Get the HDF5 equivalent of the raster type complex-integer-8-bit.
\code
hid_t hdf_type;

hdf_type = GetHDF5Type(GS_DATATYPE_CI8);
if(hdf_type<0) {
   printf("GetHDF5Type failure\n");
   exit(-1);
}
\endcode
*/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"

hid_t GS_GetHDF5Type(int datatype)
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
