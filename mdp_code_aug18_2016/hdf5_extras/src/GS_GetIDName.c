/* GS_GetIDName.c

   by Leland Pierce, Nov 17, 2013

*/

/** \brief GetIDName returns the name of the given handle

\param[in] id The handle of the file, or any object in the file.

\returns
Returns a character string (bstring) of the object name
of the given handle, or a zero-length bstring
if there is any kind of error.
bdestroy() it when done.

\par Example
\code
bstring the_name = bfromcstr("");
         ...
the_name = GS_GetIDName(id);
if(bstrlen(the_name)==0){
  printf("Could not determine the object name\n");
  bdestroy(the_name);
  return;
}
printf("The object name is: %s\n",bdata(the_name));
bdestroy(the_name);
\endcode

*/

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"

/********************************************************************/
bstring GS_GetIDName(hid_t id)
{
  ssize_t name_length;
  bstring ret = bfromcstr("");

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

  //----------------------------------------------------------------------
  // check that id is valid
  //----------------------------------------------------------------------
  if( !GS_ValidID( id ) ){
    bassignformat(error_string,"GetIDName: handle is not valid.\n");
    return NULL;
  }// endif 


  //----------------------------------------------------------------------
  // get length of the name
  //----------------------------------------------------------------------
  name_length = H5Iget_name(id, NULL, 0);
  if(name_length <0){
    bassignformat(error_string,"GetIDName: Can't get object name.\n");
    return NULL;
  }// endif

  //----------------------------------------------------------------------
  // Malloc space for the name
  //----------------------------------------------------------------------
  balloc(ret, name_length+10 );
  if(!ret){
    bassignformat(error_string,"GetIDName: Malloc error.\n");
    return NULL;
  }// endif

  //----------------------------------------------------------------------
  // get the name
  //----------------------------------------------------------------------
  name_length = H5Iget_name(id, bdata(ret), name_length+10);
  if(name_length <0){
    bassignformat(error_string,"GetIDName: Can't get object name again.\n");
    return NULL;
  }// endif

  //----------------------------------------------------------------------
  // return the name
  //----------------------------------------------------------------------
  return ret;

}
