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



namespace GeoSci {

  class ImageHDF5;

  class RasterHDF5 {

  public:
      hid_t raster_id;
      
      static RasterHDF5 *create(ImageHDF5 *image, std::string name,
                                const long int *sizes, int datatype);
      static RasterHDF5 *open(ImageHDF5 *image, std::string name);
      void close();
      static void destroy(ImageHDF5 *image, std::string name);
      int write8U(long int *offsets, long int *sizes, void *buffer);
      int read8U(long int *offsets, long int *sizes, void *buffer);
      
      
  private:
      static bool isGeoSciHDF5Raster(hid_t raster_id);
      
  };// end class RasterHDF5
  

}// end namespace GeoSci

#endif
//end RASTERHDF5_HPP_
