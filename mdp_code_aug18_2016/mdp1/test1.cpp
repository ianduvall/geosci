#include <iostream>
#include <string>

#include "File/File.hpp"
#include "FileHDF5.hpp"
#include "Exceptions.hpp"

using namespace GeoSci;

int main() {

    std::cout << "main starting\n" << std::endl;

	try {
		//File* myFile = FileFactory::createInstance("HDF5");
		File* myFile = File::create("HDF5");
		std::cout << "myFile type: ";
		myFile->printa();
		std::cout << "HDF5 instance of myFile created successfully.\n" << std::endl;
		
		File* myFileDefault = File::create();
		std::cout << "myFileDefault type: ";
		myFileDefault->printa();
		std::cout << "HDF5 instance of myFileDefault created successfully.\n" << std::endl;
		
		myFile->printa();
		myFile->printb();
		myFile->process_stuff();
		
		std::cout << "\nTesting createFile\n";
		myFile->createFile("myFirstFile");
		
		std::cout << "Testing closeFile\n";
		myFile->closeFile();
		
		std::cout << "Testing openFile\n";
		myFile->openFile("myFirstFile", "r+");
		
		std::cout << "Testing destroyFile\n";
		myFile->destroyFile("myFirstFile");
    }
    catch (exception& e) {
		std::cout << "Thrown Error: " << e.what() << '\n';
	} 
    catch (...) {
		std::cout << "Error creating HDF5 instance of myFile." << std::endl;
	}
    
    
    return 0;
}
