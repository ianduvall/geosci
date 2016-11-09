/* GS_InitializeNumberRanges.c

   by Leland Pierce, Jan 11, 2014

   Aug 10, 2014
   modified for doxygen, bstring, GS_

*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <complex.h>


#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"



extern long int intranges[GS_DATATYPE_MAX+1][2];
extern double floatranges[GS_DATATYPE_MAX+1][2];


/*! \file */

/** \brief GS_InitializeNumberRanges initializes ranges for various types

GS_InitializeNumberRanges() initializes the ranges for each of
the scalar number types used in GeoSci. This is needed during
type conversions to avoid creating variables whose value is
out-of-range.

\see GS_DataConversion()

\returns
Returns TRUE on success, FALSE on failure.

\par Details
Initializes global variables 
\c intranges and
\c floatranges
to contain the minimum and maximum numbers 
possible for each datatype that GeoSci supports.
*/


int GS_InitializeNumberRanges()
{
#define RMIN 0
#define RMAX 1

  intranges[GS_DATATYPE_UNK][RMIN]=0;//the unknown datatype
  intranges[GS_DATATYPE_UNK][RMAX]=0;

  intranges[GS_DATATYPE_UI1][RMIN]=0;
  intranges[GS_DATATYPE_UI1][RMAX]=1;
  intranges[GS_DATATYPE_UI8][RMIN]=0;
  intranges[GS_DATATYPE_UI8][RMAX]=255;
  intranges[GS_DATATYPE_SI8][RMIN]=-128;
  intranges[GS_DATATYPE_SI8][RMAX]=127;
  intranges[GS_DATATYPE_CI8][RMIN]=0;//not supported, yet
  intranges[GS_DATATYPE_CI8][RMAX]=0;//not supported, yet

  intranges[GS_DATATYPE_UI16][RMIN]=0;
  intranges[GS_DATATYPE_UI16][RMAX]=65535;
  intranges[GS_DATATYPE_SI16][RMIN]=-32768;
  intranges[GS_DATATYPE_SI16][RMAX]=32767;
  intranges[GS_DATATYPE_CI16][RMIN]=-128;
  intranges[GS_DATATYPE_CI16][RMAX]=127;

  intranges[GS_DATATYPE_UI32][RMIN]=0;
  intranges[GS_DATATYPE_UI32][RMAX]=UINT_MAX;
  intranges[GS_DATATYPE_SI32][RMIN]=INT_MIN;
  intranges[GS_DATATYPE_SI32][RMAX]=INT_MAX;
  intranges[GS_DATATYPE_CI32][RMIN]=-32768;
  intranges[GS_DATATYPE_CI32][RMAX]=32767;

  intranges[GS_DATATYPE_UI64][RMIN]=0;
  intranges[GS_DATATYPE_UI64][RMAX]=ULONG_MAX;
  intranges[GS_DATATYPE_SI64][RMIN]=LONG_MIN;
  intranges[GS_DATATYPE_SI64][RMAX]=LONG_MAX;


  intranges[GS_DATATYPE_CI64][RMIN]=INT_MIN;
  intranges[GS_DATATYPE_CI64][RMAX]=INT_MAX;

  floatranges[GS_DATATYPE_R32][RMIN]=-FLT_MAX;
  floatranges[GS_DATATYPE_R32][RMAX]=FLT_MAX;
  floatranges[GS_DATATYPE_R64][RMIN]=-DBL_MAX;
  floatranges[GS_DATATYPE_R64][RMAX]=DBL_MAX;
 
  floatranges[GS_DATATYPE_C64][RMIN]=-FLT_MAX;
  floatranges[GS_DATATYPE_C64][RMAX]=FLT_MAX;
  floatranges[GS_DATATYPE_C128][RMIN]=-DBL_MAX;
  floatranges[GS_DATATYPE_C128][RMAX]=DBL_MAX;

  return TRUE;
}
