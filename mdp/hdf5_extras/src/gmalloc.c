/* gmalloc.c

   July 27, 2014: Just stubs to standard C functions.

*/

#include <stdlib.h>
#//include <stdarg.h>
//#include <string.h>
//#include <stdio.h>




/************************************************************************/
/*                        GMalloc()                                     */
/*                                                                      */
/************************************************************************/
void      *GMalloc( unsigned long size )
{

  return malloc(size);
}

/************************************************************************/
/*                        GFree()                                       */
/*                                                                      */
/************************************************************************/
void      GFree( void *block )
{
  if( block == NULL )
    return;
  
  //lep free( block );
  return;
}


/************************************************************************/
/*                        GRealloc()                                    */
/*                                                                      */
/*      Reallocate a block of memory to be at least as large as         */
/*      the requested size.                                             */
/************************************************************************/
void      *GRealloc( void *block, unsigned long size )
{
      return( realloc( block, size ));
}


