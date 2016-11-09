/* IFileWrite.c

   by Leland Pierce, Jan 24, 2014.
  
*/

#include "ifile.h"

int write_an_element(IFILE *ifilep, hid_t datatype, hid_t mem_space_id,
                    hid_t file_space_id, char *buf);


/** \brief IFileWrite writes bytes to an internal file
\sa IFileOpen, IFileClose
\param[in] ifilep The struct of the open internal file.
\param[in,out] buf Pointer to an array where the bytes to write
are read from. This must have at least
size*count bytes of storage.
Using a void pointer here allows the user to
pass a \c buf variable of any type.
\param[in] size Size in bytes of each element to be written.
\param[in] count Number of elements to write. Each element
            has a size of "size" bytes.
\returns
\parblock
The total number of elements successfully written is returned.

If this number differs from the count parameter, a 
writing error prevented the function from completing.
In this case, the error indicator (IFileError) will be set 
for the stream.

If either size or count is less than or equal to zero, the 
function returns zero and both the internal-file state and 
the content pointed to by \c buf remain unchanged.
\endparblock

\par Example
Let's assume that one already has an GeoSci file, and one wants
to write 10 ints in an int vector (starting at index 5)
to an internal file named "/somename".
\code
hid_t file_id;
IFILE *ifilep;
int intvec[100];

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifilep < 0) {
   printf("IFileOpen failed on /somename\n");
}

if(!IFileWrite(ifilep,intvec+5, sizeof(int),10L )){
   printf("write failed\n");
}
\endcode
\par Details
This function is meant to emulate as close as possible the standard
C \c fwrite() function.
*/

//#define DEBUG

/*******************************************************************************/
int IFileWrite( IFILE *ifilep, const void *buf, int size, int count)
{
  char *status_str;
  hid_t file_space_id;
  hid_t mem_space_id;
  hsize_t mem_dims[1];
  hsize_t mem_offsets[1];
  hsize_t start[1], step[1], thecount[1];
  char return_value;
  hid_t datatype;

  int ifile_access_mode;
  long int L;

  int not_done;
  int write_error;
  int at_eof;

  int n_elements_written;


  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("IFileWrite: 1\n");
#endif

/* -------------------------------------------------------------------- */
/* Check size and count                                                 */
/* -------------------------------------------------------------------- */
  if(size <= 0){
    bassignformat(error_string,"IFileWrite: size must be >0. size=%d.",size);
    return 0;
  }// endif
  if(count <= 0){
    bassignformat(error_string,"IFileWrite: count must be >0. count=%d.",count);
    return 0;
  }// endif


#ifdef DEBUG
  printf("IFileWrite: 2\n");
#endif

/* -------------------------------------------------------------------- */
/* Get file access mode                                                 */
/* -------------------------------------------------------------------- */
  switch(ifilep->access){
  case IFILE_CLOSED:
    bassignformat(error_string,"IFileWrite: file is not open.");
    return ERROR;
  case IFILE_R:
    bassignformat(error_string,"IFileWrite: file is not in write mode.");
    return ERROR;
  case IFILE_W:
  case IFILE_RP:
  case IFILE_WP:
    // for all of these modes, write at current location
    break;
  case IFILE_A:
  case IFILE_AP:
    // for append modes, writes must be at the end of the file:
    ifilep->file_position = ifilep->size;
    break;
  default:
    bassignformat(error_string,"IFileWrite: unknown file access code.");
    return ERROR;    
  }// end-switch

#ifdef DEBUG
  printf("IFileWrite: 3\n");
#endif


/* -------------------------------------------------------------------- */
/* EOF check                                                            */
/* -------------------------------------------------------------------- */
  L =size*count;
  if( ifilep->file_position+L >= ifilep->size ){
    /* ----------------------------------------------------------- */
    /* Add enough bytes to the end of the file                     */
    /* ----------------------------------------------------------- */
    L =size*count;
    if(!IFileAllocate(ifilep,ifilep->size,
                      ifilep->file_position - ifilep->size + L)){
      bassignformat(error_string,"IFileWrite: Can't extend the file length.");
      return ERROR;
    }//endif

  }// endif

#ifdef DEBUG
  printf("IFileWrite: 4\n");
#endif


/* -------------------------------------------------------------------- */
/* Create the file space specification                                  */
/* -------------------------------------------------------------------- */
  file_space_id = H5Dget_space( ifilep->ifile_id );
  if( file_space_id < 0 ){
    bassignformat(error_string,"IFileWrite: Can't get space of dataset.");
    return ERROR;
  }// endif
  // make it have the appropriate piece of the dataset
  start[0]=ifilep->file_position;
  step[0]=1;
  thecount[0]=L;
  if(H5Sselect_hyperslab( file_space_id, H5S_SELECT_SET, 
                          start,step, thecount, NULL) < 0 ){
    H5Sclose(file_space_id);
    bassignformat(error_string,"IFileWrite: Can't set hyperslab for dataset space");
    return ERROR;
  }// endif

#ifdef DEBUG
  printf("IFileWrite: 5\n");
#endif


/* -------------------------------------------------------------------- */
/* Create the memory space specification                                */
/* -------------------------------------------------------------------- */
  mem_dims[0] = L;
  mem_space_id = H5Screate_simple( 1, mem_dims, NULL );
  if(mem_space_id < 0 ){
    H5Sclose(file_space_id);
    bassignformat(error_string,"IFileWrite: Failed to create mem-space spec size");
    return ERROR;
  }// endif


#ifdef DEBUG
  printf("IFileWrite: 6\n");
#endif

/* -------------------------------------------------------------------- */
/* Write the data                                                       */
/* -------------------------------------------------------------------- */
  datatype = GS_GetHDF5Type(GS_DATATYPE_UI8);

  if(H5Dwrite(ifilep->ifile_id, datatype, mem_space_id,
             file_space_id, H5P_DEFAULT, (void *)buf ) < 0){
    H5Sclose(mem_space_id);
    H5Sclose(file_space_id);
    bassignformat(error_string,"IFileWrite: Write error.");

    /* ---------------------------------------------------------- */
    /* Set last operation status metadata                         */
    /* ---------------------------------------------------------- */
    ifilep->last_operation_status = IFILE_FAILURE;
    bassigncstr(ifilep->last_operation_status_message,
           "IFileWrite read error");

    return ERROR;
  } else {
    ifilep->last_operation_status = IFILE_SUCCESS;
    bassigncstr(ifilep->last_operation_status_message,
           "Successfully ran IFileWrite()");
  }// endif
      
#ifdef DEBUG
  printf("IFileWrite: 7\n");
#endif



/* -------------------------------------------------------------------- */
/* Increment file_position                                              */
/* -------------------------------------------------------------------- */
  ifilep->file_position += L;

#ifdef DEBUG
  printf("IFileWrite: 8\n");
#endif


/* -------------------------------------------------------------------- */
/* Clean up                                                             */
/* -------------------------------------------------------------------- */
  H5Sclose(file_space_id);
  H5Sclose(mem_space_id);

#ifdef DEBUG
  printf("IFileWrite: 9\n");
#endif


  
/* -------------------------------------------------------------------- */
/* Return the number of elements written                                */
/* -------------------------------------------------------------------- */
  return L;
 
}






