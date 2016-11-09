/* GS_H5ObjectTypeAsString.c

   by Leland Pierce, March 30, 2014

   modified June 20, 2016
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"

/** \brief GS_H5ObjectTypeAsString returns a printable version of HDF5 object type
\param[in] object_type
                   object type, as an integer. One of:\n
                   H5O_TYPE_GROUP\n
                   H5O_TYPE_DATASET\n
                   H5O_TYPE_NAMED_DATATYPE
\returns
On success, the routine returns the string representation
of the object type according to the following table:

object type             |  returned string
------------------------|------------------
H5O_TYPE_GROUP          |  Group
H5O_TYPE_DATASET        |  Dataset
H5O_TYPE_NAMED_DATATYPE |  Named Datatype

On failure, the string "Unknown Object Type" is returned.

Note that the returned string should NOT be bdestroy'd when done.
\par Example
Obtain list of objects in a file named "testimage.hd5".
\code
    ...
char **objnames;
int objhdftypes;
int nobjs;
int i;

file_id = GS_FileOpen(bsstatic("testimage.hd5"),bsstatic("r+"));
if(file_id<0){
   printf("GS_FileOpen failed\n");
   printf("%s\n",bdata(error_string));
   exit(-1);
}//endif

if(!GS_ObjectGetH5Children(file_id, &nobjs, &objnames,&objhdftypes)){
   printf("GS_ObjectGetH5Children failed\n");
   printf("%s\n",bdata(error_string));
   exit(-1);
}

// print results:
for(i=0;i<nobjs;i++){
   printf("obj# %d: %s (%s)\n",i,bdata(objnames[i]),
           GS_H5ObjectTypeAsString(objhdftypes[i]));
}

// clean up:
for(i=0;i<nobjs;i++){
   bdestroy(objnames[i]);
}
GFree(objnames);
GFree(objhdftypes);
\endcode
*/


/**********************************************************************/
bstring GS_H5ObjectTypeAsString(int objtype)
{
  switch(objtype){
  case H5O_TYPE_GROUP:
    return bsstatic("Group");
  case H5O_TYPE_DATASET:
    return bsstatic("Dataset");
  case H5O_TYPE_NAMED_DATATYPE:
    return bsstatic("Named Datatype");
  default:
    return bsstatic("Unknown Object Type");
  }// end-switch
}
