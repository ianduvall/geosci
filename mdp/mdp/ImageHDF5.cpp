#include <iostream>
//#include <fstream>
//#include <streambuf>
#include <string>
#include <boost/filesystem.hpp>
//#include <boost/lexical_cast.hpp>
// note: the above inclde screwed up
//       my C typedefs of c*_t from hdf5mine.h
//       no idea why....?????

using namespace boost::filesystem;
using namespace std;



#include "RasterHDF5.hpp"
#include "ImageHDF5.hpp"
#include "FileHDF5.hpp"
#include "MetadataHDF5.hpp"
#include "Exceptions.hpp"
#include "attributes.hpp"

extern "C"
{
  #include "hdf5_extras.h"
}


//#define DEBUG

namespace GeoSci {


void ImageHDF5::printMyType() {
	cout << "ImageHDF5 is my type!" << endl;
	return;
}

bool ImageHDF5::isGeoSciHDF5Image(hid_t group_id) {

  if(query_attribute(group_id,"filetype")!="GEOSCI" ||
	 query_attribute(group_id,"filelib")!="HDF5") {
	  return false;
  }
  return true;
  
} // end isGeoSciHDF5Image()

// create a new image
void ImageHDF5::createImage(const string& name) {

	ImageExistsException ImageExists;
	ImageCreationErrorException ImageCreationError;

	// creates a new "image", die if already exists, ....
	string groupname = "/"+name;

	Exception::dontPrint();

	//cout << "imagehdf5::create: at 1, groupname="<<groupname<<endl;


	// see if it already exists:
	if(!GS_CheckUniqueName(this->parentFile->file, name.c_str())){
		throw ImageExists; // A group already exists with this name.
	}

	//cout << "imagehdf5::create: at 2"<<endl;


	// create (and open) the image group:
	hid_t image_id;
	bstring descr = bfromcstr("No Descriptor");
	bstring bname = bfromcstr(name.c_str());
	image_id = GS_ImageCreate(this->parentFile->file, bname, descr);
	if(image_id < 0) {
		bdestroy(bname);
		bdestroy(descr);
		throw ImageCreationError;
	}
	bdestroy(bname);
	bdestroy(descr);



	//cout << "imagehdf5::create: at 3"<<endl;

	add_attribute(image_id,"filetype","GEOSCI");
	add_attribute(image_id,"filelib","HDF5");



	//cout << "imagehdf5::create: at 5"<<endl;

	// return the new object
	this->image_id = image_id;
	this->image_name = name;
	//cout << "imagehdf5::create: at 6"<<endl;

	return;
}// end createImage


// open an existing Image:
void ImageHDF5::openImage(const string& name, const string& access) {
	
	ImageOpenErrorException ImageOpenError;
	NotGeoSciImageException NotGeoSciImage;

	
	//cout << "imagehdf5::open: at 0"<<endl;

	string groupname = "/"+name;

	Exception::dontPrint();

	//cout << "imagehdf5::open: at 1: groupname="<<groupname<<endl;

	// try to open it to see if it already exists:
	bstring bname = bfromcstr(name.c_str());
	bstring baccess = bfromcstr(access.c_str());
	hid_t image_id;
	//YET: iamgeopen needs to take an aacces arg or soething......
	image_id = GS_ImageOpen(this->parentFile->file, bname);
	if(image_id < 0){
		bdestroy(bname);
		bdestroy(baccess);
		throw ImageOpenError;
	}
	if(!isGeoSciHDF5Image(image_id)){
		throw NotGeoSciImage;
	}

	//cout << "imagehdf5::open: at 3"<<endl;


	// create Image object to return:
	this->image_id = image_id;
	//cout << "imagehdf5::open: at 4"<<endl;

	return;
} // end open()


// close an existing Image:
// just deletes the c++ object
void ImageHDF5::closeImage() {
	GS_ImageClose(this->image_id);
	delete this;
}// end close()

// must be closed, deletes the Image (the group and anything inside the group)
void ImageHDF5::destroyImage(const string& name) {
		
	ImageDestroyErrorException ImageDestroyError;
      
    string groupname = "/"+name;

    
    //first check if its open, if so close it.
    // look thru list of every open Image object.
    // close it if the filename entry is equal-to the 'ap' variable
    // yet....

    Exception::dontPrint();

    bstring bname = bfromcstr(name.c_str());
    if(!GS_ImageDelete(this->parentFile->file, bname)){
        bdestroy(bname);
        throw ImageDestroyError;
    }
    bdestroy(bname);
    
} // end destroy()


RasterHDF5 *ImageHDF5::createRaster(const std::string& name, const long int *sizes, int datatype) {
	RasterHDF5 *r = new RasterHDF5();
	r->parentImage = this;
	r->createRaster(name, sizes, datatype);
	return r;
}// end create_raster()


RasterHDF5 *ImageHDF5::openRaster(const std::string& datasetName) {
	RasterHDF5 *r = new RasterHDF5();
	r->parentImage = this;
	r->openRaster(datasetName);
	return r;
}// end open_raster()


void ImageHDF5::destroyRaster(const std::string& datasetName) {
	RasterHDF5 *r = new RasterHDF5();
	r->parentImage = this;
	r->destroyRaster(datasetName);
	r->closeRaster();
	return;
}// end destroy_raster()



}// end namespace GeoSci
