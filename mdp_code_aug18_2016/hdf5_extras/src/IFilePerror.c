/* IFilePerror.c

   by Leland Pierce, Jan 22, 2014.
  
*/

/** \brief IFilePerror prints latest error for an internal file
\sa IFileOpen, IFileClose

\param[in] ifilep
The struct of the open internal file.
\param[in] str
String containing a custom message to be printed
before the error message itself.
If it is a null pointer, no preceding custom message
is printed, but the error message is still printed.
By convention, the name of the application itself is 
generally used as parameter.

\returns
\parblock
TRUE is returned on success, FALSE otherwise.

This function should be called right after the error was produced,
otherwise the error of interest can be overwritten by calls to other functions.
\endparblock

\par Example
Let's assume that one already has an GeoSci file, and one wants
to print the latest error related to an internal file named "/somename".
\code
hid_t file_id;
IFILE *ifilep;

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifilep) {
   printf("IFileOpen failed on /somename\n");
}

if(!IFilePerror(ifilep,NULL)){
   printf("perror failed\n");
}
\endcode

 
\par Details
Prints the latest error message related to the internal file to 
\c stdout, optionally preceded by the custom message provided in str.
This function is meant to emulate as close as possible the standard
C perror() function.
*/

#include "ifile.h"


/*******************************************************************************/
int IFilePerror( IFILE *ifilep, const char *str )
{

/* -------------------------------------------------------------------- */
/* Print messages                                                       */
/* -------------------------------------------------------------------- */
  if(str){
    printf("%s: ",str);
  }// endif
  printf("%s\n",bdata(ifilep->last_operation_status_message) );

  return TRUE;
 
}
