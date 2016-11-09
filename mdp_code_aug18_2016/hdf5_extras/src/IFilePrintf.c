/* IFilePrintf.c

   by Leland Pierce, Jan 24, 2014.
  
*/

/** \brief IFilePrintf writes formatted data to an internal file
\sa IFileOpen(), IFileClose()
\param[in] ifilep
The struct of the open internal file.
\param[in] format
String that contains the text to be written to the 
internal file. It can optionally contain embedded 
format specifiers that are replaced by the values 
specified in subsequent additional arguments and 
formatted as requested.
See docs for \c fscanf() in any C book for the complete
details.
\param[in] ... (additional arguments)
Depending on the format string, the function may 
expect a sequence of additional arguments, each 
containing a pointer to allocated storage where 
the interpretation of the extracted characters is 
stored with the appropriate type.
There should be at least as many of these arguments 
as the number of values stored by the format specifiers. 
Additional arguments are ignored by the function.

\returns
\parblock
On success, the function returns the number of characters
successfully written. 

If a reading error happens or the end-of-file is reached while reading, 
the proper indicator is set (IFileEOF() or IFileError()). And, if 
either happens before any data could be successfully written, ERROR is 
returned.

The error_string variable is also set to a system-specific 
error code on failure.
\endparblock

\par Example
Let's assume that one already has an GeoSci file, and one wants
to write a string to the next line of the internal file 
named "/somename".
\code
hid_t file_id;
IFILE *ifilep;
int an_int;
int n;

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifilep) {
   printf("IFileOpen failed on /somename\n");
}
an_int = 5;
n =IFilePrintf(ifilep, "The value is: %d", an_int);
if(n==1){
   printf("successfully wrote the integer: %d\n",an_int);
} else {
   printf("problem writing the integer\n");
}
\endcode

\par Details
\parblock
This function is meant to emulate as close as possible the standard
C \c fprintf() function.

Writes the string pointed by format to the internal file. If format 
includes format specifiers (subsequences beginning with %), the 
additional arguments following format are formatted and inserted in 
the resulting string replacing their respective specifiers.

After the format parameter, the function expects at least as many 
additional arguments as specified by format.
\endparblock

*/

#include "ifile.h"

//#define DEBUG

/*******************************************************************************/
int IFilePrintf( IFILE *ifilep, const char *format, ... )
{

  char *theline;
  long int theline_length;
  int not_done;
  int count;
  long int L;
  int ifile_access_mode;
  
  va_list ap;

#ifdef DEBUG
  printf("IFIlePrintf: 0\n");
#endif

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("IFIlePrintf: 0.5\n");
#endif



/* -------------------------------------------------------------------- */
/* Get file access mode                                                 */
/* -------------------------------------------------------------------- */
  switch(ifilep->access){
  case IFILE_CLOSED:
    bassignformat(error_string,"IFilePrintf: file is not open.");
    return ERROR;
  case IFILE_R:
    bassignformat(error_string,"IFilePrintf: file is not in write mode.");
    return ERROR;
  case IFILE_W:
  case IFILE_RP:
  case IFILE_WP:
    // for all of these modes, write at current location
    break;
  case IFILE_A:
  case IFILE_AP:
    // for append modes, writes must be at the end of the file:
    ifilep->file_position = ifilep->size;
    break;
  default:
    bassignformat(error_string,"IFilePrintf: unknown file access code.");
    return ERROR;    
  }// end-switch

#ifdef DEBUG
  printf("IFIlePrintf: 2\n");
#endif

/* -------------------------------------------------------------------- */
/* Malloc a string that is big enough                                   */
/* -------------------------------------------------------------------- */
  // not particularly important, just make a bit bigger than the format string
  L = strlen(format)+1000;
  theline = (char *)GMalloc(L*sizeof(char));
  if(!theline){
    bassignformat(error_string,"IFilePrintf: Malloc error (theline).");
    return ERROR;    
  }// endif

#ifdef DEBUG
  printf("IFIlePrintf: 3\n");
#endif

/* -------------------------------------------------------------------- */
/* Let vsscanf() create the string                                      */
/* -------------------------------------------------------------------- */
  va_start (ap, format);

  // create the string:
  count = vsnprintf (theline, L, format, ap);

#ifdef DEBUG
  printf("IFIlePrintf: 4, count=%d\n",count);
#endif


  // see if it's complete:
  if(abs(count) >= L) {
    // need a longer string
    GFree(theline);
    L = abs(count) +100;
    theline = (char *)GMalloc(L*sizeof(char));
    if(!theline){
      bassignformat(error_string,"IFilePrintf: Malloc error (theline).");
      return ERROR;    
    }// endif

    // try again (should work):
    count = vsnprintf (theline, L, format, ap);

#ifdef DEBUG
  printf("IFIlePrintf: 5, count=%d\n",count);
#endif


    // if it doesn't work:
    if(count < 0 || abs(count) >= L){
      bassignformat(error_string,"IFilePrintf: vsnprintf failure.");
      GFree(theline);
      return ERROR;    
    }// endif

  }// endif
  
  va_end (ap);


#ifdef DEBUG
  printf("IFIlePrintf: about to call IFilePuts with string=|%s|\n",theline);
  printf("             ifilep=%p\n",ifilep);
  printf("             ifilep->access=%d\n",ifilep->access);
#endif

/* -------------------------------------------------------------------- */
/* Write to the internal file                                           */
/* -------------------------------------------------------------------- */
  if(!IFilePuts(ifilep,theline)){
    bassignformat(error_string,"IFilePrintf: IFilePuts failure.");
    GFree(theline);
    return ERROR;    
  }// endif


#ifdef DEBUG
  printf("IFIlePrintf: done\n");
#endif

/* -------------------------------------------------------------------- */
/* Clean up                                                             */
/* -------------------------------------------------------------------- */
  GFree(theline);

  return count;
 
}
