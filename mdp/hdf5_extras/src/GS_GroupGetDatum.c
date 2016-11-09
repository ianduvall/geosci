/* GS_GroupGetDatum.c

   by Leland Pierce, Aug 14, 2014

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


/** \brief GS_GroupGetDatum returns a description of the relevant datum
if one exists.

\see GS_GroupClose(), GS_GroupOpen()

\param[in] group_id
                   A handle for a group.

\returns
A string describing the datum that is used for the data in this group.
"No Datum" is returned if the datum cannot be determined, or on any other error.
This string should be bdestroy()'d when finished with it.

\par Example
Print the datum for a particular group:
\code
hid_t group_id;
bstring datum;
datum = GS_GroupGetDatum(group_id);
printf("%s\n",datum);
bdestroy(datum);
\endcode

\par Details
This routine looks at the "spatialref" metadata string in order to
determine the datum that is being used.
If the spatialref metadata item does not exist, then "No Datum" is returned.
If the spatialref metadata item exists, then the routine looks for the 
keyword "DATUM"
which should appear in the WKT portion of this metadata.
If it is not found, then "No Datum" is returned,
otherwise the string that follows this keyword is returned.
*/

/****************************************************************/
bstring GS_GroupGetDatum(hid_t group_id)
{
  bstring ret;
  bstring tempstr;
  int i,j1,j2;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* See if the id is a group                                             */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsGroupByID(group_id)){
    bassignformat(error_string,"GS_GroupGetDatum: ID is not a Group.");
    return bfromcstr("No Datum");
  }// endif

/* -------------------------------------------------------------------- */
/* Get the spatial reference system, if it exists                       */
/* -------------------------------------------------------------------- */
  tempstr=bfromcstr("");
  bstring spatialref = bfromcstr("spatialref");
  if(GS_GetStringAttribute( group_id, spatialref, tempstr)){

/* -------------------------------------------------------------------- */
/* Get the Datum                                                        */
/* -------------------------------------------------------------------- */
    i=binstr(tempstr,0,bsstatic("DATUM"));
    if(i > 0){
      // find the next dbl-quote:
      j1 = bstrchrp(tempstr,'"',i);
      if(j1>i){
        // find the next dbl-quote:
        j2 = bstrchrp(tempstr,'"',j1);
        if(j2 > j1){
          // grab the datum string and print it:
          ret =  bmidstr(tempstr,j1+1,j2-j1-1);
        } else {
          ret= bfromcstr("No Datum");      
        }// endif
      } else {
        ret= bfromcstr("No Datum");      
      }// endif
    } else {
      // No DATUM found:
      ret= bfromcstr("No Datum");      
    }//endif

  } else {
    // spatialref metadata does not exist:
    ret= bfromcstr("No Datum");      

  }//endif: spatialref metadata exists?
  bdestroy(tempstr);
  bdestroy(spatialref);

  return ret;
}


