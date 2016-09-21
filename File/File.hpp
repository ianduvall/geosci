#ifndef FILE_HPP_
#define FILE_HPP_

#include <iostream>
#include <string>
#include <map>

namespace GeoSci {

class File {
public:
	virtual ~File(){};
	virtual void printa()=0;
	virtual void printb()=0;
	
	virtual void createFile(const std::string&) = 0;
	virtual void destroyFile() = 0;
	virtual void openFile(const std::string&) = 0;
	virtual void closeFile() = 0;
	
	void process_stuff() {
		std::cout << "File::process_stuff\n\t";
		printb();
	}
};

template<typename T>
File * createT() {
  return new T();
}

struct FileFactory {
    typedef std::map<std::string, File*(*)()> map_type;

    static File * createInstance(std::string const& s) {

        map_type::iterator it = getMap()->find(s);
        if (it == getMap()->end()) {
	    std::cout << "createInstance failure."<<std::endl;
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
      getMap()->insert(std::make_pair(s, &createT<T>));
      std::cout<<"in derived_register: string="<<s<<std::endl;
    }
};

} // GeoSci

#endif
