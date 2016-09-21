CXXFLAGS=-std=c++14

test1.o: test1.cpp \
	File/File.o File/File.hpp File/File.cpp \
	FileHDF5/FileHDF5.o FileHDF5/FileHDF5.hpp FileHDF5/FileHDF5.hpp
	g++ -std=c++14 test1.cpp File/File.o FileHDF5/FileHDF5.o -o test1.o

clean:
	rm -r test1.o File/File.o FileHDF5/FileHDF5.o

File.o: File/File.cpp File/File.hpp
	g++ -std=c++14 -c File/File.cpp -o File/File.o
	
FileHDF5.o: FileHDF5/FileHDF5.hpp FileHDF5/FileHDF5.cpp
	g++ -std=c++14 -c FileHDF5/FileHDF5.cpp -o FileHDF5/FileHDF5.o
