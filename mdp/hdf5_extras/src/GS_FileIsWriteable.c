/* GS_FileIsWriteable.c

   by Leland Pierce, Nov 16, 2013

  
   Aug 6, 2014
   Modified for bstrings, doxygen and GS_

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


#define ERROR -1


/** \brief GS_FileIsWriteable returns if file is writeable or not

\see GS_ObjectLock(), GS_ObjectUnlock()

\param[in] id
    The handle of the file, or any object in the file.

\returns
Returns \c TRUE if the file associated with the given
handle is writeable, or \c FALSE if not. Returns \c ERROR (-1) if
cannot determine the status.

\par Example
\code
int status;
         ...
status = GS_FileIsWriteable(file_id);
if(status < 0){
  printf("Could not determine read/write status of file.\n");
  return;
}
if(status==FALSE) return;
...here we can write to the file....
\endcode

\par Details
A file is writeable if it was opened in read/write mode using GS_FileOpen, and
if the \c writeable metadata is \c TRUE, which can be changed via a call to 
GS_ObjectLock, and GS_objectUnlock.
*/


int GS_FileIsWriteable(hid_t id)
{
  unsigned int intent;
  hid_t file_id;
  
  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/*       Get a valid file                                               */
/* -------------------------------------------------------------------- */
  file_id = GS_GetValidFileID(id);
  if(!file_id){
    bassigncstr(error_string,"GS_FileIsWriteable: Can't get file_id of the given handle\n");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/*       Get read/write status of file                                  */
/* -------------------------------------------------------------------- */
  if(H5Fget_intent(file_id, &intent) < 0 ){
    bassigncstr(error_string,"GS_FileIsWriteable: Can't get file read/write status.\n");
    GS_FileClose(file_id);
    return ERROR;
  }// endif


/* -------------------------------------------------------------------- */
/*       Return TRUE if write-able                                      */
/* -------------------------------------------------------------------- */
  if(intent == H5F_ACC_RDONLY) {
    GS_FileClose(file_id); 
    return FALSE;
  } else if (intent == H5F_ACC_RDWR) {
    // we also require the writeable metadata to be TRUE (or non-existent)
    // in order for the file to be writeable:
      bstring writeable = bfromcstr("writeable");
      bstring writestring=bfromcstr("");
      bstring bfalse = bfromcstr("FALSE");
      if(!GS_GetStringAttribute(file_id,writeable,writestring)){
        //bassigncstr(error_string,"GS_FileIsWriteable: Can't get the writeability metadata.");
        GS_FileClose(file_id); 
#ifdef DEBUG
        printf("GS_FileIsWriteable: 7.1\n");
#endif
        bdestroy(writestring);
        bdestroy(writeable);
        bdestroy(bfalse);
        return TRUE;
      }// endif
      if(EQUAL(writestring,bfalse)){
        bdestroy(writestring);
        bdestroy(writeable);
        bdestroy(bfalse);
        GS_FileClose(file_id); 
#ifdef DEBUG
        printf("GS_FileIsWriteable: 7.2\n");
#endif
        return FALSE;
      } else {
        bdestroy(writeable);
        bdestroy(writestring);
        bdestroy(bfalse);
        GS_FileClose(file_id); 
#ifdef DEBUG
        printf("GS_FileIsWriteable: 7.3\n");
#endif
        return TRUE;
      }// endif






  } else {
    bassignformat(error_string,"GS_FileIsWriteable: Unknown intent code: %d.\n",intent);
    return ERROR;
  }//endif


}
