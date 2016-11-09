/* GS_DatasetRead.c

   by Leland Pierce, Aug 9, 2014

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"



/** \brief GS_DatasetRead reads data from a dataset


\see GS_DatasetWrite(), GS_DatasetOpen(), GS_DatasetCreate()

\param[in] id
          A handle for the dataset. Perhaps from GS_DatasetOpen().
\param[in] offsets
          Must supply an offset to the first data element to read 
          for each dimension. 
          Zero offset is used for reading starting with the first data-element.
\param[in] sizes
          Must supply how many data elements to read 
          for each dimension.
          Zero offset is used for reading starting with the first data-element.
          Using the offset and size for each dimension allows the
          specification of rectangular windows in n-dimensional space.
          For example, in 2-dimensions the window is:<br>
          xoffset = offset[0], yoffset= offset[1]<br>
          xsize = sizes[0], ysize = sizes[1]
\param[in] datatype
          The datatype of the buffer. 
          All the data that is read is converted to this datatype,
          and then returned to the caller in \c buffer.
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
          data that is being read.
          
\returns
\c TRUE on sucess, \c FALSE on failure.

\par Example
We have a 2D dataset named "d4",
Read in from data elements 0 to 500 in dimension 1, 
and from data elements 100 to 700 in dimension 2.
The data is stored as integers, but we'd like to read them as 
doubles.
\code
hid_t group_id;
hid_t dataset_id;
double buffer[500*600];
int offsets[10], sizes[10];
bstring name = bfromcstr("d4");
dataset_id = GS_DatasetOpen(group_id,name);
if(dataset_id < 0){
  printf("Failed to open dataset\n");
  bdestroy(name);
  return;
}
bdestroy(name);
offsets[0]=0;
offsets[1]=100;
sizes[0]=500;
sizes[1]=600;
if(!GS_DatasetRead(dataset_id, offsets, sizes, 
                   GS_DATATYPE_R64, (void *)buffer)) {
  printf("Failed to read from dataset.\n");
}
\endcode

\par Details
YET: data type conversions....<br>
YET: deal with interleaving spec.....<br>
YET: indexing...
*/

//#define DEBUG

int GS_DatasetRead(hid_t dataset_id, const long int *offsets,
                     const long int *sizes, int datatype, void *buffer)
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
  printf("GS_DatasetRead: start\n");
#endif
/* -------------------------------------------------------------------- */
/* Check if its a dataset id                                            */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsDatasetByID(dataset_id)){
    bassigncstr(error_string,"GS_DatasetRead: Not a dataset ID.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_DatasetRead: 1\n");
#endif
/* -------------------------------------------------------------------- */
/* Get dataset dimensions                                               */
/* -------------------------------------------------------------------- */
  dataset_ndims = GS_DatasetGetDimensionsByID(dataset_id, &dataset_dims);
  if(dataset_ndims <= 0) {
    bassigncstr(error_string,"GS_DatasetRead: Dataset has no dimensions.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_DatasetRead: 2\n");
#endif
/* -------------------------------------------------------------------- */
/* Check validity of offsets                                            */
/* -------------------------------------------------------------------- */
  for(i=0;i<dataset_ndims;i++){
    if(offsets[i] < 0 || offsets[i] > dataset_dims[i]){
      bassigncstr(error_string,"GS_DatasetRead: Requested offsets invalid.");
      return FALSE;
    }// endif
  }// endfor

#ifdef DEBUG
  printf("GS_DatasetRead: 3\n");
#endif
/* -------------------------------------------------------------------- */
/* Check validity of sizes                                              */
/* -------------------------------------------------------------------- */
  for(i=0;i<dataset_ndims;i++){
    if(sizes[i] < 0 || offsets[i]+sizes[i] > dataset_dims[i]){
      bassigncstr(error_string,"GS_DatasetRead: Requested sizes invalid.");
      return FALSE;
    }// endif
  }// endfor

#ifdef DEBUG
  printf("GS_DatasetRead: 4\n");
#endif
/* -------------------------------------------------------------------- */
/* Check validity of datatype                                           */
/* -------------------------------------------------------------------- */
  if(GS_ConvertToHDFDatatype(datatype) < 0 ) {
    bassigncstr(error_string,"GS_DatasetRead: Invalid datatype.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_DatasetRead: 5\n");
#endif
/* -------------------------------------------------------------------- */
/* Get datatype of dataset                                              */
/* -------------------------------------------------------------------- */
  dataset_datatype=GS_DatasetGetDatatypeByID(dataset_id);
  if(dataset_datatype ==0){
    bassigncstr(error_string,"GS_DatasetRead: Invalid dataset datatype.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_DatasetRead: 6\n");
#endif
/* -------------------------------------------------------------------- */
/* Get HDF datatype of dataset                                          */
/* -------------------------------------------------------------------- */
  hdf_datatype = GS_ConvertToHDFDatatype(dataset_datatype);
  if(hdf_datatype < 0 ) {
    bassigncstr(error_string,"GS_DatasetRead: Invalid dataset_datatype.");
    return FALSE;
  }// endif


#ifdef DEBUG
  printf("GS_DatasetRead: 7\n");
#endif
/* -------------------------------------------------------------------- */
/* Create a buffer that can hold all the data, in the                   */
/* datatype of the file:                                                */
/* -------------------------------------------------------------------- */
  size = GS_RasterTypeNumBytes(dataset_datatype);
  if(size==0){
    bassigncstr(error_string,"GS_DatasetRead: Invalid datatype size.");
    GS_HDFDatatypeClose(hdf_datatype);
    return FALSE;
  }//endif
  for(i=0;i<dataset_ndims;i++) size *= sizes[i];
  local_buffer = (unsigned char *)GMalloc(size*sizeof(unsigned char));
  if(!local_buffer){
    bassigncstr(error_string,"GS_DatasetRead: Malloc error.");
    GS_HDFDatatypeClose(hdf_datatype);
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_DatasetRead: 8\n");
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
  for(i=0;i<dataset_ndims;i++) hsizes[i] = sizes[i];
  mem_space_id = H5Screate_simple( dataset_ndims, hsizes, NULL );
  if(mem_space_id < 0 ){
    bassigncstr(error_string,"GS_DatasetRead: Failed to create mem-space spec.");
    GFree(hsizes);
    GFree(local_buffer);
    GS_HDFDatatypeClose(hdf_datatype);
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_DatasetRead: 9\n");
#endif
/* -------------------------------------------------------------------- */
/* Create Space to describe subset of file-dataset                      */
/* -------------------------------------------------------------------- */
  file_space_id = H5Dget_space(dataset_id);
  if ( file_space_id < 0 ){
    bassigncstr(error_string,"GS_DatasetRead: Can't get space of dataset.");
    GFree(hsizes);
    GFree(local_buffer);
    GS_HDFDatatypeClose(hdf_datatype);
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_DatasetRead: 10\n");
#endif
/* -------------------------------------------------------------------- */
/* Select the appropriate hyperslab of the file-dataset                 */
/* -------------------------------------------------------------------- */
  step = (hsize_t *)GMalloc(dataset_ndims*sizeof(hsize_t));
  if(!step){
    bassigncstr(error_string,"GS_DatasetRead: HMalloc error on step.");
    GFree(hsizes);
    GFree(local_buffer);
    GS_HDFDatatypeClose(hdf_datatype);
    return FALSE;
  }// endif
  for(i=0;i<dataset_ndims;i++) step[i] =1;
  hoffsets = (hsize_t *)GMalloc(dataset_ndims*sizeof(hsize_t));
  if(!hoffsets){
    bassigncstr(error_string,"GS_DatasetWrite: HMalloc error on hoffsets.");
    GFree(hsizes);
    GFree(step);
    GFree(local_buffer);
    H5Sclose(mem_space_id);
    H5Sclose(file_space_id);
    GS_HDFDatatypeClose(hdf_datatype);
    return FALSE;
  }// endif
  for(i=0;i<dataset_ndims;i++) hoffsets[i] =offsets[i];
  if(H5Sselect_hyperslab( file_space_id, H5S_SELECT_SET, 
                          hoffsets,step, hsizes, NULL) < 0 ){
    bassigncstr(error_string,"GS_DatasetRead: Can't set hyperslab for dataset space");
    GFree(hsizes);
    GFree(hoffsets);
    GFree(step);
    GFree(local_buffer);
    GS_HDFDatatypeClose(hdf_datatype);
    return FALSE;
  }// endif
  GFree(step);
  GFree(hsizes);
  GFree(hoffsets);
   
#ifdef DEBUG
  printf("GS_DatasetRead: 11\n");
#endif
/* -------------------------------------------------------------------- */
/* Do the read to our local buffer                                      */
/* -------------------------------------------------------------------- */
  if(H5Dread(dataset_id, hdf_datatype, mem_space_id,
             file_space_id, H5P_DEFAULT, local_buffer ) <0){
    bassigncstr(error_string,"GS_DatasetRead: Read error.");
    GFree(local_buffer);
    GS_HDFDatatypeClose(hdf_datatype);
    return FALSE;
  }// endif
  
#ifdef DEBUG3
  printf("GS_DatasetRead: 12\n");
  printf("local_buffer: %d, %d, %d, %d, %d, %d\n",
         ((u_int8_t *)local_buffer)[0],
         ((u_int8_t *)local_buffer)[1],
         ((u_int8_t *)local_buffer)[2],
         ((u_int8_t *)local_buffer)[3],
         ((u_int8_t *)local_buffer)[4],
         ((u_int8_t *)local_buffer)[5]);
#endif
/* -------------------------------------------------------------------- */
/* Clean up                                                             */
/* -------------------------------------------------------------------- */
  if(GS_HDFDatatypeClose(hdf_datatype)<0){
    bassigncstr(error_string,"GS_DatasetRead: failure closing hdf datatype.");
    GFree(local_buffer);
    return FALSE;
  }//endif

#ifdef DEBUG
  printf("GS_DatasetRead: 13\n");
#endif
/* -------------------------------------------------------------------- */
/* Deal with datatype conversion                                        */
/* -------------------------------------------------------------------- */
  size=1;
  for(i=0;i<dataset_ndims;i++) size *= sizes[i];
  if(!GS_DataConversion(dataset_datatype,local_buffer,
                        datatype, buffer, size)){
    bassigncstr(error_string,"GS_DatasetRead: Data type-conversion failed.");
    GFree(local_buffer);
  }// endif
  GFree(local_buffer);

#ifdef DEBUG
  printf("GS_DatasetRead: 14\n");
#endif
  return TRUE;

}
