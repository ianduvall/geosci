/* IFilePutc.c

   by Leland Pierce, Jan 22, 2014.
  
*/

/** \brief IFilePutc writes a single character to an internal file
\sa IFileOpen, IFileClose

\param[in] ifilep 
The struct of the open internal file.
\param[in] character
The character to write.

\returns
\parblock
On success, TRUE is returned.

If some writing error happens, the function returns FALSE, 
and sets its error indicator (IFileError).
\endparblock

\par Exmaple
Let's assume that one already has an GeoSci file, and one wants
to write the character 'a' to an internal file named "/somename".
\code
hid_t file_id;
IFILE *ifilep;
int the_int_char;
char the_char;

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifilep) {
   printf("IFileOpen failed on /somename\n");
}

the_char = 'a';
if(!IFilePutc(ifilep, the_char)){
   printf("write failed\n");
}
\endcode

\par Details
This function is meant to emulate as close as possible the standard
C \c fputc() function.
*/

#include "ifile.h"


/*******************************************************************************/
int IFilePutc( IFILE *ifilep, char character )
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

  int ifile_access_mode;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");


/* -------------------------------------------------------------------- */
/* Get file access mode                                                 */
/* -------------------------------------------------------------------- */
  switch(ifilep->access){
  case IFILE_CLOSED:
    bassignformat(error_string,"IFilePutc: file is not open.");
    return ERROR;
  case IFILE_R:
    bassignformat(error_string,"IFilePutc: file is not in write mode.");
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
    bassignformat(error_string,"IFilePutc: unknown file access code.");
    return ERROR;    
  }// end-switch


/* -------------------------------------------------------------------- */
/* EOF check                                                            */
/* -------------------------------------------------------------------- */
  if( ifilep->file_position >= ifilep->size ){
    /* ----------------------------------------------------------- */
    /* Add a byte to the end of the file                           */
    /* ----------------------------------------------------------- */
    if(!IFileAllocate(ifilep,ifilep->size,
                      ifilep->file_position - ifilep->size +1L)){
      bassignformat(error_string,"IFilePutc: Can't extend the file length.");
      return ERROR;
    }//endif
  }// endif

/* -------------------------------------------------------------------- */
/* Create the file space specification                                  */
/* -------------------------------------------------------------------- */
  file_space_id = H5Dget_space( ifilep->ifile_id );
  if ( file_space_id < 0 ){
    bassignformat(error_string,"IFilePutc: Can't get space of dataset.");
    return ERROR;
  }// endif
  // make it have the appropriate piece of the dataset
  start[0]=ifilep->file_position;
  step[0]=1;
  count[0]=1;
  if(H5Sselect_hyperslab( file_space_id, H5S_SELECT_SET, 
                          start,step, count, NULL) < 0 ){
    H5Sclose(file_space_id);
    bassignformat(error_string,"IFilePutc: Can't set hyperslab for dataset space");
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Create the memory space specification                                */
/* -------------------------------------------------------------------- */
  mem_dims[0] = 1;
  mem_space_id = H5Screate_simple( 1, mem_dims, NULL );
  if(mem_space_id < 0 ){
    H5Sclose(file_space_id);
    bassignformat(error_string,"IFilePutc: Failed to create mem-space spec size");
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Create space for the write                                           */
/* -------------------------------------------------------------------- */
  buf = (char *)GMalloc(10*sizeof(char));
  if(!buf) {
    H5Sclose(file_space_id);
    H5Sclose(mem_space_id);
    bassignformat(error_string,"IFilePutc: GMalloc failure on buf");
    return ERROR;
  }// endif
  ((char *)buf)[0]=character;

/* -------------------------------------------------------------------- */
/* Write the data                                                       */
/* -------------------------------------------------------------------- */
  datatype = GS_GetHDF5Type(GS_DATATYPE_UI8);
  if(H5Dwrite(ifilep->ifile_id, datatype, mem_space_id,
             file_space_id, H5P_DEFAULT, (void *)buf ) < 0){
    H5Sclose(file_space_id);
    H5Sclose(mem_space_id);
    GFree(buf);
    bassignformat(error_string,"IFilePutc: Write error.");

    /* ---------------------------------------------------------- */
    /* Set last operation status metadata                         */
    /* ---------------------------------------------------------- */
    ifilep->last_operation_status = IFILE_FAILURE;
    bassigncstr(ifilep->last_operation_status_message,
           "IFilePutc write error");

    return ERROR;
  }// endif

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
         "Successfully ran IFilePutc()");
  
/* -------------------------------------------------------------------- */
/* Clear the readPastEOF metadata                                       */
/* -------------------------------------------------------------------- */
  ifilep->readPastEOF=FALSE;

  return TRUE;
 
}
