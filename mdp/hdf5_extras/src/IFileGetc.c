/* IFileGetc.c

   by Leland Pierce, Jan 22, 2014.
  
*/

/** \brief IFileGetc reads a character from an internal file

\sa IFileOpen, IFileClose

\param[in] ifilep
The struct of the open internal file.

\returns
On success, the character read is returned (promoted to an int value).
If the file position indicator was at the end-of-file, the function 
returns ERROR and sets the end-of-file indicator (IFileEOF).
If some other reading error happens, the function also returns ERROR, 
but sets its error indicator (IFileError) instead.

\par Example
Let's assume that one already has an GeoSci file, and one wants
to read the next character from an internal file named "/somename".
\code
hid_t file_id;
IFILE *ifilep;
int the_int_char;
char the_char;

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifilep) {
   printf("IFileOpen failed on /somename\n");
}

the_int_char = IFileGetc(ifilep);
if(the_int_char < 0) {
   printf("read failed\n");
}

the_char = (char *)the_int_char;

...use the_char here....
\endcode

\par Details
This function is meant to emulate as close as possible the standard
C fgetc() function.
*/

#include "ifile.h"


/*******************************************************************************/
int IFileGetc( IFILE *ifilep )
{
  char *status_str;
  hid_t file_space_id;
  hid_t mem_space_id;
  hsize_t mem_dims[1];
  hsize_t mem_offsets[1];
  hsize_t start[1], step[1], count[1];
  void *buf;
  char return_value;
  hid_t datatype;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

/* -------------------------------------------------------------------- */
/* EOF Error-check                                                      */
/* -------------------------------------------------------------------- */
  if( ifilep->file_position >= ifilep->size ){
    /* ----------------------------------------------------------- */
    /* Set EOF metadata                                            */
    /* ----------------------------------------------------------- */
    ifilep->readPastEOF = TRUE;
    /* ---------------------------------------------------------- */
    /* Set last_operation_status_message metadata                 */
    /* ---------------------------------------------------------- */
    bassigncstr(ifilep->last_operation_status_message, "IFileGetc hit EOF");

    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Create the file space specification                                  */
/* -------------------------------------------------------------------- */
  file_space_id = H5Dget_space( ifilep->ifile_id );
  if ( file_space_id < 0 ){
    bassignformat(error_string,"IFileGetc: Can't get space of dataset.");
    return ERROR;
  }// endif
  // make it have the appropriate piece of the dataset
  start[0]=ifilep->file_position;
  step[0]=1;
  count[0]=1;
  if(H5Sselect_hyperslab( file_space_id, H5S_SELECT_SET, 
                          start,step, count, NULL) < 0 ){
    H5Sclose(file_space_id);
    bassignformat(error_string,"IFileGetc: Can't set hyperslab for dataset space");
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Create the memory space specification                                */
/* -------------------------------------------------------------------- */
  mem_dims[0] = 1;
  mem_space_id = H5Screate_simple( 1, mem_dims, NULL );
  if(mem_space_id < 0 ){
    H5Sclose(file_space_id);
    bassignformat(error_string,"IFileGetc: Failed to create mem-space spec size");
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Create space for the read                                            */
/* -------------------------------------------------------------------- */
  buf = (char *)GMalloc(10*sizeof(char));
  if(!buf) {
    H5Sclose(file_space_id);
    H5Sclose(mem_space_id);
    bassignformat(error_string,"IFileGetc: HMalloc failure on buf");
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Read the data                                                        */
/* -------------------------------------------------------------------- */
  datatype = GS_GetHDF5Type(GS_DATATYPE_UI8);
  if(H5Dread(ifilep->ifile_id, datatype, mem_space_id,
             file_space_id, H5P_DEFAULT, buf ) <0){
    H5Sclose(file_space_id);
    H5Sclose(mem_space_id);
    GFree(buf);
    bassignformat(error_string,"IFileGetc: Read error.");

    /* ---------------------------------------------------------- */
    /* Set last operation status metadata                         */
    /* ---------------------------------------------------------- */
    ifilep->last_operation_status = IFILE_FAILURE;
    bassigncstr(ifilep->last_operation_status_message,"IFileGetc read error");

    return ERROR;
  }// endif
  return_value = ((char *)buf)[0];

/* -------------------------------------------------------------------- */
/* Clean up                                                             */
/* -------------------------------------------------------------------- */
  H5Sclose(file_space_id);
  H5Sclose(mem_space_id);
  GFree(buf);
  buf=NULL;
  
/* -------------------------------------------------------------------- */
/* Increment file_position                                              */
/* -------------------------------------------------------------------- */
  ifilep->file_position++;

/* -------------------------------------------------------------------- */
/* Clear the last operation status metadata                             */
/* -------------------------------------------------------------------- */
  ifilep->last_operation_status = IFILE_SUCCESS;
  bassigncstr(ifilep->last_operation_status_message,
         "Successfully ran IFileGetc()");

/* -------------------------------------------------------------------- */
/* Clear the readPastEOF metadata                                       */
/* -------------------------------------------------------------------- */
  ifilep->readPastEOF = FALSE;

  return return_value;
}
