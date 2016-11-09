/* GS_SplitHistory.c

   by Leland Pierce, Jan 3, 2014

   July 8, 2014:
   modified for multi-level library
   modified to use bstrings

*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"



/** \brief GS_SplitHistory splits history string into front and back

\anchor GS_SplitHistory
\addindex GS_SplitHistory


\see GS_AppendHistory(), GS_AbortedHistory()

\par[in] history_string
                   The input history string.
\par[out] front
                   The output front of the history string, with
                   the last line, if any, stripped off. 
                   On input, this must be a valid bstring.
\par[out] back
                   The output back of the history string, containing
                   the last line, if any.
                   On input, this must be a valid bstring.

\returns TRUE on success, FALSE on failure.

\par Example:
Given a history string like: \n

\c "line1;\nline2;\nline3;\n" \n

This function returns: \n
front = \c "\line1;\nline2;\n"
back  = \c "line3;\n"\n

This is programmed as:
\code
bstring history=bfromcstr("line1;\nline2;\nline3;\n");
bstring front  =bfromcstr("");
bstring back   =bfromcstr("");

GS_SplitHistory(history,front, back);

     ... use front and back ...

bdestroy(history);
bdestroy(front);
bdestroy(back);
\endcode

\par Details:
\parblock
The history string is made up of lines delimited by ";\n".
There are several cases:

\li If the history string is empty, then 2 empty strings are returned.

li If the history string is non-empty, but has no ";\n" delimiters,
  then the returned front string is empty, and the back string
  is this string, with a ";\n" appended to it.

\li If the history string has 1 line, then the returned front string is
  empty, and the back string is this 1 line.

\li In all other cases, all but the the last line is copied to the
   front string, and the last line is copied into the back string.
\endparblock
*/


//#define DEBUG

/**********************************************************************/
int GS_SplitHistory(const_bstring history_string, bstring front, bstring back)
{
  int L;
  int ndelim, idelim, delim;
  int index;
  bstring end_of_line=bsstatic(";\n");

#ifdef DEBUG
  printf("GS_SplitHistory: start: input:|%s|\n",history_string);
#endif

  // 1. case 1: empty input string:
  L = blength(history_string);

  if(L==0){
    bdestroy(end_of_line);
    bassigncstr(front,"");
    bassigncstr(back,"");
    return TRUE;
  }// endif

#ifdef DEBUG
  printf("GS_SplitHistory: 1, L=%d\n",L);
#endif


  // 2. count the number of ";\n" delimiters:
  idelim = binstr(history_string,0,end_of_line);
  ndelim=0;
  while(idelim > 0){
    ndelim++;
    idelim = binstr(history_string,idelim+2,end_of_line);
  }// endwhile

#ifdef DEBUG
  printf("GS_SplitHistory: 2, ndelim=%d\n",ndelim);
#endif


  // 3. case 2: no delimiters:
  if(ndelim==0){
    bdestroy(end_of_line);
    bassigncstr(front,"");
    bassign(back,history_string);
    return TRUE;
  }// endif

#ifdef DEBUG
  printf("GS_SplitHistory: 3\n");
#endif


  // 4. case 3: 1 line:
  if(ndelim==1){
    bdestroy(end_of_line);
    bassigncstr(front,"");
    bassign(back,history_string);

#ifdef DEBUG
    printf("GS_SplitHistory: front=|%s|, back=|%s|\n",*front,*back);
#endif

    return TRUE;
    
  }// endif

#ifdef DEBUG
  printf("GS_SplitHistory: 4\n");
#endif


  // 5. case 4: 2+ lines:
  if(ndelim>1){

#ifdef DEBUG
  printf("GS_SplitHistory: 4.1\n");
#endif

    bassign(front,history_string);

#ifdef DEBUG
    printf("GS_SplitHistory: 4.2\n");
#endif


    // 5.1 find the last-but-1 delimiter
    delim = binstr(history_string,0,end_of_line);
    idelim=0;
    while(idelim < ndelim-2) {
      idelim++;
      delim = binstr(history_string,delim+2,end_of_line);
    }// endwhile

#ifdef DEBUG
    printf("GS_SplitHistory: 4.3, remaining part=|%s|\n",delim+2);
#endif

    
    // 5.2 get the index where the front string stops:
    index = delim + 2;
#ifdef DEBUG
    printf("GS_SplitHistory: 4.3.1: index=%d\n",index);
#endif
    btrunc(front,index+1); // truncate at this index

    

#ifdef DEBUG
  printf("GS_SplitHistory: 4.4\n");
#endif

    bassignmidstr(back, history_string,index,L-index);

#ifdef DEBUG
  printf("GS_SplitHistory: 4.6\n");
#endif


    return TRUE;
    
  }// endif

#ifdef DEBUG
  printf("GS_SplitHistory: 5\n");
#endif


  return FALSE;
}
