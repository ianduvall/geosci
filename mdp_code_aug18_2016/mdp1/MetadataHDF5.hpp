// MetadataHDF5.hpp, by Laura Huang, April, 2016

#ifndef METADATAHDF5_HPP_ 
#define METADATAHDF5_HPP_

#include <vector>
#include <string>

#include "H5Cpp.h"
using namespace H5;


namespace GeoSci {

    // The metdata class takes as input the pathname of an object within an
    // existing hdf5 file. It then deals with the attributes that are attached
    // to that object.
    // Requires that the file within which the Metadata is, already be open.
    class MetadataHDF5 {
    private:
        H5Location *thelocation;
        
    public:
        MetadataHDF5();
        ~MetadataHDF5();
        static MetadataHDF5 *create(H5Location *thelocation);
        static MetadataHDF5 *open(H5Location *thelocation);
        // no destroy yet.
        void close();
        void add_attribute(std::string attribute_name, std::string attribute_value);
        void modify_attribute(std::string attribute_name, std::string attribute_value);
        std::string query_attribute(std::string attribute_name);
        
    };

}// end namespace

#endif
// end METADATAHDF5_HPP_
