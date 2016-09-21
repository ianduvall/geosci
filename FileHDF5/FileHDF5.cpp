//FileHDF5.cpp

#include <iostream>
#include <string>
#include "FileHDF5.hpp"

namespace GeoSci {
    DerivedRegister<FileHDF5> FileHDF5::reg("HDF5");
    
    void FileHDF5::printa() {
		std::cout << "FileHDF5, printa\n";
		return;
    }
    
    void FileHDF5::printb() {
		std::cout << "FileHDF5, printb\n";
		return;
	}
	
    void FileHDF5::createFile(const std::string& nameIn) {
		if (this->name.empty()) {
			std::cout << "Creating FileHDF5 named " + nameIn + "\n";
			this->name = nameIn;
		}
		else if (this->name == nameIn) {
			std::cout << "FileHDF5 named " + nameIn + " already open\n";
		}
		else {
			std::cout << "Please close the current FileHDF5 named " + this->name + " before creating " + nameIn + "\n";
		}
		return;
	}
	
	void FileHDF5::destroyFile() {
		if (this->name.empty()) {
			std::cout << "No FileHDF5 to destroy\n";
		}
		else {
			std::cout << "Destroying FileHDF5 named " + this->name + "\n";
			this->name = "";
		}
		return;
	}
	
	void FileHDF5::openFile(const std::string& nameIn) {
		if (this->name.empty()) {
			std::cout << "Opening FileHDF5 named " + nameIn + "\n";
			this->name = nameIn;
		}
		else {
			std::cout << "Please close the current FileHDF5 named " + this->name + " before opening " + nameIn + "\n";
		}
		return;
	}
	
	void FileHDF5::closeFile() {
		if (this->name.empty()) {
			std::cout << "No FileHDF5 to close\n";
		}
		else {
			std::cout << "Closing FileHDF5 named " + this->name + "\n";
			this->name = "";
		}
		return;
	}
	
	
} // GeoSci
