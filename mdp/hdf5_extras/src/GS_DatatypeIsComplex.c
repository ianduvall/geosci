/* GS_DatatypeIsComplex.c

   by Leland Pierce, Nov 23, 2013

   Aug 8, 2014:
   modifed with bstrings, doxygen, GS_

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"



#define ERROR -1


/** \brief GS_DatatypeIsComplex returns TRUE if a complex datatype


\see GS_DatatypeAsString(), GS_DatatypeIsInteger(), GS_DatatypeAsInteger(), GS_DatatypeNumbytes()

\param[in] datatype
                   An integer representing a GeoSci datatype.
                   Valid datatypes:<br>
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
\c TRUE is returned for complex datatypes, \c FALSE for others,
   and \c ERROR otherwise.

\par Example
Determine if a datatype is complex:
\code
int datatype;
if(GS_DatatypeIsComplex(datatype)==TRUE){
   printf("the datatype is complex.\n);
} else {
   printf("the datatype is not complex\n");
}
\endcode
Note that if one does not compare with TRUE, but instead 
uses the default C behavior of anything not 0 being true:
both TRUE and ERROR are non-zero and so one may not get the
expected behavior.
*/

int GS_DatatypeIsComplex(int datatype)
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
