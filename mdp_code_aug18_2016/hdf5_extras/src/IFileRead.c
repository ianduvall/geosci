/* IFileRead.c

   by Leland Pierce, Jan 22, 2014.
  
*/

/** \brief IFileRead  reads the requested number of bytes from  
an internal file in a geosci file.

\sa IFileOpen, IFileClose

\param[in] ifilep  The struct of the open internal file.
\param[in,out] buf   Pointer to an array where the bytes read 
are copied to. This must have at least
size*count bytes of storage.
Using a void pointer here allows the user to
pass a buf variable of any type.
\param[in] size Size in bytes of each element to be read.
\param[in] count  Number of elements to read. Each element
has a size of "size" bytes.

\returns
\parblock
The total number of elements successfully read is returned.

If this number differs from the count parameter, either a 
reading error occurred or the end-of-file was reached while 
reading. In both cases, the proper indicator is set, which 
can be checked with IFileError and IFileEOF, respectively.

If either \c size or \c count is less than or equal to zero, the 
function returns zero and both the internal-file state and 
the content pointed to by \c buf remain unchanged.
\endparblock

\par Example
Let's assume that one already has an GeoSci file, and one wants
to read the next 10 characters from an internal file named "/somename".
\code
hid_t file_id;
IFILE *ifilep;
char *buf;

buf = (char *)GMalloc(10*sizeof(char));
if(!buf){
   printf("GMalloc error\n");
   exit(-1);
}

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifilep) {
   printf("IFileOpen failed on /somename\n");
}

if(IFileRead(ifilep, buf, sizeof(char),10) != 10){
   printf("read failed\n");
}

...use buf here....
\endcode

\par Details
\parblock
This function is meant to emulate as close as possible the standard
C \c fread() function.

The types of size and count are purposely \c int and not \c size_t
because if the user accidentally passes a negative value the
value becomes a very big number when casted to an unsigned integer,
which is what \c size_t is.
In order to detect this error, \c int is used instead.
\endparblock

*/

#include "ifile.h"

//#define DEBUG

/*******************************************************************************/
size_t IFileRead( IFILE *ifilep, void *buf, int size, int count)
{
  char *status_str;
  hid_t file_space_id;
  hid_t mem_space_id;
  hsize_t mem_dims[1];
  hsize_t mem_offsets[1];
  hsize_t start[1], step[1], thecount[1];
  long int file_position;
  hid_t dataspace;
  char *internal_buf;
  char return_value;
  hid_t datatype;
  int not_done;
  int read_error, at_eof;

  //int n_elements_read;
  register int i;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("IFileRead: start\n");
#endif

/* -------------------------------------------------------------------- */
/* Check size and count                                                 */
/* -------------------------------------------------------------------- */
  if(size <= 0){
    bassignformat(error_string,"IFileRead: size must be >0. size=%d.",size);
    return 0;
  }// endif
  if(count <= 0){
    bassignformat(error_string,"IFileRead: count must be >0. count=%d.",count);
    return 0;
  }// endif

#ifdef DEBUG
  printf("IFileRead: 1\n");
#endif

/* -------------------------------------------------------------------- */
/* EOF Error-check                                                      */
/* -------------------------------------------------------------------- */
  if( ifilep->file_position +size*count >= ifilep->size ){

    at_eof = TRUE;
    /* ----------------------------------------------------------- */
    /* Set EOF metadata                                            */
    /* ----------------------------------------------------------- */
    ifilep->readPastEOF = TRUE;
    /* ---------------------------------------------------------- */
    /* Set last_operation_status_message metadata                 */
    /* ---------------------------------------------------------- */
    bassigncstr(ifilep->last_operation_status_message,"IFileRead hit EOF");

    // update count to the number we can read before hitting eof:
    count = (ifilep->size - ifilep->file_position)/size;
    if(count <=0) return 0;
  } else {
    ifilep->readPastEOF = FALSE;
    ifilep->last_operation_status = IFILE_SUCCESS;
    bassigncstr(ifilep->last_operation_status_message, 
           "Successfully ran IFileRead()");
  }// endif

#ifdef DEBUG
  printf("IFileRead: 2\n");
#endif

/* -------------------------------------------------------------------- */
/* Create the file space specification                                  */
/* -------------------------------------------------------------------- */
  file_space_id = H5Dget_space( ifilep->ifile_id );
  if ( file_space_id < 0 ){
    bassignformat(error_string,"IFileRead: Can't get space of dataset.");
    return 0;
  }// endif
  // make it have the appropriate piece of the dataset
  start[0]=ifilep->file_position;
  step[0]=1;
  thecount[0]=size*count;
  if(H5Sselect_hyperslab( file_space_id, H5S_SELECT_SET, 
                          start,step, thecount, NULL) < 0 ){
    H5Sclose(file_space_id);
    bassignformat(error_string,"IFileRead: Can't set hyperslab for dataset space");
    return 0;
  }// endif

#ifdef DEBUG
  printf("IFileRead: 3\n");
#endif

/* -------------------------------------------------------------------- */
/* Create the memory space specification                                */
/* -------------------------------------------------------------------- */
  mem_dims[0] = thecount[0];
  mem_space_id = H5Screate_simple( 1, mem_dims, NULL );
  if(mem_space_id < 0 ){
    H5Sclose(file_space_id);
    bassignformat(error_string,"IFileRead: Failed to create mem-space spec size");
    return 0;
  }// endif

#ifdef DEBUG
  printf("IFileRead: 4\n");
#endif

/* -------------------------------------------------------------------- */
/* Read the data                                                        */
/* -------------------------------------------------------------------- */
  datatype = GS_GetHDF5Type(GS_DATATYPE_UI8);

  if(H5Dread(ifilep->ifile_id, datatype, mem_space_id,
             file_space_id, H5P_DEFAULT, (void *)buf ) < 0){
    H5Sclose(mem_space_id);
    H5Sclose(file_space_id);
    bassignformat(error_string,"IFileRead: Read error.");

    /* ---------------------------------------------------------- */
    /* Set last operation status metadata                         */
    /* ---------------------------------------------------------- */
    ifilep->last_operation_status = IFILE_FAILURE;
    bassigncstr(ifilep->last_operation_status_message, 
           "IFileRead() had a read error");

    return 0;
  }// endif

#ifdef DEBUG
  printf("IFileRead: 5\n");
#endif

/* -------------------------------------------------------------------- */
/* Increment file_position                                              */
/* -------------------------------------------------------------------- */
  ifilep->file_position += thecount[0];

#ifdef DEBUG
  printf("IFileRead: 6\n");
#endif

/* -------------------------------------------------------------------- */
/* Clean up                                                             */
/* -------------------------------------------------------------------- */
  H5Sclose(mem_space_id);
  H5Sclose(file_space_id);

/* -------------------------------------------------------------------- */
/* Return the number of elements read                                   */
/* -------------------------------------------------------------------- */
  return thecount[0];
 
}// end-function: IFileRead()





