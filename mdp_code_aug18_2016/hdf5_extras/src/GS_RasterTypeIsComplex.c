/* GS_RasterTypeIsComplex.c

   by Leland Pierce, Nov 23, 2013

   modified June 9, 2016, LEP

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"

/** \brief GS_RasterTypeIsComplex returns TRUE for complex types
\see GS_GetRasterTypeAsString, GS_RasterTypeIsInteger, 
          GS_GetRasterTypeAsInteger, GS_GetRasterTypeNumbytes
\param[in] datatype
            An integer representing a GeoSci datatype.
            Valid datatypes:\n
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
            \c GS_DATATYPE_C128 15  Complex 128-bit floating point \n
            \c GS_DATATYPE_UI64 16  Unsigned 64-bit integer\n
            \c GS_DATATYPE_SI64 17  Signed 64-bit integer\n
\returns
\c TRUE is returned for complex types, \c FALSE for scalar
types and \c ERROR otherwise.
\par Example
Query if the type GS_DATATYPE_CI32 is a complex type:
\code
int result;
result = GS_RasterTypeIsComplex(GS_DATATYPE_CI32);
if( result == TRUE ) {
   printf("It's a complex type\n");
} else if( result == FALSE ){
   printf("It's a scalar type\n");
} else {
   printf("Error!\n");
}
\endcode
*/


int GS_RasterTypeIsComplex(int datatype)
{
  switch(datatype){
  case GS_DATATYPE_UI1:
    return FALSE;
  case GS_DATATYPE_UI8:
    return FALSE;
  case GS_DATATYPE_SI8:
    return FALSE;
  case GS_DATATYPE_CI8: 
    return TRUE;
  case GS_DATATYPE_UI16:
    return FALSE;
  case GS_DATATYPE_SI16:
    return FALSE;
  case GS_DATATYPE_CI16:
    return TRUE;
  case GS_DATATYPE_UI32:
    return FALSE;
  case GS_DATATYPE_SI32:
    return FALSE;
  case GS_DATATYPE_UI64:
    return FALSE;
  case GS_DATATYPE_SI64:
    return FALSE;
  case GS_DATATYPE_CI32:
    return TRUE;
  case GS_DATATYPE_CI64:
    return TRUE;
  case GS_DATATYPE_R32: 
    return FALSE;
  case GS_DATATYPE_R64: 
    return FALSE;
  case GS_DATATYPE_C64: 
    return TRUE;
  case GS_DATATYPE_C128: 
    return TRUE;
  default:
    return ERROR;
  }// end-switch
}
