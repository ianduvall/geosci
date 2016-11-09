/* GS_DatasetGetDimensionsByID.c

   by Leland Pierce, Aug 9, 2014
   with doxygen, bstrings, GS_

*/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


//#define DEBUG

/** \brief GS_DatasetGetDimensionsByID returns the current dimensions for the Dataset 


\see GS_DatasetGetDimensions(), GS_DatasetGetDatatype(), GS_DatasetOpen(), GS_DatasetCreate()

\param[in] id
                   object handle of the Dataset.
\param[out] size
                  A vector of long integers: the size of each dimension: 
                  the number of data-objects per dimension. 

\returns
The number of dimensions in the Dataset, or zero if there is an error.
Also returns the length of each dimension (number of data-objects), in
 the integer vector \c size.
This vector is allocated by this function, and should be GFree()'d 
when done with it.


\par Example
Get the size of an image raster Dataset.
\code
hid_t raster_id;
int ndims;
long int *size;
ndims = GS_DatasetGetDimensionsByID(raster_id,&size);
if(!ndims) {
   printf("GS_DatasetGetDimensionsByID failure\n");
}
printf("the dataset has %d dimensions:\n",ndims);
for(i=0;i<ndims;i++) 
  printf("dimension %d size is: %ld data-objects\n",i+1,size[i]);
GFree(size);
\endcode

\par Details
Note that this function returns dimensions in terms of the number of data-objects.
It does NOT return the size in bytes.

*/

int GS_DatasetGetDimensionsByID(hid_t id, long int **size)
{
  hid_t space_id;
  int ndims;
  hsize_t *thesize;
  long int *thesize_long;
  int i;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Check for valid Dataset                                              */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsDatasetByID(id)){
    bassigncstr(error_string,"GS_DatasetGetDimensionsByID: is is not a Dataset.\n");
    return FALSE;
  }// endif

/* ------------------------------------------------------------ */
/* Get the Space of the Dataset                                 */
/* ------------------------------------------------------------ */
  space_id = H5Dget_space(id);
  if(space_id < 0){
    bassigncstr(error_string,"GS_DatasetGetDimensionsByID: Can't get the Space of Dataset.");
    return FALSE;
  }// endif

/* ------------------------------------------------------------ */
/* Get the number of dimensions                                */
/* ------------------------------------------------------------ */
  ndims = H5Sget_simple_extent_ndims( space_id );
  if(ndims <= 0 ){
    bassigncstr(error_string,"GS_DatasetGetDimensionsByID: Can't get the number of dimensions of Dataset.");
    H5Sclose(space_id);
    return FALSE;
  }// endif

/* ------------------------------------------------------------ */
/* Allocate space for size vector                               */
/* ------------------------------------------------------------ */
  thesize = (hsize_t *)GMalloc(ndims*sizeof(hsize_t));
  if(!thesize){
    bassigncstr(error_string,"GS_DatasetGetDimensionsByID: HMalloc failure.");
    H5Sclose(space_id);
    return FALSE;
  }// endif
  thesize_long = (long int *)GMalloc(ndims*sizeof(hsize_t));
  if(!thesize_long){
    bassigncstr(error_string,"GS_DatasetGetDimensionsByID: HMalloc failure.");
    H5Sclose(space_id);
    return FALSE;
  }// endif

/* ------------------------------------------------------------ */
/* Get the size of each dimension                               */
/* ------------------------------------------------------------ */
  if(H5Sget_simple_extent_dims(space_id, thesize, NULL ) <= 0){
    bassigncstr(error_string,"GS_DatasetGetDimensionsByID: Can't get Dataset dimensions.");
    GFree(thesize);
    H5Sclose(space_id);
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_DatasetGetDimensionsByID: ndims=%d, thesize[0]=%d, thesize[1]=%d\n",
         ndims,(int)thesize[0],(int)thesize[1]);
#endif

/* ------------------------------------------------------------ */
/* Close the Space                                              */
/* ------------------------------------------------------------ */
  if(H5Sclose(space_id) < 0 ){
    bassigncstr(error_string,"GS_DatasetGetDimensionsByID: Can't get close the Space of the Dataset.");
    GFree(thesize);
    return FALSE;
  }// endif

/* ------------------------------------------------------------ */
/* Return the vector of dimensions:                             */
/* ------------------------------------------------------------ */
#ifdef DEBUG
  printf("GS_DatasetGetDimensionsByID: thesize= %d, %d\n",
         (int)thesize[0], (int)thesize[1]);
#endif
  for(i=0;i<ndims;i++) thesize_long[i] = (long int)thesize[i]; 
  *size = thesize_long;

#ifdef DEBUG
  printf("GS_DatasetGetDimensionsByID: *size= %d, %d\n",
         (int)(*size)[0], (int)(*size)[1]);
#endif
  
  return ndims;
}
