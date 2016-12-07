#ifndef FILEHDF5_HPP_ 
#define FILEHDF5_HPP_

//#include <vector>
#include <string>

#include "H5Cpp.h"
#include "File/File.hpp"
#include "ImageHDF5.hpp"

using namespace H5;

//------------------------------------------------------
namespace GeoSci {

/** \brief FileHDF5 -- Implementation of file operations for HDF5-based GeoSciLib files.


FileHDF5 is an implementation of the File virtual base class.
This class is used as the standard interface for file operations 
 that are used for storing and processing of data in GeoSciLib using the HDF5 implementation.
In particular, first, data is imported into GeoSciLib from data provided by a remote sensing data
processing facility, or the output from another data processing system.
This data is stored in the GeoSciLib file format.
This class implements the HDF5-based file operations, while using the standard interface defined in the 
File class.

\see File, ImageHDF5, RasterHDF5, MetadataHDF5

\par Usage Overview
The FileHDF5 class is meant to be used when dealing with GeoSciLib files. 
Other classes are used to deal with external files in other formats.
This class provides methods for creating and deleting files, as well as opening and closing them.

Creation of a file requires a file name, and the file type will be defaulted to the latest default format.
If desired, the type of file created can also be given, and must be one of the supported types. 
These are specified as values from the enum FileType.

Deleting a file is only possible if it is not "open".

Opening a file requires it to already exist, and merely requires the filename.

Closing a file is a major issue, since all the other open objects within the file must be closed as well.
This class takes care of that for the user.

We can also query if a file is of this type.

*/	

class FileHDF5 : public GeoSci::File, public H5::H5File {

public:
	static GeoSci::DerivedRegister<FileHDF5> reg;
    std::string filename;
    hid_t file;

	//FileHDF5();

    // recall that the parent class defines the following public data objects:
    // FileType filetype;
    // vector<Metadata> open_metadata_objects;
    // vector<Image> open_image_objects;
    
	void printa();
	void printb();
	void printHDF5();
	
	void formatFilename(string& filename);
    
    /** \brief FileHDF5::isGeoSciHDF5File queries if a file is of type HDF5 or not.
     
     FileHDF5::isGeoSciHDF5File returns true if the file is a GeoSciLib HDF5 file, false otherwise.
     
     \see open
     
     \param[in] filename
            The name of the file in question.
            
     \returns A boolean, TRUE if the file is of this type, FALSE otherwise.
     
     \par Exceptions
          ...YET: need to define exceptions for various file operations that fail, such as existence, opening, reading...
          
     \example
          Let's see if the file name "sirc_raco" is a valid GeoSciLib HDF5 file:
          \code
       #include "GeoSciLib.h"
       #include <string>
       using namespace std;
       int main() 
       {
           string filename = "sirc_raco"; 
	       try {
	           if( GeoSciLib::FileHDF5::isGeoSciHDF5File(filename) ) {
	               cout << "file sirc_raco is a valid GeoSciLib HDF5 file"<<endl;
	           }//end-if
	       }//end-try 
	       catch (...) {
	           cerr << "GeoSciLib::FileHDF5::isGeoSciHDF5File failure"<<endl;
	       }//end-catch
       }//end-main
           \endcode
           
    */
	static bool isGeoSciHDF5File(const std::string& filename);
  
  
	/** \brief FileHDF5::create allows one to create a new HDF5 version of a GeoSciLib file.

   FileHDF5::create is used to make a new file, in the HDF5 GeoSciLib file format.
   The file is created empty, except for the needed metadata to define it as a GeoSciLib file,  
   and its type. The file is also opened, and a valid FileHDF5 object is returned.

   \see destroy, open, close

   \param[in] name
       This is a string, set by the user, that holds the desired name of the new file.

   \returns
       A valid FileHDF5 object on success.

   \par Exceptions
       Exceptions that may be raised by this method:
       std::invalid_argument for invalid filenames
       std::system_error when the file creation fails

   \example
       This function should never be called by users, instead, use the File class and its functions.

       Let's say a developer wants to create a file named "sirc_raco", with the HDF5
       GeoSciLib file format:
       \code
       #include "GeoSciLib.h"
       #include <string>
       using namespace std;
       int main() 
       {
           string newfilename = "sirc_raco"; 
	   try {
	       GeoSciLib::FileHDF5 newfile = GeoSciLib::FileHDF5::create(newfilename);
	   }//end-try 
	   catch (...) {
	       cerr << "GeoSciLib::FileHDF5::create failure"<<endl;
	   }//end-catch
       }//end-main
       \endcode

    \details
       This is the implementation of the File interface. As such, it should not 
       generally be called by users. Users should call the File::create method instead.

  
  */
	void createFile(std::string name);
	
	
		/** \brief FileHDF5::open allows one to open an existing HDF5 GeoSciLib file.

   FileHDF5::open is used to open an existing file.
   The file is opened, and the file's metadata is examined to determine if
   the file is an HDF5 GeoSciLib file. If it is a valid HDF5 GeoSciLib file
   then a valid FileHDF5 object is returned.

   \see create, destroy, close

   \param[in] name
       This is a string, set by the user, that holds the desired name of the existing file to open.
   \param[in] access
       This is a string, set by the user, that indicates whether the file is opened for 
        read-only access ("r"), or for read and write ("r+").

   \returns
       A valid FileHDF5 object on success.

   \par Exceptions
       Exceptions that may be raised by this method:
       NotGeoSciFile is the file exists but is not one of ours;
       FileDoesNotExist is the file is not found on the filesystem;
       FileOpenError for any error when we try to open the file.

   \example
       This function should never be called by users, instead, use the File class and its functions.

       Let's say a developer wants to open an existing file named "sirc_raco", which is in HDF5
       GeoSciLib file format:
       \code
       #include "GeoSciLib.h"
       #include <string>
       using namespace std;
       int main() 
       {
           string filename = "sirc_raco";
	   try {
	       GeoSciLib::FileHDF5 thefile = GeoSciLib::FileHDF5::open(filename);
	   }//end-try 
	   catch (...) {
	       cerr << "GeoSciLib::FileHDF5::open failure"<<endl;
	   }//end-catch
       }//end-main
       \endcode

    \details
       This is the implementation of the File interface. As such, it should not 
       generally be called by users. Users should call the FileHDF5::open method instead.
       This checks that the file is an HDF5 GeoSciLib file before opening it.
  
  */
	void openFile(const std::string& name, const std::string& access);

  
	/** \brief FileHDF5::destroy allows one to delete an HDF5 GeoSciLib file.

   FileHDF5::destroy is used to delete an existing HDF5 GeoSciLib file.

   \see create, open, close

   \param[in] name
       This is a string, set by the user, that holds the name of the file.

   \returns
       Nothing.

   \par Exceptions
       Exceptions that may be raised by this method:
       FileDoesNotExist for an nonexistent or invalid filename;
       FileDestroyError if something goes wrong when trying to delete the file.

   \example
       This function should never be called by users, instead, use the File class and its functions.

       Let's say a developer wants to delete an existing HDF5 file named "sirc_raco".
       \code
       #include "GeoSciLib.h"
       #include <string>
       using namespace std;
       int main() 
       {
           string filename = "sirc_raco";
	   try {
	       GeoSciLib::FileHDF5::destroy(filename);
	   }//end-try 
	   catch (...) {
	       cerr << "GeoSciLib::FileHDF5::destroy failure"<<endl;
	   }//end-catch
       }//end-main
       \endcode

    \details
       This is the implementation of the File interface. As such, it should not 
       generally be called by users. Users should call the FileHDF5::destroy method instead.
       This checks that the file is an HDF5 GeoSciLib file before deleting it.
  
  */
	void destroyFile(const std::string& name);


	/** \brief FileHDF5::close allows one to close an open HDF5 GeoSciLib file.

   FileHDF5::close is used to close an open HDF5 GeoSciLib file.

   \see create, destroy, open

   \returns
       Nothing.

   \par Exceptions
       Exceptions that may be raised by this method:
       std:system_error when the file close fails

   \example
       This function should never be called by users, instead, use the File class and its functions.

       Let's say a developer wants to close a file named "sirc_raco" that we recently opened.
       \code
       #include "GeoSciLib.h"
       #include <string>
       using namespace std;
       int main() 
       {
           string newfilename = "sirc_raco";
	   try {
	       GeoSciLib::FileHDF5 newfile = GeoSciLib::FileHDF5::open(newfilename);
	   }//end-try 
	   catch (...) {
	       cerr << "GeoSciLib::FileHDF5::open failure"<<endl;
	   }//end-catch

	   try {
               newfile.close();
           }//end-try
           catch (...) {
	       cerr << "GeoSciLib::FileHDF5::close failure"<<endl;
           }//end-catch
       }//end-main
       \endcode

    \details
       This is the implementation of the File interface. As such, it should not 
       generally be called by users. Users should call the FileHDF5::close method instead.
       This checks that the file is an HDF5 GeoSciLib file before opening it.
  
  */
	void closeFile();

	
	// covariant return type for virtual function ok
	ImageHDF5 *createImage(const std::string& name);
	
	ImageHDF5 *openImage(const std::string& name, const std::string& access);
	
	void destroyImage(const std::string& name);

  /** \brief FileHDF5::~FileHDF5 is the destructor for the FileHDF5 object.

   FileHDF5::~FileHDF5 is the  destructor for the FileHDF5 object. 

   \see create, destroy, open, close

   \returns
       Nothing.

   \par Exceptions
       No Exceptions are raised by this method.

    \details
       This should typically not be called by users.
  
  */
  //~FileHDF5();

};// end class File


}// end GeoSci namespace
 
#endif
// end FILEHDF5_HPP_
