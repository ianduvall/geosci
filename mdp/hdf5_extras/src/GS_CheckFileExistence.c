/* GS_CheckFileExistence.c

   by Leland Pierce, Jan 3, 2014

   updated June 9, 2016, LEP

*/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


/** \brief GS_CheckFileExistence checks for the existence of a  specified file. 

\param[in] filename                             
                Name of file to check for.

\returns
Returns \c TRUE if the file exists, or \c FALSE if it
does not exist (or is not accessable).

\par Example
\code
bstring name = bfromcstr("a.dat");
if(GS_CheckFileExistence(name)){
   printf("a.data exists\n");
} else {
   printf("a.data does not exist\n");
}
bdestroy(name);
\endcode

*/


int GS_CheckFileExistence(const_bstring filename)
{
    struct stat stat_buf;

    bassigncstr(status_string,"");
    bassigncstr(error_string,"");

    if( stat( bdata(filename), &stat_buf ) != 0 ) {
      bassignformat(error_string,"GS_CheckFileExistence: File does not exist: '%s'.",bdata(filename));
      return FALSE;
    }

    switch (stat_buf.st_mode & S_IFMT) {
    case S_IFBLK:  //block device
    case S_IFCHR:  //character device
    case S_IFDIR:  //directory
    case S_IFIFO:  //FIFO/pipe
    case S_IFLNK:  //symlink
    case S_IFSOCK: //socket
    case S_IFREG:  //regular file
      bassignformat(error_string,"GS_CheckFileExistence: Filename exists: '%s'.",bdata(filename));
      return TRUE;
    default: // unknown
      return FALSE;
    }// end-switch

}
