#include <iostream>
#include <string>
using namespace std;

#include "FileHDF5.hpp"
#include "ImageHDF5.hpp"
using namespace GeoSci;

int main() {
  try{
    FileHDF5 *f1 = FileHDF5::create("test2.gs");
    cout << "at a0: successfully created file test2.gs" << endl;
    f1->close();
    cout << "at a1: successfully closed file test2.gs" << endl;
    
    
    FileHDF5 *f2 = FileHDF5::open("test2.gs","r+");
    cout << "at a2: successfully opened file test2.gs" << endl;
    ImageHDF5 *i1 = f2->create_image("test24.img");
    cout << "at a3: successfully created image test24.img" << endl;
    i1->close();
    cout << "at a4: successfully closed image test24.img" << endl;
    ImageHDF5 *i11 = f2->create_image("test29.img");
    cout << "at a33: successfully created image test29.img" << endl;
    i11->close();
    cout << "at a444: successfully closed image test29.img" << endl;
    f2->close();
    cout << "at a5: successfully closed file test2.gs" << endl;
    

    FileHDF5 *f3 = FileHDF5::open("test2.gs","r+");
    cout << "at a6: successfully opened file test2.gs" << endl;
    ImageHDF5 *i2 = f3->open_image("test24.img","r+");
    cout << "at a7: successfully opened image test24.img" << endl;
    i2->close();
    cout << "at a8: successfully closed image test24.img" << endl;
    f3->close();
    cout << "at a9: successfully closed file test2.gs" << endl;
  }
  catch(...){
    cout << "error..."<<endl;
  }

  
}// end main()
