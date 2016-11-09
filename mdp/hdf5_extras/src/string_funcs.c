
/* string_funcs.c

 written by Leland Pierce on 4-24-2007

 Contains a bunch of functions for
 manipulating character strings, and single characters.

 First written for FORSIGHT, added to GeoSci on Nov 16, 2013.
*/





/*
C	
C+
C@title{substring}{Return substring}
C@keyword{substring}{strings!substring}
C
C      substring() returns the substring of the given string starting at
C      the index start and ending at the index end, inclusive.
C
C1     CALL SEQUENCE
C@verbatim
C      char *substring(string, start, end);
C
C      const char *string;
C                   The string to process.
C                   
C      int    start;
C                   The index in string of the first character to keep.
C                   indexing starts at 0.
C                   
C      int    end;
C                   The index in string of the last character to keep.
C                   indexing starts at 0.
C                   
C@end
C     
C1     RETURN VALUE
C
C      substring() returns the newly-malloc'd string. Or NULL on failure.
C
C1     EXAMPLE
C
C@verbatim
C
C      char name[100];
C      char *al;
C
C      strcpy(name,"Hello there alvin");
C
C      al = substring(name,12,16);
C
C@end
C
C@title{frstch}{Returns position of first non-blank character}
C@keyword{frstch}{strings!frstch}
C
C      frstch() returns the index of the first non-blank
C      character in the string.
C
C1     CALL SEQUENCE
C@verbatim
C      int frstch(string);
C
C      const char *string;
C                   The string to process.
C                   
C@end
C     
C     
C1     RETURN VALUE
C
C      frstch() returns the index of the first non-blank
C      character in the string. Indexing starts at 0.
C      Returns -1 if no non-blanks found.
C
C1     EXAMPLE
C
C@verbatim
C
C      char name[100];
C      int i;
C
C      strcpy(name,"   Hello there alvin");
C
C      i = frstch(name);
C
C@end
C
C
C@title{lastch}{Returns position of last non-blank character}
C@keyword{lastch}{strings!lastch}
C
C      lastch() returns the index of the last non-blank
C      character in the string.
C
C1     CALL SEQUENCE
C@verbatim
C      int lastch(string);
C
C      const char *string;
C                   The string to process.
C                   
C@end
C     
C     
C1     RETURN VALUE
C
C      lastch() returns the index of the first non-blank
C      character in the string. Indexing starts at 0.
C      Returns -1 if no non-blanks found.
C
C1     EXAMPLE
C
C@verbatim
C
C      char name[100];
C      int i;
C
C      strcpy(name,"   Hello there alvin     ");
C
C      i = lastch(name);
C
C@end
C
C@title{str_toupper}{Converts string to upper-case letters}
C@keyword{str_toupper}{strings!str_toupper}
C
C      str_toupper() replaces each lowercase character in the given string
C      with its uppercase counterpart.
C
C1     CALL SEQUENCE
C@verbatim
C      void str_toupper(string);
C
C      char *string;
C                   The string to process.
C                   
C@end
C     
C     
C1     RETURN VALUE
C
C      str_toupper() returns nothing.
C
C1     EXAMPLE
C
C@verbatim
C
C      char name[100];
C
C      strcpy(name,"Hello there alvin");
C
C      str_toupper(name);
C
C@end
C
C@title{str_tolower}{Converts string to lower-case letters}
C@keyword{str_tolower}{strings!str_tolower}
C
C      str_tolower() replaces each uppercase character in the given string
C      with its lowercase counterpart.
C
C1     CALL SEQUENCE
C@verbatim
C      void str_tolower(string);
C
C      char *string;
C                   The string to process.
C                   
C@end
C     
C     
C1     RETURN VALUE
C
C      str_tolower() returns nothing.
C
C1     EXAMPLE
C
C@verbatim
C
C      char name[100];
C
C      strcpy(name,"Hello there alvin");
C
C      str_tolower(name);
C
C@end
C
C@title{rmblanks}{Removes all blanks in the string}
C@keyword{rmblanks}{strings!rmblanks}
C
C      rmblanks() returns the string with blank characters removed.
C      The resulting string may be shorter than the original.
C
C1     CALL SEQUENCE
C@verbatim
C      void rmblanks(string);
C
C      char   *string;
C                   The string to process.
C                   
C@end
C     
C     
C1     RETURN VALUE
C
C      rmblanks() returns nothing.
C
C1     EXAMPLE
C
C@verbatim
C
C      char name[100];
C
C      strcpy(name,"Hello there alvin");
C
C      rmblanks(name);
C
C@end
C
C@title{rmcommas}{Removes all commas in the string}
C@keyword{rmcommas}{strings!rmcommas}
C
C      rmcommas() returns the string with comma characters removed.
C      The resulting string may be shorter than the original.
C
C1     CALL SEQUENCE
C@verbatim
C      void rmcommas(string);
C
C      char   *string;
C                   The string to process.
C                   
C@end
C     
C     
C1     RETURN VALUE
C
C      rmcommas() returns nothing.
C
C1     EXAMPLE
C
C@verbatim
C
C      char name[100];
C
C      strcpy(name,"Hello there alvin");
C
C      rmcommas(name);
C
C@end
C
C@title{nextdig}{Returns index of next digit in the string}
C@keyword{nextdig}{strings!nextdig}
C
C      nextdig() returns the index of next occurrence of a digit in the 
C      string, starting at the given index.
C
C1     CALL SEQUENCE
C@verbatim
C      int nextdig(chvarb, start);
C
C      const char *string;
C                   The string to process.
C                   
C      int    start;
C                   The index in string of the first character to inspect.
C                   indexing starts at 0.
C                   
C@end
C     
C     
C1     RETURN VALUE
C
C      nextdig() returns the index of the next occurrence of a digit in the 
C      string, starting at the given index. If not found, returns -1.
C
C1     EXAMPLE
C
C@verbatim
C
C      char name[100];
C      int i;
C
C      strcpy(name,"Hello there 26 alvin");
C
C      i = nextdig(name,0);
C
C@end
C
C@title{nextcomma}{Returns index of next comma in the string}
C@keyword{nextcomma}{strings!nextcomma}
C
C      nextcomma() returns the index of next occurrence of a comma in the 
C      string, starting at the given index.
C
C1     CALL SEQUENCE
C@verbatim
C      int nextcomma(chvarb, start);
C
C      const char *string;
C                   The string to process.
C                   
C      int    start;
C                   The index in string of the first character to inspect.
C                   indexing starts at 0.
C                   
C@end
C     
C     
C1     RETURN VALUE
C
C      nextcomma() returns the index of the next occurrence of a comma in the 
C      string, starting at the given index. If not found, returns -1.
C
C1     EXAMPLE
C
C@verbatim
C
C      char name[100];
C      int i;
C
C      strcpy(name,"Hello there, 26 alvin");
C
C      i = nextcomma(name,0);
C
C@end
C
C@title{nextblank}{Returns index of next blank in the string}
C@keyword{nextblank}{strings!nextblank}
C
C      nextblank() returns the index of next occurrence of a blank in the 
C      string, starting at the given index.
C
C1     CALL SEQUENCE
C@verbatim
C      int nextblank(chvarb, start);
C
C      const char *string;
C                   The string to process.
C                   
C      int    start;
C                   The index in string of the first character to inspect.
C                   indexing starts at 0.
C                   
C@end
C     
C     
C1     RETURN VALUE
C
C      nextblank() returns the index of the next occurrence of a blank in the 
C      string, starting at the given index. If not found, returns -1.
C
C1     EXAMPLE
C
C@verbatim
C
C      char name[100];
C      int i;
C
C      strcpy(name,"Hello there, 26 alvin");
C
C      i = nextblank(name,0);
C
C@end
C
C@title{count_commas}{Returns number of commas in the string}
C@keyword{count_commas}{strings!count_commas}
C
C      count_commas() returns the number of commas in the string.
C
C1     CALL SEQUENCE
C@verbatim
C      int count_commas(string);
C
C      const char *string;
C                   The string to process.
C                   
C@end
C     
C     
C1     RETURN VALUE
C
C      count_commas() returns the number of commas in the string.
C
C1     EXAMPLE
C
C@verbatim
C
C      char name[100];
C      int ncommas;
C
C      strcpy(name,"Hello there, alvin");
C
C      ncommas = count_commas(name);
C
C@end
C
C@title{binary_strstr}{Searches for substring in binary string}
C@keyword{binary_strstr}{strings!binary_strstr}
C
C      binary_strstr() returns a pointer to the first occurrence
C      of the substring in the binary string.
C
C1     CALL SEQUENCE
C@verbatim
C      int binary_strstr(string, nchars, search_string);
C
C      const char *string;
C                   The string to process.
C
C      int    nchars;
C                   The number of characters in string.
C
C      const char *search_string;
C                   The sub-string to search for.
C                   
C@end
C     
C     
C1     RETURN VALUE
C
C      binary_strstr() returns a pointer to the first occurrence
C      of the substring in the binary string, or a NULL pointer
C      if the substring is not found.
C
C1     COMMENTS
C
C      This routine is useful when one has a mixed binary-and-ascii
C      string buffer within which one wants to search for a substring.
C      The ordinary strstr() function in C will stop looking at the first
C      NULL, while this version will look past any NULLs, up to the 
C      passed-in string length.
C
C      Since this routine uses strncmp() with the substring unchanged,
C      it is not possible to search for a substring containing a NULL.
C
C1     EXAMPLE
C
C@verbatim
C
C      char name[100];
C      char *locn;
C
C      for(i=0;i<100;i++){
C          name[i]='\0';
C      }
C      strcpy(name,"Hello there, alvin");
C
C      name[50]='h';
C      name[51]='e';
C      name[52]='c';
C      name[53]='t';
C      name[54]='o';
C      name[55]='r';
C      
C      locn = binary_strstr(name,100,"hector");
C
C@end
C
C@title{find_first_char}{Find first character in string}
C@keyword{find_first_char}{strings!find_first_char}
C
C      find_first_char() returns a pointer to the first occurrence
C      of the character in the string.
C
C1     CALL SEQUENCE
C@verbatim
C      char *find_first_char(string, c)
C
C      char *string;
C                   The string to process.
C
C      char   c;
C                   The character to search for.
C                   
C@end
C     
C     
C1     RETURN VALUE
C
C      find_first_char() returns a pointer to the first occurrence
C      of the character in the string, or a NULL pointer
C      if the character is not found.
C
C1     EXAMPLE
C
C@verbatim
C
C      char name[100];
C      char *locn;
C
C      strcpy(name,"Hello there, alvin");
C
C      locn = find_first_char(name,'e');
C
C@end
C
C@title{find_last_char}{Find last character in string}
C@keyword{find_last_char}{strings!find_last_char}
C
C      find_last_char() returns a pointer to the last occurrence
C      of the character in the string.
C
C1     CALL SEQUENCE
C@verbatim
C      char *find_last_char(string, c)
C
C      char *string;
C                   The string to process.
C
C      char   c;
C                   The character to search for.
C                   
C@end
C     
C     
C1     RETURN VALUE
C
C      find_last_char() returns a pointer to the last occurrence
C      of the character in the string, or a NULL pointer
C      if the character is not found.
C
C1     EXAMPLE
C
C@verbatim
C
C      char name[100];
C      char *locn;
C
C      strcpy(name,"Hello there, alvin");
C
C      locn = find_last_char(name,'e');
C
C@end
C
C
C
C@title{ends_with}{Does the end of one string match the other string}
C@keyword{ends_with}{strings!ends_with}
C
C      ends_with() returns TRUE if the first string ends with the second.
C
C1     CALL SEQUENCE
C@verbatim
C      int ends_with(string, ending)
C
C      const char *string;
C                   The string to process.
C
C      const char *ending;
C                   The ending to look for.
C                   
C@end
C     
C     
C1     RETURN VALUE
C
C      ends_with() returns TRUE if the first string ends with the second.
C      otherwise it returns FALSE.
C
C1     EXAMPLE
C
C@verbatim
C
C      char name[100];
C
C      strcpy(name,"Hello there, alvin");
C
C      if( ends_with(name,"vin")){
C          printf("name ends with the string 'vin'\n");
C      } else {
C          printf("name does not end with the string 'vin'\n");
C      }
C
C@end
C
C@title{excise}{Remove a portion of a string}
C@keyword{excise}{strings!excise}
C
C      excise() returns a modified version of the passed string,
C      one with some characters removed.
C
C1     CALL SEQUENCE
C@verbatim
C      void excise(string, start, num)
C
C      char   *string;
C                   The string to process.
C
C      int    start;
C                   The starting index for the deletion.
C
C      int    num;
C                   The number of charcters to remove.
C                   
C@end
C     
C     
C1     RETURN VALUE
C
C      excise() returns a new string with the specified characters
C      deleted and the trailing characters moved towards the start of
C      the string by the amount deleted.
C
C1     EXAMPLE
C
C@verbatim
C
C      char name[100];
C
C      strcpy(name,"Hello there, alvin");
C
C      excise(name,6,5);
C
C@end
C      In this case the string 'name' is now "Hello , alvin".
C
C@title{strinsert}{Insert a string into another string}
C@keyword{excise}{strings!strinsert}
C
C      strinsert() returns a modified version of the first passed string,
C      one with some characters inserted.
C
C1     CALL SEQUENCE
C@verbatim
C      void strinsert(string, start, newstuff)
C
C      char   *string;
C                   IN/OUT: The string to process.
C
C      int    start;
C                   The starting index for the insertion.
C                   The newstuff is inserted starting at this index.
C
C      const char *newstuff;
C                   String to insert.
C                   
C@end
C     
C     
C1     RETURN VALUE
C
C      strinsert() returns a new string with the specified characters
C      inserted.
C
C1     EXAMPLE
C
C@verbatim
C
C      char name[100];
C
C      strcpy(name,"Hello there, alvin");
C
C      strinsert(name,6,"hah");
C
C@end
C      In this case the string 'name' is now "Hello hahthere, alvin".
C
C-
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string_funcs.h"

//#define DEBUG

/************************************************************************/
//BOP
// !ROUTINE: substring -- Returns a substring
//
// !INTERFACE:
char *substring(const char *string, int start, int end)
// !INPUT PARAMETERS:
//    const char *string;
//    int start, end;
// !DESCRIPTION:
//  Returns the substring of the given {\tt string} starting at the
//  index {\tt start} and ending at the index {\tt end}, inclusive.
//EOP
{
  char *temp;
  int L;
  int i;
  L = end-start+1;
  if(L <= 0) return (char *)NULL;

  temp = NULL;
  temp=(char *)malloc((L+10)*sizeof(char));
  if(!temp) {
    //printf("*** ERROR (substring):\n");
    //printf("malloc error: mallocing %d bytes\n",L+10);
    return (char *)NULL;
  } /* endif */
  for(i=0;i<L+10;i++) {
    temp[i]=' ';
  }
  /* copy without the trailing null: */
  for(i=0;i<L;i++) temp[i] = string[i+start];
  temp[L]='\0';
  return temp;
}/* endfunction substring */


/***************************************************************/
//BOP
// !ROUTINE: frstch -- Returns position of first non-blank character
//
// !INTERFACE:
int frstch(const char *string)
// !INPUT PARAMETERS:
//    const char *string;
// !DESCRIPTION:
//     Finds the first non-blank char in a string
//     and returns its index. Returns -1 if string is empty or blank.
//EOP
{
  int L, i;
  L = strlen(string);
  /*  if string is empty, return now */
  if (L <= 0) return -1;
  
  /* search until non-blank character */
  for(i=0;i<L;i++){
    if(string[i] != ' ') return i;
  }
  return -1;
} /* endfunction: frstch() */

/***************************************************************/
//BOP
// !ROUTINE: lastch -- Returns position of last non-blank character
//
// !INTERFACE:
int lastch(const char *string)
// !INPUT PARAMETERS:
//    const char *string;
// !DESCRIPTION:
//     Finds the last non-blank char in a string
//     and returns its index. Returns -1 if string is empty or blank.
//EOP
{
  int L, i;
  L = strlen(string);
  /*  if string is empty, return now */
  if (L <= 0) return -1;

#ifdef DEBUG
  printf("in lastch: L=%d\n",L);
  for(i=0;i<L;i++){
    printf("index: %d,  char:|%c|\n",i,string[i]);
  }
#endif
  
  /* search (from end) until non-blank character */
  for(i=0;i<L;i++){
    if(string[L-1-i] != ' ') return L-1-i;
  }
  return -1;
} /* endfunction: lastch() */


      
/**********************************************************/
//BOP
// !ROUTINE: str_toupper -- Converts string to upper-case letters.
//
// !INTERFACE:
void str_toupper(char *str)
// !INPUT PARAMETERS:
//    char *str;
// !DESCRIPTION:
//     The input string {\tt str} is replaced
//     with a new string, where all alphabetic chars
//     are now uppercase.
//EOP
{
  int L, i;
  L = strlen(str);
  for(i=0;i<L;i++){
    str[i] = toupper(str[i]);
  }
} /* endfunction: str_toupper() */

/**********************************************************/
//BOP
// !ROUTINE: str_tolower -- Converts string to lower-case letters.
//
// !INTERFACE:
void str_tolower(char *str)
// !INPUT PARAMETERS:
//    char *str;
// !DESCRIPTION:
//     The input string {\tt str}  is replaced
//     with a new string, where all alphabetic chars
//     are now lowercase.
//EOP
{
  int L, i;
  L = strlen(str);
  for(i=0;i<L;i++){
    str[i] = tolower(str[i]);
  }
} /* endfunction: str_tolower() */

/**********************************************************/
//BOP
// !ROUTINE: rmblanks -- Removes all blanks in the string
//
// !INTERFACE:
void rmblanks(char *str)
// !INPUT PARAMETERS:
//    char *str;
// !DESCRIPTION:
//     The input string {\tt str} is replaced
//     with a new string, where all blanks are deleted.
//     The new string may be fewer characters than before.      
//EOP
{
  register int i,j;
  i=0;
  while(i<strlen(str)){
    if(str[i] == ' ') {
      for(j=0;i+j<strlen(str);j++) str[i+j] = str[i+j+1];
    } else {
      i++;
    } /* end if */
  }/* end while */
} /* endfunction: rmblanks() */


/**********************************************************/
//BOP
// !ROUTINE: rmcommas -- Removes all commas in the string
//
// !INTERFACE:
void rmcommas(char *str)
// !INPUT PARAMETERS:
//    char *str;
// !DESCRIPTION:
//     The input string {\tt str} is replaced
//     with a new string, where all commas are deleted.
//     The new string may be fewer characters than before.      
//EOP
{
  register int i,j;
  i=0;
  while(i<strlen(str)){
    if(str[i] == ',') {
      for(j=0;i+j<strlen(str);j++) str[i+j] = str[i+j+1];
    } else {
      i++;
    } /* end if */
  }
} /* endfunction: rmcommas() */


/************************************************************************/
//BOP
// !ROUTINE: nextdig -- Returns index of next occurrence of a digit in the string
//
// !INTERFACE:
int nextdig(const char *chvarb, int start)
// !INPUT PARAMETERS:
//    const char *chvarb;
//    int start;
// !DESCRIPTION:
//     Looks for 1st occurrence of a digit in the {\tt chvarb},
//     following character \#{\tt start}.
//     If found, returns the starting position, else returns 0.
//EOP
{
  int L, i;
  L = strlen(chvarb);
  /*  if string is empty, return now */
  if (L <= 0) return -1;
  
  /* search until find a digit */
  for(i=start;i<L;i++){
    if(isdigit(chvarb[i])) return i;
  }
  return -1;
} /* endfunction: nextdig() */

/************************************************************************/
//BOP
// !ROUTINE: nextcomma -- Returns index of next occurrence of a comma in the string
//
// !INTERFACE:
int nextcomma(const char *chvarb, int start)
// !INPUT PARAMETERS:
//    const char *chvarb;
//    int start;
// !DESCRIPTION:
//     Looks for 1st occurrence of a comma in the {\tt chvarb},
//     following character \#{\tt start}.
//     If found, returns the starting position, else returns -1.
//EOP
{
  int L, i;
  L = strlen(chvarb);
  /*  if string is empty, return now */
  if (L <= 0) return -1;

  //  printf("nextcomma: chvarb=%s|, start=%d\n",chvarb, start);
  
  /* search until find a comma */
  for(i=start;i<L;i++){
    if(chvarb[i] == ',') return i;
  }
  return -1;
} /* endfunction: nextcomma() */



/************************************************************************/
//BOP
// !ROUTINE: count_commas -- Returns number of commas in the string
//
// !INTERFACE:
int count_commas(const char *chvarb)
// !INPUT PARAMETERS:
//    const char *chvarb;
// !DESCRIPTION:
//     Returns the number of commas found in the string.
//EOP
{
  int L, i;
  int ncommas=0;
  L = strlen(chvarb);
  /*  if string is empty, return now */
  if (L <= 0) return 0;
  
  /* search for commas */
  for(i=0;i<L;i++){
    if(chvarb[i] == ',') ncommas++;
  }
  return ncommas;
} /* endfunction: count_commas() */


/************************************************************************/
//BOP
// !ROUTINE: nextbl -- Returns index of next occurrence of a blank in the string
//
// !INTERFACE:
int nextbl(const char *chvarb, int start)
// !INPUT PARAMETERS:
//    const char *chvarb;
//    int start;
// !DESCRIPTION:
//     Looks for 1st occurrence of a blank in the {\tt chvarb},
//     following character \#{\tt start}.
//     If found, returns the starting position, else returns -1.
//EOP
{
  int L, i;
  L = strlen(chvarb);
  /*  if string is empty, return now */
  if (L <= 0) return -1;
  
  /* search until find a blank */
  for(i=start;i<L;i++){
    if(isblank(chvarb[i])) return i;
  }
  return -1;
} /* endfunction: nextbl() */


/************************************************************/
/* Binary version of strstr, with input binary string to    */
/* search in, but a normal string to search for.            */
/************************************************************/
char *binary_strstr( const char *string,       /* possibly a binary string  */
                     int length,               /* #chars in string          */
                     const char *searchString) /* must not be binary string */
{
  int L;
  int i,j;
  L = strlen(searchString);

  i=0;
  while (i<=length-L) {
    if( strncmp(string+i,searchString,L) == 0 ) { j = i; i=length; }
    i++;
  }
  return((char *)(string+j));
}


/************************************************************/
/************************************************************/
char *find_first_char(char *string, char c)
{
  int i;
  int L;

  L=strlen(string);

  for(i=0;i<L;i++){
    if( string[i] == c ) return string+i;
  }/* endfor */

  return NULL;
}/* endfunction: find_first_char() */


/************************************************************/
/************************************************************/
char *find_last_char(char *string, char c)
{
  int i;
  int L;

  L=strlen(string);

  for(i=L-1;i>=0;i--){
    if( string[i] == c ) return string+i;
  }/* endfor */

  return NULL;
}/* endfunction: find_last_char() */


/************************************************************/
/************************************************************/
int ends_with(const char *string, const char *ending)
{
  int L, Ls;
  int i,j;
  int ret;

  Ls = strlen(string);
  L= strlen(ending);

  //printf("ends_with: string=|%s|, ending=|%s|\n",string,ending);
  //printf("Ls=%d,   L=%d\n",Ls,L);

  ret = TRUE;
  j=0;
  for(i=Ls-L;i<Ls-1;i++){
    if(string[i] != ending[j++]) ret= FALSE;
    //printf("comparing %c and %c\n",string[i], ending[j-1]);
  }/* endfor */

  //printf("ends_with: returning: %d\n",ret);

  return ret;
  
}/* endfunction: ends_with() */


/************************************************************/
/************************************************************/
void excise(char *string, int start, int num)
{
  int i;
  int L;

  L=strlen(string);
  if(start+num>=L) {
    string[start]='\0';
    return;
  }/* endif */

  for(i=start+num;i<L+1;i++){
    string[i-num]=string[i];
  }/* endfor */

}/* endfunction: excise() */

/************************************************************/
/************************************************************/
void strinsert(char *target, int index, const char *newstuff)
{
  /* insert newstuff at index */
  char front[10000], back[10000];

  if(index<0 || index > strlen(target)) return;

  if(index>0){
    strncpy(front,target,index);
    front[index]='\0';
    strcpy(back,target+index);
  } else {
    front[0]='\0';
    strcpy(back,target);
  }// endif



  //printf("strinsert: target=|%s|, front=|%s|, back=|%s|\n",
  //       target, front, back);


  strcpy(target,front);
  strcat(target,newstuff);
  strcat(target,back);


}/* endfunction: strinsert() */

/************************************************************/
/************************************************************/
void char_substitute(char *target, char toreplace, char newchar)
{
  int i;
  i=0;
  while(target[i] != '\0'){
    if(target[i]==toreplace) target[i]=newchar;
    i++;
  }/* endwhile */
}/* endfunction: char_substitute() */
