#include <iostream>
#include <string>

#include "FileHDF5.hpp"
#include "ImageHDF5.hpp"
using namespace GeoSci;

int main() {

  try{

    //should work:
    FileHDF5 *f1 = FileHDF5::create("test2.gs");
    cout << "at a0: created test2.gs output file." << endl;
    ImageHDF5 *f1i = ImageHDF5::create(f1, "test3.img");
    cout << "at a1: created test3.img" << endl;
    f1i->close();
    cout << "at a2: closed test3.img" << endl;
    
    
    // should work because it already exists:
    ImageHDF5 *f2i = ImageHDF5::open(f1,"test3.img","r+");
    f2i->close();
    cout << "at a3: opened and closed test3.img" << endl;
    
    //should work:
    ImageHDF5 *f3i = ImageHDF5::create(f1,"test33.img");
    f3i->close();
    cout << "at a4: created and closed test33.img" << endl;
    
    try {
      //should NOT work:
      ImageHDF5 *f333i = ImageHDF5::create(f1,"test33.img");
      f333i->close();
      cout << "at a5.1: erroneaously created test33.img"<<endl;
    } 
    catch(...){
      cout << "at a5.2: correctly refused to create already-created image: test33.img\n";
    }
    
    
    
    ImageHDF5::destroy(f1,"test33.img");
    cout << "at a6: destroyed test33.img" << endl;
    
    f1->close();
    cout << "at a7: closed h5 file: test2.gs" << endl;
    
  }
  catch(...){
    cout << "error: caught an exception...."<<endl;
  }
  
}// end main()
