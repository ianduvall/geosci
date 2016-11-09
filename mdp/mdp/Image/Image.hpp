#ifndef IMAGE_HPP_
#define IMAGE_HPP_

#include <iostream>
#include <string>
#include <map>
//#include "../File/File.hpp"

namespace GeoSci {

class Raster;
class File;

class Image {
public:

	virtual ~Image(){};
	virtual void printMyType() = 0;
	
	virtual void createImage(const std::string&) = 0;
	virtual void openImage(const std::string&, const std::string&) = 0;
	virtual void destroyImage(const std::string&) = 0;
	virtual void closeImage() = 0;
	
	
	virtual Raster *createRaster(const std::string& name, const long int *sizes, int datatype) = 0;
    virtual Raster *openRaster(const std::string& datasetName) = 0;
    virtual void destroyRaster(const std::string& datasetName) = 0;
    
    
}; // Image

} // GeoSci

#endif
