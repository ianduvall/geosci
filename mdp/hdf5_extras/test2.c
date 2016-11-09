#include <stdio.h>

#include "hdf5_extras.h"

#define TEST8

int bstrlen();

// for testing sqlite3:
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
  int i;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}


int main()
{
  int i;
  hid_t thefile;
  int channels[4];
  bstring report_string = bfromcstr("");


  hid_t file_id, image_id;
  int npixels, nlines, nchannels;

  hid_t raster_id;

  double xsz, ysz;
  int status;

  int win1[4], win2[4];


  char *typestring="R32, R32, ";
  char **chtypes;
  int n, jj;
  char *ddd=NULL;


  GS_init();


#ifdef TEST1
  printf("testing Dirname:\n");
  ddd = GS_Dirname("/a/b/c/");
  if(ddd){
    printf("GS_Dirname('/a/b/c/')=%s\n",ddd);
  } else {
    printf("GS_Dirname('/a/b/c/')=NULL\n");
  }//endif
  GFree(ddd);
  ddd=NULL;

  ddd = GS_Dirname("/a/b/blah");
  if(ddd){
    printf("GS_Dirname('/a/b/blah')=%s\n",ddd);
  } else {
    printf("GS_Dirname('/a/b/blah')=NULL\n");
  }//endif
  GFree(ddd);
  ddd=NULL;

  ddd = GS_Dirname("/blah");
  if(ddd){
    printf("GS_Dirname('/blah')=%s\n",ddd);
  } else {
    printf("GS_Dirname('/blah')=NULL\n");
  }//endif
  GFree(ddd);
  ddd=NULL;

  ddd = GS_Dirname("/blah1/blah2/blah3");
  if(ddd){
    printf("GS_Dirname('/blah1/blah2/blah3')=%s\n",ddd);
  } else {
    printf("GS_Dirname('/blah1/blah2/blah3')=NULL\n");
  }//endif
  GFree(ddd);
  ddd=NULL;

  ddd = GS_Dirname("blah");
  if(ddd){
    printf("GS_Dirname('blah')=%s\n",ddd);
  } else {
    printf("GS_Dirname('blah')=NULL\n");
  }//endif
  GFree(ddd);
  ddd=NULL;

  ddd = GS_Dirname("blah/");
  if(ddd){
    printf("GS_Dirname('blah/')=%s\n",ddd);
  } else {
    printf("GS_Dirname('blah/')=NULL\n");
  }//endif
  GFree(ddd);
  ddd=NULL;
    
  exit(0);
#endif




#ifdef TEST2
  printf("1\n");
  //report_num_objects_open();


  //channels[0]=3;
  //channels[1]=0;
  //channels[2]=0;
  //channels[3]=0;

  bstring filename = bfromcstr("test123.h5");
  
  thefile = GS_FileCreate(filename);

  printf("2\n");
  //report_num_objects_open();


  printf("thefile=%d\n",thefile);
  if(thefile<0) {
    printf("error: %s\n",bdata(error_string));
    exit(-1);
  } else {
    printf("status: %s\n",bdata(status_string));
  }//endif

  GS_FileClose(thefile);

  printf("3\n");
  //report_num_objects_open();

  thefile = GS_FileOpen(filename,bsstatic("r+"));
  bstring imagename = bfromcstr("image_aa");
  bstring image_descriptor = bfromcstr("SIR-C Raco 102.4 SRL-1");
  image_id = GS_ImageCreate(thefile,imagename,image_descriptor);

  printf("image_id=%d\n",image_id);

  printf("4\n");
   
  bstring raster_name = bfromcstr("Stokes-1");
  bstring raster_descriptor = bfromcstr("Channel 1 of JPL compressed stokes matrix");
  raster_id = GS_RasterCreate(image_id,raster_name,
                              GS_DATATYPE_SI8,1240,2048,raster_descriptor);

  printf("raster_id=%d\n",raster_id);

  printf("5\n");

  GS_FileClose(thefile);


  printf("\nFile Report:\n");
  if(GS_FileReport(filename,report_string)){
    printf("%s",bdata(report_string));
    bdestroy(report_string);
  } else {
    printf("%s\n",bdata(error_string));
  }

  thefile = GS_FileOpen(filename,bsstatic("r+"));
  image_id = GS_ImageOpen(thefile,imagename);

  printf("\nGroup Report:\n");
  report_string = GS_GroupReport(image_id);
  printf("%s\n",bdata(report_string));
  bdestroy(report_string);

  printf("\nDataset Report:\n");
  raster_id = GS_RasterOpen(image_id,raster_name);
  report_string = GS_DatasetReport(raster_id);
  printf("%s\n",bdata(report_string));
  bdestroy(report_string);




  printf("\nTesting RasterSizeInfo():\n");
  if(GS_RasterSizeInfo( raster_id, &npixels, &nlines )){
    printf( " Pixels: %d\n Lines: %d\n",
            npixels, nlines );
  } else {
    printf("Can't get Raster size information.\n");
    printf("%s\n",bdata(error_string));
  }


  printf("\nTesting RasterPixelSize():\n");
  bstring units = bfromcstr("");
  if(GS_RasterPixelSize(raster_id,GS_READ,&xsz, &ysz, units)){
    printf( " Pixelsizes: %lf %s X,   %lf %s Y\n",xsz,bdata(units), ysz, bdata(units));
  } else {
    printf("Can't get Raster Pixelsize information.\n");
    printf("%s\n",bdata(error_string));
  }//endif

  xsz=100.;
  ysz=200.;
  bassigncstr(units,"Kmeters");
  if(GS_RasterPixelSize(raster_id,GS_WRITE,&xsz, &ysz, units)){
  } else {
    printf("Can't set Raster Pixelsize information.\n");
    printf("%s\n",bdata(error_string));
  }//endif


  if(GS_RasterPixelSize(raster_id,GS_READ,&xsz, &ysz, units)){
    printf( " Pixelsizes: %lf %s X,   %lf %s Y\n",xsz,bdata(units), ysz, bdata(units));
  } else {
    printf("Can't get Raster Pixelsize information.\n");
    printf("%s\n",bdata(error_string));
  }//endif



  printf("\nTesting getFilename:\n");
  bstring aastr;
  if(aastr=GS_GetFilename(thefile)){
    printf("Filename1 = %s\n",bdata(aastr));
  }// endif
  bdestroy(aastr);

  if(aastr=GS_GetFilename(image_id)){
    printf("Filename2 = %s\n",bdata(aastr));
  }// endif
  bdestroy(aastr);

  if(aastr=GS_GetFilename(raster_id)){
    printf("Filename3 = %s\n",bdata(aastr));
  }// endif
  bdestroy(aastr);







  printf("\nTesting GetIDName:\n");
  if(aastr=GS_GetIDName(thefile)){
    printf("name1 = %s\n",bdata(aastr));
  }// endif
  bdestroy(aastr);
  if(aastr=GS_GetIDName(image_id)){
    printf("name2 = %s\n",bdata(aastr));
  }// endif
  bdestroy(aastr);
  if(aastr=GS_GetIDName(raster_id)){
    printf("name3 = %s\n",bdata(aastr));
  }// endif
  bdestroy(aastr);








  printf("\nTesting GS_RasterCheckWindow:\n");
  win1[0]=0; win1[1]=0; win1[2]=300; win1[3]=300;
  if(GS_RasterCheckWindow(image_id,bsstatic("Stokes-1"),win1)){
    printf("1 inside\n");
  } else {
    printf("1 not inside:\n");
    printf("%s\n",bdata(error_string));
  }// endif

  win2[0]=0; win2[1]=0; win2[2]=3030; win2[3]=300;
  if(GS_RasterCheckWindow(image_id,bsstatic("Stokes-1"),win2)){
    printf("2 inside\n");
  } else {
    printf("2 not inside:\n");
    printf("%s\n", bdata(error_string));
  }// endif


  printf("\nTesting GS_CheckWindowByID:\n");
  win2[0]=0; win2[1]=0; win2[2]=3030; win2[3]=300;
  if(GS_RasterCheckWindowByID(raster_id,win2)){
    printf("2 inside\n");
  } else {
    printf("2 not inside:\n");
    printf("%s\n", bdata(error_string));
  }// endif




  printf("\nTesting GS_CheckWindowInside:\n");
  win2[0]=0; win2[1]=0; win2[2]=3030; win2[3]=300;
  if(GS_CheckWindowInside(win1,win2)){
    printf("win1 inside win2\n");
  } else {
    printf("win1 not inside win2:\n");
    printf("%s\n",bdata(error_string));
  }// endif

#endif




#ifdef TEST3

  thefile = GS_FileOpen(filename,bsstatic("r+"));



  printf("\n\n\n==================================\n");
  printf("testing IFile functions:\n");

  IFILE *ifilep;
  ifilep = IFileOpen(thefile,"/test_ifile1","w");
  if(!ifilep){
    printf("IFileOpen failed\n");
    printf("%s\n",bdata(error_string));
    exit(-1);
  } else {
    printf("IFileOpen succeeded\n");
  }// endif

  if(IFilePrintf(ifilep,"Hello world") <= 0){
    printf("IFilePrintf failed\n");
    printf("%s\n",bdata(error_string));
    exit(-1);
  } else {
    printf("IFilePrintf succeeded\n");
  }// endif

  printf("main: after IFilePrintf, ifilep->file_position=%ld\n",ifilep->file_position);


  if(IFileRewind(ifilep)){
    printf("IFileRewind succeeded\n");
  } else {
    printf("IFileRewind failed\n");
    printf("%s\n",bdata(error_string));
    exit(-1);
  }// endif

  printf("main: after IFileRewind, ifilep->file_position=%ld\n",ifilep->file_position);


  char readstring[1000];



  if(IFileGets(ifilep,readstring,1000)){
    printf("IFileGets succeeded\n");
  } else {
    printf("IFileGets failed\n");
    printf("%s\n",bdata(error_string));
    exit(-1);
  }// endif

  printf("the string I just read with IFileGets: |%s|\n",readstring);




  if(IFileEOF(ifilep)==TRUE){
    printf("IFileEOF says the last read hit EOF (good)\n");
  } else if(IFileEOF(ifilep)==FALSE){
    printf("IFileEOF says the last read DID NOT hit EOF\n");
  } else {
    printf("IFileEOF returned an error\n");
  }

  if(IFileSeek(ifilep,100L,SEEK_SET)){
    printf("IFileSeek succeeded\n");
  } else {
    printf("IFileSeek failed\n");
    printf("%s\n",bdata(error_string));
    exit(-1);
  }// endif




  strcpy(readstring,"Just some junk");
  int LR;
  LR = strlen(readstring);
  printf("about to write a string using IFileWrite, string=|%s|, length=%d\n",
         readstring,LR);

  int iw;
  iw = IFileWrite(ifilep,readstring,sizeof(char),LR);
  if(iw != LR){
    printf("IFileWrite failed, only wrote %d elements, expected: %d\n",
           iw, LR);
    printf("%s\n",bdata(error_string));
    exit(-1);
  } else {
    printf("IFileWrite succeeded.\n");
  } // endif 

  
  if(IFileSeek(ifilep,100L,SEEK_SET)){
    printf("IFileSeek succeeded\n");
  } else {
    printf("IFileSeek failed\n");
    printf("%s\n",bdata(error_string));
    exit(-1);
  }// endif



  int thechar;
  thechar = IFileGetc(ifilep);
  if(thechar < 0){
    printf("IFileGetc failed\n");
    printf("%s\n",bdata(error_string));
    exit(-1);
  } else {
    printf("IFileGetc succeeded: |%c|\n",thechar);
  }// endif
  thechar = IFileGetc(ifilep);
  if(thechar < 0){
    printf("IFileGetc failed\n");
    printf("%s\n",bdata(error_string));
    exit(-1);
  } else {
    printf("IFileGetc succeeded: |%c|\n",thechar);
  }// endif
  thechar = IFileGetc(ifilep);
  if(thechar < 0){
    printf("IFileGetc failed\n");
    printf("%s\n",bdata(error_string));
    exit(-1);
  } else {
    printf("IFileGetc succeeded: |%c|\n",thechar);
  }// endif



  if(IFileError(ifilep)){
    printf("IFileError says that last-op caused an error\n");
  } else {
    printf("IFileError says that last-op DID NOT cause an error\n");
  } // endif

  if(!IFilePerror(ifilep,NULL)){
    printf("IFilePerror failed\n");
  }// endif
  printf("IFilePerror (1) succeeded\n");

  if(!IFilePerror(ifilep,"byebye")){
    printf("IFilePerror failed\n");
  }// endif
  printf("IFilePerror (2) succeeded\n");

  long int pos;
  pos = IFileTell(ifilep);
  if(pos<0){
    printf("IFileTell failed\n");
  }// endif
  printf("IFileTell says filepos is %ld\n",pos);




  IFileRewind(ifilep);
  strcpy(readstring,"");
  iw = IFileRead(ifilep,readstring,sizeof(char),5);
  if(iw != 5) {
    printf("IFileRead Failed: #elmnts read=%d, expecting 5\n",iw);
  } else {
    printf("IFileRead succeeded: first 5 chars are now:\n");
    printf("|%c%c%c%c%c|\n",readstring[0],readstring[1],readstring[2],readstring[3],readstring[4]);
  }// endif



  IFileRewind(ifilep);
  printf("writing test string\n");
  strcpy(readstring,"1 2.3 6 find a dope\n");
  if(!IFilePuts(ifilep,readstring)){
    printf("IFilePuts failed\n");
    exit(-1);
  }// endif
  printf("wrote test string\n");
  IFileRewind(ifilep);


  int ii1, ii2;
  float ff1;
  char ss[1000];
  iw = IFileScanf(ifilep,"%d %f %d %s", &ii1, &ff1, &ii2, ss);
  if(iw != 4) {
    printf("IFileScanf Failed: #elmnts read=%d, expecting 4\n",iw);
  } else {
    printf("IFileScanf succeeded:\n");
    printf("          ii1=%d, ff1=%f, ii2=%d, ss=|%s|\n",ii1, ff1, ii2, ss);
  }// endif


  char ttyy;
  IFileRewind(ifilep);
  ttyy='a';
  if(!IFilePutc(ifilep,ttyy)){
    printf("IFilePutc failed\n");
  } else {
    printf("IFilePutc succeeded\n");
  }// endif
  ttyy='b';
  if(!IFilePutc(ifilep,ttyy)){
    printf("IFilePutc failed\n");
  } else {
    printf("IFilePutc succeeded\n");
  }// endif
  ttyy='c';
  if(!IFilePutc(ifilep,ttyy)){
    printf("IFilePutc failed\n");
  } else {
    printf("IFilePutc succeeded\n");
  }// endif



  if(!IFileClose(ifilep)){
    printf("IFileClose failed\n");
    printf("%s\n",bdata(error_string));
    exit(-1);
  } else {
    printf("IFileClose succeeded\n");
  }// endif

  printf("\nDONE\n");

#endif








#ifdef TEST4
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;

  printf("\n\n\n==================================\n");
  printf("testing sqlite3 functions with IFiles:\n");
  char sqlite_name[1000];


  
  bstring filename = bfromcstr("test789.h5");
  file_id = GS_FileCreate(filename);
  if(file_id < 0){
    printf("Can't open the file  test789.h5.\n");
    exit(-1);
  }// endif


  sqlite3_vfs_register(sqlite3_HDFvfs(), 1);
  sprintf(sqlite_name,"%d:%s",file_id,"/test1hdf");

  //sqlite3_vfs_register(sqlite3_demovfs(), 1);
  //strcpy(sqlite_name,"/home/lep/notpci9/src/apps_hdf5/test55.db");

  printf("the sqlite filename: |%s|\n",sqlite_name);

  rc = sqlite3_open(sqlite_name, &db);
  if( rc ){
    fprintf(stderr, "Can't open sqlite internal database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    exit(-1);
  }
  
  printf("running 1st sql statement\n");
  rc = sqlite3_exec(db, "create table tbl1(one varchar(10), two smallint);", 
                    callback, 0, &zErrMsg);
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "(1) SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    exit(-1);
  }


  printf("running 2nd sql statement\n");
  rc = sqlite3_exec(db, "insert into tbl1 values('hello!',10);", 
                    callback, 0, &zErrMsg);
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "(2) SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    exit(-1);
  }


  printf("running 3rd sql statement\n");
  rc = sqlite3_exec(db, "insert into tbl1 values('goodbye', 20);", 
                    callback, 0, &zErrMsg);
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "(3) SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    exit(-1);
  }


  printf("running 4th sql statement\n");
  rc = sqlite3_exec(db, "select * from tbl1;", 
                    callback, 0, &zErrMsg);
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "(4) SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    exit(-1);
  }

  printf("closing the sqlite db\n");
  sqlite3_close(db);

  GS_FileClose(file_id);


  printf("\nDONE\n");

#endif



#ifdef TEST5

  printf("testing GS_ObjectGetH5Children\n");
  int ncobjs;
  int *hdfobjtypes;



  bstring filename = bfromcstr("test123.h5");
  
  thefile = GS_FileCreate(filename);

  printf("2\n");
  //report_num_objects_open();


  printf("thefile=%d\n",thefile);
  if(thefile<0) {
    printf("error: %s\n",bdata(error_string));
    exit(-1);
  } else {
    printf("status: %s\n",bdata(status_string));
  }//endif

  GS_FileClose(thefile);

  printf("3\n");
  //report_num_objects_open();

  thefile = GS_FileOpen(filename,bsstatic("r+"));
  bstring imagename = bfromcstr("image_aa");
  bstring image_descriptor = bfromcstr("SIR-C Raco 102.4 SRL-1");
  image_id = GS_ImageCreate(thefile,imagename,image_descriptor);

  printf("image_id=%d\n",image_id);

  printf("4\n");
   
  bstring raster_name = bfromcstr("Stokes-1");
  bstring raster_descriptor = bfromcstr("Channel 1 of JPL compressed stokes matrix");
  raster_id = GS_RasterCreate(image_id,raster_name,
                              GS_DATATYPE_SI8,1240,2048,raster_descriptor);

  printf("raster_id=%d\n",raster_id);

  printf("5\n");

  GS_FileClose(thefile);





   printf("\ntesting GS_ObjectGetH5Children\n");
 
  file_id = GS_FileOpen(bsstatic("test123.h5"),bsstatic("r+"));
  if(file_id<0){
    printf("GS_FileOpen failed\n");
    printf("%s\n",bdata(error_string));
    exit(-1);
  }//endif
  
  bstring *cobjnames;
  if(!GS_ObjectGetH5Children(file_id, &ncobjs, &cobjnames,&hdfobjtypes)){
    printf("GS_ObjectGetH5Children failed\n");
    printf("%s\n",bdata(error_string));
    exit(-1);
  }
  
  // print results:
  for(i=0;i<ncobjs;i++){
    printf("obj# %d: %s (%s)\n",i,bdata(cobjnames[i]),
           bdata(GS_H5ObjectTypeAsString(hdfobjtypes[i])) );
  }

  // clean up:
  for(i=0;i<ncobjs;i++){
    bdestroy(cobjnames[i]);
  }
  GFree(cobjnames);
  GFree(hdfobjtypes);

  GS_FileClose(thefile);

  /********************************/
   printf("\ntesting GS_ObjectGetGSChildren\n");

  file_id = GS_FileOpen(bsstatic("test123.h5"),bsstatic("r+"));
  if(file_id<0){
    printf("GS_FileOpen failed\n");
    printf("%s\n",bdata(error_string));
    exit(-1);
  }//endif
  
  //bstring *cobjnames;
  if(!GS_ObjectGetGSChildren(file_id, &ncobjs, &cobjnames,&hdfobjtypes)){
    printf("GS_ObjectGetGSChildren failed\n");
    printf("%s\n",bdata(error_string));
    exit(-1);
  }
  
  // print results:
  for(i=0;i<ncobjs;i++){
    printf("obj# %d: %s (%s)\n",i,bdata(cobjnames[i]),
           bdata(GS_GSObjectTypeAsString(hdfobjtypes[i])) );
  }

  // clean up:
  for(i=0;i<ncobjs;i++){
    bdestroy(cobjnames[i]);
  }
  GFree(cobjnames);
  GFree(hdfobjtypes);

  GS_FileClose(thefile);



#endif //TEST5


#ifdef TEST6


  printf("testing GS_ObjectLock/Unlock\n");
  int ncobjs;
  int *hdfobjtypes;



  bstring filename = bfromcstr("test123.h5");
  
  file_id = GS_FileCreate(filename);

  printf("2\n");
  //report_num_objects_open();


  printf("file_id=%d\n",file_id);
  if(file_id<0) {
    printf("error: %s\n",bdata(error_string));
    exit(-1);
  } else {
    printf("status: %s\n",bdata(status_string));
  }//endif

  GS_FileClose(file_id);

  printf("3\n");
  //report_num_objects_open();

  file_id = GS_FileOpen(filename,bsstatic("r+"));
  bstring imagename = bfromcstr("image_aa");
  bstring image_descriptor = bfromcstr("SIR-C Raco 102.4 SRL-1");
  image_id = GS_ImageCreate(file_id,imagename,image_descriptor);

  printf("image_id=%d\n",image_id);

  printf("4\n");
   
  bstring raster_name = bfromcstr("Stokes-1");
  bstring raster_descriptor = bfromcstr("Channel 1 of JPL compressed stokes matrix");
  raster_id = GS_RasterCreate(image_id,raster_name,
                              GS_DATATYPE_SI8,1240,2048,raster_descriptor);

  printf("raster_id=%d\n",raster_id);

  printf("5\n");

  GS_FileClose(file_id);


  // test locking/unlocking file/image/raster separately

  // file:
  printf("=======locking file:\n");
  if(!GS_ObjectLock(file_id)){
    printf("Could not make file read-only.\n");
  }
  printf("File Report:\n");
  if(GS_FileReport(filename,report_string)){
    printf("%s",bdata(report_string));
    bdestroy(report_string);
  } else {
    printf("%s\n",bdata(error_string));
  }
  printf("=======UNlocking file:\n");
  if(!GS_ObjectUnlock(file_id)){
    printf("Could not make file read/write.\n");
  }
  printf("File Report:\n");
  if(GS_FileReport(filename,report_string)){
    printf("%s",bdata(report_string));
    bdestroy(report_string);
  } else {
    printf("%s\n",bdata(error_string));
  }




  // image:
  printf("=======locking image:\n");
  if(!GS_ObjectLock(image_id)){
    printf("Could not make image read-only.\n");
  }
  printf("Image Report:\n");
  report_string = GS_GroupReport(image_id);
  if(bstrlen(report_string)>0){
    printf("%s\n",bdata(report_string));
    bdestroy(report_string);
  } else {
    printf("%s\n",bdata(error_string));
  }
  printf("=======UNlocking image:\n");
  if(!GS_ObjectUnlock(image_id)){
    printf("Could not make image read/write.\n");
  }
  printf("Image Report:\n");
  report_string = GS_GroupReport(image_id);
  if(bstrlen(report_string)>0){
    printf("%s\n",bdata(report_string));
    bdestroy(report_string);
  } else {
    printf("%s\n",bdata(error_string));
  }






  // raster:
  printf("=======locking raster:\n");
  if(!GS_ObjectLock(raster_id)){
    printf("Could not make raster read-only.\n");
  }
  printf("Raster Report:\n");
  report_string = GS_DatasetReport(raster_id);
  if(bstrlen(report_string)>0){
    printf("%s\n",bdata(report_string));
    bdestroy(report_string);
  } else {
    printf("%s\n",bdata(error_string));
  }
  printf("=======UNlocking raster:\n");
  if(!GS_ObjectUnlock(raster_id)){
    printf("Could not make raster read/write.\n");
  }
  printf("Raster Report:\n");
  report_string = GS_DatasetReport(raster_id);
  if(bstrlen(report_string)>0){
    printf("%s\n",bdata(report_string));
    bdestroy(report_string);
  } else {
    printf("%s\n",bdata(error_string));
  }




  

#endif //TEST6


#ifdef TEST7
  printf("\n\nTesting Imagedelete**************************************:\n");


  bstring filename = bfromcstr("test123.h5");
  
  file_id = GS_FileCreate(filename);

  //printf("2\n");
  //report_num_objects_open();


  //printf("file_id=%d\n",file_id);
  if(file_id<0) {
    printf("error: %s\n",bdata(error_string));
    exit(-1);
  } else {
    printf("status: %s\n",bdata(status_string));
  }//endif

  GS_FileClose(file_id);

  //printf("3\n");
  //report_num_objects_open();

  file_id = GS_FileOpen(filename,bsstatic("r+"));
  bstring imagename = bfromcstr("image_aa");
  bstring image_descriptor = bfromcstr("SIR-C Raco 102.4 SRL-1");
  image_id = GS_ImageCreate(file_id,imagename,image_descriptor);

  //printf("image_id=%d\n",image_id);

  //printf("4\n");
   
  bstring raster_name = bfromcstr("Stokes-1");
  bstring raster_descriptor = bfromcstr("Channel 1 of JPL compressed stokes matrix");
  raster_id = GS_RasterCreate(image_id,raster_name,
                              GS_DATATYPE_SI8,1240,2048,raster_descriptor);

  //printf("raster_id=%d\n",raster_id);

  //printf("5\n");

  printf("before ImageDelete:\n");
  report_string = GS_GroupReport(image_id);
  if(bstrlen(report_string)>0){
    printf("%s\n",bdata(report_string));
  } else {
    printf("%s\n",bdata(error_string));
  }

  // should close image and raster:
  if(!GS_ImageClose(image_id)){
    printf("GS_ImageClose failed\n");
    printf("%s\n",bdata(error_string));    
  }

  if(!GS_ImageDelete(file_id,imagename)){
    printf("GS_ImageDelete failed\n");
    printf("%s\n",bdata(error_string));
  }// endif

  printf("after ImageDelete:\n");
  report_string = GS_GroupReport(image_id);
  if(bstrlen(report_string)>0){
    printf("%s\n",bdata(report_string));
  } else {
    printf("%s\n",bdata(error_string));
  }

  if(GS_FileReport(filename,report_string)){
    printf("%s",bdata(report_string));
    bdestroy(report_string);
  } else {
    printf("%s\n",bdata(error_string));
  }

  printf("\nDONE\n");

#endif //TEST7



#ifdef TEST8

  printf("\n\nTesting Rasterdelete**************************************:\n");


  bstring filename = bfromcstr("test123.h5");
  
  file_id = GS_FileCreate(filename);

  //printf("2\n");
  //report_num_objects_open();


  //printf("file_id=%d\n",file_id);
  if(file_id<0) {
    printf("error: %s\n",bdata(error_string));
    exit(-1);
  } else {
    printf("status: %s\n",bdata(status_string));
  }//endif

  GS_FileClose(file_id);

  //printf("3\n");
  //report_num_objects_open();

  file_id = GS_FileOpen(filename,bsstatic("r+"));
  bstring imagename = bfromcstr("image_aa");
  bstring image_descriptor = bfromcstr("SIR-C Raco 102.4 SRL-1");
  printf("DEBUG: file_id=%d, imagename=%s, image_descriptor=%s\n",file_id, bdata(imagename),bdata(image_descriptor));
  image_id = GS_ImageCreate(file_id,imagename,image_descriptor);

  //printf("image_id=%d\n",image_id);

  //printf("4\n");
   
  bstring raster_name = bfromcstr("Stokes-1");
  bstring raster_descriptor = bfromcstr("Channel 1 of JPL compressed stokes matrix");
  raster_id = GS_RasterCreate(image_id,raster_name,
                              GS_DATATYPE_SI8,1240,2048,raster_descriptor);

  printf("raster_id=%d\n",raster_id);

  //printf("5\n");

  printf("before RasterDelete:\n");
  report_string = GS_DatasetReport(raster_id);
  if(bstrlen(report_string)>0){
    printf("%s: %s\n",bdata(raster_name),bdata(report_string));
  } else {
    printf("%s\n",bdata(error_string));
  }

  // should close raster:
  if(!GS_RasterClose(raster_id)){
    printf("GS_RasterClose failed\n");
    printf("%s\n",bdata(error_string));    
  }

  if(!GS_RasterDelete(image_id,raster_name)){
    printf("GS_RasterDelete failed\n");
    printf("%s\n",bdata(error_string));
  }// endif

  printf("after RasterDelete:\n");
  report_string = GS_DatasetReport(raster_id);
  if(bstrlen(report_string)>0){
    printf("%s: %s\n",bdata(raster_name),bdata(report_string));
  } else {
    printf("%s\n",bdata(error_string));
  }

  printf("\nFileReport:\n");
  if(GS_FileReport(filename,report_string)){
    printf("%s",bdata(report_string));
    bdestroy(report_string);
  } else {
    printf("%s\n",bdata(error_string));
  }

  printf("GroupReport:\n");
  report_string = GS_GroupReport(image_id);
  if(bstrlen(report_string)>0){
    printf("%s\n",bdata(report_string));
  } else {
    printf("%s\n",bdata(error_string));
  }



  printf("\nDONE\n");


#endif //TEST8

}
