#ifndef ATTRIBUTES_HPP_ 
#define ATTRIBUTES_HPP_

#include <string>

#include "H5Cpp.h"
using namespace H5;

  
//------------------------------------------------------
namespace GeoSci {
    std::string query_attribute(hid_t obj, std::string key);
    std::string modify_attribute(hid_t obj, std::string key);
    void add_attribute(hid_t obj, std::string key, std::string value);
 
}// end GeoSci namespace
 
#endif
// end ATTRIBUTES_HPP_
