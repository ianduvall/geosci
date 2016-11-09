// bstrlen.c
#include "bstrlib.h"
int bstrlen(bstring str)
{
  return strlen(bdata(str));
}
