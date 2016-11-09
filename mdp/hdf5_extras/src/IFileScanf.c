/* IFileScanf.c

   by Leland Pierce, Jan 20, 2014.
  
*/

/** \brief  IFileScanf reads formatted data from an internal file
\sa IFileOpen, IFileClose
\param[in] ifilep  The struct of the open internal file.
\param[in] format String that contains a sequence of characters 
that control how characters extracted from the 
stream are treated.
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
These arguments are expected to be pointers: to store
the result of a IFileScanf operation on a regular 
variable, its name should be preceded by the reference
operator (&) (see example).

\returns
\parblock
On success, the function returns the number of items of the argument 
list successfully filled. This count can match the expected number 
of items or be less (even zero) due to a matching failure, a reading 
error, or the reaching of the end-of-file.

If a reading error happens or the end-of-file is reached while reading, 
the proper indicator is set (IFileEOF() or IFileError()). And, if 
either happens before any data could be successfully read, \c ERROR is 
returned.

The \c error_string variable is also set to a system-specific 
error code on failure.
\endparblock

\par Example
Let's assume that one already has an GeoSci file, and one wants
to read an integer from the next line of the internal file 
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
n =IFileScanf(ifilep, "%d", &an_int);
if(n==1){
   printf("successfully read the integer: %d\n",an_int);
} else {
   printf("problem reading the integer\n");
}
\endcode

\par Details
This function is meant to emulate as close as possible the standard
C \c fscanf() function.
*/

#include "ifile.h"


/*******************************************************************************/
int IFileScanf( IFILE *ifilep, const char *format, ... )
{

  char *theline;
  long int theline_length;
  int not_done;
  long int starting_position;
  int count;

  va_list ap;


  bassigncstr(status_string,"");
  bassigncstr(error_string,"");


/* -------------------------------------------------------------------- */
/* Read the next line (up to \n or EOF)                                 */
/* -------------------------------------------------------------------- */
  theline_length=10000L;
  theline = (char *)GMalloc(theline_length*sizeof(char));
  if(!theline){
    bassignformat(error_string,"IFileScanf: HMalloc error, size=%ld",theline_length);
    return ERROR;    
  }// endif

  starting_position = ifilep->file_position;

  not_done = TRUE;
  while(not_done){
    // 1. read the next line
    if(!IFileGets( ifilep, theline, theline_length)){
      GFree(theline);
      bassignformat(error_string,"IFileScanf: read error.");
      return ERROR;    
    }// endif

    // 2. did we get the whole line?
    if(theline[strlen(theline)-1]=='\n'){
      not_done=FALSE;
    }//endif
    if(not_done && IFileEOF(ifilep)){
      // didn't read a \n, but hit EOF, so we're done
      not_done=FALSE;
    }// endif

    // 3. malloc more space and try again
    if(not_done){
      GFree(theline);
      theline=NULL;
      theline_length *= 2;
      theline = (char *)GMalloc(theline_length*sizeof(char));
      if(!theline){
        bassignformat(error_string,"IFileScanf: HMalloc error, size=%ld",theline_length);
        return ERROR;    
      }// endif
      ifilep->file_position =  starting_position;
    }// endif

  }// endwhile

/* -------------------------------------------------------------------- */
/* Now let vsscanf() handle it                                          */
/* -------------------------------------------------------------------- */
  va_start (ap, format);
  count = vsscanf (theline, format, ap);
  va_end (ap);

/* -------------------------------------------------------------------- */
/* Clean up                                                             */
/* -------------------------------------------------------------------- */
  GFree(theline);

  return count;
 
}
