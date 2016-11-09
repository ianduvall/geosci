/* IFileReadALine.c

   by Leland Pierce, Jan 22, 2014.
  
*/

/** \brief IFileReadALine reads a string from an internal file

IFileReadALine() reads characters from  an internal file
in a geosci file, and stores them as a string 
until the maximum number of characters have been read or 
either a newline or the end-of-file is reached, whichever 
happens first.

\sa IFileOpen, FileClose, IFileGets

\param[in] ifilep The struct of the open internal file.
\param[in,out] str  Pointer to an array of chars where the 
            string read is copied.
\param[in] num  Maximum number of characters to be copied into 
            \c str (including the terminating null-character).

\returns
\parblock
This function uses IFileGets() to read the data, any errors
encountered will cause this routine to return ERROR.

If the read succeeds, but the last character read is NOT 
a newline, then it returns FALSE.

If there are no read errors, and a complete line is read, then
TRUE is returned.
\endparblock

\par Example
Let's assume that one already has an GeoSci file, and one wants
to read the next line of characters (up to the next "\n") from an 
internal file named "/somename".
\code
hid_t file_id;
IFILE *ifilep;
char the_string[1000];

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifielp) {
   printf("IFileOpen failed on /somename\n");
}

status = IFileReadALine(ifilep, the_string, 1000);
if(status==ERROR){
   printf("read failed\n");
} else if (status=FALSE){
   printf("string is incomplete\n");
} else {
   printf('complete string read successfully.\n");

   ...use the_string here....
}
\endcode

\par Details
Similar to IFileGets(), but provides clearer return error codes.

*/

#include "ifile.h"


//#define DEBUG

/*******************************************************************************/
int IFileReadALine( IFILE *ifilep, char *str, int num )
{
  char *str2;
  int L;

  str2 = IFileGets(ifilep,str,num);
  L =strlen(str);

  if(str2==NULL){
    return ERROR;
  } else if(str[L-1] != '\n'){
    return FALSE;
  } else {
    return TRUE;
  } // endif

 
}// end-function: IFileReadALine()

