BOOST_INCLUDE_DIR=/home/lep/OGRSF/libraries/boost/boost_1_60_0
BOOST_LIBRARY_DIR=/home/lep/OGRSF/libraries/boost/boost_1_60_0/stage/lib
BOOST_LIBRARIES=-lboost_filesystem -lboost_system
BOOST=-I$(BOOST_INCLUDE_DIR) -L$(BOOST_LIBRARY_DIR) $(BOOST_LIBRARIES)
## also need to do: export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/lep/OGRSF/libraries/boost/boost_1_60_0/stage/lib

HDF5_INCLUDE_DIR=/usr/include/hdf5/serial
HDF5_LIBRARY_DIR=/usr/lib/x86_64-linux-gnu/hdf5/serial
HDF5_LIBRARIES=-lhdf5 -lhdf5_cpp
HDF5=-I$(HDF5_INCLUDE_DIR) -L$(HDF5_LIBRARY_DIR) $(HDF5_LIBRARIES)
## also need to do: export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/i386-linux-gnu/hdf5/serial


LIBS=/home/iand/mdp/GeoSci/dev/mdp_code_aug18_2016

GEOPACKAGE_INCLUDES=-I$(LIBS)/gaia-gis/installdir/include
GEOPACKAGE_LIBRARY_DIR=-L$(LIBS)/gaia-gis/installdir/lib
GEOPACKAGE=$(GEOPACKAGE_INCLUDES) $(GEOPACKAGE_LIBRARY_DIR) -lsqlite3 -lgeos_c -lgeos -lproj -lcairo -lpixman-1 -lfreexl -lspatialite -lrasterlite2 $(SQDB_LIBRARIES)
## also need to do: export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/lep/OGRSF/libraries/gaia-gis/installdir/lib

HDF5_EXTRAS=-I$(LIBS)/hdf5_extras/src -L$(LIBS)/hdf5_extras/src -lgs





## base class stuff:
# for now, only need this for Exceptions.hpp
BASE_INCLUDE_DIR=..

## depends on the compiler:
#DIALECT=-std=c++0x
#DIALECT=-std=c++11
DIALECT=-std=c++14

DEBUG=-g

test1.o: test1.cpp File/File.hpp File/File.o FileHDF5.hpp FileHDF5.o ImageHDF5.hpp ImageHDF5.o RasterHDF5.hpp RasterHDF5.o MetadataHDF5.hpp MetadataHDF5.o  attributes.o attributes.hpp
	g++ $(DEBUG) $(DIALECT) -o test1.o test1.cpp File/File.o FileHDF5.o ImageHDF5.o RasterHDF5.o MetadataHDF5.o attributes.o -I$(BASE_INCLUDE_DIR) $(BOOST) $(HDF5)  $(HDF5_EXTRAS) $(HDF5)

test2: test2.cpp FileHDF5.hpp FileHDF5.o ImageHDF5.hpp ImageHDF5.o RasterHDF5.hpp RasterHDF5.o MetadataHDF5.hpp MetadataHDF5.o  attributes.o attributes.hpp
	g++ $(DEBUG) $(DIALECT) -o test2 test2.cpp FileHDF5.o ImageHDF5.o RasterHDF5.o MetadataHDF5.o attributes.o -I$(BASE_INCLUDE_DIR) $(BOOST) $(HDF5)  $(HDF5_EXTRAS) $(HDF5)

test3: test3.cpp FileHDF5.hpp FileHDF5.o ImageHDF5.hpp ImageHDF5.o RasterHDF5.hpp RasterHDF5.o MetadataHDF5.hpp MetadataHDF5.o attributes.o attributes.hpp
	g++ $(DEBUG) $(DIALECT) -o test3 test3.cpp FileHDF5.o ImageHDF5.o RasterHDF5.o MetadataHDF5.o attributes.o -I$(BASE_INCLUDE_DIR) $(BOOST) $(HDF5)  $(HDF5_EXTRAS) $(HDF5)

test4: test4.cpp FileHDF5.hpp FileHDF5.o ImageHDF5.hpp ImageHDF5.o RasterHDF5.hpp RasterHDF5.o MetadataHDF5.hpp MetadataHDF5.o attributes.o attributes.hpp
	g++ $(DEBUG) $(DIALECT) -o test4 test4.cpp FileHDF5.o ImageHDF5.o RasterHDF5.o MetadataHDF5.o attributes.o -I$(BASE_INCLUDE_DIR) $(BOOST) $(HDF5)  $(HDF5_EXTRAS) $(HDF5)

test5: test5.cpp FileHDF5.hpp FileHDF5.o ImageHDF5.hpp ImageHDF5.o RasterHDF5.hpp RasterHDF5.o MetadataHDF5.hpp MetadataHDF5.o attributes.o attributes.hpp
	g++ $(DEBUG) $(DIALECT) -o test5 test5.cpp FileHDF5.o ImageHDF5.o RasterHDF5.o MetadataHDF5.o attributes.o -I$(BASE_INCLUDE_DIR) $(BOOST) $(HDF5)  $(HDF5_EXTRAS) $(HDF5)

test6: test6.cpp FileHDF5.hpp FileHDF5.o ImageHDF5.hpp ImageHDF5.o RasterHDF5.hpp RasterHDF5.o MetadataHDF5.hpp MetadataHDF5.o attributes.o attributes.hpp
	g++ $(DEBUG) $(DIALECT) -o test6 test6.cpp FileHDF5.o ImageHDF5.o RasterHDF5.o MetadataHDF5.o attributes.o -I$(BASE_INCLUDE_DIR) $(BOOST) $(HDF5)  $(HDF5_EXTRAS) $(HDF5)

test7: test7.cpp FileHDF5.hpp FileHDF5.o ImageHDF5.hpp ImageHDF5.o RasterHDF5.hpp RasterHDF5.o MetadataHDF5.hpp MetadataHDF5.o attributes.o attributes.hpp
	g++ $(DEBUG) $(DIALECT) -o test7 test7.cpp FileHDF5.o ImageHDF5.o RasterHDF5.o MetadataHDF5.o attributes.o -I$(BASE_INCLUDE_DIR) $(BOOST) $(HDF5)  $(HDF5_EXTRAS) $(HDF5)


File/File.o: File/File.cpp File/File.hpp
	g++ -std=c++14 -c File/File.cpp -o File/File.o

FileHDF5.o: FileHDF5.cpp FileHDF5.hpp ../Exceptions.hpp attributes.hpp
	g++ $(DEBUG) -c -o FileHDF5.o FileHDF5.cpp -I$(BOOST_INCLUDE_DIR) -I$(BASE_INCLUDE_DIR) -I$(HDF5_INCLUDE_DIR) $(HDF5_EXTRAS)

ImageHDF5.o: ImageHDF5.cpp ImageHDF5.hpp ../Exceptions.hpp attributes.hpp
	g++ $(DEBUG) -c -o ImageHDF5.o ImageHDF5.cpp -I$(BOOST_INCLUDE_DIR) -I$(BASE_INCLUDE_DIR) -I$(HDF5_INCLUDE_DIR)  $(HDF5_EXTRAS)

RasterHDF5.o: RasterHDF5.cpp RasterHDF5.hpp ../Exceptions.hpp attributes.hpp
	g++ $(DEBUG) -c -o RasterHDF5.o RasterHDF5.cpp -I$(BOOST_INCLUDE_DIR) -I$(BASE_INCLUDE_DIR) -I$(HDF5_INCLUDE_DIR)  $(HDF5_EXTRAS)

MetadataHDF5.o: MetadataHDF5.cpp MetadataHDF5.hpp ../Exceptions.hpp attributes.hpp
	g++ $(DEBUG) -c -o MetadataHDF5.o MetadataHDF5.cpp -I$(BOOST_INCLUDE_DIR) -I$(BASE_INCLUDE_DIR) -I$(HDF5_INCLUDE_DIR)  $(HDF5_EXTRAS)

attributes.o: attributes.cpp attributes.hpp
	g++ $(DEBUG) -c -o attributes.o attributes.cpp -I$(BASE_INCLUDE_DIR) -I$(HDF5_INCLUDE_DIR)  $(HDF5_EXTRAS)



clean:
	rm -r *.hd5 test*.o
