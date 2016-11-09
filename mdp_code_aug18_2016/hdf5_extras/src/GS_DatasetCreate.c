/* GS_DatasetCreate.c

   by Leland Pierce, Aug 7, 2014
   with bstrings, doxygen, GS_

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


#define ERROR -1


/** \brief GS_DatasetCreate creates a dataset in a GeoSci datafile

\see GS_DatasetOpen(), GS_DatasetClose()

\param[in] source
                   A handle for the file or other container 
                   within which to create the dataset.
\param[in] name
                   The name of the dataset to create.
\param[in] datatype
                   The datatype of the numbers to store.
                   Must be one of:<br>
\c GS_DATATYPE_UI1 (Unsigned Integer, 1-bit)\n
\c GS_DATATYPE_UI8 (Unsigned Integer, 8-bit)\n
\c GS_DATATYPE_SI8 (Signed Integer, 8-bit)\n
\c GS_DATATYPE_CI8 (Complex Integer, 8-bit (not supported))\n
\c GS_DATATYPE_UI16 (Unsigned Integer, 16-bit)\n
\c GS_DATATYPE_SI16 (Signed Integer, 16-bit)\n
\c GS_DATATYPE_CI16 (Complex Integer, 16-bit)\n
\c GS_DATATYPE_UI32 (Unsigned Integer, 32-bit)\n
\c GS_DATATYPE_SI32 (Signed Integer, 32-bit)\n
\c GS_DATATYPE_CI32 (Complex Integer, 32-bit)\n
\c GS_DATATYPE_CI64 (Complex Integer, 64-bit)\n
\c GS_DATATYPE_R32 (Real, 32-bit)\n
\c GS_DATATYPE_R64 (Real, 64-bit)\n
\c GS_DATATYPE_C64 (Complex, 64-bit)\n
\c GS_DATATYPE_C128 (Complex, 128-bit)\n
\c GS_DATATYPE_UI64 (Unsigned Integer, 64-bit)\n
\c GS_DATATYPE_SI64 (Signed Integer, 64-bit)\n
\param[in] ndims
                   The number of dimensions for the dataset.
\param[in] size
                   The (initial) size for each dimension
\param[in] is_extendable
                   Set to TRUE to make the dataset size extendable,
                   set to FALSE to make the dataset size fixed.
\param[in] is_compressed
                   Set to TRUE to make the dataset compressed,
                   set to FALSE to make the dataset uncompressed. 


\returns
The handle of the newly-created and opened dataset is returned.
If it is less than zero, the creation failed.

\par Example
Create an image raster dataset, with size 1024X512:
\code
hid_t image_id, raster_id;
int   size[2];
size[0]=512;
size[1]=1024;
bstring name = bfromcstr("r1");
raster_id = GS_DatasetCreate(image_id, name,2,size,FALSE,FALSE);
bdestroy(name);
if(raster_id < 0) {
  printf("Failed to create dataset.\n");
}
\endcode
*/


hid_t GS_DatasetCreate(hid_t source, const_bstring name, int datatype,
                       int ndims, int *size, int is_extendable, 
                       int is_compressed)
{
  int i;
  hid_t hdf_datatype;
  hsize_t *maxdims;
  hid_t space_id;
  hid_t plist;
  const_bstring type_string;
  hid_t dataset_id;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* Check if this name already exists                                    */
/* -------------------------------------------------------------------- */
  if(GS_CheckInternalName(source, name)){
    bassigncstr(error_string,"GS_DatasetCreate: Name already exists.");
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Check validity of datatype                                           */
/* -------------------------------------------------------------------- */
  hdf_datatype = GS_ConvertToHDFDatatype(datatype);
  if(hdf_datatype < 0){
    bassigncstr(error_string,"GS_DatasetCreate: Invalid datatype.");
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Check validity of dimensions                                         */
/* -------------------------------------------------------------------- */
  if(ndims < 1) {
    bassigncstr(error_string,"GS_DatasetCreate: ndims must be >= 1.");
    GS_HDFDatatypeClose(hdf_datatype);
    return ERROR;
  }// endif

  /* sizes can be zero, but not negative */
  for(i=0;i<ndims;i++){
    if(size[i]<0){
      bassigncstr(error_string,"GS_DatasetCreate: size must be >= 0.");
      GS_HDFDatatypeClose(hdf_datatype);
      return ERROR;
    }// endif
  }// endfor

/* -------------------------------------------------------------------- */
/* Create HDF5 Dataspace                                                */
/* -------------------------------------------------------------------- */
  maxdims=NULL;
  if(is_extendable){
    maxdims = (hsize_t *)GMalloc(ndims*sizeof(hsize_t));
    if(!maxdims){
      bassigncstr(error_string,"GS_DatasetCreate: GMalloc error.");
      GS_HDFDatatypeClose(hdf_datatype);
      return ERROR;
    } 
    for(i=0;i<ndims;i++) maxdims[i]=H5S_UNLIMITED;
    space_id = H5Screate_simple( ndims, (const hsize_t *)size, maxdims );
  } else {
    space_id = H5Screate_simple( ndims, (const hsize_t *)size, NULL );
  }// endif

/* -------------------------------------------------------------------- */
/* Create HDF5 Property List                                            */
/* -------------------------------------------------------------------- */
  plist = H5Pcreate( H5P_DATASET_CREATE );
  if(is_extendable || is_compressed){
    // extendable or compressed datasets must be "chunked"
    if(!maxdims){
      maxdims = (hsize_t *)GMalloc(ndims*sizeof(hsize_t));
      if(!maxdims){
        bassigncstr(error_string,"GS_DatasetCreate: GMalloc error.");
        GS_HDFDatatypeClose(hdf_datatype);
        return ERROR;
      }// endif 
    }// endif
    for(i=0;i<ndims;i++) maxdims[i]=1024;
    if(H5Pset_chunk(plist, ndims, maxdims) < 0){
      bassigncstr(error_string,"GS_DatasetCreate: Setting chunking failed.");
      GFree(maxdims);
      H5Sclose(space_id);
      GS_HDFDatatypeClose(hdf_datatype);
      return ERROR;
    }// endif
    GFree(maxdims);
  }// endif

  if(is_compressed ){
    if(H5Pset_deflate(plist,8) < 0) {
      bassigncstr(error_string,"GS_DatasetCreate: Unable to set compression.");
      H5Sclose(space_id);
      GS_HDFDatatypeClose(hdf_datatype);
      return ERROR;
    }// endif
  }// endif

/* -------------------------------------------------------------------- */
/* Commit the datatype if Complex                                       */
/* -------------------------------------------------------------------- */
  if(GS_DatatypeIsComplex(datatype)==TRUE){
    type_string = GS_DatatypeAsString(datatype);
    if(!GS_CheckInternalName(source,type_string)) {
      // datatype not in the file, add it:
      if(H5Tcommit(source, bdata(type_string), hdf_datatype, 
                   H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT) <0){
        bassigncstr(error_string,"GS_DatasetCreate: Committing the complex datatype failed.");
        H5Sclose(space_id);
        H5Pclose(plist);
        GS_HDFDatatypeClose(hdf_datatype);
        return ERROR;
      }// endif
    }//endif: type already in the file?
  }// endif: complex type?

/* -------------------------------------------------------------------- */
/* Create the Dataset                                                   */
/* -------------------------------------------------------------------- */
  dataset_id = H5Dcreate(source, bdata(name), hdf_datatype, space_id, 
                      H5P_DEFAULT, plist, H5P_DEFAULT);
  
/* -------------------------------------------------------------------- */
/* Clean up                                                             */
/* -------------------------------------------------------------------- */
  if(GS_HDFDatatypeClose(hdf_datatype)<0){
    bassigncstr(error_string,"GS_DatasetCreate: Error closing dataspace.");
    if(dataset_id >= 0) H5Dclose(dataset_id);
    H5Pclose(space_id);
    H5Pclose(plist);
    return ERROR;
  }//endif

  if(H5Sclose(space_id) < 0){
    bassigncstr(error_string,"GS_DatasetCreate: Error closing dataspace.");
    if(dataset_id >= 0) H5Dclose(dataset_id);
    H5Pclose(plist);
    return ERROR;
  }// endif
  
  if(H5Pclose(plist) < 0){
    bassigncstr(error_string,"GS_DatasetCreate: Error closing property list.");
    if(dataset_id >= 0) H5Dclose(dataset_id);
    return ERROR;
  }// endif
  
  return dataset_id;

}
