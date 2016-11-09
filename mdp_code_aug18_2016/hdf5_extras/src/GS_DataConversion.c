/* GS_DataConversion.c

   by Leland Pierce, Aug 9, 2014

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




/* -------------------------------------------------------------------- */
/*	For allowable ranges of difft types                             */
/* -------------------------------------------------------------------- */
long int intranges[GS_DATATYPE_MAX+1][2];
double floatranges[GS_DATATYPE_MAX+1][2];
int GS_InitializeNumberRanges(void);

#define RMIN 0
#define RMAX 1


/*! \file */

/** \brief GS_DataConversion converts a data buffer between different types

\see GS_DatasetOpen(), GS_DatasetCreate(), GS_DatasetRead()

\param[in] in_type
               The type of data in \c in_buffer.
          The valid datatypes are:
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
\param[in] in_buffer
               The data we want to convert to a different type.
\param[in] out_type
               The type of data we want in \c out_buffer. 
               Same valid datatypes as in \c in_type.
\param[in] out_buffer
               The destination of the converted data from \c in_buffer.
\param[in] nelements
               The number of elements to convert.

\returns
\c TRUE on sucess, \c FALSE on failure.

\par Example
We have a buffer that is 1000 ints, and we'd like to convert it to 
doubles.
\code
int inbuf[1000];
double outbuf[1000];
if(!GS_DataConversion(GS_DATATYPE_SI32,inbuf,
                      GS_DATATYPE_R64, outbuf,1000)){
  printf("data conversion failed\n");
}
...here use outbuf...
\endcode

\par Details
YET: data type conversions....
*/


//#define DEBUG

int GS_DataConversion( int in_type, const void *in_buffer,
                       int out_type, void *out_buffer, 
                       int nelements)
{
  int i;
  int totype_is_int;
  double complex tempdc;
  double realpart, imagpart;
  static int number_ranges_initialized=FALSE;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Special case of no conversion                                        */
/* -------------------------------------------------------------------- */
  if(in_type==out_type){
    nelements *= GS_RasterTypeNumBytes(in_type); // now nbytes!
    for(i=0;i<nelements;i++){
      ((char *)out_buffer)[i] = ((char *)in_buffer)[i];
    }// endfor
#ifdef DEBUG
    printf("GS_DataConversion: returning after no-conversion copy.\n");
#endif
    return TRUE;
  }// endif

/* -------------------------------------------------------------------- */
/* Get the index for the range variables,                               */
/* and get if type is integer or real                                   */
/* -------------------------------------------------------------------- */
  totype_is_int = GS_RasterTypeIsInteger(out_type);
  if(totype_is_int < 0) {
    bassigncstr(status_string,"GS_DataConversion: to-Type is Unknown, setting to UINT8.");
    out_type = GS_DATATYPE_UI8;
    totype_is_int = TRUE;
  }// endif

/* -------------------------------------------------------------------- */
/* Prepare to deal with ranges of to-type                               */
/* -------------------------------------------------------------------- */
/* note: the vectors intranges[][] and floatranges[][] are defined in   */
/*       geoscipy_c.h. number_ranges_initialized is global, and is used */
/*       to determine if need to init the values in these arrays, or not*/
/* -------------------------------------------------------------------- */
  if(!number_ranges_initialized) {
    GS_InitializeNumberRanges();
    number_ranges_initialized = TRUE;
  }// endif


/* -------------------------------------------------------------------- */
/* Convert                                                              */
/* -------------------------------------------------------------------- */
  if(GS_RasterTypeIsComplex(in_type) == FALSE){
    // deal with the usual scalar types here
    switch(in_type){
    case GS_DATATYPE_UI1:
        switch(out_type){
        case GS_DATATYPE_UI1:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI8:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          } //endfor
          break;
        case GS_DATATYPE_SI8:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI16:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int16_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI16:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI32:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int32_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI32:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int64_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int64_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_R32:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ i ] = (float) realpart;
          }// endfor
          break;
        case GS_DATATYPE_R64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ i ] =  realpart;
          }// endfor
          break;

        case GS_DATATYPE_CI16:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int8_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_CI32:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int16_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_CI64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int32_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_C64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ 2*i + 0] = (float) realpart;
            ((float *)out_buffer)[ 2*i + 1] = 0.0;
          }// endfor
          break;
        case GS_DATATYPE_C128:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ 2*i + 0] =  realpart;
            ((double *)out_buffer)[ 2*i + 1] = 0.0;
          }// endfor
          break;
        }// endswitch
      break;
      //-----------------------------------------------------------------------------------
    case GS_DATATYPE_UI8:
        switch(out_type){
        case GS_DATATYPE_UI1:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endif
          break;
        case GS_DATATYPE_UI8:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI8:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI16:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int16_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI16:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI32:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int32_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI32:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int64_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int64_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_R32:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ i ] = (float) realpart;
          }// endfor
          break;
        case GS_DATATYPE_R64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ i ] =  realpart;
          }// endfor
          break;

        case GS_DATATYPE_CI16:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int8_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_CI32:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int16_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_CI64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int32_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_C64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ 2*i + 0] = (float) realpart;
            ((float *)out_buffer)[ 2*i + 1] = 0.0;
          }// endfor
          break;
        case GS_DATATYPE_C128:
          for(i=0;i<nelements;i++){
            realpart = ((u_int8_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ 2*i + 0] =  realpart;
            ((double *)out_buffer)[ 2*i + 1] = 0.0;
          }// endfor
          break;
        }// endswitch
      break;
      //---------------------------------------------------------------------------------------
    case GS_DATATYPE_SI8:
        switch(out_type){
        case GS_DATATYPE_UI1:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[i]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI8:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[i]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI8:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[i]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI16:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[i]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int16_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI16:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[i]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI32:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[i]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int32_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI32:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[i]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI64:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[i]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int64_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI64:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[i]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int64_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_R32:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[i]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ i ] = (float) realpart;
          }// endfor
          break;
        case GS_DATATYPE_R64:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[i]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ i ] =  realpart;
          }// endfor
          break;

        case GS_DATATYPE_CI16:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[i]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int8_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_CI32:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[i]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int16_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_CI64:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[i]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int32_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_C64:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[i]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ 2*i + 0] = (float) realpart;
            ((float *)out_buffer)[ 2*i + 1] = 0.0;
          }// endfor
          break;
        case GS_DATATYPE_C128:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[i]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ 2*i + 0] =  realpart;
            ((double *)out_buffer)[ 2*i + 1] = 0.0;
          }// endfor
          break;
        }// endswitch
        break;
        //--------------------------------------------------------------------------------------
    case GS_DATATYPE_UI16:
        switch(out_type){
        case GS_DATATYPE_UI1:
          for(i=0;i<nelements;i++){
            realpart = ((u_int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI8:
          for(i=0;i<nelements;i++){
            realpart = ((u_int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI8:
          for(i=0;i<nelements;i++){
            realpart = ((u_int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI16:
          for(i=0;i<nelements;i++){
            realpart = ((u_int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int16_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI16:
          for(i=0;i<nelements;i++){
            realpart = ((u_int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI32:
          for(i=0;i<nelements;i++){
            realpart = ((u_int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int32_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI32:
          for(i=0;i<nelements;i++){
            realpart = ((u_int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int64_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int64_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_R32:
          for(i=0;i<nelements;i++){
            realpart = ((u_int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ i ] = (float) realpart;
          }// endfor
          break;
        case GS_DATATYPE_R64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ i ] =  realpart;
          }// endfor
          break;

        case GS_DATATYPE_CI16:
          for(i=0;i<nelements;i++){
            realpart = ((u_int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int8_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_CI32:
          for(i=0;i<nelements;i++){
            realpart = ((u_int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int16_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_CI64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int32_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_C64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ 2*i + 0] = (float) realpart;
            ((float *)out_buffer)[ 2*i + 1] = 0.0;
          }// endfor
          break;
        case GS_DATATYPE_C128:
          for(i=0;i<nelements;i++){
            realpart = ((u_int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ 2*i + 0] =  realpart;
            ((double *)out_buffer)[ 2*i + 1] = 0.0;
          }//endfor
          break;
        }// endswitch
        break;
        //---------------------------------------------------------------------------------
    case GS_DATATYPE_SI16:
        switch(out_type){
        case GS_DATATYPE_UI1:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI8:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI8:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI16:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int16_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI16:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI32:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int32_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI32:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI64:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int64_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI64:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int64_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_R32:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ i ] = (float) realpart;
          }// endfor
          break;
        case GS_DATATYPE_R64:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ i ] =  realpart;
          }// endfor
          break;

        case GS_DATATYPE_CI16:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int8_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_CI32:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int16_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_CI64:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int32_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_C64:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ 2*i + 0] = (float) realpart;
            ((float *)out_buffer)[ 2*i + 1] = 0.0;
          }// endfor
          break;
        case GS_DATATYPE_C128:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ i ];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ 2*i + 0] =  realpart;
            ((double *)out_buffer)[ 2*i + 1] = 0.0;
          }// endfor
          break;
        }// endswitch
      break;
      //----------------------------------------------------------------------------------------------
    case GS_DATATYPE_UI32:
        switch(out_type){
        case GS_DATATYPE_UI1:
          for(i=0;i<nelements;i++){
            realpart = ((u_int32_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI8:
          for(i=0;i<nelements;i++){
            realpart = ((u_int32_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI8:
          for(i=0;i<nelements;i++){
            realpart = ((u_int32_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI16:
          for(i=0;i<nelements;i++){
            realpart = ((u_int32_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int16_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI16:
          for(i=0;i<nelements;i++){
            realpart = ((u_int32_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI32:
          for(i=0;i<nelements;i++){
            realpart = ((u_int32_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int32_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI32:
          for(i=0;i<nelements;i++){
            realpart = ((u_int32_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int32_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int64_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int32_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int64_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_R32:
          for(i=0;i<nelements;i++){
            realpart = ((u_int32_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ i ] = (float) realpart;
          }// endfor
          break;
        case GS_DATATYPE_R64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int32_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ i ] =  realpart;
          }// endfor
          break;

        case GS_DATATYPE_CI16:
          for(i=0;i<nelements;i++){
            realpart = ((u_int32_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int8_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_CI32:
          for(i=0;i<nelements;i++){
            realpart = ((u_int32_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int16_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_CI64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int32_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int32_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_C64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int32_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ 2*i + 0] = (float) realpart;
            ((float *)out_buffer)[ 2*i + 1] = 0.0;
          }// endfor
          break;
        case GS_DATATYPE_C128:
          for(i=0;i<nelements;i++){
            realpart = ((u_int32_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ 2*i + 0] =  realpart;
            ((double *)out_buffer)[ 2*i + 1] = 0.0;
          }// endfor
          break;
        }// endswitch
      break;
      //------------------------------------------------------------------------------------------
    case GS_DATATYPE_SI32:
        switch(out_type){
        case GS_DATATYPE_UI1:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI8:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI8:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI16:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int16_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI16:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI32:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int32_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI32:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI64:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int64_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI64:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int64_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_R32:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ i ] = (float) realpart;
          }// endfor
          break;
        case GS_DATATYPE_R64:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ i ] =  realpart;
          }// endfor
          break;

        case GS_DATATYPE_CI16:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int8_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_CI32:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int16_t *)out_buffer)[ 2*i + 1] = 0;
          }//endfor
          break;
        case GS_DATATYPE_CI64:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int32_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_C64:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ 2*i + 0] = (float) realpart;
            ((float *)out_buffer)[ 2*i + 1] = 0.0;
          }// endfor
          break;
        case GS_DATATYPE_C128:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ 2*i + 0] =  realpart;
            ((double *)out_buffer)[ 2*i + 1] = 0.0;
          }// endfor
          break;
        }// endswitch
      break;
      //------------------------------------------------------------------------------------------------
    case GS_DATATYPE_UI64:
        switch(out_type){
        case GS_DATATYPE_UI1:
          for(i=0;i<nelements;i++){
            realpart = ((u_int64_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI8:
          for(i=0;i<nelements;i++){
            realpart = ((u_int64_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI8:
          for(i=0;i<nelements;i++){
            realpart = ((u_int64_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI16:
          for(i=0;i<nelements;i++){
            realpart = ((u_int64_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int16_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI16:
          for(i=0;i<nelements;i++){
            realpart = ((u_int64_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI32:
          for(i=0;i<nelements;i++){
            realpart = ((u_int64_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int32_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI32:
          for(i=0;i<nelements;i++){
            realpart = ((u_int64_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int64_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int64_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int64_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int64_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_R32:
          for(i=0;i<nelements;i++){
            realpart = ((u_int64_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ i ] = (float) realpart;
          }// endfor
          break;
        case GS_DATATYPE_R64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int64_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ i ] =  realpart;
          }// endfor
          break;

        case GS_DATATYPE_CI16:
          for(i=0;i<nelements;i++){
            realpart = ((u_int64_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int8_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_CI32:
          for(i=0;i<nelements;i++){
            realpart = ((u_int64_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int16_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_CI64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int64_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int32_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_C64:
          for(i=0;i<nelements;i++){
            realpart = ((u_int64_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ 2*i + 0] = (float) realpart;
            ((float *)out_buffer)[ 2*i + 1] = 0.0;
          }// endfor
          break;
        case GS_DATATYPE_C128:
          for(i=0;i<nelements;i++){
            realpart = ((u_int64_t *)in_buffer)[i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ 2*i + 0] =  realpart;
            ((double *)out_buffer)[ 2*i + 1] = 0.0;
          }// endfor
          break;
        }// endswitch
      break;
      //--------------------------------------------------------------------------------------------------
    case GS_DATATYPE_SI64:
        switch(out_type){
        case GS_DATATYPE_UI1:
          for(i=0;i<nelements;i++){
            realpart = ((int64_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI8:
          for(i=0;i<nelements;i++){
            realpart = ((int64_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI8:
          for(i=0;i<nelements;i++){
            realpart = ((int64_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI16:
          for(i=0;i<nelements;i++){
            realpart = ((int64_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int16_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI16:
          for(i=0;i<nelements;i++){
            realpart = ((int64_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI32:
          for(i=0;i<nelements;i++){
            realpart = ((int64_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int32_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI32:
          for(i=0;i<nelements;i++){
            realpart = ((int64_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI64:
          for(i=0;i<nelements;i++){
            realpart = ((int64_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int64_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI64:
          for(i=0;i<nelements;i++){
            realpart = ((int64_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int64_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_R32:
          for(i=0;i<nelements;i++){
            realpart = ((int64_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ i ] = (float) realpart;
          }// endfor
          break;
        case GS_DATATYPE_R64:
          for(i=0;i<nelements;i++){
            realpart = ((int64_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ i ] =  realpart;
          }// endfor
          break;

        case GS_DATATYPE_CI16:
          for(i=0;i<nelements;i++){
            realpart = ((int64_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int8_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_CI32:
          for(i=0;i<nelements;i++){
            realpart = ((int64_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int16_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_CI64:
          for(i=0;i<nelements;i++){
            realpart = ((int64_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int32_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_C64:
          for(i=0;i<nelements;i++){
            realpart = ((int64_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ 2*i + 0] = (float) realpart;
            ((float *)out_buffer)[ 2*i + 1] = 0.0;
          }// endfor
          break;
        case GS_DATATYPE_C128:
          for(i=0;i<nelements;i++){
            realpart = ((int64_t *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ 2*i + 0] =  realpart;
            ((double *)out_buffer)[ 2*i + 1] = 0.0;
          }// endfir
          break;
        }// endswitch
      break;
      //----------------------------------------------------------------------------------------
    case GS_DATATYPE_R32:
        switch(out_type){
        case GS_DATATYPE_UI1:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI8:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI8:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI16:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int16_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI16:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI32:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int32_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI32:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI64:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int64_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI64:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int64_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_R32:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ i ] = (float) realpart;
          }// endfor
          break;
        case GS_DATATYPE_R64:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ i ] =  realpart;
          }// endfor
          break;

        case GS_DATATYPE_CI16:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int8_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_CI32:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int16_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_CI64:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int32_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_C64:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ 2*i + 0] = (float) realpart;
            ((float *)out_buffer)[ 2*i + 1] = 0.0;
          }// endfor
          break;
        case GS_DATATYPE_C128:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ i ]; 
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ 2*i + 0] =  realpart;
            ((double *)out_buffer)[ 2*i + 1] = 0.0;
          }// endfor
          break;
        }// endswitch
      break;
      //---------------------------------------------------------------------------------------------
    case GS_DATATYPE_R64:
        switch(out_type){
        case GS_DATATYPE_UI1:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI8:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI8:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_UI16:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int16_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI16:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI32:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int32_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI32:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI64:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int64_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI64:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int64_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_R32:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ i ] = (float) realpart;
          }//endfor
          break;
        case GS_DATATYPE_R64:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ i ] =  realpart;
          }//endfor
          break;

        case GS_DATATYPE_CI16:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int8_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_CI32:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int16_t *)out_buffer)[ 2*i + 1] = 0;
          }//endfor
          break;
        case GS_DATATYPE_CI64:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int32_t *)out_buffer)[ 2*i + 1] = 0;
          }// endfor
          break;
        case GS_DATATYPE_C64:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ 2*i + 0] = (float) realpart;
            ((float *)out_buffer)[ 2*i + 1] = 0.0;
          }//endfor
          break;
        case GS_DATATYPE_C128:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ i];
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ 2*i + 0] =  realpart;
            ((double *)out_buffer)[ 2*i + 1] = 0.0;
          }//endfor
          break;
        }// endswitch
      break;
      //---------------------------------------------------------------------------------------
    default:
      // unknown in-type
        switch(out_type){
        case GS_DATATYPE_UI1:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endif
          break;
        case GS_DATATYPE_UI8:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }//endif
          break;
        case GS_DATATYPE_SI8:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ i ] = (int) realpart;
          }//endif
          break;
        case GS_DATATYPE_UI16:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int16_t *)out_buffer)[ i ] = (int) realpart;
          }//endif
          break;
        case GS_DATATYPE_SI16:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ i ] = (int) realpart;
          }//endif
          break;
        case GS_DATATYPE_UI32:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int32_t *)out_buffer)[ i ] = (int) realpart;
          }//endif
          break;
        case GS_DATATYPE_SI32:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ i ] = (int) realpart;
          }//endif
          break;
        case GS_DATATYPE_UI64:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int64_t *)out_buffer)[ i ] = (int) realpart;
          }//endif
          break;
        case GS_DATATYPE_SI64:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int64_t *)out_buffer)[ i ] = (int) realpart;
          }//endif
          break;
        case GS_DATATYPE_R32:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ i ] = (float) realpart;
          }//endif
          break;
        case GS_DATATYPE_R64:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ i ] =  realpart;
          }//endif
          break;

        case GS_DATATYPE_CI16:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int8_t *)out_buffer)[ 2*i + 1] = 0;
          }//endif
          break;
        case GS_DATATYPE_CI32:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int16_t *)out_buffer)[ 2*i + 1] = 0;
          }//endif
          break;
        case GS_DATATYPE_CI64:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int32_t *)out_buffer)[ 2*i + 1] = 0;
          }//endif
          break;
        case GS_DATATYPE_C64:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ 2*i + 0] = (float) realpart;
            ((float *)out_buffer)[ 2*i + 1] = 0.0;
          }//endif
          break;
        case GS_DATATYPE_C128:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ 2*i + 0] =  realpart;
            ((double *)out_buffer)[ 2*i + 1] = 0.0;
          }//endif
          break;
        }// endswitch
    }//end-switch
    
    
    
    
  } else {
    // deal with complex in_type's
    // since complex there are 2X as many numbers as when it is a scalar type.
    // the index is for scalar types, and so we do "2*index+reim", with reim being 0 or 1.
    switch(in_type){
    case GS_DATATYPE_CI16:
        
        switch(out_type){
        case GS_DATATYPE_UI1:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int8_t *)in_buffer)[ 2*i + 1]; 
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI8:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int8_t *)in_buffer)[ 2*i + 1]; 
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }// endfor
          break;
        case GS_DATATYPE_SI8:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int8_t *)in_buffer)[ 2*i + 1]; 
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI16:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int8_t *)in_buffer)[ 2*i + 1]; 
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int16_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI16:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int8_t *)in_buffer)[ 2*i + 1]; 
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI32:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int8_t *)in_buffer)[ 2*i + 1]; 
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int32_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI32:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int8_t *)in_buffer)[ 2*i + 1]; 
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI64:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int8_t *)in_buffer)[ 2*i + 1]; 
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int64_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI64:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int8_t *)in_buffer)[ 2*i + 1]; 
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int64_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_R32:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int8_t *)in_buffer)[ 2*i + 1]; 
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ i ] = (float) realpart;
          }//endofr
          break;
        case GS_DATATYPE_R64:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int8_t *)in_buffer)[ 2*i + 1]; 
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ i ] =  realpart;
          }//endfor
          break;

        case GS_DATATYPE_CI16:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int8_t *)in_buffer)[ 2*i + 1]; 
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int8_t *)out_buffer)[ 2*i + 1] = (int) imagpart;
          }//endfor
          break;
        case GS_DATATYPE_CI32:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int8_t *)in_buffer)[ 2*i + 1]; 
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int16_t *)out_buffer)[ 2*i + 1] = (int) imagpart;
          }//endfor
          break;
        case GS_DATATYPE_CI64:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int8_t *)in_buffer)[ 2*i + 1]; 
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int32_t *)out_buffer)[ 2*i + 1] = (int) imagpart;
          }//endfor
          break;
        case GS_DATATYPE_C64:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int8_t *)in_buffer)[ 2*i + 1]; 
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ 2*i + 0] = (float) realpart;
            ((float *)out_buffer)[ 2*i + 1] = (float) imagpart;
          }//endif
          break;
        case GS_DATATYPE_C128:
          for(i=0;i<nelements;i++){
            realpart = ((int8_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int8_t *)in_buffer)[ 2*i + 1]; 
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ 2*i + 0] =  realpart;
            ((double *)out_buffer)[ 2*i + 1] =  imagpart;
          }//endfor
          break;
        }// endswitch
      break;
      //-----------------------------------------------------------------------------------
    case GS_DATATYPE_CI32:
        
        switch(out_type){
        case GS_DATATYPE_UI1:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int16_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI8:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int16_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI8:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int16_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI16:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int16_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int16_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI16:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int16_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI32:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int16_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int32_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI32:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int16_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI64:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int16_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int64_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI64:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int16_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int64_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_R32:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int16_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ i ] = (float) realpart;
          }//endfor
          break;
        case GS_DATATYPE_R64:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int16_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ i ] =  realpart;
          }//endfor
          break;

        case GS_DATATYPE_CI16:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int16_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int8_t *)out_buffer)[ 2*i + 1] = (int) imagpart;
          }//endfor
          break;
        case GS_DATATYPE_CI32:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int16_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int16_t *)out_buffer)[ 2*i + 1] = (int) imagpart;
          }//endfor
          break;
        case GS_DATATYPE_CI64:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int16_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int32_t *)out_buffer)[ 2*i + 1] = (int) imagpart;
          }//endfor
          break;
        case GS_DATATYPE_C64:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int16_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ 2*i + 0] = (float) realpart;
            ((float *)out_buffer)[ 2*i + 1] = (float) imagpart;
          }//endfor
          break;
        case GS_DATATYPE_C128:
          for(i=0;i<nelements;i++){
            realpart = ((int16_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int16_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ 2*i + 0] =  realpart;
            ((double *)out_buffer)[ 2*i + 1] =  imagpart;
          }//endfor
          break;
        }// endswitch

      break;
      //------------------------------------------------------------------------------------
    case GS_DATATYPE_CI64:
        
        switch(out_type){
        case GS_DATATYPE_UI1:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int32_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI8:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int32_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI8:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int32_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI16:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int32_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int16_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI16:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int32_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI32:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int32_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int32_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI32:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int32_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI64:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int32_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int64_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI64:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int32_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int64_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_R32:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int32_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ i ] = (float) realpart;
          }//endfor
          break;
        case GS_DATATYPE_R64:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int32_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ i ] =  realpart;
          }//endfor
          break;

        case GS_DATATYPE_CI16:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int32_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int8_t *)out_buffer)[ 2*i + 1] = (int) imagpart;
          }//endfor
          break;
        case GS_DATATYPE_CI32:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int32_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int16_t *)out_buffer)[ 2*i + 1] = (int) imagpart;
          }//endfor
          break;
        case GS_DATATYPE_CI64:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int32_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int32_t *)out_buffer)[ 2*i + 1] = (int) imagpart;
          }//endfor
          break;
        case GS_DATATYPE_C64:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int32_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ 2*i + 0] = (float) realpart;
            ((float *)out_buffer)[ 2*i + 1] = (float) imagpart;
          }//endfor
          break;
        case GS_DATATYPE_C128:
          for(i=0;i<nelements;i++){
            realpart = ((int32_t *)in_buffer)[ 2*i + 0];
            imagpart = ((int32_t *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ 2*i + 0] =  realpart;
            ((double *)out_buffer)[ 2*i + 1] =  imagpart;
          }//endfor
          break;
        }// endswitch
      
      break;
      //------------------------------------------------------------------------------------------------
    case GS_DATATYPE_C64:
      //printf("type_convert: fromtype is C64, i=%d, reim=%d, newi=%d\n",
      //       i, reim, 2*i+reim);
      //if(i<10)printf("type_convert: buf[0]=%f, buf[1]=%f, buf[2]=%f, buf[3]=%f\n",
      //                   ((float *)in_buffer)[0],((float *)in_buffer)[1],
      //                   ((float *)in_buffer)[2],((float *)in_buffer)[3]);
      
        
        switch(out_type){
        case GS_DATATYPE_UI1:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ 2*i + 0];
            imagpart = ((float *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI8:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ 2*i + 0];
            imagpart = ((float *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI8:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ 2*i + 0];
            imagpart = ((float *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI16:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ 2*i + 0];
            imagpart = ((float *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int16_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI16:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ 2*i + 0];
            imagpart = ((float *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI32:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ 2*i + 0];
            imagpart = ((float *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int32_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI32:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ 2*i + 0];
            imagpart = ((float *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI64:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ 2*i + 0];
            imagpart = ((float *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int64_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI64:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ 2*i + 0];
            imagpart = ((float *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int64_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_R32:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ 2*i + 0];
            imagpart = ((float *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ i ] = (float) realpart;
          }//endfor
          break;
        case GS_DATATYPE_R64:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ 2*i + 0];
            imagpart = ((float *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ i ] =  realpart;
          }//endfor
          break;

        case GS_DATATYPE_CI16:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ 2*i + 0];
            imagpart = ((float *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int8_t *)out_buffer)[ 2*i + 1] = (int) imagpart;
          }//endfor
          break;
        case GS_DATATYPE_CI32:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ 2*i + 0];
            imagpart = ((float *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int16_t *)out_buffer)[ 2*i + 1] = (int) imagpart;
          }//endfor
          break;
        case GS_DATATYPE_CI64:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ 2*i + 0];
            imagpart = ((float *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int32_t *)out_buffer)[ 2*i + 1] = (int) imagpart;
          }//endfor
          break;
        case GS_DATATYPE_C64:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ 2*i + 0];
            imagpart = ((float *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ 2*i + 0] = (float) realpart;
            ((float *)out_buffer)[ 2*i + 1] = (float) imagpart;
          }//endfor
          break;
        case GS_DATATYPE_C128:
          for(i=0;i<nelements;i++){
            realpart = ((float *)in_buffer)[ 2*i + 0];
            imagpart = ((float *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ 2*i + 0] =  realpart;
            ((double *)out_buffer)[ 2*i + 1] =  imagpart;
          }//endfor
          break;
        }// endswitch
      break;
      //------------------------------------------------------------------------------------------------
    case GS_DATATYPE_C128:
        
        switch(out_type){
        case GS_DATATYPE_UI1:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ 2*i + 0];
            imagpart = ((double *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI8:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ 2*i + 0];
            imagpart = ((double *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI8:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ 2*i + 0];
            imagpart = ((double *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI16:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ 2*i + 0];
            imagpart = ((double *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int16_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI16:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ 2*i + 0];
            imagpart = ((double *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI32:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ 2*i + 0];
            imagpart = ((double *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int32_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI32:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ 2*i + 0];
            imagpart = ((double *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI64:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ 2*i + 0];
            imagpart = ((double *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((u_int64_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI64:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ 2*i + 0];
            imagpart = ((double *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int64_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_R32:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ 2*i + 0];
            imagpart = ((double *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ i ] = (float) realpart;
          }//endfor
          break;
        case GS_DATATYPE_R64:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ 2*i + 0];
            imagpart = ((double *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ i ] =  realpart;
          }//endfor
          break;

        case GS_DATATYPE_CI16:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ 2*i + 0];
            imagpart = ((double *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int8_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int8_t *)out_buffer)[ 2*i + 1] = (int) imagpart;
          }//endfor
          break;
        case GS_DATATYPE_CI32:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ 2*i + 0];
            imagpart = ((double *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int16_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int16_t *)out_buffer)[ 2*i + 1] = (int) imagpart;
          }//endfor
          break;
        case GS_DATATYPE_CI64:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ 2*i + 0];
            imagpart = ((double *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((int32_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int32_t *)out_buffer)[ 2*i + 1] = (int) imagpart;
          }//endfor
          break;
        case GS_DATATYPE_C64:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ 2*i + 0];
            imagpart = ((double *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((float *)out_buffer)[ 2*i + 0] = (float) realpart;
            ((float *)out_buffer)[ 2*i + 1] = (float) imagpart;
          }//endfor
          break;
        case GS_DATATYPE_C128:
          for(i=0;i<nelements;i++){
            realpart = ((double *)in_buffer)[ 2*i + 0];
            imagpart = ((double *)in_buffer)[ 2*i + 1];
            
            if(totype_is_int) {
              if(realpart < intranges[out_type][RMIN]) realpart = intranges[out_type][RMIN];
              if(realpart > intranges[out_type][RMAX]) realpart = intranges[out_type][RMAX];
              if(imagpart < intranges[out_type][RMIN]) imagpart = intranges[out_type][RMIN];
              if(imagpart > intranges[out_type][RMAX]) imagpart = intranges[out_type][RMAX];
            } else {
              if(realpart < floatranges[out_type][RMIN]) realpart = floatranges[out_type][RMIN];
              if(realpart > floatranges[out_type][RMAX]) realpart = floatranges[out_type][RMAX];
              if(imagpart < floatranges[out_type][RMIN]) imagpart = floatranges[out_type][RMIN];
              if(imagpart > floatranges[out_type][RMAX]) imagpart = floatranges[out_type][RMAX];
            }// endif
            ((double *)out_buffer)[ 2*i + 0] =  realpart;
            ((double *)out_buffer)[ 2*i + 1] =  imagpart;
          }//endfor
          break;
        }// endswitch
      break;

    default:
       
        switch(out_type){
        case GS_DATATYPE_UI1:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            imagpart = 0.0;
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI8:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            imagpart = 0.0;
            ((u_int8_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI8:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            imagpart = 0.0;
            ((int8_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI16:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            imagpart = 0.0;
            ((u_int16_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI16:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            imagpart = 0.0;
            ((int16_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI32:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            imagpart = 0.0;
            ((u_int32_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI32:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            imagpart = 0.0;
            ((int32_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_UI64:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            imagpart = 0.0;
            ((u_int64_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_SI64:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            imagpart = 0.0;
            ((int64_t *)out_buffer)[ i ] = (int) realpart;
          }//endfor
          break;
        case GS_DATATYPE_R32:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            imagpart = 0.0;
            ((float *)out_buffer)[ i ] = (float) realpart;
          }//endfor
          break;
        case GS_DATATYPE_R64:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            imagpart = 0.0;
            ((double *)out_buffer)[ i ] =  realpart;
          }//endfor
          break;

        case GS_DATATYPE_CI16:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            imagpart = 0.0;
            ((int8_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int8_t *)out_buffer)[ 2*i + 1] = (int) imagpart;
          }//endfor
          break;
        case GS_DATATYPE_CI32:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            imagpart = 0.0;
            ((int16_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int16_t *)out_buffer)[ 2*i + 1] = (int) imagpart;
          }//endfor
          break;
        case GS_DATATYPE_CI64:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            imagpart = 0.0;
            ((int32_t *)out_buffer)[ 2*i + 0] = (int) realpart;
            ((int32_t *)out_buffer)[ 2*i + 1] = (int) imagpart;
          }//endfor
          break;
        case GS_DATATYPE_C64:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            imagpart = 0.0;
            ((float *)out_buffer)[ 2*i + 0] = (float) realpart;
            ((float *)out_buffer)[ 2*i + 1] = (float) imagpart;
          }//endfor
          break;
        case GS_DATATYPE_C128:
          for(i=0;i<nelements;i++){
            realpart = 0.0;
            imagpart = 0.0;
            ((double *)out_buffer)[ 2*i + 0] =  realpart;
            ((double *)out_buffer)[ 2*i + 1] =  imagpart;
          }//endfor
          break;
        }// endswitch
      
      break;
      
    }//end-switch
    
    
  }// endif
  
  

}

