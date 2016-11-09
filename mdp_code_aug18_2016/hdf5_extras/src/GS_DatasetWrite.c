/* GS_DatasetWrite.c

   by Leland Pierce, Aug 10, 2014

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"



/** \brief GS_DatasetWrite writes data to a dataset

\see GS_DatasetRead(), GS_DatasetOpen(), GS_DatasetCreate()

\param[in] id
          A handle for the dataset. Perhaps from GS_DatasetOpen().
\param[in] offsets
          Must supply an offset to the first data element to write 
          for each dimension. 
          Zero offset is used for writing starting with the first data-element.
\param[in] sizes
          Must supply how many data elements to write 
          for each dimension.
          Zero offset is used for writing starting with the first data-element.
          Using the offset and size for each dimension allows the
          specification of rectangular windows in n-dimensional space.
          For example, in 2-dimensions the window is:<br>
          xoffset = offset[0], yoffset= offset[1]<br>
          xsize = sizes[0], ysize = sizes[1]
\param[in] datatype
          The datatype of the buffer. 
          All the data that is written is converted from this datatype,
          to the datatype used by the Dataset.
          See the Details section for how this conversion is done.
          The valid datatypes are:<br>
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
\param[out] buffer
          This is a pointer to enough memory to hold the 
          data that is being written.
          
\returns
\c TRUE on sucess, \c FALSE on failure.

\par Example
We have a 2D dataset named "d4",
Write to data elements 0 to 500 in dimension 1, 
and to data elements 100 to 700 in dimension 2.
The data is stored in the Dataset as integers, 
and we are sending doubles, which therefore need to be converted.
\code
hid_t group_id;
hid_t dataset_id;
double buffer[500*600];
int offsets[10], sizes[10];
dataset_id = GS_DatasetOpen(group_id,"d4");
if(dataset_id < 0){
  printf("Failed to open dataset\n");
  return;
}
offsets[0]=0;
offsets[1]=100;
sizes[0]=500;
sizes[1]=600;
if(!GS_DatasetWrite(dataset_id, offsets, sizes, 
                   GS_DATATYPE_R64, (void *)buffer)) {
  printf("Failed to write to dataset.\n");
}
\endcode

\par Details
YET: data type conversions....<br>
YET: deal with interleaving spec.....<br>
YET: indexing...
*/

//#define DEBUG

int GS_DatasetWrite(hid_t dataset_id, const long int *offsets,
                     const long int *sizes, int datatype, const void *buffer)
{
  int dataset_ndims;
  long int *dataset_dims;
  int i;
  hid_t hdf_datatype;
  hid_t file_space_id;
  hid_t mem_space_id;

  unsigned char *local_buffer;
  unsigned long int size;
  int dataset_datatype;
  hsize_t *step;
  hsize_t *hoffsets;
  hsize_t *hsizes;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("GS_DatasetWrite: start\n");
#endif
/* -------------------------------------------------------------------- */
/* Check if its a dataset id                                            */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsDatasetByID(dataset_id)){
    bassigncstr(error_string,"GS_DatasetWrite: Not a dataset ID.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_DatasetWrite: 1\n");
#endif
/* -------------------------------------------------------------------- */
/* Get dataset dimensions                                               */
/* -------------------------------------------------------------------- */
  dataset_ndims = GS_DatasetGetDimensionsByID(dataset_id, &dataset_dims);
  if(dataset_ndims <= 0) {
    bassigncstr(error_string,"GS_DatasetWrite: Dataset has no dimensions.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_DatasetWrite: 2\n");
  printf("                 dataset_dims=%d,%d\n",(int)dataset_dims[0],
         (int)dataset_dims[1]);
#endif
/* -------------------------------------------------------------------- */
/* Check validity of offsets                                            */
/* -------------------------------------------------------------------- */
  for(i=0;i<dataset_ndims;i++){
    if(offsets[i] < 0 || offsets[i] > dataset_dims[i]){
      bassigncstr(error_string,"GS_DatasetWrite: Requested offsets invalid.");
      return FALSE;
    }// endif
  }// endfor

#ifdef DEBUG
  printf("GS_DatasetWrite: 3\n");
#endif
/* -------------------------------------------------------------------- */
/* Check validity of sizes                                              */
/* -------------------------------------------------------------------- */
  for(i=0;i<dataset_ndims;i++){
#ifdef DEBUG2
  printf("GS_DatasetWrite: 3.1\n");
  printf("                 dataset_dims[%d]=%d\n",i,(int)dataset_dims[i]);
  printf("                 sizes[%d]=%d, offsets[%d]=%d\n",i,(int)sizes[i],
         i,(int)offsets[i]);
#endif
    if(sizes[i] < 0 || offsets[i]+sizes[i] > dataset_dims[i]){
      bassigncstr(error_string,"GS_DatasetWrite: Requested sizes invalid.");
      return FALSE;
    }// endif
  }// endfor

#ifdef DEBUG
  printf("GS_DatasetWrite: 4\n");
#endif
/* -------------------------------------------------------------------- */
/* Check validity of datatype                                           */
/* -------------------------------------------------------------------- */
  if(GS_ConvertToHDFDatatype(datatype) < 0 ) {
    bassigncstr(error_string,"GS_DatasetWrite: Invalid datatype.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_DatasetWrite: 5\n");
#endif
/* -------------------------------------------------------------------- */
/* Get datatype of dataset                                              */
/* -------------------------------------------------------------------- */
  dataset_datatype=GS_DatasetGetDatatypeByID(dataset_id);
  if(dataset_datatype ==0){
    bassigncstr(error_string,"GS_DatasetWrite: Invalid dataset datatype.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_DatasetWrite: 6\n");
#endif
/* -------------------------------------------------------------------- */
/* Get HDF datatype of dataset                                          */
/* -------------------------------------------------------------------- */
  hdf_datatype = GS_ConvertToHDFDatatype(dataset_datatype);
  if(hdf_datatype < 0 ) {
    bassigncstr(error_string,"GS_DatasetWrite: Invalid dataset_datatype.");
    return FALSE;
  }// endif


#ifdef DEBUG
  printf("GS_DatasetWrite: 7\n");
#endif
/* -------------------------------------------------------------------- */
/* Create a buffer that can hold all the data, in the                   */
/* datatype of the file:                                                */
/* -------------------------------------------------------------------- */
  size = GS_RasterTypeNumBytes(dataset_datatype);
#ifdef DEBUG
  printf("GS_DatasetWrite: 7.1\n");
#endif
  if(size==0){
    bassigncstr(error_string,"GS_DatasetWrite: Invalid datatype size.");
    GS_HDFDatatypeClose(hdf_datatype);
    return FALSE;
  }//endif
#ifdef DEBUG
  printf("GS_DatasetWrite: 7.2\n");
#endif
  for(i=0;i<dataset_ndims;i++) size *= sizes[i];
  local_buffer = (unsigned char *)GMalloc(size*sizeof(unsigned char));
#ifdef DEBUG
  printf("GS_DatasetWrite: 7.3\n");
#endif
  if(!local_buffer){
    bassigncstr(error_string,"GS_DatasetWrite: Malloc error.");
    GS_HDFDatatypeClose(hdf_datatype);
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_DatasetWrite: 8\n");
#endif
/* -------------------------------------------------------------------- */
/* Create Space to describe local_buffer                                */
/* -------------------------------------------------------------------- */
  hsizes = (hsize_t *)GMalloc(dataset_ndims*sizeof(hsize_t));
  if(!hsizes){
    bassigncstr(error_string,"GS_DatasetWrite: HMalloc error on hsizes.");
    GFree(local_buffer);
    H5Sclose(mem_space_id);
    H5Sclose(file_space_id);
    GS_HDFDatatypeClose(hdf_datatype);
    return FALSE;
  }// endif
  for(i=0;i<dataset_ndims;i++) hsizes[i] =sizes[i];
  mem_space_id = H5Screate_simple( dataset_ndims, hsizes, NULL );
  if(mem_space_id < 0 ){
    bassigncstr(error_string,"GS_DatasetWrite: Failed to create mem-space spec.");
    GFree(hsizes);
    GFree(local_buffer);
    GS_HDFDatatypeClose(hdf_datatype);
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_DatasetWrite: 9\n");
#endif
/* -------------------------------------------------------------------- */
/* Create Space to describe subset of file-dataset                      */
/* -------------------------------------------------------------------- */
  file_space_id = H5Dget_space(dataset_id);
  if ( file_space_id < 0 ){
    bassigncstr(error_string,"GS_DatasetWrite: Can't get space of dataset.");
    GFree(hsizes);
    GFree(local_buffer);
    H5Sclose(mem_space_id);
    GS_HDFDatatypeClose(hdf_datatype);
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_DatasetWrite: 10\n");
#endif
/* -------------------------------------------------------------------- */
/* Select the appropriate hyperslab of the file-dataset                 */
/* -------------------------------------------------------------------- */
  step = (hsize_t *)GMalloc(dataset_ndims*sizeof(hsize_t));
  if(!step){
    bassigncstr(error_string,"GS_DatasetWrite: HMalloc error on step.");
    GFree(hsizes);
    GFree(local_buffer);
    H5Sclose(mem_space_id);
    H5Sclose(file_space_id);
    GS_HDFDatatypeClose(hdf_datatype);
    return FALSE;
  }// endif
  for(i=0;i<dataset_ndims;i++) step[i] =1;
  hoffsets = (hsize_t *)GMalloc(dataset_ndims*sizeof(hsize_t));
  if(!hoffsets){
    bassigncstr(error_string,"GS_DatasetWrite: HMalloc error on hoffsets.");
    GFree(step);
    GFree(hsizes);
    GFree(local_buffer);
    H5Sclose(mem_space_id);
    H5Sclose(file_space_id);
    GS_HDFDatatypeClose(hdf_datatype);
    return FALSE;
  }// endif
  for(i=0;i<dataset_ndims;i++) hoffsets[i] =offsets[i];
  if(H5Sselect_hyperslab( file_space_id, H5S_SELECT_SET, 
                          hoffsets,step, hsizes, NULL) < 0 ){
    bassigncstr(error_string,"GS_DatasetWrite: Can't set hyperslab for dataset space");
    GFree(step);
    GFree(hoffsets);
    GFree(hsizes);
    GFree(local_buffer);
    H5Sclose(mem_space_id);
    H5Sclose(file_space_id);
    GS_HDFDatatypeClose(hdf_datatype);
    return FALSE;
  }// endif
  GFree(step);
  GFree(hoffsets);
  GFree(hsizes);
  
#ifdef DEBUG
  printf("GS_DatasetWrite: 11\n");
#endif
/* -------------------------------------------------------------------- */
/* Deal with datatype conversion                                        */
/* convert passed buffer to local_buffer                                */
/* -------------------------------------------------------------------- */
  size=1;
  for(i=0;i<dataset_ndims;i++) size *= sizes[i];
  if(!GS_DataConversion(datatype, buffer,
                        dataset_datatype,local_buffer,
                        size)){
    bassigncstr(error_string,"GS_DatasetWrite: Data type-conversion failed.");
    GFree(local_buffer);
    H5Sclose(mem_space_id);
    H5Sclose(file_space_id);
    GS_HDFDatatypeClose(hdf_datatype);
  }// endif

#ifdef DEBUG3
  printf("GS_DatasetWrite: 12\n");
  printf("                 buffer=%d,%d,%d,%d\n",
         ((u_int8_t *)buffer)[0],
         ((u_int8_t *)buffer)[1],
         ((u_int8_t *)buffer)[2],
         ((u_int8_t *)buffer)[3]);
  printf("                 local_buffer=%d,%d,%d,%d\n",
         ((u_int8_t *)local_buffer)[0],
         ((u_int8_t *)local_buffer)[1],
         ((u_int8_t *)local_buffer)[2],
         ((u_int8_t *)local_buffer)[3]);
#endif
/* -------------------------------------------------------------------- */
/* Do the write from our local buffer                                   */
/* -------------------------------------------------------------------- */
  if(H5Dwrite(dataset_id, hdf_datatype, mem_space_id,
             file_space_id, H5P_DEFAULT, local_buffer ) <0){
    bassigncstr(error_string,"GS_DatasetWrite: Read error.");
    GFree(local_buffer);
    H5Sclose(mem_space_id);
    H5Sclose(file_space_id);
    GS_HDFDatatypeClose(hdf_datatype);
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_DatasetWrite: 13\n");
#endif
/* -------------------------------------------------------------------- */
/* Clean up                                                             */
/* -------------------------------------------------------------------- */
  GFree(local_buffer);
  if(GS_HDFDatatypeClose(hdf_datatype)<0){
    bassigncstr(error_string,"GS_DatasetWrite: failure closing hdf datatype.");    
    H5Sclose(mem_space_id);
    H5Sclose(file_space_id);
    return FALSE;
  }//endif

  if(H5Sclose(mem_space_id) < 0){
    bassigncstr(error_string,"GS_DatasetWrite: failure closing mem-space.");    
    H5Sclose(file_space_id);
    return FALSE;
  }//endif

  if(H5Sclose(file_space_id) < 0){
    bassigncstr(error_string,"GS_DatasetWrite: failure closing file-space.");    
    return FALSE;
  }//endif

  

#ifdef DEBUG
  printf("GS_DatasetWrite: 14\n");
#endif
  return TRUE;

}
