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


#include "RasterHDF5.hpp"

namespace GeoSci {

  class FileHDF5;

  class ImageHDF5 {

  public:
    hid_t image_id;

    static ImageHDF5 *create(FileHDF5 *file, std::string name);
      static ImageHDF5 *open(FileHDF5 *file, std::string name, std::string access);
    void close();
    static void destroy(FileHDF5 *file, std::string name);

      RasterHDF5 *create_raster(string name, const long int *sizes, int datatype);
    RasterHDF5 *open_raster(string name);
    void destroy_raster(string name);
      
  private:
    static bool isGeoSciHDF5Image(hid_t group_id);

  };// end class ImageHDF5
  

}// end namespace GeoSci

#endif
//end IMAGEHDF5_HPP_
