#include <iostream>
#include <string>
#include "H5Cpp.h"

#include "StringAttributeHDF5.hpp"
using namespace GeoSci;
using namespace std;
using namespace H5;


int main() {
  try{     
      
      cout << "starting..." << endl;

      H5File file = H5File("test1.h5", H5F_ACC_TRUNC);
      cout << "test1.h5 created"<< endl;

      DataSpace attr_dataspace = DataSpace(H5S_SCALAR);
      cout << "attr_dataspace created"<< endl;

      StrType strdatatype(PredType::C_S1, 256);
      cout << "strddatatype created"<< endl;
      
      Attribute att1 = file.createAttribute("filetype",strdatatype,attr_dataspace);
      cout << "att1 created"<< endl;

      att1.write(strdatatype,"the value of the attribute");
      cout << "wrote value to att1"<<endl;

      string read_value("");
      att1.read(strdatatype,read_value);
      cout << "read value from att1"<<endl;

      cout << "value read=|"<<read_value<<"|"<<endl;
      
      
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
