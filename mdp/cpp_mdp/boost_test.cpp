/*
#define BOOST_TEST_MODULE simple
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(testSimple)
{
   BOOST_CHECK_EQUAL(true, true);
}
*/

#define BOOST_TEST_MODULE Boost_Test_Module

#include <boost/test/included/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <string>

#include "Raster/Raster.hpp"
#include "Image/Image.hpp"
#include "File/File.hpp"
#include "Exceptions.hpp"

extern "C" {
	#include "hdf5_extras.h" // GS_DATATYPE_UI8
};

using namespace boost::filesystem;
using namespace boost::unit_test;
using namespace GeoSci;
using namespace std;


// FILE TEST SUIT
BOOST_AUTO_TEST_SUITE( File_Test_Suit )

BOOST_AUTO_TEST_CASE( File_Test_Create ) {
	string filename = "boost_test_create.h5";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
    File *myFile = nullptr;
    BOOST_REQUIRE_NO_THROW(myFile = File::create(filename, "HDF5"));
    BOOST_REQUIRE(myFile != nullptr);
    myFile->closeFile();
}

BOOST_AUTO_TEST_CASE( File_Test_Create_Exists ) {
    // tests file create for directory that already exists
	string filename = "boost_test_create.h5";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}

	// create blank file
	ofstream some_file("./"+filename);
	some_file.close();
	
	File *myFile = nullptr;
    BOOST_REQUIRE_THROW(myFile = File::create(filename, "HDF5"), FileExistsException);
    BOOST_CHECK(myFile == nullptr);
}

BOOST_AUTO_TEST_CASE( File_Test_Create_Empty_Filename ) {
	// tests file create with empty filename
    string filename = "";
	
	File *myFile = nullptr;
    BOOST_REQUIRE_THROW(myFile = File::create(filename, "HDF5"), FileInvalidNameException);
    BOOST_CHECK(myFile == nullptr);
}

BOOST_AUTO_TEST_CASE( File_Test_Create_Invalid_Filename ) {
    // tests file create with invalid extension
    string filename = "name.wrong";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
	File *myFile = nullptr;
    BOOST_REQUIRE_THROW(myFile = File::create(filename, "HDF5"), FileInvalidNameException);
    BOOST_CHECK(myFile == nullptr);
}

BOOST_AUTO_TEST_CASE( File_Test_Destroy ) {
	string filename = "boost_test_destroy.h5";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
	// create blank file
	ofstream some_file("./"+filename);
	some_file.close();
	
    BOOST_REQUIRE_NO_THROW(File::destroy(filename));
    BOOST_CHECK(!exists(filename));
}

BOOST_AUTO_TEST_CASE( File_Test_Destroy_Not_Exist ) {
	string filename = "boost_test_destroy.h5";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
    BOOST_REQUIRE_THROW(File::destroy(filename), FileDoesNotExistException);
}

BOOST_AUTO_TEST_CASE( File_Test_Open ) {
	string filename = "boost_test_open.h5";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
	File *createF = nullptr;
	BOOST_REQUIRE_NO_THROW(createF = File::create(filename, "HDF5"));
	BOOST_CHECK(createF != nullptr);
	createF->closeFile();
	
    File *myFile = nullptr;
    BOOST_REQUIRE_NO_THROW(myFile = File::open(filename, "r+", "HDF5"));
    /*
    try {
		myFile = File::open(filename, "r+", "HDF5");
	} catch (Exception &e) {
		cout << "caught exception" << endl;
		e.what();
	} catch (...) {
		cout << "Default error" << endl;
	}
    //BOOST_CHECK(myFile != nullptr);
    //myFile->closeFile();
    */
}

BOOST_AUTO_TEST_CASE( File_Test_Open_Not_Exist ) {
    // tests file open for directory that already exists
	string filename = "boost_test_create_not_exist.h5";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
	File *myFile = nullptr;
    BOOST_REQUIRE_THROW(myFile = File::open(filename, "r+", "HDF5"), FileDoesNotExistException);
    BOOST_CHECK(myFile == nullptr);
}

BOOST_AUTO_TEST_CASE( File_Test_Open_Empty_Filename ) {
	// tests file open with empty filename
    string filename = "";
	
	File *myFile = nullptr;
    BOOST_REQUIRE_THROW(myFile = File::open(filename, "r+", "HDF5"), FileInvalidNameException);
    BOOST_CHECK(myFile == nullptr);
}

BOOST_AUTO_TEST_CASE( File_Test_Open_Invalid_Filename ) {
    // tests file open with invalid extension
    string filename = "name.wrong";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
	File *myFile = nullptr;
    BOOST_REQUIRE_THROW(myFile = File::open(filename, "HDF5"), FileInvalidNameException);
    BOOST_CHECK(myFile == nullptr);
}

BOOST_AUTO_TEST_CASE( File_Test_Close ) {
	string filename = "boost_test_close.h5";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
    File *myFile = nullptr;
    BOOST_REQUIRE_NO_THROW(myFile = File::create(filename, "HDF5"));
    BOOST_CHECK(myFile != nullptr);
    myFile->closeFile();
}

BOOST_AUTO_TEST_SUITE_END()



// IMAGE TEST SUIT
BOOST_AUTO_TEST_SUITE( Image_Test_Suit )

BOOST_AUTO_TEST_CASE( Image_Test_Create ) {
	string filename = "boost_test_create.h5";
	string imagename = "test_image_create";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
    File *myFile = nullptr;
    BOOST_REQUIRE_NO_THROW(myFile = File::create(filename, "HDF5"));
    BOOST_CHECK(myFile != nullptr);
    
    Image *img = nullptr;
    BOOST_REQUIRE_NO_THROW(img = myFile->createImage(imagename));
    BOOST_CHECK(img != nullptr);
    
    img->closeImage();
    myFile->closeFile();
}

BOOST_AUTO_TEST_CASE( Image_Test_Create_Exists ) {
    // tests image create for image that already exists
	string filename = "boost_test_create_exists.h5";
	string imagename = "test_image_create";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
    File *myFile = nullptr;
    BOOST_REQUIRE_NO_THROW(myFile = File::create(filename, "HDF5"));
    BOOST_CHECK(myFile != nullptr);
    
    Image *img = nullptr;
    BOOST_REQUIRE_NO_THROW(img = myFile->createImage(imagename));
    BOOST_CHECK(img != nullptr);
    
    Image *img2 = nullptr;
    BOOST_REQUIRE_THROW(img2 = myFile->createImage(imagename), ImageExistsException);
    BOOST_CHECK(img2 == nullptr);
    
    img->closeImage();
    myFile->closeFile();
}

BOOST_AUTO_TEST_CASE( Image_Test_Create_Empty_Filename ) {
	// tests image create with empty imagename
    string filename = "boost_test_create_empty_filename.h5";
	string imagename = "";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
    File *myFile = nullptr;
    BOOST_REQUIRE_NO_THROW(myFile = File::create(filename, "HDF5"));
    BOOST_CHECK(myFile != nullptr);
    
    Image *img = nullptr;
    BOOST_REQUIRE_THROW(img = myFile->createImage(imagename), ImageCreationErrorException);
    BOOST_CHECK(img == nullptr);
    
    myFile->closeFile();
}

BOOST_AUTO_TEST_CASE( Image_Test_Destroy ) {
	string filename = "boost_test_image_destroy.h5";
	string imagename = "test_image_destroy";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
    File *myFile = nullptr;
    BOOST_REQUIRE_NO_THROW(myFile = File::create(filename, "HDF5"));
    BOOST_CHECK(myFile != nullptr);
    
    Image *img = nullptr;
    BOOST_REQUIRE_NO_THROW(img = myFile->createImage(imagename));
    BOOST_CHECK(img != nullptr);
    
    BOOST_REQUIRE_NO_THROW(img->destroyImage(imagename));
    
    img->closeImage();
    myFile->closeFile();
}

BOOST_AUTO_TEST_CASE( Image_Test_Open ) {
	string filename = "boost_test_open.h5";
	string imagename = "test_image_open";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
    File *myFile = nullptr;
    BOOST_REQUIRE_NO_THROW(myFile = File::create(filename, "HDF5"));
    BOOST_CHECK(myFile != nullptr);
    
    Image *img = nullptr;
    BOOST_REQUIRE_NO_THROW(img = myFile->createImage(imagename));
    BOOST_CHECK(img != nullptr);
    img->closeImage();
    
    Image *img2 = nullptr;
    BOOST_REQUIRE_NO_THROW(img2 = myFile->openImage(imagename, "r+"));
    BOOST_CHECK(img2 != nullptr);
    img2->closeImage();
    myFile->closeFile();
}

BOOST_AUTO_TEST_CASE( Image_Test_Open_Not_Exist ) {
    // tests image open for iamge that doesn't exist
	string filename = "boost_test_open.h5";
	string imagename = "test_image_open";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
    File *myFile = nullptr;
    BOOST_REQUIRE_NO_THROW(myFile = File::create(filename, "HDF5"));
    BOOST_CHECK(myFile != nullptr);
    
    Image *img = nullptr;
    BOOST_REQUIRE_THROW(img = myFile->openImage(imagename, "r+"), ImageOpenErrorException);
    BOOST_CHECK(img == nullptr);
    
    myFile->closeFile();
}

BOOST_AUTO_TEST_CASE( Image_Test_Open_Empty_Filename ) {
    // tests image open for empty filename
	string filename = "boost_test_open.h5";
	string imagename = "";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
    File *myFile = nullptr;
    BOOST_REQUIRE_NO_THROW(myFile = File::create(filename, "HDF5"));
    BOOST_CHECK(myFile != nullptr);
    
    Image *img = nullptr;
    BOOST_REQUIRE_THROW(img = myFile->openImage(imagename, "r+"), ImageOpenErrorException);
    BOOST_CHECK(img == nullptr);
    
    myFile->closeFile();
}

BOOST_AUTO_TEST_CASE( Image_Test_Close ) {
	string filename = "boost_test_open.h5";
	string imagename = "test_image_close";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
    File *myFile = nullptr;
    BOOST_REQUIRE_NO_THROW(myFile = File::create(filename, "HDF5"));
    BOOST_CHECK(myFile != nullptr);
    
    Image *img = nullptr;
    BOOST_REQUIRE_THROW(img = myFile->openImage(imagename, "r+"), ImageOpenErrorException);
    BOOST_CHECK(img == nullptr);
    
    myFile->closeFile();
}

BOOST_AUTO_TEST_SUITE_END()



// RASTER TEST SUIT
BOOST_AUTO_TEST_SUITE( Raster_Test_Suit )

BOOST_AUTO_TEST_CASE( Raster_Test_Create ) {
	long int sizes[] = {20, 20};
	
	string filename = "boost_test_create.h5";
	string imagename = "test_image_create.img";
	string rastername = "test_raster_create.rst";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
    File *myFile = nullptr;
    BOOST_REQUIRE_NO_THROW(myFile = File::create(filename, "HDF5"));
    BOOST_CHECK(myFile != nullptr);
    
    Image *img = nullptr;
    BOOST_REQUIRE_NO_THROW(img = myFile->createImage(imagename));
    BOOST_CHECK(img != nullptr);
    
    Raster *rst = nullptr;
    BOOST_REQUIRE_NO_THROW(rst = img->createRaster(rastername, sizes, GS_DATATYPE_UI8));
    BOOST_CHECK(rst != nullptr);
    
    rst->closeRaster();
    img->closeImage();
    myFile->closeFile();
}

BOOST_AUTO_TEST_CASE( Raster_Test_Create_Exists ) {
    // tests raster create for raster that already exists
	long int sizes[] = {20, 20};
	
	string filename = "boost_test_create.h5";
	string imagename = "test_image_create.img";
	string rastername = "test_raster_create.rst";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
    File *myFile = nullptr;
    BOOST_REQUIRE_NO_THROW(myFile = File::create(filename, "HDF5"));
    BOOST_CHECK(myFile != nullptr);
    
    Image *img = nullptr;
    BOOST_REQUIRE_NO_THROW(img = myFile->createImage(imagename));
    BOOST_CHECK(img != nullptr);
    
    Raster *rst = nullptr;
    BOOST_REQUIRE_NO_THROW(rst = img->createRaster(rastername, sizes, GS_DATATYPE_UI8));
    BOOST_CHECK(rst != nullptr);
    
    Raster *rst2 = nullptr;
    BOOST_REQUIRE_THROW(rst2 = img->createRaster(rastername, sizes, GS_DATATYPE_UI8), RasterCreationErrorException); // should throw RasterExistsException
    BOOST_CHECK(rst2 == nullptr);
    
    rst->closeRaster();
    img->closeImage();
    myFile->closeFile();
}

BOOST_AUTO_TEST_CASE( Raster_Test_Create_Empty_Filename ) {
	// tests raster create with empty raster name
	long int sizes[] = {20, 20};
	
	string filename = "boost_test_create.h5";
	string imagename = "test_image_create.img";
	string rastername = "";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
    File *myFile = nullptr;
    BOOST_REQUIRE_NO_THROW(myFile = File::create(filename, "HDF5"));
    BOOST_CHECK(myFile != nullptr);
    
    Image *img = nullptr;
    BOOST_REQUIRE_NO_THROW(img = myFile->createImage(imagename));
    BOOST_CHECK(img != nullptr);
    
    Raster *rst = nullptr;
    BOOST_REQUIRE_THROW(rst = img->createRaster(rastername, sizes, GS_DATATYPE_UI8), RasterCreationErrorException); // currently throwing RasterCreationErrorException
    BOOST_CHECK(rst == nullptr);
	
    img->closeImage();
    myFile->closeFile();
}

BOOST_AUTO_TEST_CASE( Raster_Test_Destroy ) {
	long int sizes[] = {20, 20};
	
	string filename = "boost_test_raster_destroy.h5";
	string imagename = "test_raster_destroy";
	string rastername = "test_raster_destroy.rst";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
    File *myFile = nullptr;
    BOOST_REQUIRE_NO_THROW(myFile = File::create(filename, "HDF5"));
    BOOST_CHECK(myFile != nullptr);
    
    Image *img = nullptr;
    BOOST_REQUIRE_NO_THROW(img = myFile->createImage(imagename));
    BOOST_CHECK(img != nullptr);
    
    Raster *rst = nullptr;
    BOOST_REQUIRE_NO_THROW(rst = img->createRaster(rastername, sizes, GS_DATATYPE_UI8));
    BOOST_CHECK(rst != nullptr);
    
    BOOST_REQUIRE_NO_THROW(rst->destroyRaster(rastername));
    
    rst->closeRaster();
    img->closeImage();
    myFile->closeFile();
}

BOOST_AUTO_TEST_CASE( Raster_Test_Open ) {
	long int sizes[] = {20, 20};
	
	string filename = "boost_test_raster_open.h5";
	string imagename = "test_raster_open";
	string rastername = "test_raster_open.rst";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
    File *myFile = nullptr;
    BOOST_REQUIRE_NO_THROW(myFile = File::create(filename, "HDF5"));
    BOOST_CHECK(myFile != nullptr);
    
    Image *img = nullptr;
    BOOST_REQUIRE_NO_THROW(img = myFile->createImage(imagename));
    BOOST_CHECK(img != nullptr);
    
    Raster *rst = nullptr;
    BOOST_REQUIRE_NO_THROW(rst = img->createRaster(rastername, sizes, GS_DATATYPE_UI8));
    BOOST_CHECK(rst != nullptr);
    
    Raster *rst2 = nullptr;
    BOOST_REQUIRE_NO_THROW(rst2 = img->openRaster(rastername));
    BOOST_CHECK(rst2 != nullptr);
    
    rst2->closeRaster();
    rst->closeRaster();
    img->closeImage();
    myFile->closeFile();
}

BOOST_AUTO_TEST_CASE( Raster_Test_Open_Not_Exist ) {
    // tests raster open for iamge that doesn't exist
	long int sizes[] = {20, 20};
	
	string filename = "boost_test_raster_open.h5";
	string imagename = "test_raster_open";
	string rastername = "test_raster_open.rst";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
    File *myFile = nullptr;
    BOOST_REQUIRE_NO_THROW(myFile = File::create(filename, "HDF5"));
    BOOST_CHECK(myFile != nullptr);
    
    Image *img = nullptr;
    BOOST_REQUIRE_NO_THROW(img = myFile->createImage(imagename));
    BOOST_CHECK(img != nullptr);
    
    Raster *rst = nullptr;
    BOOST_REQUIRE_THROW(rst = img->openRaster(rastername), RasterOpenErrorException); // should throw RasterDoesNotExistException
    BOOST_CHECK(rst == nullptr);
    
    img->closeImage();
    myFile->closeFile();
}

BOOST_AUTO_TEST_CASE( Raster_Test_Open_Empty_Filename ) {
    // tests raster open for empty filename
	long int sizes[] = {20, 20};
	
	string filename = "boost_test_raster_open.h5";
	string imagename = "test_raster_open";
	string rastername = "";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
    File *myFile = nullptr;
    BOOST_REQUIRE_NO_THROW(myFile = File::create(filename, "HDF5"));
    BOOST_CHECK(myFile != nullptr);
    
    Image *img = nullptr;
    BOOST_REQUIRE_NO_THROW(img = myFile->createImage(imagename));
    BOOST_CHECK(img != nullptr);
    
    Raster *rst = nullptr;
    BOOST_REQUIRE_THROW(rst = img->openRaster(rastername), RasterOpenErrorException); // should throw RasterDoesNotExistException
    BOOST_CHECK(rst == nullptr);
    
    img->closeImage();
    myFile->closeFile();
}

BOOST_AUTO_TEST_CASE( Raster_Test_Close ) {
	long int sizes[] = {20, 20};
	
	string filename = "boost_test_create.h5";
	string imagename = "test_image_create.img";
	string rastername = "test_raster_create.rst";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
    File *myFile = nullptr;
    BOOST_REQUIRE_NO_THROW(myFile = File::create(filename, "HDF5"));
    BOOST_CHECK(myFile != nullptr);
    
    Image *img = nullptr;
    BOOST_REQUIRE_NO_THROW(img = myFile->createImage(imagename));
    BOOST_CHECK(img != nullptr);
    
    Raster *rst = nullptr;
    BOOST_REQUIRE_NO_THROW(rst = img->createRaster(rastername, sizes, GS_DATATYPE_UI8));
    BOOST_CHECK(rst != nullptr);
    BOOST_REQUIRE_NO_THROW(rst->closeRaster());
    
    img->closeImage();
    myFile->closeFile();
}

BOOST_AUTO_TEST_CASE( Raster_Test_Write ) {
	long int sizes[] = {20, 20};
	long int offsets[] = {0, 0};
	int retVal = 0;

    u_int8_t buffer[20*20];
    for (int i=0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            buffer[20 * j + i] = 20 * j + i;
        }
    }
	
	string filename = "boost_test_create.h5";
	string imagename = "test_image_create.img";
	string rastername = "test_raster_create.rst";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
    File *myFile = nullptr;
    BOOST_REQUIRE_NO_THROW(myFile = File::create(filename, "HDF5"));
    BOOST_CHECK(myFile != nullptr);
    
    Image *img = nullptr;
    BOOST_REQUIRE_NO_THROW(img = myFile->createImage(imagename));
    BOOST_CHECK(img != nullptr);
    
    Raster *rst = nullptr;
    BOOST_REQUIRE_NO_THROW(rst = img->createRaster(rastername, sizes, GS_DATATYPE_UI8));
    BOOST_CHECK(rst != nullptr);
    
    BOOST_REQUIRE_NO_THROW(retVal = rst->write8U(offsets, sizes, buffer));
    BOOST_CHECK(retVal == 1);
    
    rst->closeRaster();
    img->closeImage();
    myFile->closeFile();
}

BOOST_AUTO_TEST_CASE( Raster_Test_Read ) {
	long int sizes[] = {20, 20};
	long int offsets[] = {0, 0};
	int retVal = 0;

    u_int8_t write_buffer[20 * 20];
    u_int8_t read_buffer[20 * 20];
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            write_buffer[20 * j + i] = 20 * j + i;
            read_buffer[20 * j + i] = 0;
        }
    }
	
	string filename = "boost_test_create.h5";
	string imagename = "test_image_create.img";
	string rastername = "test_raster_create.rst";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
    File *myFile = nullptr;
    BOOST_REQUIRE_NO_THROW(myFile = File::create(filename, "HDF5"));
    BOOST_CHECK(myFile != nullptr);
    
    Image *img = nullptr;
    BOOST_REQUIRE_NO_THROW(img = myFile->createImage(imagename));
    BOOST_CHECK(img != nullptr);
    
    Raster *rst = nullptr;
    BOOST_REQUIRE_NO_THROW(rst = img->createRaster(rastername, sizes, GS_DATATYPE_UI8));
    BOOST_CHECK(rst != nullptr);
    
    BOOST_REQUIRE_NO_THROW(retVal = rst->write8U(offsets, sizes, write_buffer));
    BOOST_CHECK(retVal == 1);
    
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            BOOST_CHECK(read_buffer[20 * j + i] == 0);
        }
    }
    
    BOOST_REQUIRE_NO_THROW(retVal = rst->read8U(offsets, sizes, read_buffer));
    BOOST_CHECK(retVal == 1);
    
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
			if (((20 * j + i) % 256)) {
				BOOST_CHECK(read_buffer[20 * j + i] != 0);
			}
			else {
				BOOST_CHECK(read_buffer[20 * j + i] == 0);
			}
            BOOST_CHECK(read_buffer[20 * j + i] == (20 * j + i) % 256);
            cout << int(read_buffer[20 * j + i]) << " ";
        }
        cout << endl;
    }
    
    rst->closeRaster();
    img->closeImage();
    myFile->closeFile();
}

BOOST_AUTO_TEST_SUITE_END()

