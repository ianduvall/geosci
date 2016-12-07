//FileHDF5.cpp
//#include <H5File.h>

#include <string>
#include <iostream>
#include <boost/filesystem.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "H5Cpp.h"

#include "FileHDF5.hpp"
#include "ImageHDF5.hpp"
#include "MetadataHDF5.hpp"
#include "Exceptions.hpp"
#include "attributes.hpp"
#include "StringAttributeHDF5.hpp"

using namespace boost::filesystem;
using namespace boost::posix_time;
using namespace H5;
using namespace std;

extern "C" {
  #include "hdf5_extras.h"
}

//#ifdef COMMENT
#define DEBUG

namespace GeoSci {

	DerivedRegister<FileHDF5> FileHDF5::reg("HDF5");
    
    FileHDF5::FileHDF5() : filename(""), file(0) {}
    
    FileHDF5::FileHDF5 (string name, unsigned int flags) {
        this->formatFilename(name);
        this->the_h5file = new H5::H5File(name,flags);
        this->createFileAttributes("creating FileHDF5 " + name);
        this->file = this->getId();
    }
    
    FileHDF5::~FileHDF5() {
        delete this->the_h5file;
    }
    
    void FileHDF5::createFileAttributes(const std::string &descriptor) {
#ifdef DEBUG
        cout << "start of createFileAttributes"<<endl;
#endif
        createStringAttribute("filetype").write("GEOSCI");
#ifdef DEBUG
        cout << "createFileAttributes: created filetype"<<endl;
#endif
        
        createStringAttribute("software_version").write("V0.0.1");
#ifdef DEBUG
        cout << "createFileAttributes: created software_version"<<endl;
#endif
        
        createStringAttribute("descriptor").write(descriptor);
#ifdef DEBUG
        cout << "createFileAttributes: created descriptor"<<endl;
#endif
        
        createStringAttribute("writeable").write("TRUE");
#ifdef DEBUG
        cout << "createFileAttributes: created writeable"<<endl;
#endif
        
        ptime t(second_clock::universal_time());
        //string datetime=to_iso_extended_string(t)+"UTC";
        string datetime=to_simple_string(t)+"UTC";
        createStringAttribute("creation_datetime").write(datetime);
#ifdef DEBUG
        cout << "createFileAttributes: created creaton_datetime"<<endl;
#endif
        
        createStringAttribute("last_update_datetime").write(datetime);
#ifdef DEBUG
        cout << "createFileAttributes: created last_update_datetime"<<endl;
#endif
        
        createStringAttribute("history").write(datetime+": Creation;");
#ifdef DEBUG
        cout << "createFileAttributes: created history"<<endl;
        cout << "                       "<<openStringAttribute("history").read()<<endl;
#endif
    }
    
    
    Attribute FileHDF5::createAttribute(const std::string &name, const DataType &type, const DataSpace &space) const {
        return the_h5file->createAttribute(name,type,space);
    }
    
    Attribute FileHDF5::openAttribute(const std::string &name) const {
        return the_h5file->openAttribute(name);
    }
    
    StringAttributeHDF5 FileHDF5::createStringAttribute(const std::string &name) const {
        DataSpace space = DataSpace(H5S_SCALAR);
        StrType   type(PredType::C_S1, H5T_VARIABLE);
        return static_cast<StringAttributeHDF5>(the_h5file->createAttribute(name,type,space));
    }
    
    StringAttributeHDF5 FileHDF5::openStringAttribute(const std::string &name) const {
        return static_cast<StringAttributeHDF5>(the_h5file->openAttribute(name));
    }
    
    
    
    
	
	void FileHDF5::printa() {
		std::cout << "FileHDF5, printa\n";
		
		return;
    }
    
    void FileHDF5::printb() {
		std::cout << "FileHDF5, printb\n";
		return;
	}
	
	void FileHDF5::printHDF5() {
		std::cout << "FileHDF5, printHDF5" << endl;
		return;
	}
	
	void FileHDF5::formatFilename(string& filename) {
		const string extension = ".h5";
		
		// check for empty filename
		if (filename.empty()) {
			throw FileInvalidNameException();
		}
		
		// check if filename has an extension ie contains "."
		size_t extensionIdx = filename.find('.');
		if (extensionIdx == string::npos) {
			filename.append(extension);
		}
		// check for invalid extension
		else if (filename.find(extension, extensionIdx) == string::npos) {
			// OPTION: replace with correct
			// filename.erase(extensionIdx);
			// filename.append(extension);
			throw FileInvalidNameException();
		}
		
		return;
	}

	bool FileHDF5::isGeoSciHDF5File(const std::string& filename) {

#ifdef DEBUG
        std::cout << "start of isGeoSciHDF5File\n";
#endif
        path p(filename);
        path ap(p);
#ifdef DEBUG
        std::cout << "path: " << ap << std::endl;
        std::cout << "checking if file " << filename << " exists\n";
#endif
        if (exists(ap) && is_regular_file(ap)) {
#ifdef DEBUG
            std::cout << "file exists\n";
#endif
            if (!isHdf5(filename)) {
                //throw NotGeoSciFileException();
                return false;
            }
#ifdef DEBUG
            std::cout << "file is GeoSci\n";
#endif
            
            H5::H5File *f = new H5File(filename, H5F_ACC_RDONLY);
            StrType type(PredType::C_S1, H5T_VARIABLE);
            H5std_string attrVal1, attrVal2;
            f->openAttribute("filetype").read(type, attrVal1);
            f->openAttribute("filelib").read(type, attrVal2);
            // !!! need query attribute function
            if (attrVal1 != "GEOSCI" || attrVal2 != "HDF5") {
                //throw NotGeoSciFileException(); // filehdf5 specific exc?
                return false;
            }
            
            f->close();
            delete f;
            
#ifdef DEBUG
            // std::cout << "file is GeoSci HDF5, attributes exist\n";
#endif
            return true;
        }
        
        return false;
    } // end isGeoSciHDF5File()
	
	
    void FileHDF5::createFile(std::string name) {
		if (name.empty()) {
			throw FileInvalidNameException();
		}
		
		// check extension
		formatFilename(name);
		
		const H5std_string FILE_NAME(name);
	#ifdef DEBUG
		cout << "FileHDF5::create: at a1\n";
	#endif

		GS_init();
		
		// 2. create the file
		bstring bname = bfromcstr(name.c_str());
		hid_t file_id;
		
		
		if (exists(name)) {
			throw FileExistsException();
		}
		
		if( ( file_id = GS_FileCreate(bname)) == ERROR){
			bdestroy(bname);
			throw FileCreationErrorException();
		}  // endif
		bdestroy(bname);
		
	#ifdef DEBUG
		cout << "FileHDF5::create: at a2\n";
	#endif    

		// 3. open the file metadata in the file metadata group, _Header
		hid_t file_header_id;
		if((file_header_id =GS_GroupOpen(file_id,file_metadata_name)) < 0 ) {
			bassignformat(error_string,"FileHDF5: Error opening metadata.");
			throw MetadataDoesNotExistException();
		}

	#ifdef DEBUG
		cout << "FileHDF5::create: at a3\n";
	#endif    

		add_attribute(file_header_id,"filelib","HDF5");

	#ifdef DEBUG
		cout << "FileHDF5::create: at a4\n";
	#endif

		// 4. Close the file-metadata group
		if(!GS_GroupClose(file_header_id)){
			bassignformat(error_string,"FileHDF5: Error closing metadata group.");
			throw AttributeErrorException();
		}// endif

		
		// 5. add name and file_id
		this->filename = filename;
		this->file = file_id;
	#ifdef DEBUG
		cout << "FileHDF5::create: at a5\n";
	#endif    

	} // FileHDF5::createFile



	void FileHDF5::openFile(const std::string& nameIn, const std::string& access) {
    
    if (nameIn.empty()) {
		throw FileInvalidNameException();
	}
	// copy
	string name = nameIn;
	// check extension
	formatFilename(name);
	
    //string tail = ".h5";	
    //name = name.append(tail);
    const H5std_string FILE_NAME( name );
    
    //Exception::dontPrint();

    
#ifdef DEBUG
    cout << "FileHDF5::open: at a1\n";
#endif    

    //comment out just to try......
    
    // 2. open existing file. fails if does not exist.
    if (!exists(name)) {
      throw FileDoesNotExistException();
    }//endif
    
#ifdef DEBUG
    cout << "FileHDF5::open: at a2\n";
#endif   
    
    // 1. is the file one of ours?
    if(!isGeoSciHDF5File(name)) {
      throw NotGeoSciFileException();
    }//endif

    
    bstring bname = bfromcstr(name.c_str());
    bstring baccess = bfromcstr(access.c_str());

#ifdef DEBUG
    cout << "FileHDF5::open: at a3\n";
#endif    
    
    hid_t file_id = GS_FileOpen(bname,baccess);
#ifdef DEBUG
    cout << "FileHDF5::open: at a3.1: file_id="<<file_id<<"\n";
#endif    
    if(file_id < 0) {
        bdestroy(bname);
        bdestroy(baccess);
        throw FileOpenErrorException();
    }
    bdestroy(bname);
    bdestroy(baccess);
#ifdef DEBUG
    cout << "FileHDF5::open: at a4\n";
#endif    
    
    
    // 3. create File object to return:
	this->filename = name;
	this->file = file_id;
    //open_file->filetype = FileType::HDF5;
    return;
    // keep track of files we have open:
    //open_files.insert(pair(name,file));
    
	} // FileHDF5::openFile
	
	
	
	void FileHDF5::destroyFile(const std::string& nameIn) {
		
		if (nameIn.empty()) {
			throw FileInvalidNameException();
		}
		// check extension
		string name = nameIn;
		formatFilename(name);
		
		//string tail = ".h5";
		//string newname = name.append(tail);
		path p(name);
		
		// make sure its not open, and nothing inside it is open...YET
		// delete the file from the filesystem:
		// then delete the file:
		if (exists(p)) {
			try {
				remove(p);
			}
			catch (...) {
				throw FileDestroyErrorException();
			}
		} else {
			throw FileDoesNotExistException();
		} // endif
    
		return;
	} // FileHDF5::destroyFile
  
  
  
	void FileHDF5::closeFile() {
    
    // check on other objects that are open,
    // and close them first.  
    /*for(auto const & mobj: open_metadata_objects){
      MetadataHDF5::close(mobj);
      }//endfor
      for(auto const & iobj: open_image_objects){
      ImageHDF5::close(iobj);
      }//endfor
      // remove this file from the list of open files.
      open_files.erase(name);
    */
    
    //cout << "in filehdf5::close: at a1\n";
    // yet to figure out how to do this......LEP: April 9, 2016
    //delete file; //deletes the hdf5 file object. segfault
    //file->close(); //deletes the hdf5 file object. segfault
    //cout << "in filehdf5::close: at a2\n";
    GS_FileClose(file);
      
    
    //delete this; // deletes the geosci FileHDF5 object.
    //cout << "in filehdf5::close: at a3\n";
    delete this;
    
	} // FileHDF5::closeFile
  
	
	ImageHDF5 *FileHDF5::createImage(const string& name) {
		ImageHDF5 *i = new ImageHDF5;
		i->parentFile = this;
		i->createImage(name);
		return i;
	} // end create image
	
	ImageHDF5 *FileHDF5::openImage(const string& name, const string& access) {
		ImageHDF5 *i = new ImageHDF5;
		i->parentFile = this;
		i->openImage(name, access);
		return i;
	} // end open image
	
	void FileHDF5::destroyImage(const string& name) {
		ImageHDF5 *i = new ImageHDF5();
		i->parentFile = this;
		i->destroyImage(name);
		return;
	}// end destroy_image()
	
} // end-GeoSci namespace


//#endif
