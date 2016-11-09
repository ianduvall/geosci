#ifndef IFILE_H_
#define IFILE_H_

#include <hdf5.h>
#include <sqlite3.h>
#include <string.h>

#include "globals.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file */


//int ObjectIsMetadataIFile( hid_t id, const char *name );
//int ObjectIsMetadataIFileByID( hid_t dataset_id );
//int ObjectIsIFile( hid_t id, const char *name );
//int ObjectIsIFileByID( hid_t datasetid );

// ifile stuff: ----------------------------------
#define IFILE_CLOSED 0
#define IFILE_R 1
#define IFILE_W 2
#define IFILE_A 3
#define IFILE_RP 4
#define IFILE_WP 5
#define IFILE_AP 6
#define IFILE_WX 7
#define IFILE_WXP 8

typedef struct {
  hid_t ifile_id;
  int readPastEOF;
  int last_operation_status;
  int access;
  long int file_position;
  long int size;
  bstring  last_operation_status_message;
} IFILE;

IFILE *IFileOpen(hid_t file_id, const char *ifilename, const char *access);
int IFileClearError( IFILE *ifilep);
int IFileClose( IFILE *ifilep);
int IFileEOF( IFILE *ifilep);
int IFileError( IFILE *ifilep);
int IFileFlush( IFILE *ifilep);
int IFileGetc( IFILE *ifilep);
char *IFileGets( IFILE *ifilep, char *str, int num );
int IFileSeek( IFILE *ifilep, long int offset, int origin );
long int IFileTell( IFILE *ifilep );
int IFileRewind( IFILE *ifilep );
int IFilePerror( IFILE *ifilep, const char *str );
int IFileReadAccess( IFILE *ifilep);
int IFileReadAccessHDF( hid_t dataset_id);
int IFileWriteAccess( IFILE *ifilep, int code);
int IFileAllocate( IFILE *ifilep, long int offset, long int length );
int IFilePrintf( IFILE *ifilep, const char *format, ... );
int IFilePutc( IFILE *ifilep, char character );
int IFilePuts( IFILE *ifilep, const char *string );
size_t IFileRead( IFILE *ifilep, void *buf, int size, int count);
int IFileReadStatus( IFILE *ifilep, char **message);
int IFileWriteStatus( IFILE *ifilep, int status, const char *message);
int IFileWriteStatusMessage( IFILE *ifilep, const char *message);
int IFileScanf( IFILE *ifilep, const char *format, ... );
int IFileSetEOF( IFILE *ifilep, int value );
int IFileSetWriteability( hid_t file_id, const char *name, int property );
int IFileGetWrite( IFILE *ifilep );
int IFileSetWrite( IFILE *ifilep, int property );
int IFileGetWriteDataset( hid_t dataset_id ); /////////////consider renaming
long int IFileSize( IFILE *ifilep );
long int IFileSizeHDF( IFILE *ifilep );
int IFileTruncate( IFILE *ifilep, long int length );
int IFileWrite( IFILE *ifilep, const void *buf, int size, int count);
int IFileReadALine(IFILE *ifilep, char *str, int num );
//---------------------------------------------------

//#define DATASET_TYPE_UNKNOWN       0
//#define DATASET_TYPE_INTERNAL_FILE 1
//#define DATASET_TYPE_RASTER        2
//#define DATASET_TYPE_METADATA      3
//#define DATASET_TYPE_METADATA_IFILE 4

#define IFILE_SUCCESS 1
#define IFILE_FAILURE 2


#ifdef __cplusplus
}
#endif

#endif
