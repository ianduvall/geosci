//FileHDF5.cpp, by Laura Huang, April, 2016
//#include <H5File.h>
#include "H5Cpp.h"
#include <string>
#include <iostream>
#include <boost/filesystem.hpp>

#include "ImageHDF5.hpp"
#include "FileHDF5.hpp"
#include "MetadataHDF5.hpp"
#include "Exceptions.hpp"
#include "attributes.hpp"

using namespace boost::filesystem;
using namespace H5;
using namespace std;

extern "C"
{
  #include "hdf5_extras.h"
}


//#define DEBUG

namespace GeoSci {
  // use default constructor and destructors:
  //FileHDF5::FileHDF5(){}
  //FileHDF5::~FileHDF5(){}

    //const int ERROR = -1;

	DerivedRegister<FileHDF5> FileHDF5::reg("HDF5");
	
	void FileHDF5::printa() {
		std::cout << "FileHDF5, printa\n";
		return;
    }
    
    void FileHDF5::printb() {
		std::cout << "FileHDF5, printb\n";
		return;
	}
	
	

	bool FileHDF5::isGeoSciHDF5File(const std::string& filename) {
      FileOpenErrorException FileOpenError;
      MetadataDoesNotExistException MetadataDoesNotExist;

      
      int cset;
    path p(filename);
    path ap(p);
    if(exists(ap) && is_regular_file(ap) ){
      //if(!isHDF5(filename.c_str()) ) {
      //  return false;
      //  }//endif
      
      const H5std_string FILE_NAME( filename );
      hid_t file_id;
      bstring bfilename = bfromcstr(filename.c_str());
      bstring baccess = bfromcstr("r+");
      file_id = GS_FileOpen(bfilename,baccess);
      if(file_id <0){
          bdestroy(bfilename);
          bdestroy(baccess);
          throw FileOpenError;
      }
      bdestroy(bfilename);
      bdestroy(baccess);

      hid_t file_header_id;
      if((file_header_id =GS_GroupOpen(file_id,file_metadata_name)) < 0 ) {
          bassignformat(error_string,"FileHDF5: Error opening metadata.");
          GS_FileClose(file_id);
          throw MetadataDoesNotExist;
      }

      if(query_attribute(file_header_id,"filetype")!="GEOSCI" ||
         query_attribute(file_header_id,"filelib")!="HDF5") {
          GS_GroupClose(file_header_id);
          GS_FileClose(file_id);
          return false;
      }
      GS_GroupClose(file_header_id);
      if(!GS_FileClose(file_id)){
#ifdef DEBUG
          printf("isGeoSciHDF5File: GS_FileClose failed.\n");
#endif          
      }else {
#ifdef DEBUG
          printf("isGeoSciHDF5File: GS_FileClose succeeded.\n");
#endif          
      }
      
      
      
      
    } else {
      return false;
    }// endif
    return true;
  }// end isGeoSciHDF5File()
  
  
  
	void FileHDF5::createFile(const std::string& name) {
		FileCreationErrorException FileCreationError;
		MetadataDoesNotExistException MetadataDoesNotExist;
		AttributeErrorException AttributeError;
      
    // 1. create the appropriate filename:
    //string tail = ".h5";	
    //name = name.append(tail);
    const H5std_string FILE_NAME(name);
#ifdef DEBUG
    cout << "FileHDF5::create: at a1\n";
#endif

    GS_init();
    
    // 2. create the file
    bstring bname = bfromcstr(name.c_str());
    hid_t file_id;
    
    if( ( file_id = GS_FileCreate(bname)) == ERROR){
        bdestroy(bname);
        throw FileCreationError;
    }  // endif
    bdestroy(bname);
    
  #ifdef DEBUG
    cout << "FileHDF5::create: at a2\n";
#endif    

    // 3. open the file metadata in the file metadata group, _Header
    hid_t file_header_id;
    if((file_header_id =GS_GroupOpen(file_id,file_metadata_name)) < 0 ) {
        bassignformat(error_string,"FileHDF5: Error opening metadata.");
        throw MetadataDoesNotExist;
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
    throw AttributeError;
  }// endif

    
    // 5. add name and file_id
    this->filename = name;
    this->file = file_id;
#ifdef DEBUG
    cout << "FileHDF5::create: at a5\n";
#endif    
    
    return;	
	} // FileHDF5::createFile



	void FileHDF5::openFile(const std::string& name, const std::string& access) {
      NotGeoSciFileException NotGeoSciFile;
      FileDoesNotExistException FileDoesNotExist;
      FileOpenErrorException FileOpenError;
      
      
    //string tail = ".h5";	
    //name = name.append(tail);
    const H5std_string FILE_NAME( name );
    
    Exception::dontPrint();

    
#ifdef DEBUG
    cout << "FileHDF5::open: at a1\n";
#endif    

    //comment out just to try......
    
    // 1. is the file one of ours?
    if(!isGeoSciHDF5File(name)) {
      throw NotGeoSciFile;
    }//endif

#ifdef DEBUG
    cout << "FileHDF5::open: at a2\n";
#endif    
    
    
    // 2. open existing file. fails if does not exist.
    if (!exists(name)) {
      throw FileDoesNotExist;
    }//endif
    bstring bname = bfromcstr(name.c_str());
    bstring baccess =bfromcstr(access.c_str());

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
        throw FileOpenError;
    }
    bdestroy(bname);
    bdestroy(baccess);
#ifdef DEBUG
    cout << "FileHDF5::open: at a4\n";
#endif    
    
    
    // 3. create File object to return:
    this->filename = name;
    this->file = file_id;

#ifdef DEBUG
    cout << "FileHDF5::open: at a5\n";
#endif    
    
    //open_file->filetype = FileType::HDF5;
		return;
    // keep track of files we have open:
    //open_files.insert(pair(name,file));
    
	} // FileHDF5::openFile
	
	
	
	void FileHDF5::destroyFile(const std::string& name) {
      FileDestroyErrorException FileDestroyError;
      FileDoesNotExistException FileDoesNotExist;
      
    //string tail = ".h5";	
    //string newname = name.append(tail);
    path p(name);
    
    // make sure its not open, and nothing inside it is open...YET
    // delete the file from the filesystem:
    // then delete the file:
    if(exists(p)){
        try{
            remove(p);
        }
        catch (...){
            throw FileDestroyError;
        }
        
    } else {
      throw FileDoesNotExist;
    }// endif 
    
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
    
    this->filename = "";
    this->file = -1;
    
	} // FileHDF5::closeFile
  
  
  ImageHDF5 * FileHDF5::create_image(string name) {
    return ImageHDF5::create(this,name);
  }// end create_image()
  
    ImageHDF5 * FileHDF5::open_image(string name, string access) {
        return ImageHDF5::open(this,name,access);
  }// end open_image()
  
  void FileHDF5::destroy_image(string name) {
    ImageHDF5::destroy(this,name);
  }// end destroy_image()
  
  
}//end-GeoSci namespace


   
