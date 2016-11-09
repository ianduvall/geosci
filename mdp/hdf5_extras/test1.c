#include <stdio.h>

#include "hdf5_extras.h"

int main(){
  IFILE *ifilep;
  hid_t file_id=0;
  bstring report=bfromcstr("");
  hid_t group_id=0;


  GS_init_error_reporting();

  printf("hello\n");
  bstring filename = bfromcstr("test1.hd5");
  file_id = GS_FileCreate(filename);
  printf("at 1\n");

  bstring groupname = bfromcstr("/group1");
  group_id = GS_GroupCreate(file_id,groupname);
  printf("at 2\n");

  GS_GroupClose(group_id);
  printf("at 3\n");

  GS_FileClose(file_id);
  printf("at 4\n");

  if(!GS_FileReport(filename,report)){
    printf("error-string=%s\n",bdata(error_string));
  } else {
    printf("at 5\n");
    printf("%s\n",bdata(report));
  }

    printf("at 5.5\n");
  bstring access = bfromcstr("r+");
  file_id = GS_FileOpen(filename, access);
  printf("at 6\n");

  ifilep = IFileOpen(file_id, "/ifile34", "w+");
  printf("at 7\n");
  IFilePrintf(ifilep,"Hello there\n");
  printf("at 8\n");
  IFileClose(ifilep);
  printf("at 9\n");
  ifilep = IFileOpen(file_id,"/ifile34","r");
  char theline[100];
  IFileGets(ifilep,theline,100);
  printf("theline=|%s|\n",theline);
  IFileClose(ifilep);

return (0);
}// end-main
