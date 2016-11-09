/* GS_CreateAccessPlist.c

   by Leland Pierce, Nov 12, 2013.
  
   Aug 3, 2014
   Modified for bstrings, doxygen and GS_

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <hdf5.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


#define TRUE  1
#define FALSE 0


#define CHUNK_CACHE_NELMTS  521
//"""Number of elements for HDF5 chunk cache."""

#define CHUNK_CACHE_PREEMPT  0.0
//"""Chunk preemption policy.  This value should be between 0 and 1
//inclusive and indicates how much chunks that have been fully read are
//favored for preemption. A value of zero means fully read chunks are
//treated no differently than other chunks (the preemption is strictly
//LRU) while a value of one means fully read chunks are always preempted
//before other chunks."""

#define CHUNK_CACHE_SIZE  (2 * 1024 *1024)
//"""Size (in bytes) for HDF5 chunk cache."""
// in this case: 2 MegaBytes


#define DRIVER "H5FD_SEC2"
//"""The HDF5 driver that should be used for reading/writing to the file.
//Following drivers are supported:
//
//    * H5FD_SEC2: this driver uses POSIX file-system functions like read
//      and write to perform I/O to a single, permanent file on local
//      disk with no system buffering.
//      This driver is POSIX-compliant and is the default file driver for
//      all systems.
//
//    * H5FD_STDIO: this driver uses functions from the standard C
//      stdio.h to perform I/O to a single, permanent file on local disk
//      with additional system buffering.
//
//    * H5FD_CORE: with this driver, an application can work with a file
//      in memory for faster reads and writes. File contents are kept in
//      memory until the file is closed. At closing, the memory version
//      of the file can be written back to disk or abandoned.
//
//The following drivers are not currently supported:
//
//    * H5FD_LOG: this is the H5FD_SEC2 driver with logging capabilities.
//
//    * H5FD_FAMILY: with this driver, the HDF5 file’s address space is
//      partitioned into pieces and sent to separate storage files using
//      an underlying driver of the user’s choice.
//      This driver is for systems that do not support files larger than
//      2 gigabytes.
//
//    * H5FD_MULTI: with this driver, data can be stored in multiple
//      files according to the type of the data. I/O might work better if
//      data is stored in separate files based on the type of data.
//      The Split driver is a special case of this driver.
//
//    * H5FD_SPLIT: this file driver splits a file into two parts.
//      One part stores metadata, and the other part stores raw data.
//      This splitting a file into two parts is a limited case of the
//      Multi driver.
//
//    * H5FD_MPIO: this is the standard HDF5 file driver for parallel
//      file systems. This driver uses the MPI standard for both
//      communication and file I/O.
//
//    * H5FD_MPIPOSIX: this parallel file system driver uses MPI for
//      communication and POSIX file-system calls for file I/O.
//
//    * H5FD_STREAM: this driver is no longer available.
//
//.. seealso:: the `Drivers section`_ of the `HDF5 User's Guide`_ for
//   more information.

#define DRIVER_CORE_INCREMENT  (100 * 1024 * 1024)
//"""Core driver memory increment.
//
//Specifies the increment by which allocated memory is to be increased
//each time more memory is required.

#define DRIVER_CORE_BACKING_STORE  TRUE
//"""Enables backing store for the core driver.
//
//With the H5FD_CORE driver, if the DRIVER_CORE_BACKING_STORE is set
//to 1 (True), the file contents are flushed to a file with the same name
//as this core file when the file is closed or access to the file is
//terminated in memory.
//
//The application is allowed to open an existing file with H5FD_CORE
//driver. In that case, if the DRIVER_CORE_BACKING_STORE is set to 1 and
//the flags for :func:`tables.open_file` is set to H5F_ACC_RDWR, any change
//to the file contents are saved to the file when the file is closed.
//If backing_store is set to 0 and the flags for :func:`tables.open_file`
//is set to H5F_ACC_RDWR, any change to the file contents will be lost
//when the file is closed. If the flags for :func:`tables.open_file` is
//set to H5F_ACC_RDONLY, no change to the file is allowed either in
//memory or on file.


//#define DEBUG22






/*! \brief GS_CreateAccessPlist creates an HDF5 access parameter list


The list that GS_CreateAccessPlist() returns is for use
when creating or opening an HDF5 file.
It sets raw data chunk cache parameters, as well as a driver
for file I/O.

\see GS_FileOpen(), GS_FileCreate()

\returns
This routine returns a valid HDF5 handle to an access_plist,
which can be used when creating or opening an HDF5 file.

\par Example
When creating a new file, we need to specify the access parameters.
This routine returns an access list with "typical" parameters and 
parameter values set for most general uses.
\code
  hid_t access_plist;
  access_plist = GS_CreateAccessPlist();
\endcode
*/


/*************************************************************************/
hid_t GS_CreateAccessPlist()
{
  hid_t access_plist;
  size_t n_elements, size;
  double preempt;
  herr_t err = 0;

  int increment;
  hbool_t backing_store;


/* -------------------------------------------------------------------- */
/*      Set raw data chunk cache parameters                             */
/* -------------------------------------------------------------------- */
  // File access property list
  access_plist = H5Pcreate(H5P_FILE_ACCESS);
  
#ifdef DEBUG22
  printf("GS_CreateAccessPlist: used H5Pcreate: access_plist = %d\n",(int)access_plist);
#endif

  // Set parameters for chunk cache
  n_elements = CHUNK_CACHE_NELMTS;
  size = CHUNK_CACHE_SIZE;
  preempt = CHUNK_CACHE_PREEMPT;
  err = H5Pset_cache(access_plist, 0, n_elements, size, preempt);
  if(err < 0){
    //Unable to set the raw data chunk cache parameters, not fatal....
    H5Pclose(access_plist);
    access_plist = H5P_DEFAULT;
  }//endif
        
/* -------------------------------------------------------------------- */
/*      Set driver for file i/o                                         */
/* -------------------------------------------------------------------- */
  bstring driver = bfromcstr(DRIVER); // DRIVER is an include file...
  bstring dsec2  = bfromcstr("H5FD_SEC2");
  bstring dstdio = bfromcstr("H5FD_STDIO");
  bstring dcore  = bfromcstr("H5FD_CORE");
  if( EQUAL(driver, dsec2)){
    err = H5Pset_fapl_sec2(access_plist);
  } else if ( EQUAL(driver, dstdio)){
    err = H5Pset_fapl_stdio(access_plist);
  } else if ( EQUAL(driver, dcore)){
    increment     = DRIVER_CORE_INCREMENT;
    backing_store = DRIVER_CORE_BACKING_STORE;
    err = H5Pset_fapl_core(access_plist, increment, backing_store);
  } else {
    // use default driver
    err = H5Pset_fapl_sec2(access_plist);
  }// endif
  bdestroy(driver);
  bdestroy(dsec2);
  bdestroy(dstdio);
  bdestroy(dcore);
  
  if(err < 0){
    // use default:
    err = H5Pset_fapl_sec2(access_plist);
    if(err<0) {
      H5Pclose(access_plist);
      access_plist = H5P_DEFAULT;
    }
  }// endif
  
  
  return access_plist;
}
