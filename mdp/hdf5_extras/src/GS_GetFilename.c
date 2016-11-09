/* GS_GetFilename.c

   by Leland Pierce, Nov 17, 2013

*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"

/** \brief  GetFilename returns the filename associated with an ID

\param[in] id The handle of the file, or any object in the file.

\returns
Returns a character string of the filename
within which the given handle is defined, or a zero-length bstring
if there is any kind of error.
bdestroy() it when done.

\par Example
\code
bstring the_filename= bfromcstr("");
         ...
the_filename = GS_GetFilename(file_id);
if(bstrlen(the_filename)==0){
  printf("Could not determine the filename\n");
  bdestroy(the_filename);
  return;
}
printf("The filename is: %s\n",bdata(the_filename));
bdestroy(the_filename);
\endcode
*/

//#define DEBUG

/************************************************************************/
bstring GS_GetFilename(hid_t id)
{
  ssize_t name_length;
  char *str;
  bstring ret=bfromcstr("");

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("GS_GetFilename: start\n");
#endif

/* -------------------------------------------------------------------- */
/* See if the Id is valid                                               */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID(id)){
    bassigncstr(error_string,"GetFilename: Invalid ID.");
    return NULL;    
  }// endif
#ifdef DEBUG
  printf("GS_GetFilename: 1\n");
#endif

/* -------------------------------------------------------------------- */
/* get length of the filename                                           */
/* -------------------------------------------------------------------- */
  name_length = H5Fget_name(id, NULL, 0);
  if(name_length <0){
    bassigncstr(error_string,"GS_GetFilename: Can't get filename.\n");
    return NULL;
  }// endif
#ifdef DEBUG
  printf("GS_GetFilename: 2\n");
#endif

/* -------------------------------------------------------------------- */
/* GMalloc a string that's big enough                                   */
/* -------------------------------------------------------------------- */
  str = (char *)GMalloc( (name_length+10)*sizeof(char) );
  if(!str){
    bassigncstr(error_string,"GS_GetFilename: GMalloc error.\n");
    return NULL;
  }// endif
#ifdef DEBUG
  printf("GS_GetFilename: 3\n");
#endif

/* -------------------------------------------------------------------- */
/* Get the filename                                                     */
/* -------------------------------------------------------------------- */
  name_length = H5Fget_name(id, str, name_length+10);
  if(name_length <0){
    bassigncstr(error_string,"GS_GetFilename: Can't get filename again.\n");
    return NULL;
  }// endif
#ifdef DEBUG
  printf("GS_GetFilename: 4: str=|%s|\n",str);
#endif

/* -------------------------------------------------------------------- */
/* Return the filename                                                  */
/* -------------------------------------------------------------------- */
  bassigncstr(ret,str);
#ifdef DEBUG
  printf("GS_GetFilename: 5\n");
#endif
  GFree(str);
#ifdef DEBUG
  printf("GS_GetFilename: 6\n");
#endif
  return ret;

}
