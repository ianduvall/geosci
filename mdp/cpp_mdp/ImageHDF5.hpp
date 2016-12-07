#ifndef IMAGEHDF5_HPP_
#define IMAGEHDF5_HPP_

#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>
#include <boost/filesystem.hpp>
using namespace boost::filesystem;
using namespace std;

#include "H5Cpp.h"
using namespace H5;

//#include "FileHDF5.hpp"
#include "Image/Image.hpp"
//#include "Raster/Raster.hpp"
#include "RasterHDF5.hpp"

namespace GeoSci {
	
class FileHDF5;

class ImageHDF5 : public GeoSci::Image {
private:
	static bool isGeoSciHDF5Image(hid_t group_id);
	
protected:
	string image_name;

public:
	FileHDF5 *parentFile;
	hid_t image_id;
    void printMyType();
    //ImageHDF5() : image_id(0), image_name("") {};
	
	
    void createImage(const std::string& name);
    void openImage(const std::string& name, const std::string& access);
    void closeImage();
    void destroyImage(const std::string& name);
    
    
	RasterHDF5 *createRaster(const std::string& name, const long int *sizes, int datatype);
    RasterHDF5 *openRaster(const std::string& datasetName);
    void destroyRaster(const std::string& datasetName);
    
    
};// end class ImageHDF5
  

}// end namespace GeoSci

#endif
//end IMAGEHDF5_HPP_
