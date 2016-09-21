#ifndef FILEHDF5_HPP_
#define FILEHDF5_HPP_

#include <string>
#include "../File/File.hpp"

namespace GeoSci {
	
    class FileHDF5 : public GeoSci::File {
    protected:
        static GeoSci::DerivedRegister<FileHDF5> reg;
        std::string name = "";
        
    public:        
        void printa();
        void printb();
        
        void createFile(const std::string&);
		void destroyFile();
		void openFile(const std::string&);
		void closeFile();
    };
    
    
} // GeoSci

#endif
