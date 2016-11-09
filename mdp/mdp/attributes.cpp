#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>
#include <boost/filesystem.hpp>
//#include <boost/lexical_cast.hpp>

using namespace boost::filesystem;
using namespace std;

#include "ImageHDF5.hpp"
#include "RasterHDF5.hpp"
#include "MetadataHDF5.hpp"
#include "Exceptions.hpp"
#include "attributes.hpp"

extern "C"
{
  #include "hdf5_extras.h"
}


namespace GeoSci {
    std::string query_attribute(hid_t obj, std::string key)
    {
        AttributeErrorException AttributeError;
        
        bstring bkey=bfromcstr(key.c_str());
        bstring bvalue=bfromcstr("");
        int cset;
        if( H5ATTRget_attribute_string( obj, bkey,bvalue,
                                        &cset) == ERROR){
            bdestroy(bkey);
            bdestroy(bvalue);
            throw AttributeError;
        }
        bdestroy(bkey);
        return string(bdata(bvalue));
    }// endfunc: query_attribute

    void add_attribute(hid_t obj, std::string key, std::string value)
    {
        AttributeErrorException AttributeError;
        
        bstring bkey=bfromcstr(key.c_str());
        bstring bvalue = bfromcstr(value.c_str());
        if(!GS_UpdateMetadata(obj,bkey,bvalue)){
            bassignformat(error_string,"add_attribute: Error writing atribute.");
            bdestroy(bkey);
            bdestroy(bvalue);
            throw AttributeError;
        }
        bdestroy(bkey);
        bdestroy(bvalue);
    }//endfunc: add_attribute
    
    void modify_attribute(hid_t obj, std::string key, std::string value)
    {
        AttributeDoesNotExistException AttributeDoesNotExist;
        
        bstring bkey=bfromcstr(key.c_str());
        
        if(H5ATTRfind_attribute(obj,bkey)>0){
            bdestroy(bkey);
            add_attribute(obj,key,value);
        } else {
            bdestroy(bkey);
            throw AttributeDoesNotExist;
        }       
    }//endfunc: modify_attribute
    
}
// end namespace GeoSci
