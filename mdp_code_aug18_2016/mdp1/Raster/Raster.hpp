#ifndef RASTER_HPP_
#define RASTER_HPP_

#include <iostream>
#include <string>
#include <map>
#include "../Image/Image.hpp"

namespace GeoSci {

class Raster {
public:
	virtual ~Raster(){};
	virtual void printMyType() = 0;
	
	virtual void createRaster(const std::string& name, const long int *sizes, int datatype) = 0;
	virtual void closeRaster() = 0;
	virtual void destroyRaster(const std::string& datasetName) = 0;
	virtual void openRaster(const std::string& datasetName) = 0;
	
	// can we abstract this into a generic write function?
	virtual int write8U(long int *offsets, long int *sizes, void *buffer) = 0;
	virtual int read8U(long int *offsets, long int *sizes, void *buffer) = 0;
	
}; // Raster

} // GeoSci

#endif
