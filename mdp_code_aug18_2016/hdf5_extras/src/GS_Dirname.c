/* GS_Dirname.c

   by Leland Pierce, Jan 20, 2014.
  
*/
#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"
#include "string_funcs.h"

/** \brief Dirname returns the directory name for a UNIX pathname
\sa GS_FileOpen(), GS_GroupCreate()
\param[in] name The name of an object in an existing GeoSci file.
Use Unix filenaming conventions, giving the full 
pathname, starting with "/".

\returns
A character string is returned with the directory part of the name
which does not end in a '/'. GFree() it when done.
If there are errors, a NULL pointer is returned.
\par Example
\code
char *dir;

dir = Dirname("/groupa/group1/image4");
if(!dir){
  printf("Dirname failed\n");
  exit(-1);
}

...use dir here....

GFree(dir);
\endcode
*/

/***********************************************************************/
char *GS_Dirname(const char *name)
{
  // return string from 1st '/' through the character before the last '/'
  // if no '/'s return NULL

  char  *first, *last;
  int L;
  char *dir;

  char *local_name=0;

  local_name = (char *)GMalloc(sizeof(char)*(strlen(name)+10));
  if(!local_name) {
    return NULL;
  }
  strcpy(local_name,name);

  first = find_first_char(local_name, '/');
  if(!first) return NULL;

  // must start with a '/'
  if(first != local_name) return NULL;

  last = find_last_char(local_name, '/');
  if(!last) return NULL;

  if(last==first) {
    dir = (char *)GMalloc(2*sizeof(char));
    if(!dir) return NULL;
    dir[0]='/';
    dir[1]='\0';
    GFree(local_name);
    return dir;
  }// endif

  L = last-first + 1;
  //printf("Dirname: L=%d\n",L);
  dir = (char *)GMalloc(L*sizeof(char));
  if(!dir) {
    GFree(local_name);
    return NULL;
  }

  strncpy(dir,first,L-1);
  dir[L-1]='\0';

  GFree(local_name);
  //printf("Dirname: dir=|%s|\n",dir);

  return dir;
}
