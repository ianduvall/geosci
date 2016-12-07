#ifndef RASTERHDF5_HPP_
#define RASTERHDF5_HPP_

#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>
#include <boost/filesystem.hpp>
using namespace boost::filesystem;
using namespace std;

#include "H5Cpp.h"
using namespace H5;

//#include "ImageHDF5.hpp"
#include "Raster/Raster.hpp"

namespace GeoSci {
	
class ImageHDF5;

class RasterHDF5 : public GeoSci::Raster {
private:
	static bool isGeoSciHDF5Raster(hid_t raster_id);
	
public:
	ImageHDF5 *parentImage;
	hid_t raster_id;
	
	void printMyType();
    //RasterHDF5() : raster_id(0) {};
    
	void createRaster(const std::string& name, const long int *sizes, int datatype);
	void closeRaster();
	void destroyRaster(const std::string& datasetName);
	void openRaster(const std::string& datasetName);
	
	
	int write8U(long int *offsets, long int *sizes, void *buffer);
	int read8U(long int *offsets, long int *sizes, void *buffer);
	

};// end class RasterHDF5

}// end namespace GeoSci

#endif
//end RASTERHDF5_HPP_
