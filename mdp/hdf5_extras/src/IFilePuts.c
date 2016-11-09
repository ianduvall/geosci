/* IFilePuts.c

   by Leland Pierce, Jan 22, 2014.
  
*/

/** \brief IFilePuts writes a string to an internal file

\sa IFileOpen, IFileClose

\param[in] ifilep  The struct of the open internal file.
\param[in] string The character string to write.

\returns
On success, TRUE is returned.
If some writing error happens, the function returns FALSE, 
and sets its error indicator (IFileError).

\par Example
Let's assume that one already has an GeoSci file, and one wants
to write the character string "already" to an internal file named 
"/somename".
\code
hid_t file_id;
IFILE *ifilep;

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifilep) {
   printf("IFileOpen failed on /somename\n");
}

if(!IFilePuts(ifilep, "already")){
   printf("write failed\n");
}
\endcode

\par Details
This function is meant to emulate as close as possible the standard
C \c fputs() function.
*/

#include "ifile.h"

//#define DEBUG

/*******************************************************************************/
int IFilePuts( IFILE *ifilep, const char *string )
{
  char *status_str;
  hid_t file_space_id;
  hid_t mem_space_id;
  hsize_t mem_dims[1];
  hsize_t mem_offsets[1];
  hsize_t start[1], step[1], count[1];
  char return_value;
  hid_t datatype;

  long int L;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("IFilePuts: at 1, ifilep=%p, string=|%s| \n",ifilep,string);
  printf("IFilePuts: at 1, ifilep->access=%d\n",ifilep->access);
#endif


/* -------------------------------------------------------------------- */
/* Get file access mode                                                 */
/* -------------------------------------------------------------------- */
  switch(ifilep->access){
  case IFILE_CLOSED:
    bassignformat(error_string,"IFilePuts: file is not open.");
    return ERROR;
  case IFILE_R:
    bassignformat(error_string,"IFilePuts: file is not in write mode.");
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
    bassignformat(error_string,"IFilePuts: unknown file access code.");
    return ERROR;    
  }// end-switch


#ifdef DEBUG
  printf("IFilePuts: file_position=%ld, file_size=%ld\n",
         ifilep->file_position, ifilep->size);
#endif


  L =strlen(string);

/* -------------------------------------------------------------------- */
/* EOF check                                                            */
/* -------------------------------------------------------------------- */
  if( ifilep->file_position >= ifilep->size ){

#ifdef DEBUG
    printf("IFilePuts: calling IFileAllocate(%d, %d)\n",
           ifilep->size,ifilep->file_position - ifilep->size + L);
#endif

    /* ----------------------------------------------------------- */
    /* Add enough bytes to the end of the file                     */
    /* ----------------------------------------------------------- */
    if(!IFileAllocate(ifilep,ifilep->size,
                      ifilep->file_position - ifilep->size + L)){
      bassignformat(error_string,"IFilePuts: Can't extend the file length.");
      return ERROR;
    }//endif

#ifdef DEBUG
    printf("IFilePuts: L=%ld, extended by %ld bytes \n",L,ifilep->file_position-ifilep->size+L);
#endif

  }// endif


#ifdef DEBUG
  printf("IFilePuts: 2, file_position=%d\n",ifilep->file_position);
#endif


/* -------------------------------------------------------------------- */
/* Create the file space specification                                  */
/* -------------------------------------------------------------------- */
  file_space_id = H5Dget_space( ifilep->ifile_id );
  if( file_space_id < 0 ){
    bassignformat(error_string,"IFilePuts: Can't get space of dataset.");
    return ERROR;
  }// endif
  // make it have the appropriate piece of the dataset
  start[0]=ifilep->file_position;
  step[0]=1;
  count[0]=L;
  if(H5Sselect_hyperslab( file_space_id, H5S_SELECT_SET, 
                          start,step, count, NULL) < 0 ){
    H5Sclose(file_space_id);
    bassignformat(error_string,"IFilePuts: Can't set hyperslab for dataset space");
    return ERROR;
  }// endif

#ifdef DEBUG
  printf("IFilePuts: 3\n");
#endif


/* -------------------------------------------------------------------- */
/* Create the memory space specification                                */
/* -------------------------------------------------------------------- */
  mem_dims[0] = L;
  mem_space_id = H5Screate_simple( 1, mem_dims, NULL );
  if(mem_space_id < 0 ){
    H5Sclose(file_space_id);
    bassignformat(error_string,"IFilePuts: Failed to create mem-space spec size");
    return ERROR;
  }// endif


#ifdef DEBUG
  printf("IFilePuts: 4\n");
#endif

/* -------------------------------------------------------------------- */
/* Write the data                                                       */
/* -------------------------------------------------------------------- */
  datatype = GS_GetHDF5Type(GS_DATATYPE_UI8);
  if(H5Dwrite(ifilep->ifile_id, datatype, mem_space_id,
             file_space_id, H5P_DEFAULT, (void *)string ) < 0){
    H5Sclose(file_space_id);
    H5Sclose(mem_space_id);
    bassignformat(error_string,"IFilePuts: Write error.");

    /* ---------------------------------------------------------- */
    /* Set last operation status metadata                         */
    /* ---------------------------------------------------------- */
    ifilep->last_operation_status = IFILE_FAILURE;
    bassigncstr(ifilep->last_operation_status_message,
           "IFilePuts write error");

    return ERROR;
  }// endif


#ifdef DEBUG
  printf("IFilePuts: 5\n");
#endif


/* -------------------------------------------------------------------- */
/* Clean up                                                             */
/* -------------------------------------------------------------------- */
  H5Sclose(file_space_id);
  H5Sclose(mem_space_id);
  
/* -------------------------------------------------------------------- */
/* Increment file_position                                              */
/* -------------------------------------------------------------------- */
  ifilep->file_position += L;

/* -------------------------------------------------------------------- */
/* Clear the last operation status metadata                             */
/* -------------------------------------------------------------------- */
  ifilep->last_operation_status = IFILE_SUCCESS;
  bassigncstr(ifilep->last_operation_status_message,
         "Successfully ran IFilePuts()");

/* -------------------------------------------------------------------- */
/* Clear the readPastEOF metadata                                       */
/* -------------------------------------------------------------------- */
  ifilep->readPastEOF = FALSE;

  return TRUE;
 
}
