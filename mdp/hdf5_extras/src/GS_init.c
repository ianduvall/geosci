// GS_init.c 
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "bstrlib.h"
//#include "gmalloc.h"
//#include "H5ATTR.h"
//#include "globals.h"


// globals for error-reporting, etc
bstring error_string;
bstring status_string;

bstring file_metadata_name;


/** \brief GS_init initialize some global strings

\par Example
Before the first use of any GS_* function  call this first:
\code
...

GS_init();

... call GS_* functions here...
\endcode

\par Details
This function must be called once, before any other 
GeoSci functions in order to initialize some important things.

*/


void GS_init()
{
  error_string = bfromcstr("");
  status_string  = bfromcstr("");
  
  file_metadata_name = bfromcstr("/_Header");
}
