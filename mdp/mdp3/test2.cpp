#include <iostream>
#include <string>
#include "H5Cpp.h"

#include "StringAttributeHDF5.hpp"
#include "FileHDF5.hpp"
using namespace GeoSci;
using namespace std;
using namespace H5;


int main() {
  try{     
      
      cout << "starting..." << endl;

      FileHDF5 file = FileHDF5("test2.h5", H5F_ACC_TRUNC);
      cout << "test2.h5 created"<< endl;

      file.createFileAttributes("example descriptor");
      cout << "test2.h5 attributes written"<< endl;
      

      //DataSpace attr_dataspace = DataSpace(H5S_SCALAR);
      //cout << "attr_dataspace created"<< endl;

      //StrType strdatatype(PredType::C_S1, 256);
      //cout << "strddatatype created"<< endl;
      
      StringAttributeHDF5 att1 = file.createStringAttribute("filetype33");
      cout << "att1 created"<< endl;

      att1.write("the value of the attribute");
      cout << "wrote value to att1"<<endl;

      string read_value("");
      read_value = att1.read();
      cout << "read value from att1"<<endl;
      cout << "value read=|"<<read_value<<"|"<<endl;


      att1.append("\n more stuff in the value;");
      read_value = att1.read();
      cout << "read value from att1"<<endl;
      cout << "value read=|"<<read_value<<"|"<<endl;
      

      att1.close();
      
      
      cout << "done..." << endl;
      

  }
  catch (exception& e)
  {
      cout << e.what() << '\n';
  }  
  catch(...) {
      cout << "error: caught an exception...."<<endl;
  }
  
}// end main()
