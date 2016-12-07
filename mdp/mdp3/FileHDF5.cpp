//FileHDF5.cpp,
#include "H5Cpp.h"
#include <string>
#include <iostream>
#include <boost/filesystem.hpp>

//lep//#include "ImageHDF5.hpp"
#include "FileHDF5.hpp"
#include "StringAttributeHDF5.hpp"
//lep//#include "MetadataHDF5.hpp"
//lep//#include "Exceptions.hpp"
//lep//#include "attributes.hpp"

#include "boost/date_time/posix_time/posix_time.hpp"

using namespace boost::filesystem;
using namespace boost::posix_time;
using namespace H5;
using namespace std;


#define DEBUG

namespace GeoSci {

#ifdef COMMENT    

  bool FileHDF5::isGeoSciHDF5File(string filename) {
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
      bstring baccess = bfromcstr("r");
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
      GS_FileClose(file_id);
      
      
    } else {
      return false;
    }// endif
    return true;
  }// end isGeoSciHDF5File()
  
#endif  



    FileHDF5::FileHDF5 (const std::string &name, unsigned int flags)
    {
        the_h5file = new H5::H5File(name,flags);
    }

    void FileHDF5::createFileAttributes(const std::string &descriptor)
    {
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
    

    Attribute FileHDF5::createAttribute(const std::string &name, const DataType &type, const DataSpace &space) const
    {
        return the_h5file->createAttribute(name,type,space); 
    }

    Attribute FileHDF5::openAttribute(const std::string &name) const
    {
        return the_h5file->openAttribute(name); 
    }

    StringAttributeHDF5 FileHDF5::createStringAttribute(const std::string &name) const
    {
      DataSpace space = DataSpace(H5S_SCALAR);
      StrType   type(PredType::C_S1, H5T_VARIABLE);        
      return static_cast<StringAttributeHDF5>(the_h5file->createAttribute(name,type,space)); 
    }
    
    StringAttributeHDF5 FileHDF5::openStringAttribute(const std::string &name) const
    {
      return static_cast<StringAttributeHDF5>(the_h5file->openAttribute(name)); 
    }

    FileHDF5::~FileHDF5()
    {
        delete the_h5file;
    }
    
    
    

#ifdef COMMENT    
  
  FileHDF5 *FileHDF5::create(string name)
  {
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

    
    // 5. create and return FileHDF5 object
    FileHDF5 *new_file = new FileHDF5();
    new_file->filename = name;
    new_file->file = file_id;
#ifdef DEBUG
    cout << "FileHDF5::create: at a5\n";
#endif    
    return new_file;	
    
  }


#endif








    





    
#ifdef COMMENT
    
    FileHDF5 *FileHDF5::open(string name, string access) 
  {
      NotGeoSciFileException NotGeoSciFile;
      FileDoesNotExistException FileDoesNotExist;
      FileOpenErrorException FileOpenError;
      
      
    //string tail = ".h5";	
    //name = name.append(tail);
    const H5std_string FILE_NAME( name );
    
    Exception::dontPrint();
    
    // 1. is the file one of ours?
    if(!isGeoSciHDF5File(name)) {
      throw NotGeoSciFile;
    }//endif
    
    // 2. open existing file. fails if does not exist.
    if (!exists(name)) {
      throw FileDoesNotExist;
    }//endif
    bstring bname = bfromcstr(name.c_str());
    bstring baccess =bfromcstr(access.c_str());
    hid_t file_id = GS_FileOpen(bname,baccess);
    if(file_id < 0) {
        bdestroy(bname);
        bdestroy(baccess);
        throw FileOpenError;
    }
    bdestroy(bname);
    bdestroy(baccess);
    
    
    // 3. create File object to return:
    FileHDF5 *open_file = new FileHDF5();
    open_file->filename = name;
    open_file->file = file_id; 
    //open_file->filetype = FileType::HDF5;
    return open_file;
    // keep track of files we have open:
    //open_files.insert(pair(name,file));
    
  }//end-open
  
  
  
  void FileHDF5::destroy(std::string name)
  {
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
    
  }
  
  
  
  void FileHDF5::close() 
  {   
    
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
      
    
    delete this; // deletes the geosci FileHDF5 object.
    //cout << "in filehdf5::close: at a3\n";
    
  }//end-close
  
  
  ImageHDF5 * FileHDF5::create_image(string name) {
    return ImageHDF5::create(this,name);
  }// end create_image()
  
    ImageHDF5 * FileHDF5::open_image(string name, string access) {
        return ImageHDF5::open(this,name,access);
  }// end open_image()
  
  void FileHDF5::destroy_image(string name) {
    ImageHDF5::destroy(this,name);
  }// end destroy_image()

#endif
    
  
}//end-GeoSci namespace


   
