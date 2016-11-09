/* GS_AppendHistory.c

   by Leland Pierce, Jan 3, 2014

   converted Jun 9, 2016

*/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"



/** \brief GS_AppendHistory appends to the history metadata

AppendHistory() appends the given string to the history 
metadata item, and updates the last_update_datetime metadata item.
This works for any object with a "history" metadata item, and
a "last_update_datetime" metadata item.

\see GS_AppendMetadata()

\param[in]  object_id 
            ID of a file, an image, or any other valid object
            in a GeoSci file. 
\param[in history_line
            This string is appended to the history metadata item
            for the given object.
\returns
Returns TRUE on success, FALSE on failure.

\par Example
After writing to an image, update it's history, 
and also update the file's history.
\code
hid_t image_id;

bstring name = bfromcstr("/Image3");
image_id=GS_ImageOpen(file_id,name);
bdestroy(name);

     ... modify the image ....

bstring str = bfromcstr("Task_name(task_param1,task_param2,...)");
GS_AppendHistory(image_id,str);
bassigncstr(str,"Task_name: param1");
GS_AppendHistory(file_id,str);
bdestroy(str);
\endcode
*/



//#define DEBUG

/**********************************************************************/

int GS_AppendHistory(hid_t object_id, const_bstring history_line)
{

  hid_t header_id;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* See if the object-Id is valid                                        */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID(object_id)){
    bassigncstr(error_string,"GS_AppendHistory: invalid id.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_AppendHistory: 1\n");
#endif


  if(GS_ObjectIsFile(object_id)){
/* -------------------------------------------------------------------- */
/*	Open the file header.                      			*/
/* -------------------------------------------------------------------- */
    header_id = GS_GroupOpen(object_id, file_metadata_name);
    if(header_id < 0){
      bassignformat(error_string,"GS_AppendHistory: Can't open the file metadata group %s.",
                    bdata(file_metadata_name));
      return FALSE;
    }// endif
#ifdef DEBUG
  printf("GS_AppendHistory: 2\n");
#endif
  } else {
    header_id = object_id;
  }// endif


/* -------------------------------------------------------------------- */
/*   Prepend date/time,                                                 */
/*   Append a semicolon and newline to the history_line                 */
/* -------------------------------------------------------------------- */
  bstring datetime = bfromcstr("");
  GS_Time(datetime,7);
  bstring newvalue = bfromcstr("");
  bassignformat(newvalue,"%s: %s;\n",bdata(datetime),bdata(history_line));

#ifdef DEBUG
  printf("GS_AppendHistory: 3, newhistorystring=|%s|\n",bdata(newvalue));
#endif

/* -------------------------------------------------------------------- */
/*   Update last_update_time, and history                               */
/* -------------------------------------------------------------------- */
  bstring history = bfromcstr("history");
  if( !GS_AppendMetadata( header_id, history, newvalue)){
    bdestroy(newvalue);
    bdestroy(history);
    bassigncstr(error_string,"GS_AppendHistory: Can't append to history.");
    return FALSE;
  }// endif
  bdestroy(history);
  bdestroy(newvalue);

#ifdef DEBUG
  printf("GS_AppendHistory: 4\n");
#endif

  GS_Time(datetime,7);
  bstring name = bfromcstr("last_update_datetime");
  if( !GS_UpdateMetadata( header_id, name, datetime)){
    bassigncstr(error_string,"AppendHistory: Can't alter last_update_time.");
    bdestroy(name);
    return FALSE;
  }// endif
  bdestroy(name);
  bdestroy(datetime);

#ifdef DEBUG
  printf("GS_AppendHistory: 5\n");
#endif

/* -------------------------------------------------------------------- */
/*   Clean up                                                           */
/* -------------------------------------------------------------------- */
   if(GS_ObjectIsFile(object_id)){
     GS_GroupClose(header_id);
   }

#ifdef DEBUG
  printf("GS_AppendHistory: 6\n");
#endif

   return TRUE;
}
