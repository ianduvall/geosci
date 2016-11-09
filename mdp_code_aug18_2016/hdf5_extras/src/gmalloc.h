/* gmalloc.h

   July 27, 2014: Just stubs to standard C functions.

*/
#ifndef GMALLOC_H_
#define GMALLOC_H_

#ifdef __cplusplus
extern "C" {
#endif

/*! \file */

void      *GMalloc( unsigned long size );
void      GFree( void *block );
void      *GRealloc( void *block, unsigned long size );

#ifdef __cplusplus
}
#endif

#endif
