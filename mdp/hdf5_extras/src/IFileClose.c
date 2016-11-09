/* IFileClose.c

   by Leland Pierce, Jan 20, 2014.
  
*/


/** \brief IFileClose closes an internal file in a GeoSci File

\sa IFileOpen

\param[in] ifilep
   The struct of the open internal file.

\returns
 TRUE is returned on success, FALSE on failure.
The error_string variable is also set to a system-specific 
error code on failure.

\par Example 1
Let's assume that one already has an GeoSci file, and one wants
to close an internal file name "/somename":
\code
hid_t file_id;
IFILE *ifilep;

ifilep = IFileOpen(file_id,"/somename","w");
if(!ifilep) {
   printf("IFileOpen failed on /somename\n");
}
if(!IFileClose(ifilep)){
   printf("IFileClose failed on /somename\n");
}
\endcode

\par Example 2
Let's assume that one already has an GeoSci file, and one wants
to close an internal file that's inside another group:
\code
hid_t file_id, group_id;
IFILE *ifilep;

group_id = GS_GroupCreate(file_id,"/group1");
if(group_id < 0) {
   printf("GS_GroupCreate failed on /group1\n");
   exit(-1);
}
ifilep = IFileOpen(file_id,"/group1/somename","w");
if(!ifilep) {
   printf("IFileOpen failed on /group1/somename\n");
   exit(-1);
}
if(!IFileClose(ifilep)){
   printf("IFileClose failed on /group1/somename\n");
   exit(-1);
}
\endcode

\par Details
This function is meant to emulate as close as possible the standard
C fclose() function.
*/

#include "ifile.h"

//#define DEBUG

/*******************************************************************************/
int IFileClose( IFILE *ifilep )
{
  hid_t dataset_id;
  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

  dataset_id = ifilep->ifile_id;

#ifdef DEBUG
 printf("IFileClose: start\n");
#endif

/* -------------------------------------------------------------------- */
/* Make sure dataset is the right type                                  */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsIFileByID(dataset_id)){
  bassignformat(error_string,"IFileClose: dataset is not an internal file.");
    return FALSE;
  }// endif

#ifdef DEBUG
 printf("IFileClose: 1\n");
#endif

/* -------------------------------------------------------------------- */
/*      Flush any pending updates                                       */
/* -------------------------------------------------------------------- */
  if(!GS_FileFlush(dataset_id)){
    bassignformat(error_string,"IFileClose: Can't flush file.");
    return FALSE;
  }// endif

#ifdef DEBUG
 printf("IFileClose: 2\n");
#endif

/* -------------------------------------------------------------------- */
/* Terminate access                                                     */
/* -------------------------------------------------------------------- */
  if(!IFileWriteAccess(ifilep,IFILE_CLOSED)){
    bassignformat(error_string,"IFileClose: Can't write the access metadata");
    return FALSE;
  }//endif

#ifdef DEBUG
 printf("IFileClose: 3\n");
#endif

/* -------------------------------------------------------------------- */
/* Clean up                                                             */
/* -------------------------------------------------------------------- */
  GFree(ifilep);

#ifdef DEBUG
 printf("IFileClose: 4\n");
#endif

/* -------------------------------------------------------------------- */
/* Close it                                                             */
/* -------------------------------------------------------------------- */
  if(H5Dclose(dataset_id) <0){
#ifdef DEBUG
 printf("IFileClose: 5: failure\n");
#endif

    return FALSE;
  } else {
#ifdef DEBUG
 printf("IFileClose: 6: sucess\n");
#endif

    return TRUE;
  }// endif

}
