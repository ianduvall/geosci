#include <iostream>
#include <string>

#include "FileHDF5.hpp"
#include "ImageHDF5.hpp"
#include "RasterHDF5.hpp"
#include "Exceptions.hpp"
using namespace GeoSci;
using namespace std;


#include <sys/types.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <complex.h>

#include "globals.h"
#include "hdf5_extras.h"


int main() {
    int i,j;
    
    long int sizes[2];

    sizes[0]=20;
    sizes[1]=20;

	GS_init();

    long int offsets[2];
    offsets[0]=0;
    offsets[1]=0;

    u_int8_t buffer[20*20];
    u_int8_t buffer2[20*20];
    for(i=0;i<20;i++){
        for(j=0;j<20;j++){
            buffer[20*j+i]=20*j+i;
            buffer2[20*j+i]=0;
            if(i==5)printf("(%d,%d)=%d",i,j,buffer[20*j+i]);
        }
      if(i==5)printf("\n");
    }
    printf("\n");
    
    

  try {
   FileHDF5 *f1 = FileHDF5::create("test2.gs");
   cout << "successfully created file test2.gs" << endl;
   f1->close();
   cout << "successfully closed file test2.gs" << endl;


   FileHDF5 *f2 = FileHDF5::open("test2.gs","r+");
  cout << "successfully opened file test2.gs" << endl;
  ImageHDF5 *i1 = f2->create_image("test24.img");
  cout << "successfully created image test24.img" << endl;
  i1->close();
  cout << "successfully closed image test24.img" << endl;
  f2->close();
  cout << "successfully closed file test2.gs" << endl;


  FileHDF5 *f3 = FileHDF5::open("test2.gs","r+");
  cout << "successfully opened file test2.gs" << endl;
  ImageHDF5 *i2 = f3->open_image("test24.img","r+");
  cout << "successfully opened image test24.img" << endl;
  RasterHDF5 *r1 = i2->create_raster("test26.ras",sizes,GS_DATATYPE_UI8);
  cout << "successfully created raster test26.ras" << endl;


  
  //write to the raster here....
  i=r1->write8U(offsets,sizes,buffer);
  cout << "successfully wrote to raster test26.ras, i=" <<i<< endl;
  



  


  
  r1->close();
  cout << "successfully closed raster test26.ras" << endl;
  i2->close();
  cout << "successfully closed image test24.img" << endl;
  f3->close();
  cout << "successfully closed file test2.gs" << endl;


  FileHDF5 *f31 = FileHDF5::open("test2.gs","r+");
  cout << "successfully opened file test2.gs" << endl;
  ImageHDF5 *i21 = f31->open_image("test24.img","r+");
  cout << "successfully opened image test24.img" << endl;
  RasterHDF5 *r11 = i21->open_raster("test26.ras");
  cout << "successfully opened raster test26.ras" << endl;



  // read from raster here.
  i=r11->read8U(offsets,sizes,buffer2);
  cout << "successfully read from raster test26.ras: i="<<i << endl;

  for(i=0;i<20;i++){
      for(j=0;j<20;j++){
          if(i==5)printf("(%d,%d)=%d",i,j,buffer2[20*j+i]);
      }
      if(i==5)printf("\n");
  }
  printf("\n");
    
  
  r11->close();
  cout << "successfully closed raster test26.ras" << endl;
  i21->close();
  cout << "successfully closed image test24.img" << endl;
  f31->close();
  cout << "successfully closed file test2.gs" << endl;

  }
catch (exception& e)
  {
    cout << e.what() << '\n';
  }  
  catch (...) {
    cout << "error"<< endl;
  }
  
}// end main()
