// cpp_boost_test.cpp

#define BOOST_TEST_MODULE Boost_Test_Module

#include <boost/test/included/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <string>

#include "Raster/Raster.hpp"
#include "Image/Image.hpp"
#include "File/File.hpp"
#include "Exceptions.hpp"

#include "FileHDF5.hpp"

using namespace boost::filesystem;
using namespace boost::unit_test;
using namespace GeoSci;
using namespace std;


// FILE TEST SUIT
BOOST_AUTO_TEST_SUITE( File_Test_Suit )

BOOST_AUTO_TEST_CASE( File_Test_Create ) {
	string filename = "boost_test_create.h5";
	string filename2 = "2" + filename;
	if (exists(filename)) {
		boost::filesystem::remove(filename);
	}
	if (exists(filename2)) {
		boost::filesystem::remove(filename2);
	}
	
    File *myFile = nullptr;
    BOOST_REQUIRE_NO_THROW(myFile = File::create(filename, "HDF5"));
    BOOST_CHECK(myFile != nullptr);
    //myFile->printHDF5();
    
    try {
		//cout << "myFile's id: " << myFile->getId() << endl;
    } catch (...) {
		cout << "myFile's getId didn't work" << endl;
	}
	
	FileHDF5 *myFile2 = nullptr;
    BOOST_REQUIRE_NO_THROW(myFile2 = dynamic_cast<FileHDF5*>(File::create(filename2)));
    BOOST_CHECK(myFile2 != nullptr);
    myFile2->printHDF5();
	
	try {
		cout << "myFile2's id: " << myFile2->getId() << endl;
    } catch (...) {
		cout << "myFile2's getId didn't work" << endl;
	}
	
	myFile2->closeFile();
    myFile->closeFile();
    
    
}

BOOST_AUTO_TEST_SUITE_END()
