/* GS_GroupReport.c

   by Leland Pierce, Aug 12, 2014

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


#define ERROR -1


/** \brief GS_GroupReport returns a one-line brief description of the group.

\see GS_GroupClose(), GS_GroupOpen()

\param[in] group_id
                   A handle for a group.

\returns
A string that provides a brief overview of the group.
The string is zero-length on any error.
Always bdestroy() the string when done with it.

\par Example
Get a report string on an image.
\code
hid_t raster_id;
bstring the_report;
the_report = GS_GroupReport(raster_id);
printf("Report for the dataset: %s\n",bdata(the_report));
bdestroy(the_report);
\endcode

\par Details
For Metadata Groups, we use the following string:
\code
[Metadata][RW] name: descriptor; last-update-datetime
\endcode
For Image Groups, we use the following string:
\code
[Image][RW] name: descriptor; last-update-datetime
\endcode
For Vector Groups, we use the following string:
\code
[Vector][RW] name: descriptor; last-update-datetime
Datum, Projection, Upper-left Lat/long, Lower-right Lat/Long
\endcode
Where "RW" stands for a group that is both Readable and Writeable, 
while "RO" would be used for a group that is Read-only.
Other Group-types have yet to be defined.
*/

//#define DEBUG                              

/***************************************************/
bstring GS_GroupReport(hid_t group_id)
{

  bstring ret = bfromcstr("");
  int *size;
  int ndims;
  bstring tempstr;
  int i;
  int j1, j2;
  bstring datum;
  bstring proj;
  double xmin, xmax;
  double ymin, ymax;
  int print_georef;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");


#ifdef DEBUG
  printf("GS_GroupReport: start\n");
#endif

/* -------------------------------------------------------------------- */
/* See if the id is a dataset                                           */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsGroupByID(group_id)){
    bassignformat(error_string,"GS_GroupReport: ID is not a Group.");
    return ret;
  }// endif

#ifdef DEBUG
  printf("GS_GroupReport: 1\n");
#endif

/* -------------------------------------------------------------------- */
/* Add the type of the group                                            */
/* -------------------------------------------------------------------- */
  print_georef=FALSE;
  switch(GS_GroupGetType(group_id)){
  case GS_OBJECT_TYPE_METADATA_GROUP:
    bconcat(ret,bsstatic("[Metadata]"));
    break;
  case GS_OBJECT_TYPE_IMAGE:
    bconcat(ret,bsstatic("[Image]"));
    break;
  case GS_OBJECT_TYPE_VECTOR:
    bconcat(ret,bsstatic("[Vector]"));
    print_georef=TRUE;
    break;
  case GS_OBJECT_TYPE_VECTOR2D:
    bconcat(ret,bsstatic("[Vector2D]"));
    print_georef=TRUE;
    break;
  case GS_OBJECT_TYPE_VECTOR3D:
    bconcat(ret,bsstatic("[Vector3D]"));
    print_georef=TRUE;
    break;
  case GS_OBJECT_TYPE_TIN:
    bconcat(ret,bsstatic("[TIN]"));
    break;
  case GS_OBJECT_TYPE_MESH2D:
    bconcat(ret,bsstatic("[Mesh2D]"));
    break;
  case GS_OBJECT_TYPE_MESH3D:
    bconcat(ret,bsstatic("[Mesh3D]"));
    break;
  case GS_OBJECT_TYPE_UNKNOWN:
  default:
    bconcat(ret,bsstatic("[Unknown]"));
    break;
  }// end-switch

#ifdef DEBUG
  printf("GS_GroupReport: 2\n");
#endif

/* -------------------------------------------------------------------- */
/* Add the Read/Write status of the group                               */
/* -------------------------------------------------------------------- */
  if(GS_GroupIsWriteableByID(group_id)){
    bconcat(ret,bsstatic("[RW] "));
  } else {
    bconcat(ret,bsstatic("[RO] "));
  }//endif

#ifdef DEBUG
  printf("GS_GroupReport: 3\n");
#endif

/* -------------------------------------------------------------------- */
/* Add the name of the dataset                                          */
/* -------------------------------------------------------------------- */
  tempstr=GS_GetIDName(group_id);
  bconcat(ret,tempstr);
  bdestroy(tempstr);
  bconcat(ret,bsstatic(": "));

#ifdef DEBUG
  printf("GS_GroupReport: 4\n");
#endif

/* -------------------------------------------------------------------- */
/* Add the descriptor of the dataset                                    */
/* -------------------------------------------------------------------- */
  tempstr = bfromcstr("");
  if(GS_GetStringAttribute( group_id, bsstatic("descriptor"), tempstr)){
    bconcat(ret,tempstr);
    bconcat(ret,bsstatic("; "));
  }// endif

#ifdef DEBUG
  printf("GS_GroupReport: 5\n");
#endif

/* -------------------------------------------------------------------- */
/* Add the last-update-datetime of the dataset                          */
/* -------------------------------------------------------------------- */
  if(GS_GetStringAttribute( group_id, bsstatic("last_update_datetime"), tempstr)){
    bconcat(ret,tempstr);
    bconcat(ret,bsstatic("\n"));
  }// endif
  bdestroy(tempstr);

#ifdef DEBUG
  printf("GS_GroupReport: 6\n");
#endif


  if(print_georef){

/* -------------------------------------------------------------------- */
/* Add the Datum and Projection                                         */
/* -------------------------------------------------------------------- */
  datum = GS_GroupGetDatum(group_id);
  bconcat(ret,datum);
  bconcat(ret,bsstatic(", "));
  bdestroy(datum);
#ifdef DEBUG
  printf("GS_GroupReport: 6.5\n");
#endif

  proj = GS_GroupGetProjection(group_id);
  bconcat(ret,proj);
  bconcat(ret,bsstatic(", "));
  bdestroy(proj);

#ifdef DEBUG
  printf("GS_GroupReport: 7\n");
#endif

/* -------------------------------------------------------------------- */
/* Add the Bounds                                                       */
/* -------------------------------------------------------------------- */
  if(GS_GroupGetBounds(group_id, &xmin, &xmax, &ymin, &ymax)){
    tempstr=bfromcstr("");
    bassignformat(tempstr,"X-Range: %lf to %lf, ",xmin,xmax);
    bconcat(ret,tempstr);
    bassignformat(tempstr,"Y-Range: %lf to %lf",  ymin,ymax);
    bconcat(ret,tempstr);
    bdestroy(tempstr);
  }// endif

  }// endif: print_georef?

#ifdef DEBUG
  printf("GS_GroupReport: 8\n");
#endif


  return ret;
}
