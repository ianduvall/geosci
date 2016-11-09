/*
#define BOOST_TEST_MODULE simple
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(testSimple)
{
   BOOST_CHECK_EQUAL(true, true);
}
*/

#define BOOST_TEST_MODULE File_Test_Module

#include <boost/test/included/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <string>

#include "Raster/Raster.hpp"
#include "Image/Image.hpp"
#include "File/File.hpp"
#include "Exceptions.hpp"

using namespace boost::filesystem;
using namespace boost::unit_test;
using namespace GeoSci;
using namespace std;



// file create
void test_file_create();
void test_file_create_file_exists();
void test_file_create_invalid_filename();

// file destroy
void test_file_destroy();

// file open
void test_file_open();
void test_file_open_exc();

// file close
void test_file_close();

BOOST_AUTO_TEST_SUITE( File_Test_Suit )

BOOST_AUTO_TEST_CASE( File_Test_Create ) {
	string filename = "boost_test_create.h5";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
    File *myFile = NULL;
    BOOST_CHECK_NO_THROW(myFile = File::create(filename, "HDF5"));
    BOOST_CHECK(myFile != NULL);
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
	
	File *myFile = NULL;
    BOOST_CHECK_THROW(myFile = File::create(filename, "HDF5"), FileExistsException);
    BOOST_CHECK(myFile == NULL);
}

BOOST_AUTO_TEST_CASE( File_Test_Create_Empty_Filename ) {
	// tests file create with empty filename
    string filename = "";
	
	File *myFile = NULL;
    BOOST_CHECK_THROW(myFile = File::create(filename, "HDF5"), FileCreationInvalidNameException);
    BOOST_CHECK(myFile == NULL);
}

BOOST_AUTO_TEST_CASE( File_Test_Create_Invalid_Filename ) {
    // tests file create with invalid extension
    string filename = "name.wrong";
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	
	File *myFile = NULL;
    BOOST_CHECK_THROW(myFile = File::create(filename, "HDF5"), FileCreationInvalidNameException);
    BOOST_CHECK(myFile == NULL);
}

BOOST_AUTO_TEST_SUITE_END()

