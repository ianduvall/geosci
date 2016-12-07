// MetadataHDF5.cpp, by Laura Huang, April, 2016

#include <string>
#include <iostream>

#include <boost/filesystem.hpp>

#include "H5Cpp.h"
#include <H5Attribute.h>

#include "Exceptions.hpp"
#include "MetadataHDF5.hpp"

using namespace boost::filesystem;
using namespace H5;
using namespace std;

namespace GeoSci {
    MetadataHDF5::MetadataHDF5(){}
    MetadataHDF5::~MetadataHDF5(){}

    MetadataHDF5 *MetadataHDF5::create(H5Location *thelocation) {  
        // nothing to create inside the file, yet.
        // just keep track of file and object name for future use.
        MetadataHDF5 *m = new MetadataHDF5();
        m->thelocation = thelocation;
        return m;
    }//endfunc: create


    MetadataHDF5 *MetadataHDF5::open(H5Location *thelocation) {

        // nothing to open inside the file, yet.
        // just keep track of file and object name for future use.
        MetadataHDF5 *m = new MetadataHDF5();
        m->thelocation = thelocation;
        return m;
    }//endfunc: open

    void MetadataHDF5::close() {
        delete this;
    }

    // no destroy yet.

    void MetadataHDF5::add_attribute(std::string attribute_name, std::string attribute_value) 
    {
        AttributeExistsException AttributeExists;
        
        H5std_string ATTR_NAME(attribute_name);
        H5std_string ATTR_VAL(attribute_value);
        
        // Create new string datatype for attribute
        StrType strtype(PredType::C_S1, H5T_VARIABLE);

        //check that attribute does not exist
        if (!thelocation->attrExists(ATTR_NAME)) {
            // Create new dataspace for attribute
            DataSpace attr_dataspace = DataSpace(H5S_SCALAR);
            Attribute a = thelocation->createAttribute(ATTR_NAME, strtype, attr_dataspace);
            a.write(strtype, ATTR_VAL);
            //cout << ATTR_NAME << endl;
            //cout << ATTR_VAL << endl;
            a.close();
        }
        else {
            throw AttributeExists;
        }
    }// endfunc: add_attribute


    void MetadataHDF5::modify_attribute(std::string attribute_name, std::string attribute_value)
    {
        AttributeDoesNotExistException AttributeDoesNotExist;

        H5std_string ATTR_NAME(attribute_name);
        H5std_string ATTR_VAL(attribute_value);
		StrType strtype(PredType::C_S1, H5T_VARIABLE);
        //check that attribute exists:
        if (thelocation->attrExists(ATTR_NAME)) {
            Attribute a = thelocation->openAttribute(ATTR_NAME);
            a.write(strtype, ATTR_VAL);
            a.close();
        }
        else {
            throw AttributeDoesNotExist;
        }

    }// endfunc: modify_attribute

    
    string MetadataHDF5::query_attribute(std::string attribute_name)
    {
        AttributeDoesNotExistException AttributeDoesNotExist;

        H5std_string ATTR_NAME(attribute_name);
        H5std_string ATTR_VAL("");
        //char bfr[1000];
        // Create new string datatype for attribute
        StrType strtype(PredType::C_S1, H5T_VARIABLE);

        //check that attribute exists:
        if (thelocation->attrExists(ATTR_NAME)) {
            Attribute a = thelocation->openAttribute(ATTR_NAME);
            a.read(strtype, ATTR_VAL);
            //cout << "attrval after: " << ATTR_VAL << endl;
            a.close();
        }
        else {
            throw AttributeDoesNotExist;
        }
        return string(ATTR_VAL);

    }//endfunc: query_attribute
    
        
        
}//end namespace
        
