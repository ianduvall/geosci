#include <iostream>
#include <string>

#include "FileHDF5.hpp"
#include "ImageHDF5.hpp"
#include "RasterHDF5.hpp"
using namespace GeoSci;
using namespace std;

int main() {
  try {
    FileHDF5 *f1 = FileHDF5::create("test2.gs");
    cout << "at a0: successfully created file test2.gs" << endl;    
    ImageHDF5 *i1 = f1->create_image("test24.img");
    cout << "at a1: successfully opened image test24.img" << endl;

    RasterHDF5 *r1 = RasterHDF5::create(i1,"test5.ras");
    cout << "at a2: successfully created raster test5.ras" << endl;    
    r1->close();
    cout << "at a3: successfully closed raster test5.ras" << endl;

    RasterHDF5 *r2 = RasterHDF5::open(i1,"test5.ras");
    cout << "at a4: successfully opened raster test5.ras" << endl;    
    r2->close();
    cout << "at a5: successfully closed raster test5.ras" << endl;

    i1->close();
    cout << "at a6: successfully closed image test24.img" << endl;
    f1->close();
    cout << "at a7: successfully closed file test2.gs" << endl;

    }
    catch (...) {
      cout << "error..."<<endl;
    }
  
  
}// end main()
