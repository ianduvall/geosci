/* GS_AbortedHistory.c

   by Leland Pierce, Jan 3, 2014

   updated June 9, 2016, LEP

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


/** \brief GS_AbortedHistory writes aborted message to history metadata

AbortedHistory() writes an "Aborted" message to the history metadata,
and updates the last_update_datetime metadata item.
This works for any object with a "history" metadata item, and
a "last_update_datetime" metadata item.

\see GS_AppendHistory(), GS_SplitHistory()
\param[in] object_id
            ID of a file, an image, or any other valid object
            in a GeoSci file. 

\param[in] history_line
            This string is appended to the history metadata item
            for the given object, along with " ***ABORTED*** "

\returns
\c TRUE on success, \c FALSE otherwise.
\par Example
Before writing to an image, write an aborted message in case 
of a run-time failure. After successfully modifying the image, 
write a new history, which over-writes the aborted message.
\code
hid_t image_id;
bstring name = bfromcstr("/Image3");
image_id=GS_ImageOpen(file_id,name);
bdestroy(name);

bstring tname = bfromcstr("Task_name");
GS_AbortedHistory(image_id,tname);
bdestroy(tname);

     ... modify the image ....

bstring tname2 = bfromcstr("Task_name(task_param1,task_param2,...)");
GS_AppendHistory(image_id,tname2);
bassigncstr(tname2,"Task_name: param1");
GS_AppendHistory(file_id,tname2);
bdestroy(tname2);
\endcode
*/


//#define DEBUG

/**********************************************************************/

int GS_AbortedHistory(hid_t object_id, const_bstring history_line)
{


  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("GS_AbortedHistory: 1\n");
#endif

/* -------------------------------------------------------------------- */
/* See if the object-Id is valid                                        */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID(object_id)){
    bassigncstr(error_string,"GS_AbortedHistory: invalid id.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_AbortedHistory: 2, object_id=%d\n",(int)object_id);
#endif



/* -------------------------------------------------------------------- */
/*   Error-check                                                        */
/* -------------------------------------------------------------------- */
  if(GS_ObjectIsFile(object_id)){
    bassigncstr(error_string,"GS_AbortedHistory: not valid for files.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("AbortedHistory: 3\n");
#endif



/* -------------------------------------------------------------------- */
/*   Prepend date/time,                                                 */
/*   Append "*** ABORTED ***", semicolon, newline to the history_line   */
/* -------------------------------------------------------------------- */
  bstring datetime = bfromcstr("");
  Time(datetime,7);
  bstring newvalue = bfromcstr("");
  bassignformat(newvalue,"%s: %s*** ABORTED ***;\n",bdata(datetime),bdata(history_line));
  bdestroy(datetime);


#ifdef DEBUG
  printf("GS_AbortedHistory: 4\n");
#endif



/* -------------------------------------------------------------------- */
/*   Update last_update_time, and history                               */
/* -------------------------------------------------------------------- */
  bstring history = bfromcstr("history");
  if( !GS_AppendMetadata( object_id, history, newvalue)){
    bassigncstr(error_string,"GS_AbortedHistory: Can't append to history.");
    bdestroy(newvalue);
    bdestroy(history);
    return FALSE;
  }// endif
  bdestroy(history);
  bdestroy(newvalue);

#ifdef DEBUG
  printf("GS_AbortedHistory: 5\n");
#endif

            
  bstring name = bfromcstr("last_update_datetime");
  if( !GS_UpdateMetadata( object_id, name, datetime)){
    bassigncstr(error_string,"GS_AbortedHistory: Can't alter last_update_time.");
    bdestroy(name);
    return FALSE;
  }// endif
  bdestroy(name);

#ifdef DEBUG
  printf("GS_AbortedHistory: 6\n");
#endif


  return TRUE;
}
