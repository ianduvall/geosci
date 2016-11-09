/* GS_FileCreate.c

   by Leland Pierce, Nov 16, 2013.
  
   Aug 3, 2014
   Modified for bstrings, doxygen and GS_

*/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


#define ERROR -1


#define USER_BLOCK_SIZE  0
//"""Sets the user block size of a file.
//The default user block size is 0; it may be set to any power of 2 equal
//to 512 or greater (512, 1024, 2048, etc.).






#define METADATA_CACHE_SIZE  (1 * 1024* 1024)
//"""Size (in bytes) of the HDF5 metadata cache."""
// 1 MB is the default for HDF5



#define PRINTIT FALSE




/** \brief GS_FileCreate creates an empty GeoSci datafile

The basic file metadata is created, but nothing else.
GS_FileCreate() assumes the file does NOT exist before creating it, 
and will return an error if a file with the same name already 
exists.

\see GS_FileOpen(), GS_ImageGeorefIO()

\param[in] filename
Name of file to be created.
If there is no ".hd5" extension, this routine
will create a file with the ".hdf5" extension.

\returns
The routine returns a file handle on success,
which is negative if creation fails.

\par Example
Create an empty file named "testimage.hd5".
\code
hid_t       file_id;
bstring     filename=bfromcstr("testimage.hd5");
file_id = GS_FileCreate( filename );
    ... use the file ...
GS_FileClose( file_id );
\endcode

\par Details
For every GeoSci file we create we also "commit" 
every complex datatype into it.
That way, no matter what a user does, the datatypes 
are there for them to use.
This simplifies the other codes, who now need not be 
concerned about dealing with this.

*/

//#define DEBUG

//***********************************************************************
hid_t GS_FileCreate(const_bstring filename)
{

  int i;

  herr_t err = 0;
  hid_t access_plist;
  hid_t create_plist = H5P_DEFAULT;
  size_t user_block_size = 0;

  bstring buffer;

  int is_pow_of_2;

  hid_t file_id;
  int cache_size;

#ifdef DEBUG
  printf("GS_FileCreate: 1, filename=%s\n",bdata(filename));
#endif

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
 printf("GS_FileCreate: 2\n");
#endif


/* -------------------------------------------------------------------- */
/*	If the fiel does not already have an extension, give it	        */
/*	the extension .hd5.                                         	*/
/* -------------------------------------------------------------------- */
  buffer = bfromcstr("");
  bassign( buffer, filename );
  for( i = bstrlen(filename)-1; 
       bchar(filename,i) != '.' && bchar(filename,i) != ']' 
         && bchar(filename,i) != '/' && bchar(filename,i) != '\\' 
         && i > 0; i-- ) {}
  
  if( bchar(filename,i) != '.' ) {
    bstring ending = bfromcstr(".hd5");
    bconcat( buffer, ending );
    bdestroy(ending);
  }
  

#ifdef DEBUG
 printf("GS_FileCreate: 3\n");
#endif

/* -------------------------------------------------------------------- */
/*      Check if file already exists     				*/
/* -------------------------------------------------------------------- */
  if( GS_CheckFileExistence(buffer) ) {
    bassignformat(error_string,"GS_FileCreate: File already exists: %s.",bdata(buffer));
    bdestroy(buffer);
    return ERROR;
  }// endif

#ifdef DEBUG
 printf("GS_FileCreate: 4\n");
#endif

/* -------------------------------------------------------------------- */
/*      Set USER_BLOCK_SIZE for the new file                            */
/* -------------------------------------------------------------------- */
  user_block_size = (int)(USER_BLOCK_SIZE);
  is_pow_of_2 = ((user_block_size & (user_block_size - 1)) == 0);
  
  // The USER_BLOCK_SIZE must be a power of 2 greater than 512 or zero
  if(user_block_size !=0 & user_block_size < 512){
    user_block_size =0;
  }//endif
  if(user_block_size > 512 && !is_pow_of_2) {
    user_block_size =0;
  } // endif
  
  // File creation property list
  create_plist = H5Pcreate(H5P_FILE_CREATE);
  err = H5Pset_userblock(create_plist, user_block_size);
  if(err < 0){
    //Unable to set the user block size, not fatal....
    H5Pclose(create_plist);
    create_plist = H5P_DEFAULT;
  }//endif
        
#ifdef DEBUG
 printf("GS_FileCreate: 5\n");
#endif

/* -------------------------------------------------------------------- */
/*	Create the access parameter list          			*/
/* -------------------------------------------------------------------- */
  access_plist = GS_CreateAccessPlist();

/* -------------------------------------------------------------------- */
/*	Create the file, creates the root group also   			*/
/* -------------------------------------------------------------------- */
  bassignformat(status_string,"Creating empty file: %-40s", bdata(buffer) );
        
  file_id = H5Fcreate(bdata(buffer), H5F_ACC_TRUNC, create_plist, access_plist);
  if( file_id < 0){
    bassignformat(error_string,"GS_FileCreate: File creation failed.");
    H5Pclose(create_plist);
    H5Pclose(access_plist);
    bdestroy(buffer);
    return file_id;
  }// endif
     
#ifdef DEBUG
 printf("GS_FileCreate: 6\n");
#endif
   
/* -------------------------------------------------------------------- */
/*	Clean up                                			*/
/* -------------------------------------------------------------------- */
  bdestroy(buffer);
  H5Pclose(create_plist);
  H5Pclose(access_plist);
      
#ifdef DEBUG
 printf("GS_FileCreate: 7\n");
#endif
  
/* -------------------------------------------------------------------- */
/*	Set the hdf5 metadata cache size.       			*/
/* -------------------------------------------------------------------- */
  cache_size = METADATA_CACHE_SIZE;
  err = GS_SetCacheSize(file_id, cache_size);
  if(err < 0){
    // failure is OK.
  }// endif
    
#ifdef DEBUG
 printf("GS_FileCreate: 8\n");
#endif

/* -------------------------------------------------------------------- */
/*	Commit the complex datatypes                                    */
/* -------------------------------------------------------------------- */
  if(!GS_FileCommitDatatypes(file_id) ){
#ifdef DEBUG
 printf("GS_FileCreate: 8.1\n");
#endif
    bassignformat(error_string,"GS_FileCreate: Can't commit the complex datatypes.\n");
#ifdef DEBUG
 printf("GS_FileCreate: 8.2\n");
#endif
    GS_FileClose(file_id);
#ifdef DEBUG
 printf("GS_FileCreate: 8.3\n");
#endif
    return ERROR;
  }// endif

#ifdef DEBUG
 printf("GS_FileCreate: 9\n");
#endif

/* -------------------------------------------------------------------- */
/*	Write File Metadata                                             */
/* -------------------------------------------------------------------- */
 bstring descriptor = bfromcstr("Empty File");
 if(!GS_FileCreateMetadata(file_id,descriptor) ){
    bassignformat(error_string,"GS_FileCreate: Can't create the file metadata.\n");
#ifdef DEBUG
    printf("GS_FileCreate: 9.1: error creating file-metadata, error_string=|%s|\n",bdata(error_string));
#endif
    GS_FileClose(file_id);
    bdestroy(descriptor);
    return ERROR;
  }// endif
 bdestroy(descriptor);

#ifdef DEBUG
 printf("GS_FileCreate: 10\n");
#endif

/* -------------------------------------------------------------------- */
/*      Flush changes to hard drive.                                    */
/* -------------------------------------------------------------------- */
  if(GS_FileFlush(file_id) < 0 ){
    bassignformat(error_string,"GS_FileCreate: Can't flush the new file.\n");
    GS_FileClose(file_id);
    return ERROR;
  }// endif

#ifdef DEBUG
 printf("GS_FileCreate: 11\n");
#endif

/* -------------------------------------------------------------------- */
/*      Return the file handle.                                         */
/* -------------------------------------------------------------------- */
  return file_id;
}
