#include <iostream>
#include <string>

#include "File/File.hpp"
#include "FileHDF5/FileHDF5.hpp"

using namespace GeoSci;

int main() {

    std::cout << "main starting" << std::endl;

	try {
		File* myFile = FileFactory::createInstance("HDF5");
		std::cout << "HDF5 instance of myFile created successfully." << std::endl;
		myFile->printa();
		myFile->printb();
		myFile->process_stuff();
		
		std::cout << "\nTesting createFile, closeFile, openFile, destroyFile\n";
		
		myFile->createFile("myFirstFile");
		myFile->createFile("myFirstFile");
		myFile->createFile("mySecondFile");
		myFile->closeFile();
		myFile->closeFile();
		myFile->openFile("myFirstFile");
		myFile->openFile("mySecondFile");
		myFile->destroyFile();
		myFile->destroyFile();
    }
    catch (...) {
		std::cout << "Error creating HDF5 instance of myFile." << std::endl;
	}
    
    
    return 0;
}
