#ifndef STRING_FUNCS_H_
#define STRING_FUNCS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*! \file */

#define TRUE 1
#define FALSE 0

char *substring(const char *string, int start, int end);
int frstch(const char *string);
int lastch(const char *string);
void str_toupper(char *str);
void str_tolower(char *str);
void rmblanks(char *str);
void rmcommas(char *str);
int nextdig(const char *chvarb, int start);
int nextcomma(const char *chvarb, int start);
int count_commas(const char *chvarb);
int nextbl(const char *chvarb, int start);
char *binary_strstr( const char *string,       /* possibly a binary string  */
                     int length,         /* #chars in string          */
                     const char *searchString); /* must not be binary string */
char *find_first_char(char *string, char c);
char *find_last_char(char *string, char c);
int ends_with(const char *string, const char *ending);
void excise(char *string, int start, int num);
void strinsert(char *target, int index, const char *newstuff);
void char_substitute(char *target, char toreplace, char newchar);

#ifdef __cplusplus
}
#endif

#endif
