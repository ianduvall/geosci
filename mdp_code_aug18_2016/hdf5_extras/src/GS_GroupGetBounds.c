/* GS_GroupGetBounds.c

   by Leland Pierce, Aug 14, 2014

*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


/** \brief GS_GroupGetBounds returns the rectangular bounding coordinates


\see GS_GroupClose(), GS_GroupOpen(), GS_GroupGetProjection()

\param[in] group_id
                   A handle for a group.
\param[out] xmin
\param[out] xmax
\param[out] ymin
\param[out] ymax
        The coordinates are returned with units 
that are consistent with the projection that is used.
Typically the units are decimal degrees or meters.

\returns
\c TRUE on success, \c FALSE if there are any errors.

\par Example
Print the bounds for a particular group:
\code
hid_t group_id;
double xmin, xmax, ymin, ymax;
if(GS_GroupGetBounds(group_id, &xmin, &xmax, &ymin, &ymax)){
  printf("X-range: %lf to %lf, Y-range: %lf to %lf\n", xmin,xmax, ymin,ymax);
} else {
  printf("no bounds\n");
}
\endcode

\par Details
This routine looks at the "bounds" metadata string in order to
determine the bounds that go with the data in this group.
If this metadata does not exist, or there is an error in 
obtaining it or parsing it, \c FALSE is returned.
Otherwise, the correct numbers are filled in, and \c TRUE is returned.
*/

/************************************************************************/
int GS_GroupGetBounds(hid_t group_id, double *xmin, double *xmax, 
                      double *ymin, double *ymax)
{
  bstring tempstr;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* See if the id is a group                                             */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsGroupByID(group_id)){
    bassignformat(error_string,"GS_GroupGetBounds: ID is not a Group.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Get the bounds metadata, if it exists                                */
/* -------------------------------------------------------------------- */
  tempstr=bfromcstr("");
  bstring bounds = bfromcstr("bounds");
  if(GS_GetStringAttribute( group_id, bounds, tempstr)){

/* -------------------------------------------------------------------- */
/* Parse the Bounds                                                     */
/* -------------------------------------------------------------------- */
    if(sscanf(bdata(tempstr),"%lf, %lf, %lf, %lf",xmin,xmax,ymin,ymax) !=4 ){
      bassignformat(error_string,"GS_GroupGetBounds: parsing error.");
      bdestroy(tempstr);
      bdestroy(bounds);
      return FALSE;
    } else {
      bdestroy(tempstr);
      bdestroy(bounds);
      return TRUE;
    }//endif

  } else {
    bassignformat(error_string,"GS_GroupGetBounds: bounds metadata does not exist.");
    bdestroy(tempstr);
    bdestroy(bounds);
    return FALSE;
  }//endif: bounds metadata exists?

}


