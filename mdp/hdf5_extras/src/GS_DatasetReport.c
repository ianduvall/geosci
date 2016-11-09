/* GS_DatasetReport.c

   by Leland Pierce, Aug 9, 2014

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


#define ERROR -1

//#define DEBUG

/** \brief GS_DatasetReport returns a brief description of the dataset.


\see GS_DatasetClose(), GS_DatasetOpen()

\param[in] dataset_id
                   A handle for a dataset.

\returns
A string that provides a brief overview of the dataset.
The string is zero-length on any error.
Always bdestroy() the string when done with it.

\par Example
Get a report string on an image raster dataset.
\code
hid_t raster_id;
bstring the_report;
the_report = GS_DatasetReport(raster_id);
printf("Report for the dataset: %s\n",bdata(the_report));
bdestroy(the_report);
\endcode

\par Details
For an image raster, the report string looks like the following:
\code
[Raster] Landsat-Washita-SMEX3: Channel02; UINT8, 1024 X 4096
WGS84, UTM16T, X-Range: 274716.95 to 275716.95, Y-Range: 4684434.95 to 4684534.95
\endcode
or more generally:
\code
[dataset-type] name: descriptor; datatype, dx X dy
Datum, Projection, Bounds
\endcode
where the first string is the type of dataset, which, besides "Raster",
could include: "Internal File", "Metadata", "Metadata/Internal-File", 
or "Unknown".
The second string is the name of the Dataset, while the third is the descriptor.
The next one is the datatype. 
See \ref GS_DatatypeAsString() for a list of possible datatypes.
The last string is the dimensions of the raster: dx X dy.
For non-image rasters, the dimensions are 1-deminsional, and there 
are no Datum, Projection, or Bounds entries.
NOTE: so far this only works for Image Rasters.

*/

/******************************************************/
bstring GS_DatasetReport(hid_t dataset_id)
{

  bstring ret = bfromcstr("");
  long int *size;
  int ndims;
  bstring tempstr;
  int i;
  double xmin, xmax, ymin, ymax;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");


/* -------------------------------------------------------------------- */
/* See if the id is a dataset                                           */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsDatasetByID(dataset_id)){
    bassignformat(error_string,"GS_DatasetReport: ID is not a Dataset.");
    return ret;
  }// endif

/* -------------------------------------------------------------------- */
/* Add the type of the dataset                                          */
/* -------------------------------------------------------------------- */
  switch(GS_DatasetGetType(dataset_id)){
  case GS_OBJECT_TYPE_IFILE:
    bconcat(ret,bsstatic("[Internal File]"));
    break;
  case GS_OBJECT_TYPE_RASTER:
    bconcat(ret,bsstatic("[Raster]"));
    break;
  case GS_OBJECT_TYPE_METADATA_DATASET:
    bconcat(ret,bsstatic("[Metadata]"));
    break;
  case GS_OBJECT_TYPE_METADATA_IFILE:
    bconcat(ret,bsstatic("[Metadata/Internal-File]"));
    break;
  default:
    bconcat(ret,bsstatic("[Unknown]"));
    break;
  }// end-switch
/* -------------------------------------------------------------------- */
/* Add the Read/Write status of the dataset                             */
/* -------------------------------------------------------------------- */
  if(GS_DatasetIsWriteableByID(dataset_id)){
    bconcat(ret,bsstatic("[RW] "));
  } else {
    bconcat(ret,bsstatic("[RO] "));
  }//endif

/* -------------------------------------------------------------------- */
/* Add the name of the dataset                                          */
/* -------------------------------------------------------------------- */
  tempstr = GS_GetIDName(dataset_id);
  bconcat(ret,tempstr);
  bdestroy(tempstr);
  bconcat(ret,bsstatic(": "));

/* -------------------------------------------------------------------- */
/* Add the descriptor of the dataset                                    */
/* -------------------------------------------------------------------- */
  tempstr = bfromcstr("");
  bstring descriptor = bfromcstr("descriptor");
  if(GS_GetStringAttribute( dataset_id, descriptor, tempstr)){
    bdestroy(descriptor);
    bconcat(ret,tempstr);
    bconcat(ret,bsstatic("; "));
  }// endif
  //lep bdestroy(descriptor);

/* -------------------------------------------------------------------- */
/* Add the datatype of the dataset                                      */
/* -------------------------------------------------------------------- */
  bconcat(ret,GS_DatatypeAsString(GS_DatasetGetDatatypeByID(dataset_id)));
  bconcat(ret,bsstatic(", "));

/* -------------------------------------------------------------------- */
/* Add the dimensions of the dataset                                    */
/* -------------------------------------------------------------------- */
  ndims = GS_DatasetGetDimensionsByID(dataset_id,&size);
  for(i=ndims-1;i>=0;i--){
    bassignformat(tempstr,"%ld",size[i]);
    bconcat(ret,tempstr);
    if(i!= 0) bconcat(ret,bsstatic(" X "));
  }// endfor
  GFree(size);
  bdestroy(tempstr);
  bconcat(ret,bsstatic("\n"));


/* -------------------------------------------------------------------- */
/* Add the Datum and Projection                                         */
/* -------------------------------------------------------------------- */
  bstring datum = GS_DatasetGetDatum(dataset_id);
  bconcat(ret,datum);
  bconcat(ret,bsstatic(", "));
  bdestroy(datum);
#ifdef DEBUG
  printf("GS_DatasetReport: 6.5\n");
#endif

  bstring proj = GS_DatasetGetProjection(dataset_id);
  bconcat(ret,proj);
  bconcat(ret,bsstatic(", "));
  bdestroy(proj);

#ifdef DEBUG
  printf("GS_DatasetReport: 7\n");
#endif

/* -------------------------------------------------------------------- */
/* Add the Bounds                                                       */
/* -------------------------------------------------------------------- */
  if(GS_DatasetGetBounds(dataset_id, &xmin, &xmax, &ymin, &ymax)){
    tempstr=bfromcstr("");
    bassignformat(tempstr,"X-Range: %lf to %lf, ",xmin,xmax);
    bconcat(ret,tempstr);
    bassignformat(tempstr,"Y-Range: %lf to %lf",  ymin,ymax);
    bconcat(ret,tempstr);
    bdestroy(tempstr);
  } else {
    bconcat(ret,bsstatic("No Bounds"));
  }// endif





  return ret;
}
