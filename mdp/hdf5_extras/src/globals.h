#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <sys/types.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <complex.h>


#include "gmalloc.h"
#include "bstrlib.h"

#include "hdf5mine.h"

/*! \file */


#define TRUE 1
#define FALSE 0
#define ERROR -1






  //globals for IFiles:
extern bstring status_string;
extern bstring error_string;



extern bstring file_metadata_name;



/** \brief DATA_TYPE

\anchor DATA_TYPE

\addindex DATA_TYPE

The Data_Type* #defines are used for the 17 possible data types that
are supported for images in a GeoSCi file. They are:\n

\c GS_DATATYPE_UI1 1\n
\c GS_DATATYPE_UI8 2\n
\c GS_DATATYPE_SI8 3\n
\c GS_DATATYPE_CI8 4\n
\c GS_DATATYPE_UI16 5\n
\c GS_DATATYPE_SI16 6\n
\c GS_DATATYPE_CI16 7\n
\c GS_DATATYPE_UI32 8\n
\c GS_DATATYPE_SI32 9\n
\c GS_DATATYPE_CI32 10\n
\c GS_DATATYPE_CI64 11\n
\c GS_DATATYPE_R32 12\n
\c GS_DATATYPE_R64 13\n
\c GS_DATATYPE_C64 14\n
\c GS_DATATYPE_C128 15\n
\c GS_DATATYPE_UI64 16\n
\c GS_DATATYPE_SI64 17\n

\par Details:
\parblock
Datatypes in GeoSciPy consist of a set of pre-defined symbols
that can be used for creating new data in files.

The following lists the datatype symbols and their definitions:

\li GS_DATATYPE_UI1
         Unsigned Integer, 1-bit.
         This is used for creating bitmaps, or other logical-type
         variables that can only take on 2 values: 0 and 1.\n
         Declaration in C: u_int8_t
\li GS_DATATYPE_UI8
         Unsigned Integer, 8-bit.
         This can be used for creating images that can take on
         the values 0 through 255.
         Many different kinds of data use this type.\n
         Declaration in C: u_int8_t
\li GS_DATATYPE_SI8
         Signed Integer, 8-bit.
         This is for data where the value of the data goes from -128
         to +127.\n
         Declaration in C: int8_t
\li GS_DATATYPE_CI8
         Complex Integer, 8-bit.
         This is used for data where the real part of a complex
         number is represented by a signed-4-bit integer, and so is
         the imaginary part, making a total of 8 bits for the complex
         number. This is relatively rare.\n
         Declaration in C: ..currently not supported..
\li GS_DATATYPE_UI16
         Unsigned Integer, 16-bit.
         This can be used for creating images that can take on
         the values 0 through 65535.\n
         Declaration in C: u_int16_t
\li GS_DATATYPE_SI16
         Signed Integer, 16-bit.
         This is for data where the value of the data goes from
         -32768 to +32767.\n
         Declaration in C: int16_t
\li GS_DATATYPE_CI16
         Complex Integer, 16-bit.
         This is used for data where the real part of a complex
         number is represented by a signed-8-bit integer, and so is
         the imaginary part, making a total of 16 bits for the complex
         number.\n
         Declaration in C: ci16_t
\li GS_DATATYPE_UI32
         Unsigned Integer, 32-bit.
         This can be used for creating images that can take on
         the values 0 through (2^32) - 1.\n
         Declaration in C: u_int32_t
\li GS_DATATYPE_SI32
         Signed Integer, 32-bit.
         This is for data where the value of the data goes from
         -(2^31) to (2^31)-1.\n
         Declaration in C: int32_t
\li GS_DATATYPE_CI32
         Complex Integer, 32-bit.
         This is used for data where the real part of a complex
         number is represented by a signed-16-bit integer, and so is
         the imaginary part, making a total of 32 bits for the complex
         number.\n
         Declaration in C: ci32_t
\li GS_DATATYPE_CI64
         Complex Integer, 64-bit.
         This is used for data where the real part of a complex
         number is represented by a signed-32-bit integer, and so is
         the imaginary part, making a total of 64 bits for the complex
         number.\n
         Declaration in C: ci64_t
\li GS_DATATYPE_R32
         Real, 32-bit.
         This is for real data where the value of the data goes from
         approximately -3.402823466e+38 to 3.402823466e+38, with
         about 7 digits of precision.\n
         Declaration in C: float
\li GS_DATATYPE_R64
         Real, 64-bit.
         This is for real data where the value of the data goes from
         approximately -1.797693135e+308 to 1.797693135e+308, with
         about 16 digits of precision.\n
         Declaration in C: double
\li GS_DATATYPE_C64
         Complex, 64-bit.
         This is used for data where the real part of a complex
         number is represented by a 32-bit real number, and so is
         the imaginary part, making a total of 64 bits for the complex
         number.\n
         Declaration in C: c64_t
\li GS_DATATYPE_C128
         Complex, 128-bit.
         This is used for data where the real part of a complex
         number is represented by a 64-bit real number, and so is
         the imaginary part, making a total of 128 bits for the complex
         number.\n
         Declaration in C: c128_t
\li GS_DATATYPE_UI64
         Unsigned Integer, 64-bit.
         This can be used for creating images that can take on
         the values 0 through (2^64) - 1.\n
         Declaration in C: u_int64_t
\li GS_DATATYPE_SI64
         Signed Integer, 64-bit.
         This is for data where the value of the data goes from
         -(2^63) to (2^63)-1.\n
         Declaration in C: int64_t
\endparblock

*/

// geoscipy data types:
#define GS_DATATYPE_UNK 0
#define GS_DATATYPE_UI1 1
#define GS_DATATYPE_UI8 2
#define GS_DATATYPE_SI8 3
#define GS_DATATYPE_CI8 4
#define GS_DATATYPE_UI16 5
#define GS_DATATYPE_SI16 6
#define GS_DATATYPE_CI16 7
#define GS_DATATYPE_UI32 8
#define GS_DATATYPE_SI32 9
#define GS_DATATYPE_CI32 10
#define GS_DATATYPE_CI64 11
#define GS_DATATYPE_R32 12
#define GS_DATATYPE_R64 13
#define GS_DATATYPE_C64 14
#define GS_DATATYPE_C128 15
#define GS_DATATYPE_UI64 16
#define GS_DATATYPE_SI64 17
#define GS_DATATYPE_MAX 17



/* -------------------------------------------------------------------- */
/*      Useful macros.  Note that MAX and MIN may evaluate              */
/*      their arguments more than once!                                 */
/* -------------------------------------------------------------------- */
#ifndef MAX
#  define MIN(a,b)      ((a<b) ? a : b)
#  define MAX(a,b)      ((a>b) ? a : b)
#endif

#ifndef ABS
#  define ABS(x)        ((x<0) ? (-1*(x)) : x)
#endif

// case-insensitive comparison funcs for bstrings:
#ifndef EQUALN
#define EQUALN(a,b,n)           (bstrnicmp(a,b,n)==0)
#endif

#ifndef EQUAL
#define EQUAL(a,b)              (bstricmp(a,b)==0)
#endif

#ifndef EQUALC
#define EQUALC(a,b)              (strcasecmp(a,b)==0)
#endif

#define ByteCopy(s,d,n)       bcopy((char *) (s),(char *) (d), n)
#define BstringCopy(s,d,n)       bassignblk((bstring)(d), (char *)(s), n)




#endif

