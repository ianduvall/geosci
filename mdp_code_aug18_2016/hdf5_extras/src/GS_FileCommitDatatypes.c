/* GS_FileCommitDatatypes.c

   by Leland Pierce, Aug 14, 2014
  
*/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


#define ERROR -1

//#define DEBUG


/** \brief GS_FileCommitDatatypes commits the complex raster datatypes to a file

\see GS_FileCreate()

\param[in] file_id
                   The file ID

\returns TRUE on success, FALSE on failure.

\par Example:
After creating a new file, commit the complex raster datatypes to it:
\code
hid_t file_id;
if(!GS_FileCommitDatatypes(file_id)){
   printf("failed to commit the complex raster datatypes to the file\n");
}
\endcode

\par Details
HDF-5 requires user-created raster datatypes to be "committed" to a file 
where these datatypes are used.
Since all the complex raster datatypes used in GeoSci are "user-created",
we need to make sure that they are always committed.
*/



int GS_FileCommitDatatypes(hid_t file_id)
{
  int datatype;
  bstring type_string;
  hid_t hdf_datatype;
  // we don't support CI*, yet.....LEP: May 26, 2016
  //int datatypes[]={GS_DATATYPE_CI8, GS_DATATYPE_CI16, GS_DATATYPE_CI32,
  //                GS_DATATYPE_CI64, GS_DATATYPE_C64, GS_DATATYPE_C128};
  int datatypes[]={GS_DATATYPE_CI16, GS_DATATYPE_CI32,
                  GS_DATATYPE_CI64, GS_DATATYPE_C64, GS_DATATYPE_C128};
  int i;
  char *type_string2;

#ifdef DEBUG
  printf("GS_FileCommitDatatypes: at 1\n");
#endif

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("GS_FileCommitDatatypes: at 2\n");
#endif

/* -------------------------------------------------------------------- */
/* Make sure I have a FILE id, not something else                       */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsFile(file_id)){
    bassignformat(error_string,"GS_FileCommitDatatypes: Invalid file_id.");
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_FileCommitDatatypes: at 3\n");
#endif


/* -------------------------------------------------------------------- */
/* HDF5 needs a real cstring later                                      */
/* -------------------------------------------------------------------- */
  type_string2=(char*)GMalloc(sizeof(char)*100);
  if(!type_string2){
    bassignformat(error_string,"GS_FileCommitDatatypes: GMalloc failure.");
    return FALSE;
  }

/* -------------------------------------------------------------------- */
/* Commit each complex datatype                                         */
/* -------------------------------------------------------------------- */
  for(i=0;i<5;i++){
    datatype = datatypes[i];

    type_string = bfromcstr("");
    type_string = GS_DatatypeAsString(datatype);

    type_string->mlen = type_string->slen;
#ifdef DEBUG
    printf("GS_FileCommitDatatypes: at 4, i=%d, typestring=%s, datatype=%d\n"
           ,i,bdata(type_string),datatype);
#endif


    if(!GS_CheckInternalName(file_id,type_string)) {
      // datatype not in the file, add it:

#ifdef DEBUG
    printf("GS_FileCommitDatatypes: at 4.1, i=%d\n",i);
#endif

      hdf_datatype = GS_ConvertToHDFDatatype(datatype);
#ifdef DEBUG
      printf("GS_FileCommitDatatypes: at 4.2, i=%d, hdf_datatype=%d\n",i,hdf_datatype);
#endif
      if(hdf_datatype < 0){
#ifdef DEBUG
    printf("GS_FileCommitDatatypes: at 4.3, i=%d\n",i);
#endif
        bassigncstr(error_string,"GS_FileCommitDatatypes: Invalid datatype.");
#ifdef DEBUG
    printf("GS_FileCommitDatatypes: at 4.4, i=%d\n",i);
#endif
    //GS_FileClose(file_id);
#ifdef DEBUG
    printf("GS_FileCommitDatatypes: at 4.5, i=%d\n",i);
#endif
        return FALSE;
      }// endif

#ifdef DEBUG
    printf("GS_FileCommitDatatypes: at 5, i=%d\n",i);
#endif

      strcpy(type_string2,bdata(type_string));
      bdestroy(type_string);
      if(H5Tcommit(file_id, type_string2, hdf_datatype, 
                   H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT) <0){
        bassigncstr(error_string,"GS_FileCommitDatatypes: Committing the complex datatype failed.");
        GS_HDFDatatypeClose(hdf_datatype);
        GFree(type_string2);
        //GS_FileClose(file_id);
        return FALSE;
      }// endif

#ifdef DEBUG
    printf("GS_FileCommitDatatypes: at 6, i=%d\n",i);
    //j=GS_HDFDatatypeClose(hdf_datatype);
    //printf("return value of GS_HDFDatatypeClose is: %d\n",j);
    //return FALSE;
#endif

      if(!GS_HDFDatatypeClose(hdf_datatype)){
        bassigncstr(error_string,"GS_FileCommitDatatypes: Closing the complex datatype failed.");
        //GS_FileClose(file_id);
        GFree(type_string2);
        return FALSE;
      }// endif

#ifdef DEBUG
    printf("GS_FileCommitDatatypes: at 7, i=%d\n",i);
#endif

    }//endif: type already in the file?

  }//endfor
  GFree(type_string2);

#ifdef DEBUG
    printf("GS_FileCommitDatatypes: at 8\n");
#endif

  return TRUE;
}

