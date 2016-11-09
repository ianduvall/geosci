#include <iostream>
#include <string>

#include "Raster/Raster.hpp"
#include "Image/Image.hpp"
#include "File/File.hpp"
#include "Exceptions.hpp"

using namespace GeoSci;
using namespace std;

#include <sys/types.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <complex.h>

extern "C" {
	#include "hdf5_extras.h" //../hdf5_extras/src/
};

int main() {
    cout << "main starting\n" << endl;
    
    int i,j;
    
    long int sizes[2];

    sizes[0]=20;
    sizes[1]=20;

	GS_init();

    long int offsets[2];
    offsets[0]=0;
    offsets[1]=0;

    u_int8_t buffer[20*20];
    u_int8_t buffer2[20*20];
    for(i=0;i<20;i++){
        for(j=0;j<20;j++){
            buffer[20*j+i]=20*j+i;
            buffer2[20*j+i]=0;
            if(i==5)printf("(%d,%d)=%d\n",i,j,buffer[20*j+i]);
        }
    }
    cout << endl;

	try {
		
		string filename = "myFirstFile.h5";
		File *myFile = File::create(filename, "HDF5");
		cout << "myFile type: ";
		myFile->printa();
		cout << "HDF5 instance of myFirstFile.h5 created successfully.\n" << endl;
		
		File *myFileDefault = File::create("myFileDefault.h5");
		cout << "myFileDefault type: ";
		myFileDefault->printa();
		cout << "HDF5 instance of myFileDefault.h5 created successfully.\n" << endl;
		
		cout << "Testing closeFile\n";
		myFileDefault->closeFile();
		cout << "Testing destroyFile\n";
		File::destroy("myFileDefault.h5");
		cout << "Closed and destroyed myFileDefault.h5" << endl;
		
		myFile->printa();
		myFile->printb();
		myFile->process_stuff();
		
		cout << "Testing closeFile\n";
		myFile->closeFile();
		
		cout << "Testing openFile\n";
		File *openedFile = File::open(filename, "r+");
		
		cout << "Testing createImage\n";
		Image *myImage = openedFile->createImage("myImage.img");
		
		cout << "Testing closeImage\n";
		myImage->closeImage();
		
		cout << "Testing openImage\n";
		Image *openedImage = openedFile->openImage("myImage.img", "r+");
		
		cout << "Testing createRaster\n";
		Raster *myRaster = openedImage->createRaster("myRaster.ras", sizes, GS_DATATYPE_UI8); //GS_DATATYPE_UI8
		
		myRaster->printMyType();
		
		cout << "Testing writing to myRaster.ras\n";
		i = myRaster->write8U(offsets, sizes, buffer);
		cout << "Successfully wrote to myRaster.ras, return value: " << i << endl;
		
		cout << "Testing closeRaster\n";
		myRaster->closeRaster();
		openedImage->closeImage();
		openedFile->closeFile();
		
		File *f = File::open(filename, "r+");
		Image *img = f->openImage("myImage.img", "r+");
		Raster *ras = img->openRaster("myRaster.ras");
		
		cout << "Testing reading from myRaster.ras\n";
		int readVal = ras->read8U(offsets, sizes, buffer2);
		cout << "Successfully read from myRaster.ras, return value: " << readVal << endl;
		
		for(i=0;i<20;i++){
			for(j=0;j<20;j++){
				if (i==5) {
					printf("(%d,%d)=%d\n",i,j,buffer2[20*j+i]);
				}
			}
		}
		cout << endl;
		
		ras->closeRaster();
		img->closeImage();
		f->closeFile();
		
		
		cout << "End of test case! Everything ran successfully!" << endl;
    }
    catch (exception& e) {
		std::cout << "Thrown Error: " << e.what() << '\n';
	} 
    catch (...) {
		std::cout << "There was an error somewhere..." << std::endl;
	}
    
    
    return 0;
} // main
