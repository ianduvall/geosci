/* GS_RasterTypeNumBytes.c

   by Leland Pierce, Nov 22, 2013

   Aug 9, 2014
   convert with doxygen, bstring, GS_
*/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


/*! \file */

/** \brief GS_RasterTypeNumBytes returns the number of bytes in a raster datatype

\see GS_DatasetOpen(), GS_DatasetCreate(), GS_DatasetRead()

\param[in] datatype
                   An integer representing a GeoSciPy datatype.
                   Valid datatypes:
\c                   GS_DATATYPE_UI1  1   A single bit \n
\c                   GS_DATATYPE_UI8  2   Unsigned 8-bit integer \n
\c                   GS_DATATYPE_SI8  3   Signed 8-bit integer \n
\c                   GS_DATATYPE_CI8  4   Complex 8-bit integer \n
\c                   GS_DATATYPE_UI16 5   Unsigned 16-bit integer \n
\c                   GS_DATATYPE_SI16 6   Signed 16-bit integer \n
\c                   GS_DATATYPE_CI16 7   Complex 16-bit integer \n
\c                   GS_DATATYPE_UI32 8   Unsigned 32-bit integer \n
\c                   GS_DATATYPE_SI32 9   Signed 32-bit integer \n
\c                   GS_DATATYPE_CI32 10  Complex 32-bit integer \n
\c                   GS_DATATYPE_CI64 11  Complex 64-bit integer \n
\c                   GS_DATATYPE_R32  12  32-bit Real number \n
\c                   GS_DATATYPE_R64  13  64-bit Real number \n
\c                   GS_DATATYPE_C64  14  Complex 64-bit floating-point number \n
\c                   GS_DATATYPE_C128 15  Complex 128-bit floating point  \n
\c                   GS_DATATYPE_UI64 16  Unsigned 64-bit integer \n
\c                   GS_DATATYPE_SI64 17  Signed 64-bit integer \n

\returns
The number of bytes is returned on success: 1, 2, 4, 8, or 16.
Zero is returned on failure.
*/


int GS_RasterTypeNumBytes(int datatype)
{
  switch(datatype){
  case GS_DATATYPE_UI1:
    return 1;
  case GS_DATATYPE_UI8:
    return 1;
  case GS_DATATYPE_SI8:
    return 1;
  case GS_DATATYPE_CI8: 
    return 1;
  case GS_DATATYPE_UI16:
    return 2;
  case GS_DATATYPE_SI16:
    return 2;
  case GS_DATATYPE_CI16:
    return 2;
  case GS_DATATYPE_UI32:
    return 4;
  case GS_DATATYPE_SI32:
    return 4;
  case GS_DATATYPE_UI64:
    return 8;
  case GS_DATATYPE_SI64:
    return 8;
  case GS_DATATYPE_CI32:
    return 4;
  case GS_DATATYPE_CI64:
    return 8;
  case GS_DATATYPE_R32: 
    return 4;
  case GS_DATATYPE_R64: 
    return 8;
  case GS_DATATYPE_C64: 
    return 8;
  case GS_DATATYPE_C128: 
    return 16;
  default:
    return 0;
  }// end-switch

}
