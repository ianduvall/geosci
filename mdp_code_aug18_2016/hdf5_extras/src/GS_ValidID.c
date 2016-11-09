/* GS_ValidID.c

   by Leland Pierce, Nov 20, 2013

   Aug 2, 2014
   Modified for doxygen and GS_

*/


#include <hdf5.h>
#define TRUE  1
#define FALSE 0


/** \brief GS_ValidID returns whether the given object-id is a valid identifier.


\see GS_FileCreate()

\param[in] id
An identifier that may have been returned
from one of the other routines in the library,
possibly referring to a file, an image, or some
other object of interest in the file.

\returns
TRUE is returned if the identifier is valid, FALSE otherwise.

\par Exmaple:
We have obtained an identifier for an image object, 
and we wish to see if it is still valid:
\code
hid_t id;
if(GS_ValidID(id)){
  printf("The id is valid\n");
} else {
  printf("The id is invalid\n");
}
\endcode

*/


int GS_ValidID(hid_t id)
{
  if(H5Iis_valid( id ) <= 0 ){
    return FALSE;
  }// endif
  return TRUE;
}
