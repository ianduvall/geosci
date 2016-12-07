#include <iostream>
#include <string>

#include "FileHDF5.hpp"
#include "ImageHDF5.hpp"
#include "RasterHDF5.hpp"
#include "Exceptions.hpp"
using namespace GeoSci;
using namespace std;

int main() {

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
  RasterHDF5 *r1 = i2->create_raster("test26.ras");
  cout << "successfully created raster test26.ras" << endl;
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
