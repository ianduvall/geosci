// StringAttributeHDF5.cpp

#include "H5Cpp.h"
#include <string>

#include "StringAttributeHDF5.hpp"

using namespace H5;
using namespace std;



namespace GeoSci {

    // constructor: saves a ptr to the Attribute, using
    // parent Attribute constructor.
    StringAttributeHDF5::StringAttributeHDF5 (H5::Attribute att)
    {
        the_h5attribute = new H5::Attribute(att);
    }
    

    // uses the parent Attribute read method,
    // specialized for a string type
    string StringAttributeHDF5::read() 
    {
      StrType type(PredType::C_S1, H5T_VARIABLE);
      string read_value("");
      the_h5attribute->read(type,read_value);
      return read_value;
    }

    // uses the parent Attribute write method,
    // specialized for a string value
    void StringAttributeHDF5::write(std::string str) 
    {
      StrType type(PredType::C_S1, H5T_VARIABLE);
      the_h5attribute->write(type,str);
    }
    
    // uses the parent Attribute write method,
    // specialized for a string value
    void StringAttributeHDF5::append(std::string str) 
    {
      StrType type(PredType::C_S1, H5T_VARIABLE);
      the_h5attribute->write(type,read()+str);
    }

    void StringAttributeHDF5::close()
    {
        // not sure what to do here....
        int i=0;
    }
    

    StringAttributeHDF5::~StringAttributeHDF5()
    {
        delete the_h5attribute;
    }
    
    


}//end-GeoSci namespace
