/* IFileSizeHDF.c

   by Leland Pierce, Feb 1,2014
  
*/

/** \brief IFileSizeHDF gets the #bytes in an internal file
\sa IFileOpen, IFileClose
\param[in] ifilep The struct of the open internal file.
\returns
On success, the number of characters in the internal file is returned.
-1 is returned on error.
\par Example
Let's assume that one already has an GeoSci file, and one wants
to know the size of an internal file named "/somename".
\code
hid_t file_id;
IFILE *ifilep;
long int numbytes;

ifilep = IFileOpen(file_id,"/somename","w");
if(ifilep < 0) {
   printf("IFileOpen failed on /somename\n");
}

numbytes = IFileSizeHDF(ifilep);
if(numbytes < 0) {
   printf("failed to get file size\n");
}
\endcode
\par Details
There is no equivalent C function to get the size of an open file.
This is distinct from IFileSize() because this function asks the HDF5
system how big the dataset is. This is useful when we need to 
initialize the size when we open an existing IFILE.

*/

#include "ifile.h"

/*********************************************************************/
long int IFileSizeHDF( IFILE *ifilep )
{
  hid_t dataspace;
  long int file_size;
  hid_t dataset_id;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

  dataset_id = ifilep->ifile_id;

/* -------------------------------------------------------------------- */
/* Make sure dataset is the right type                                  */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsIFileByID(dataset_id)){
    bassignformat(error_string,"IFileReadAccessHDF: dataset is not an internal file.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Get file-size                                                        */
/* -------------------------------------------------------------------- */
  dataspace = H5Dget_space(dataset_id);
  if(dataspace <0){
    bassignformat(error_string,"IFileSize: Can't get internal file size.");
    return ERROR;
  }// endif
  file_size  = (long int) H5Sget_simple_extent_npoints(dataspace);
  H5Sclose(dataspace);

  return file_size;

}
