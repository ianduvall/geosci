LIBS=/home/iand/Downloads/mdp_code_aug18_2016

HDF5_INCLUDE_DIR=/usr/include/hdf5/serial
HDF5_LIBRARY_DIR=/usr/lib/i386-linux-gnu/hdf5/serial
HDF5_LIBRARIES=-lhdf5 -lhdf5_cpp
HDF5=-I$(HDF5_INCLUDE_DIR) -L$(HDF5_LIBRARY_DIR) $(HDF5_LIBRARIES)
## also need to do: export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/i386-linux-gnu/hdf5/serial

GEOPACKAGE_INCLUDES=-I$(LIBS)/gaia-gis/installdir/include
GEOPACKAGE_LIBRARY_DIR=-L$(LIBS)/gaia-gis/installdir/lib
GEOPACKAGE=$(GEOPACKAGE_INCLUDES) $(GEOPACKAGE_LIBRARY_DIR) -lsqlite3 -lgeos_c -lgeos -lproj -lcairo -lpixman-1 -lfreexl -lspatialite -lrasterlite2 $(SQDB_LIBRARIES)
## also need to do: export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/lep/OGRSF/libraries/gaia-gis/installdir/lib


DEBUG=-g

all: libgs.a

bstrlib.o: bstrlib.c
	gcc $(DEBUG) -c -o bstrlib.o bstrlib.c

gmalloc.o: gmalloc.c gmalloc.h
	gcc $(DEBUG) -c -o gmalloc.o gmalloc.c


H5ATTR.o: H5ATTR.c H5ATTR.h 
	gcc $(DEBUG) -c -o H5ATTR.o H5ATTR.c $(HDF5) $(GEOPACKAGE)

string_funcs.o: string_funcs.c string_funcs.h
	gcc $(DEBUG) -c -o string_funcs.o string_funcs.c

hdf5vfs.o: hdf5vfs.c hdf5vfs.h
	gcc $(DEBUG) -c -o hdf5vfs.o hdf5vfs.c $(HDF5) $(GEOPACKAGE)

bstrlen.o: bstrlen.c
	gcc $(DEBUG) -c -o bstrlen.o bstrlen.c

.SUFFIXES:
.SUFFIXES: .h .c .o .so

AA = $(shell ls *IFile*.c GS_*.c)
SOURCES = $(AA)
OBJECTS = $(SOURCES:.c=.o)
SOBJECTS = $(SOURCES:.c=.so)

.c.o: ifile.h
	gcc ${DEBUG} -c -o $@ ${HDF5}  $(GEOPACKAGE) \
	-DSQLITE_TEMP_STORE=3  -DSQLITE_ENABLE_LOCKING_STYLE=0 -pg $<

libgs.a: $(OBJECTS) bstrlib.o gmalloc.o H5ATTR.o string_funcs.o bstrlen.o hdf5vfs.o
	rm -f libgs.a
	ar cr libgs.a $(OBJECTS) bstrlib.o gmalloc.o H5ATTR.o string_funcs.o bstrlen.o hdf5vfs.o
	ranlib libgs.a


