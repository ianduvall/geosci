/* GS_HDFDatatypeClose.c
   
   by Leland Pierce, Aug 10, 2014

*/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"



/** \brief GS_HDFDatatypeClose closes an HDF datatype

\see GS_ConvertToHDFDatatype()

\param[in] id
          The id of the datatype.

\returns
\c TRUE on success, ERROR (-1) on failure.

\par Example
After opening a datatype using GS_ConvertToHDFDatatype(), close it:
\code
hid_t hdf_type;
hdf_type = GS_ConvertToHDFDatatype(GS_DATATYPE_R64);
if(hdf_type<0){
   printf("GS_ConvertToHDFDatatype failed\n");
   return;
}
if(!GS_HDFDatatypeClose(hdf_type)){
  printf("failed to close the datatype\n");
}
\endcode

\par Details
This function is not needed for most types, as they are not really opened to begin with.
However, the complex datatypes are opened, and so need to be closed.
*/

//#define DEBUG

int GS_HDFDatatypeClose(hid_t id)
{
  int i;
  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Check if its a complex datatype                                      */
/* -------------------------------------------------------------------- */
  if(GS_DatatypeIsComplex(id)){
    if( H5Tclose(id) <= 0) {
#ifdef DEBUG
      printf("GS_HDFDatatypeClose: returning TRUE\n");
#endif
      return TRUE;
    } else { 
#ifdef DEBUG
      printf("GS_HDFDatatypeClose: returning FALSE\n");
#endif
      return FALSE;
    }
  } else {
#ifdef DEBUG
      printf("GS_HDFDatatypeClose: returning TRUE, 2\n");
#endif
    return TRUE;
  }//endif

}

