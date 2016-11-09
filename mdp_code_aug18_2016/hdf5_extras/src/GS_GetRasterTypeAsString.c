/* GS_GetRasterTypeAsString.c

   by Leland Pierce, Nov 18, 2013

   modified June 9, 2016, LEP  

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"

/** \brief GS_GetRasterTypeAsString returns a string representing the datatype.
\see GS_GetRasterTypeAsInteger, GS_GetRasterTypeNumbytes
\param[in]  datatype
            An integer representing a GeoSci raster datatype.
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
The bstring is returned on success, or a zero-length bstring on failure.
bdestroy() it when done.
\par Example
Get a string represenation of the raster type GS_DATATYPE_R64:
\code
bstring str = bfromcstr("");
bassignstr(str,GS_GetRasterTypeAsString(GS_DATATYPE_R64));
printf("the string is: %s\n",bdata(str));
bdestroy(str);
\endcode
*/


/*********************************************************************/
bstring GS_GetRasterTypeAsString(int datatype)
{
  switch(datatype){
  case GS_DATATYPE_UI1:
    return bfromcstr("UINT1");
  case GS_DATATYPE_UI8:
    return bfromcstr("UINT8");
  case GS_DATATYPE_SI8:
    return bfromcstr("INT8");
  case GS_DATATYPE_CI8: 
    return bfromcstr("CINT8");
  case GS_DATATYPE_UI16:
    return bfromcstr("UINT16");
  case GS_DATATYPE_SI16:
    return bfromcstr("INT16");
  case GS_DATATYPE_CI16:
    return bfromcstr("CINT16");
  case GS_DATATYPE_UI32:
    return bfromcstr("UINT32");
  case GS_DATATYPE_SI32:
    return bfromcstr("INT32");
  case GS_DATATYPE_UI64:
    return bfromcstr("UINT64");
  case GS_DATATYPE_SI64:
    return bfromcstr("INT64");
  case GS_DATATYPE_CI32:
    return bfromcstr("CINT32");
  case GS_DATATYPE_CI64:
    return bfromcstr("CINT64");
  case GS_DATATYPE_R32: 
    return bfromcstr("R32");
  case GS_DATATYPE_R64: 
    return bfromcstr("R64");
  case GS_DATATYPE_C64: 
    return bfromcstr("C64");
  case GS_DATATYPE_C128: 
    return bfromcstr("C128");
  default:
    return bfromcstr("");
  }// end-switch
}
