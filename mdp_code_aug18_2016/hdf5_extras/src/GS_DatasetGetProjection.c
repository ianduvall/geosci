/* GS_DatasetGetProjection.c

   by Leland Pierce, JUne 18, 2016

*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


/** \brief GS_DatasetGetProjection returns a description of the relevant projection


\see GS_DatasetClose(), GS_DatasetOpen()

\param[in] dataset_id
                   A handle for a dataset.

\returns
A string describing the projection that is used for the data in this dataset.
"No Projection" is returned if the projection cannot be determined, or on any other error.
This string should be bdestroy()'d when finished with it.

\par Example
Print the projection for a particular dataset:
\code
hid_t dataset_id;
bstring projection;
projection = GS_DatasetGetProjection(dataset_id);
printf("%s\n",bdata(projection));
bdestroy(projection);
\endcode

\par Details
This routine looks at the "spatialref" metadata string in order to
determine the projection that is being used.
If the spatialref metadata item does not exist, then "No Projection" is returned.
If the spatialref metadata item exists, then the routine looks for the 
keyword "PROJCS"
which should appear in the WKT portion of this metadata.
If it is found, the string that follows this keyword is returned.
If it is not found, then the routine looks for the keyword "GEOGCS"
which should appear in the WKT portion of this metadata.
If it is found, then the string "Geographic" is returned,
otherwise the string "No Projection" is returned.
*/

/***********************************************************************/
bstring GS_DatasetGetProjection(hid_t dataset_id)
{
  bstring ret=bfromcstr("No Projection");
  bstring tempstr;
  int i,j1,j2;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* See if the id is a dataset                                           */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsDatasetByID(dataset_id)){
    bassignformat(error_string,"GS_DatasetGetProjection: ID is not a Dataset.");
    return bfromcstr("No Projection");
  }// endif

/* -------------------------------------------------------------------- */
/* Get the spatial reference system, if it exists                       */
/* -------------------------------------------------------------------- */
  tempstr=bfromcstr("");
  bstring spatialref = bfromcstr("spatialref");
  if(GS_GetStringAttribute( dataset_id, spatialref, tempstr)){

/* -------------------------------------------------------------------- */
/* Get the Projection                                                        */
/* -------------------------------------------------------------------- */
    i=binstr(tempstr,0,bsstatic("PROJCS"));
    if(i <= 0){
      // the word PROJCS was not found,
      // look for GEOGCS instead:
      i=binstr(tempstr,0,bsstatic("GEOGCS"));
      if(i>0){
        // Found: use "Geographic" as the projection
        ret= bfromcstr("Geographic");      
      } else {
        // nothing: give up
        ret= bfromcstr("No Projection");      
      }// endif
      
    } else {
      // found the word PROJCS: get its value:
      // find the next dbl-quote:
      j1 = bstrchrp(tempstr,'"',i);
      if(j1>i){
        // find the next dbl-quote:
        j2 = bstrchrp(tempstr,'"',j1);
        if(j2 > j1){
          // grab the projection string and print it:
          ret = bmidstr(tempstr,j1+1,j2-j1-1);
        } else {
          ret= bfromcstr("No Projection");      
        }// endif
      } else {
        ret= bfromcstr("No Projection");      
      }// endif
    }//endif

  }//endif: spatialref metadata exists?
  bdestroy(tempstr);
  bdestroy(spatialref);

  return ret;
}


