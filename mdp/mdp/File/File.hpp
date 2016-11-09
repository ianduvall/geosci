#ifndef FILE_HPP_
#define FILE_HPP_

#include <iostream>
#include <string>
#include <map>
//#include "../Image/Image.hpp"

using namespace std;

namespace GeoSci {
	
class Image;
class File;


template<typename T>
File * createFlT() {
	return new T();
}

struct FileFactory {
    typedef std::map<std::string, File*(*)()> map_type;

	// add another createInstance with no parameter for default
	
    static File * createInstance(std::string const& s) {

        map_type::iterator it = getMap()->find(s);
        if (it == getMap()->end()) {
			std::cout << "createInstance failure." << std::endl;
			for (map_type::iterator it = getMap()->begin(); it != getMap()->end(); ++it) {
				std::cout << it->first << ", " << it->second << endl;
			}
			return 0;
		}
        return it->second();
    }

protected:
    static map_type * getMap() {
        // never delete'ed. (exist until program termination)
        // because we can't guarantee correct destruction order
      if(!map) { map = new map_type; }
        return map;
    }

private:
    static map_type * map;
};

template<typename T>
struct DerivedRegister : public FileFactory {
    DerivedRegister(std::string const& s) {
      getMap()->insert(std::make_pair(s, &createFlT<T>));
      std::cout<<"in derived_register: string=" << s << std::endl;
    }
};


class File {
public:
	virtual ~File(){};
	virtual void printa()=0;
	virtual void printb()=0;

	static File *create(const std::string& name, const std::string& filetype = "HDF5") {
		cout << "in File create" << endl;
		File* newFile = FileFactory::createInstance(filetype);
		if (newFile == 0) {
			cout << "newFile == 0" << endl;
			exit(1);
		}
		cout << "FileFactory created instance" << endl;
		newFile->createFile(name);
		cout << "hdf5 createFile ran" << endl;
		return newFile;
	}
	
	static File *open(const std::string& name, const std::string& access) {
		// TODO: determine what file format the file is
		// assuming HDF5 for everything for now
		File *aFile = FileFactory::createInstance("HDF5");
		aFile->openFile(name, access);
		return aFile;
	}
	
	static void destroy(const std::string& name) {
		// TODO: determine what file format the file is
		// assuming HDF5 for everything for now
		// do we need HDF5 specific destroy?
		//FileFactory::createInstance("HDF5")->destroyFile(name);
		return;
	}
	
	virtual void createFile(const std::string&) = 0;
	virtual void destroyFile(const std::string&) = 0;
	virtual void openFile(const std::string&, const std::string&) = 0;
	virtual void closeFile() = 0;
	
	virtual Image *createImage(const std::string& name) = 0;
	virtual Image *openImage(const std::string& name, const std::string& access) = 0;
	virtual void destroyImage(const std::string& name) = 0;
	
	void process_stuff() {
		std::cout << "File::process_stuff\n\t";
		printb();
	}
};

} // GeoSci

#endif
