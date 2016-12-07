#ifndef STRINGATTRIBUTEHDF5_HPP_ 
#define STRINGATTRIBUTEHDF5_HPP_

#include <vector>
#include <string>

#include "H5Cpp.h"
using namespace H5;
using namespace std;


  
//------------------------------------------------------
namespace GeoSci {

    class StringAttributeHDF5: public H5::Attribute
    {
    private:
        H5::Attribute *the_h5attribute;
        
    public:
        StringAttributeHDF5(H5::Attribute att);                
        string read();
        void write(std::string str);
        void append(std::string str);
        void close();
        ~StringAttributeHDF5();

    };// end class: StringAttributeHDF5


}// end GeoSci namespace

#endif
// end STRINGATTRIBUTEHDF5_HPP_
