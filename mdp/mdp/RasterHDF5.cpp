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


//#define DEBUG


namespace GeoSci {

  
bool RasterHDF5::isGeoSciHDF5Raster(hid_t raster_id) {

  if(query_attribute(raster_id,"filetype")!="GEOSCI" ||
	 query_attribute(raster_id,"filelib")!="HDF5") {
	  return false;
  }
  return true;
  
}// end isGeoSciHDF5Raster()

void RasterHDF5::printMyType() {
	cout << "RasterHDF5 is my type!" << endl;
	return;
}

// create a new raster dataset
void RasterHDF5::createRaster(const std::string& datasetName, const long int *sizes, int datatype) {
	
	// creates a new "raster", die if already exists, ....
	
	RasterCreationErrorException RasterCreationError;
	
#ifdef DEBUG      
	cout << "RasterHDF5::Create: 1 sizes="<<sizes[0]<<", "<<sizes[1]<<endl;
#endif      
	hid_t raster_id;
	bstring dname = bfromcstr(datasetName.c_str());
	bstring desc = bfromcstr("no descriptor");
	raster_id = GS_RasterCreate(this->parentImage->image_id, dname, datatype, sizes[0], sizes[1], desc);
	bdestroy(dname);
	bdestroy(desc);
	if (raster_id < 0) {
		throw RasterCreationError;
	}

#ifdef DEBUG
	cout << "RasterHDF5::Create: 2\n";
#endif

	//cout << "rasterhdf5::create: at 2"<<endl;
	//cout << "rasterhdf5::create: at 3"<<endl;
	
	add_attribute(raster_id,"filetype","GEOSCI");
	add_attribute(raster_id,"filelib","HDF5");
#ifdef DEBUG    
	cout << "RasterHDF5::Create: 4\n";
#endif
	
	//cout << "rasterhdf5::create: at 5"<<endl;
	
	this->raster_id = raster_id;
    //cout << "rasterhdf5::create: at 6"<<endl;
#ifdef DEBUG
          cout << "RasterHDF5::Create: 5\n";
#endif
	return;
}// end create()

// close an existing raster file:
// the file is always left in a closed state, so:
// just deletes the c++ object
void RasterHDF5::closeRaster() {
	GS_RasterClose(this->raster_id);
	delete this;
}// end close()


// open an existing raster dataset:
void RasterHDF5::openRaster(const std::string& datasetName) {
	
	RasterOpenErrorException RasterOpenError;
	NotGeoSciRasterException NotGeoSciRaster;
	
	Exception::dontPrint();
	
	//cout << "rasterhdf5::open: at 1: name="<<name<<endl;
	
	hid_t raster_id;
    bstring dname = bfromcstr(datasetName.c_str());
    raster_id=GS_RasterOpen(this->parentImage->image_id, dname);
    bdestroy(dname);

    if (raster_id < 0) {
        throw RasterOpenError;
    }
    
    if (!isGeoSciHDF5Raster(raster_id)) {
      throw NotGeoSciRaster;
    }
    
    this->raster_id = raster_id;
    
    return;
}// end open()

  
// must be closed, deletes the file, and the assoc system metadata
void RasterHDF5::destroyRaster(const std::string& datasetName) {
	
	RasterDestroyErrorException RasterDestroyError;
    Exception::dontPrint();
    
    bstring dname = bfromcstr(datasetName.c_str());
    if (!GS_RasterDelete(this->parentImage->image_id,dname)) {
		bdestroy(dname);
		throw RasterDestroyError;
	}
    bdestroy(dname);
    
    return;
}// end destroy()



int RasterHDF5::write8U(long int *offsets, long int *sizes, void *buffer) {
	return GS_DatasetWrite(this->raster_id, offsets, sizes, GS_DATATYPE_UI8, buffer);
}// end write()

int RasterHDF5::read8U(long int *offsets, long int *sizes, void *buffer) {
	return GS_DatasetRead(this->raster_id, offsets, sizes, GS_DATATYPE_UI8, buffer);
}// end write()


}// end namespace GeoSci
