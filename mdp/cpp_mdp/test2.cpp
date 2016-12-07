#include <iostream>
#include <string>

#include "FileHDF5.hpp"
#include "hdf5_extras.h"
using namespace GeoSci;
using namespace std;



int main() {
	
  try{     
      
cout << "starting..." << endl;
   FileHDF5 *f1 = FileHDF5::create("test2.gs");
cout << "at a0: successfully created file: test2.gs" << endl;
   f1->close();
cout << "at a1: successfully closed test2.gs" << endl;


FileHDF5 *f2 = FileHDF5::open("test2.gs","r+");
cout << "at a2: successfully opened test2.gs" << endl;
  f2->close();
cout << "at a3: successfully closed test2.gs" << endl;

  FileHDF5 *f3 = FileHDF5::create("test22.gs");
cout << "at a4: successfully created file: test22.gs" << endl;
  f3->close();
cout << "at a5: successfully closed test22.gs" << endl;

  FileHDF5::destroy("test22.gs");
cout << "at a5: successfully destroyed test22.gs" << endl;
 }
catch (exception& e)
  {
    cout << e.what() << '\n';
  }  
  catch(...) {
    cout << "error: caught an exception...."<<endl;
  }
  
}// end main()
