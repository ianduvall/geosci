## in installing all this, needed to have hdf5 installed separately,
## and also libgs (no: I shold not have done this......)
## and need to recompile all the gaia stuff....
## wants 1.15 version of automake, I have 1.14....ugh.
## libcairo too


LIBS=/home/iand/mdp/GeoSci/dev/mdp_code_aug18_2016

HDF5_INCLUDE_DIR=/usr/include/hdf5/serial
HDF5_LIBRARY_DIR=/usr/lib/x86_64-linux-gnu/hdf5/serial
HDF5_LIBRARIES=-lhdf5 -lhdf5_cpp
##HDF5_LIBRARY_DIR=/usr/lib/i386-linux-gnu 
##HDF5_LIBRARIES=-lhdf5_serial -lhdf5_cpp
HDF5=-I$(HDF5_INCLUDE_DIR) -L$(HDF5_LIBRARY_DIR) $(HDF5_LIBRARIES)
## also need to do: export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/i386-linux-gnu/hdf5/serial


GEOPACKAGE_INCLUDES=-I$(LIBS)/gaia-gis/installdir/include
GEOPACKAGE_LIBRARY_DIR=-L$(LIBS)/gaia-gis/installdir/lib
GEOPACKAGE=$(GEOPACKAGE_INCLUDES) $(GEOPACKAGE_LIBRARY_DIR) -lsqlite3 -lgeos_c -lgeos -lproj -lcairo -lpixman-1 -lfreexl -lspatialite -lrasterlite2 $(SQDB_LIBRARIES)
## also need to do: export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/lep/OGRSF/libraries/gaia-gis/installdir/lib

HDF5_EXTRAS=-I$(LIBS)/hdf5_extras/src -L$(LIBS)/hdf5_extras/src -lgs

DEBUG=-g

test1: test1.c
	gcc $(DEBUG) -o test1 test1.c $(HDF5_EXTRAS) $(HDF5) $(GEOPACKAGE)

test2: test2.c
	gcc $(DEBUG) -o test2 test2.c $(HDF5_EXTRAS) $(HDF5) $(GEOPACKAGE)

docs: FORCE
	doxygen doxygen.config
FORCE:
