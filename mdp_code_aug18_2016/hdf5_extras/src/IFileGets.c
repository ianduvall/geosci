/* IFileGets.c

   by Leland Pierce, Jan 22, 2014.
  
*/

#include "ifile.h"

int read_a_character(IFILE *ifilep, hid_t datatype, hid_t mem_space_id,
                     hid_t file_space_id, char *buf);


/** \brief IFileGets reads characters from  an internal file
in a geosci file, and stores them as a string 
until the maximum number of characters have been read or 
either a newline or the end-of-file is reached, whichever 
happens first.

\sa IFileOpen, FileClose

\param[in] ifilep
The struct of the open internal file.
\param[in,out] str
Pointer to an array of chars where the string read is copied.
\param[in] num
Maximum number of characters to be copied into 
\c str (including the terminating null-character).

\returns
\parblock
On success, the function returns \c str.

If the end-of-file is encountered while attempting to read 
a character, the eof indicator is set (IFileEOF). If this happens 
before any characters could be read, the pointer returned is 
a null pointer (and the contents of \c str remain unchanged).

If a read error occurs, the error indicator (IFileError) is set 
and a null pointer is also returned (but the contents pointed-to
by \c str may have changed).
\endparblock

\par Example
Let's assume that one already has an GeoSci file, and one wants
to read the next line of characters (up to the next "\n") from an 
internal file named "/somename".
\code
hid_t file_id;
IFILE *ifilep;
char the_string[1000];

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifielp) {
   printf("IFileOpen failed on /somename\n");
}

the_string = IFileGets(ifilep, the_string, 1000);
if(!the_string) {
   printf("read failed\n");
}

...use the_string here....
\endcode

\par Details
This function is meant to emulate as close as possible the standard
 C fgets() function.
*/


//#define DEBUG

/*******************************************************************************/
char *IFileGets( IFILE *ifilep, char *str, int num )
{
  char *status_str;
  hid_t file_space_id;
  hid_t mem_space_id;
  hsize_t mem_dims[1];
  hsize_t mem_offsets[1];
  hsize_t start[1], step[1], count[1];
  hid_t dataspace;
  char *buf;
  char return_value;
  hid_t datatype;
  int not_done;
  int str_index;
  int read_error, at_eof;
  int achar;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("IFileGets: a\n");
#endif

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
    bassigncstr(ifilep->last_operation_status_message,"IFileGets hit EOF");

    return NULL;
  }// endif

#ifdef DEBUG
  printf("IFileGets: b\n");
#endif


/* -------------------------------------------------------------------- */
/* Create the file space specification                                  */
/* -------------------------------------------------------------------- */
  file_space_id = H5Dget_space( ifilep->ifile_id );
  if ( file_space_id < 0 ){
    bassignformat(error_string,"IFileGets: Can't get space of dataset.");
    return NULL;
  }// endif

#ifdef DEBUG
  printf("IFileGets: c\n");
#endif

  // make it have the appropriate piece of the dataset
  start[0]=ifilep->file_position;
  step[0]=1L;
  count[0]=1L;

#ifdef DEBUG
  printf("IFileGets: c1: start=%ld, step=%ld, count=%ld\n",
         start[0],step[0],count[0]);
#endif

  if(H5Sselect_hyperslab( file_space_id, H5S_SELECT_SET, 
                          start,step, count, NULL) < 0 ){
    H5Sclose(file_space_id);
    bassignformat(error_string,"IFileGets: Can't set hyperslab for dataset space");
    return NULL;
  }// endif

#ifdef DEBUG
  printf("IFileGets: d\n");
#endif


/* -------------------------------------------------------------------- */
/* Create the memory space specification                                */
/* -------------------------------------------------------------------- */
  mem_dims[0] = 1;
  mem_space_id = H5Screate_simple( 1, mem_dims, NULL );
  if(mem_space_id < 0 ){
    H5Sclose(file_space_id);
    bassignformat(error_string,"IFileGets: Failed to create mem-space spec size");
    return NULL;
  }// endif

#ifdef DEBUG
  printf("IFileGets: e\n");
#endif


/* -------------------------------------------------------------------- */
/* Create space for the read                                            */
/* -------------------------------------------------------------------- */
  buf = (char *)GMalloc(10*sizeof(char));
  if(!buf) {
    H5Sclose(mem_space_id);
    H5Sclose(file_space_id);
    bassignformat(error_string,"IFileGets: HMalloc failure on buf");
    return NULL;
  }// endif

#ifdef DEBUG
  printf("IFileGets: f\n");
#endif


/* -------------------------------------------------------------------- */
/* Read the data                                                        */
/* -------------------------------------------------------------------- */
  datatype = GS_GetHDF5Type(GS_DATATYPE_UI8);
  not_done = TRUE;
  str_index = 0;
  read_error = FALSE;
  at_eof = FALSE;
  while(not_done){
    if((achar = read_a_character(ifilep, datatype, mem_space_id,
                                 file_space_id,buf))<0){
      // read failed:
      // add the trailing NULL to str:
      str[str_index]='\0';
      not_done = FALSE;
      read_error = TRUE;
    } else {
      // read succeeded:

      // 0. if too many characters, finish up:
      if(str_index >= num-1) {
        // 1. append the read-in character to the string:
        str[str_index++]=achar;
        str[num]='\0';
        not_done = FALSE;

      } else {
        // 1. append the read-in character to the string:
        str[str_index++]=achar;
        
        // 2. if '\n'  we're done:
        if(achar == '\n') {
          // add the trailing NULL to str:
          str[str_index]='\0';
          not_done = FALSE;
        }// endif
      }// endif

      
      // 3. Increment file_position
      ifilep->file_position++;

      // 4. set next read to new position:
      if(not_done){
        start[0]=ifilep->file_position;
        step[0]=1;
        count[0]=1;
        if(H5Sselect_hyperslab( file_space_id, H5S_SELECT_SET, 
                                start,step, count, NULL) < 0 ){
          bassignformat(error_string,"IFileGets: Can't set hyperslab for dataset space");
          str[str_index]='\0';
          not_done = FALSE;
        }// endif
      }// endif

      // 5. check if at EOF:
      if( not_done && ifilep->file_position >= ifilep->size ){
        // Set EOF metadata
        ifilep->readPastEOF = TRUE;
        // Set last_operation_status_message metadata
        bassigncstr(ifilep->last_operation_status_message,"IFileGets hit EOF");

        str[str_index]='\0';
        not_done = FALSE;
        at_eof = TRUE;
      }// endif

    }// endif: was read successful?

  }// endwhile: read until "done"

#ifdef DEBUG
  printf("IFileGets: g\n");
#endif


/* -------------------------------------------------------------------- */
/* Clean up                                                             */
/* -------------------------------------------------------------------- */
  H5Sclose(mem_space_id);
  H5Sclose(file_space_id);
  GFree(buf);
  

/* -------------------------------------------------------------------- */
/* Update the readPastEOF metadata                                      */
/* -------------------------------------------------------------------- */
  if(at_eof){
    ifilep->readPastEOF = TRUE;
  } else {
    ifilep->readPastEOF = FALSE;
  }// endif

#ifdef DEBUG
  printf("IFileGets: h\n");
#endif


/* -------------------------------------------------------------------- */
/* Set last operation status metadata                                   */
/* -------------------------------------------------------------------- */
  if(read_error){
    ifilep->last_operation_status = IFILE_FAILURE;
    bassigncstr(ifilep->last_operation_status_message,
           "IFileGets() had a read error");

  } else if (at_eof) {
    bassigncstr(ifilep->last_operation_status_message,
           "IFileGets() hit EOF");

  } else {
    ifilep->last_operation_status = IFILE_SUCCESS;
    bassigncstr(ifilep->last_operation_status_message,
           "Successfully ran IFileGets()");
  }//endif

#ifdef DEBUG
  printf("IFileGets: i\n");
#endif

  return str;
 
}// end-function: IFileGets()


/*************************************************************************/
int read_a_character(IFILE *ifilep, hid_t datatype, hid_t mem_space_id,
                     hid_t file_space_id, char *buf)
{
  // assumes everything is set-up by caller for a single-char read
  // from somewhere in the dataset:

  char return_value;

#ifdef DEBUG
  printf("IFileGets: aa\n");
#endif


  if(H5Dread(ifilep->ifile_id, datatype, mem_space_id,
             file_space_id, H5P_DEFAULT, (void *)buf ) < 0){
    H5Sclose(mem_space_id);
    H5Sclose(file_space_id);
    GFree(buf);
    bassigncstr(error_string,"IFileGets: Read error.");

    /* ---------------------------------------------------------- */
    /* Set last_operation_status metadata                         */
    /* ---------------------------------------------------------- */
    ifilep->last_operation_status = IFILE_FAILURE;
    bassigncstr(ifilep->last_operation_status_message,
           "IFileGets() had a read error");

    return ERROR;
  }// endif
  return_value = buf[0];

#ifdef DEBUG
  printf("IFileGets: bb\n");
#endif


  return return_value;

}// endfunction: read_a_character()






