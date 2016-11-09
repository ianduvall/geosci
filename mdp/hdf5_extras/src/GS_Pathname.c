/* GS_Pathname.c

   by Leland Pierce, Nov 17, 2013
   added PathnameJoin, May 8, 2014

   So far, these funcs are only tested on UNIX, 
   but are written to work on mac and windows also.

   Aug 9, 2014
   added doxygen, bstrings, GS_
   
*/



#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


#define ERROR -1







/** \brief GS_PathnameNoDir strips the directory name from the pathname

\see GS_PathnameGetDir(), GS_PathnameJoin()

\param[in] name
		    The pathname to process.

\returns
Returns a bstring of the stripped name,
which has zero length if there is any kind of error.
bdestroy() it when done.

\par Example
This example starts with a path of "/some/path/name/here/blah"
and the returned value from GS_PathnameNoDir is: "blah".
\code
bstring longname=bfromcstr("/some/path/name/here/blah");
bstring the_name;
the_name = GS_PathnameNoDir(longname);
if(bstrlen(the_name)==0){
  printf("Could not determine the name\n");
}
printf("The stripped name is: %s\n",bdata(the_name));
bdestroy(the_name);
bdestroy(long_name);
\endcode

\par Details
For filenames that end with a "/" ('\' in windows), 
a zero-length string is returned. 
It's not null. It still needs to be bdestroy()'d when
you are done with it.

*/

bstring GS_PathnameNodir(const_bstring name)
{

  ssize_t name_length;
  bstring ret=bfromcstr("");
  int i;
  int done;
#ifdef __linux__
  bstring dirsep=bfromcstr("/");
#else
#ifdef __APPLE__ & __MACH__
  bstring dirsep=bfromcstr("/");
#else
#ifdef __MSDOS__
  bstring  dirsep=bfromcstr("\\");
#else
#ifdef _WIN32
  bstring dirsep=bfromcstr("\\");
#else
  // should not be here....
  bstring dirsep=bfromcstr("/");
#endif /* _WIN32 */
#endif /* __MSDOS__ */
#endif /*  __APPLE__ & __MACH__ */
#endif /* __linux__ */


  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Remove directory part of the pathname                                */
/* -------------------------------------------------------------------- */
  if(binstr(name,0,dirsep) < 0){
    // no directory separator characters:
    bassign(ret,name);
  } else {
    i=bstrlen(name);
    done = FALSE;
    while(!done){
      if(bchar(name,i)==bchar(dirsep,0)) {
        done = TRUE;
      } else {
        i--;
      }// endif
      if(i<0) {
        done=TRUE;
      }// endif
    }// endfor
    // i is where the last "/" is:
    if(i==bstrlen(name)){
      bassignformat(ret,""); // "/" is the last character. no filename.
    } else {
      bassignmidstr(ret,name,i+1,bstrlen(name)-(i+1));    
    }//endif
  }// endif

/* -------------------------------------------------------------------- */
/* Return the new string                                                */
/* -------------------------------------------------------------------- */
  bdestroy(dirsep);
  return ret;

}

//#define DEBUG
/** \brief GS_PathnameGetDir returns the directory name of the pathname


\see GS_PathnameJoin()

\param[in] name
		    The pathname to process.

\returns
Returns a bstring of the stripped name,
which has zero length if there is any kind of error.
bdestroy() it when done.

\par Example
This example starts with a path of "/some/path/name/here/blah"
and the returned value from GS_PathnameGetDir is: 
"/some/path/name/here".
\code
bstring longname=bfromcstr("/some/path/name/here/blah");
bstring the_name;
the_name = GS_PathnameGetDir(longname);
if(bstrlen(the_name)==0){
  printf("Could not determine the name\n");
}
printf("The directory name is: %s\n",bdata(the_name));
bdestroy(the_name);
bdestroy(long_name);
\endcode

\par Details
For filenames that have no "/" characters ('\' in windows), 
a zero-length string is returned. 
It's not null. It still needs to be bdestroy()'d when
you are done with it.

*/
/*************************************************************************/
bstring GS_PathnameGetDir(const_bstring name)
{
  ssize_t name_length;
  bstring ret=bfromcstr("");
  int i;
  int done;

#ifdef __linux__
  const_bstring dirsep=bfromcstr("/");
#else
#ifdef __APPLE__ & __MACH__
  const_bstring dirsep=bfromcstr("/");
#else
#ifdef __MSDOS__
  const_bstring  dirsep=bfromcstr("\\");
#else
#ifdef _WIN32
  const_bstring dirsep=bfromcstr("\\");
#else
  // should not be here....
  const_bstring dirsep=bfromcstr("/");
#endif /* _WIN32 */
#endif /* __MSDOS__ */
#endif /*  __APPLE__ & __MACH__ */
#endif /* __linux__ */

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");
#ifdef DEBUG
  printf("GS_PathnameGetDir: 1, dirsep=|%s|, name=|%s|\n",
         bdata(dirsep),bdata(name));
#endif

/* -------------------------------------------------------------------- */
/* Get directory part of the pathname                                   */
/* -------------------------------------------------------------------- */
  if(binstr(name,0,dirsep) < 0){
    // no directory-separator characters:
    // return empty string
#ifdef DEBUG
    printf("GS_PathnameGetDir: 1.1\n");
#endif
  } else {
    i=bstrlen(name);
    done = FALSE;
    while(!done){
      if(bchar(name,i)==bchar(dirsep,0)) {
        done = TRUE;
#ifdef DEBUG
    printf("GS_PathnameGetDir: 2\n");
#endif
      } else {
        i--;
      }// endif
      if(i<0) {
#ifdef DEBUG
    printf("GS_PathnameGetDir: 3\n");
#endif
        done=TRUE;
      }// endif
    }// endfor
    bassignmidstr(ret,name,0,i);
#ifdef DEBUG
    printf("GS_PathnameGetDir: 4\n");
#endif
  }// endif

/* -------------------------------------------------------------------- */
/* Return the new string                                                */
/* -------------------------------------------------------------------- */
#ifdef DEBUG
    printf("GS_PathnameGetDir: 5\n");
#endif
  bdestroy(dirsep);
  return ret;

}



/** \brief GS_PathnameGetHDFDir returns the directory name of the pathname

\see GS_PathnameNoDir(), GS_PathnameJoin()

\param[in] name
		    The pathname to process.

\returns
Returns a bstring of the processed name,
which has zero length if there is any kind of error.
bdestroy() it when done.

\par Example
This example starts with a path of "/some/path/name/here/blah"
and the returned value from GS_PathnameGetHDFDir is: 
"/some/path/name/here".
\code
bstring longname=bfromcstr("/some/path/name/here/blah");
bstring the_name;
the_name = GS_PathnameGetHDFDir(longname);
if(bstrlen(the_name)==0){
  printf("Could not determine the name\n");
}
printf("The directory name is: %s\n",bdata(the_name));
bdestroy(the_name);
bdestroy(long_name);
\endcode

\par Details
Assumes directories are separated with "/".
For filenames that have no "/" characters, 
a zero-length string is returned. 
It's not null. It still needs to be bdestroy()'d when
you are done with it.

*/
/*************************************************************************/
bstring GS_PathnameGetHDFDir(const_bstring name)
{
  ssize_t name_length;
  bstring ret=bfromcstr("");
  int i;
  int done;

  bstring dirsep=bfromcstr("/");

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Get directory part of the pathname                                   */
/* -------------------------------------------------------------------- */
  if(binstr(name,0,dirsep) < 0){
    // no directory-separator characters:
    // return empty string
  } else {
    i=bstrlen(name);
    done = FALSE;
    while(!done){
      if(bchar(name,i)==bchar(dirsep,0)) {
        done = TRUE;
      } else {
        i--;
      }// endif
      if(i<0) {
        done=TRUE;
      }// endif
    }// endfor
    bassignmidstr(ret,name,0,i);
  }// endif

/* -------------------------------------------------------------------- */
/* Return the new string                                                */
/* -------------------------------------------------------------------- */
  bdestroy(dirsep);
  return ret;

}


/** \brief GS_PathnameJoin joins 2 pieces of a pathname

\see GS_PathnameGetDir()

\param[in] front
		    The front part of the pathname.
                   Should not end with a '/' or a '\'.
\param[in] back
		    The back part of the pathname.
                   Should not start or end end with a '/' or a '\'.

\returns
Returns a bstring of the joined names,
which has zero length if there is any kind of error.
bdestroy() it when done.

\par Example
This example starts with a path of "/some/path/name/here/blah"
and a relative filename of "trash.dat".
The returned value from GS_PathnameJoin (on unix) is: 
"/some/path/name/here/blah/trash.dat".
\code
bstring longname=bfromcstr("/some/path/name/here/blah");
bstring ending=bfromcstr("trash.dat");
bstring the_name;
the_name = GS_PathnameJoin(longname,ending);
if(bstrlen(the_name)==0){
  printf("Could not join the names\n");
  return;
}
printf("The joined name is: %s\n",bdata(the_name));
bdestroy(the_name);
bdestroy(longname);
bdestroy(ending);
\endcode
\par Details
It uses the correct joining character for the operating system:
     for LINUX, MACOS: '/', for  Windows: '\'.

*/
/*************************************************************************/
bstring GS_PathnameJoin(const_bstring front, const_bstring back)
{
  ssize_t name_length;
  bstring ret=bfromcstr("");
  int i;
  int done;

#ifdef __linux__
  bstring dirsep=bfromcstr("/");
#else
#ifdef __APPLE__ & __MACH__
  bstring dirsep=bfromcstr("/");
#else
#ifdef __MSDOS__
  bstring  dirsep=bfromcstr("\\");
#else
#ifdef _WIN32
  bstring dirsep=bfromcstr("\\");
#else
  // should not be here....
  bstring dirsep=bfromcstr("/");
#endif /* _WIN32 */
#endif /* __MSDOS__ */
#endif /*  __APPLE__ & __MACH__ */
#endif /* __linux__ */

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Assemble the string                                                  */
/* -------------------------------------------------------------------- */
  bconcat(ret,front);
  bconcat(ret,dirsep);
  bconcat(ret,back);

/* -------------------------------------------------------------------- */
/* Return the new string                                                */
/* -------------------------------------------------------------------- */
  bdestroy(dirsep);
  return ret;

}

