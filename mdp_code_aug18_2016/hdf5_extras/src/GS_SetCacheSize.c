/* GS_SetCacheSize.c

   by Leland Pierce, Nov 12, 2013.
  
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


// for herr_t
#define ERROR -1
#define SUCCESS 0



/** \brief GS_SetCacheSize sets the metadata cache size for a GeoSci datafile


\param[in] file_id
                   File handle for GeoSci datafile
\param[in] cache_size
                   Size in bytes for the metadata cache. 
                   1048576 (1 MByte) is a good default.

\returns
A non-negative integer is returned on success, while a negative
integer is returned on failure.

\par Example
Set the cache size to be 1MByte:
\code
hid_t file_id;
if(GS_SetCacheSize(file_id,1048576) < 0){
   printf("Error setting cache size.\n");
}
\endcode
*/

herr_t GS_SetCacheSize(hid_t file_id, size_t cache_size) 
{
  H5AC_cache_config_t config;
  herr_t code;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

  // default:
  code = 0;

/* -------------------------------------------------------------------- */
/* Make sure I have a FILE id, not something else                       */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsFile(file_id)){
    bassignformat(error_string,"GS_SetCacheSize: Invalid file_id.");
    return FALSE;
  }// endif

/* -------------------------------------------------------------------- */
/* Get the configuration                                                */
/* -------------------------------------------------------------------- */
  config.version = H5AC__CURR_CACHE_CONFIG_VERSION;
  code = H5Fget_mdc_config(file_id, &config);
  if(code < 0) {
    bassignformat(error_string,"GS_SetCacheSize: unable to get mdc_config.");
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Modify the configuration                                             */
/* -------------------------------------------------------------------- */
  config.set_initial_size = TRUE;
  config.initial_size = cache_size;
  code = H5Fset_mdc_config(file_id, &config);
  if(code < 0) {
    bassignformat(error_string,"GS_SetCacheSize: unable to set mdc_config.");
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/*      Flush any pending updates                                       */
/* -------------------------------------------------------------------- */
  if(!GS_FileFlush(file_id)){
    bassignformat(error_string,"GS_SetCacheSize: Can't flush file.");
    return ERROR;
  }// endif

/* -------------------------------------------------------------------- */
/* Return status code                                                   */
/* -------------------------------------------------------------------- */
  return code;

}
