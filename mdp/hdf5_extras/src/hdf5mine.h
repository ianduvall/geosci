#ifndef HDF5MINE_H_
#define HDF5MINE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*! \file */

//#define GSP_READ 0
//#define GSP_WRITE 1

bstring GS_H5ObjectTypeAsString(int objtype);
bstring GS_GSObjectTypeAsString(int objtype);

int GS_CheckFileExistence(const_bstring filename);
void GS_init();

int     GS_ValidID(hid_t id);
bstring GS_GetIDName(hid_t id);

int GS_GetStringAttribute( hid_t object_id, const_bstring name, bstring value);
int GS_SetStringAttribute( hid_t object_id, const_bstring name, const_bstring value);

int GS_ObjectIsFile(hid_t id);
int GS_ObjectIsDataset(hid_t id, const_bstring name);
int GS_ObjectIsDatasetByID(hid_t id);
int GS_ObjectIsGroup(hid_t id, const_bstring name);
int GS_ObjectIsGroupByID(hid_t id);

int GS_CheckInternalName(hid_t object_id, const_bstring thename);
hid_t GS_CreateAccessPlist(void);

hid_t GS_FileCreate(const_bstring filename);
int   GS_FileDelete(const_bstring filename);
hid_t GS_FileOpen(const_bstring dbname, const_bstring access);
int   GS_FileFlush(hid_t object_id);
int   GS_FileClose(hid_t file_id);
int   GS_FileCloseAllObjects(hid_t file_id);
int   GS_FileIsWriteable(hid_t id);
int   GS_FileCopy(bstring oldname, bstring newname);
int   GS_FileIsOpen(const_bstring filename);
int   GS_FileRename(const_bstring oldname, const_bstring newname);
int   GS_FileReport(const_bstring filename, bstring report_string);
hid_t GS_GetValidFileID(hid_t id);

bstring GS_GetFilename(hid_t id);
herr_t GS_SetCacheSize(hid_t file_id, size_t cache_size);
int GS_FileCommitDatatypes(hid_t file_id);


char *GS_Dirname(const char *name);


hid_t GS_DatasetCreate(hid_t source, const_bstring name, int datatype,
                       int ndims, int *size, int is_extendable, 
                       int is_compressed);
int   GS_DatasetDelete(hid_t file_id, const_bstring dataset_name);
hid_t GS_DatasetOpen(hid_t source, const_bstring name);
hid_t GS_DatasetClose(hid_t id);
int   GS_DatasetRename(hid_t group_id, const_bstring oldname, const_bstring newname);
hid_t GS_DatasetGetParent(hid_t id);
int   GS_DatasetGetDatatype(hid_t id, const_bstring name);
int   GS_DatasetGetDatatypeByID(hid_t id);
int   GS_DatasetGetDimensionsByID(hid_t id, long int **size);
int   GS_DatasetGetDimensions(hid_t id, const_bstring name, long int **size);
hid_t GS_DatasetCopy(hid_t source, hid_t destination_group_id, 
                     const_bstring destination_dataset_name);

    int GS_DatasetRead(hid_t dataset_id, const long int *offsets,
                     const long int *sizes, int datatype, void *buffer);
int GS_DatasetWrite(hid_t dataset_id, const long int *offsets,
                     const long int *sizes, int datatype, const void *buffer);


int GS_DataConversion( int in_type, const void *in_buffer,
                       int out_type, void *out_buffer, 
                       int nelements);
int GS_RasterTypeNumbytes(int datatype );


int   GS_ConvertFromHDFDatatype(hid_t datatype);
hid_t GS_ConvertToHDFDatatype(int datatype);
int   GS_HDFDatatypeClose(hid_t id);




bstring GS_PathnameNodir(const_bstring name);
bstring GS_PathnameGetDir(const_bstring name);
bstring GS_PathnameGetHDFDir(const_bstring name);
bstring GS_PathnameJoin(const_bstring front, const_bstring back);


hid_t   GS_GroupCreate(hid_t file_id, const_bstring groupname);
hid_t   GS_GroupOpen(hid_t source, const_bstring name);
int     GS_GroupClose(hid_t id);
int     GS_GroupCloseAllObjects(hid_t id);
hid_t   GS_GroupCopy(hid_t source, hid_t destination_object_id, 
                     const_bstring destination_group_name);
int GS_GroupRename(hid_t id, const_bstring oldname, const_bstring newname);
int GS_GroupDelete(hid_t file_id, const_bstring group_name );


  ////////////////////////////////////////////////////////////////////////

#define GS_READ 0
#define GS_WRITE 1

  //#define GS_DATASET_TYPE_UNKNOWN       0
  //#define GS_DATASET_TYPE_INTERNAL_FILE 1
  //#define GS_DATASET_TYPE_RASTER        2
  //#define GS_DATASET_TYPE_METADATA      3
  //#define GS_DATASET_TYPE_METADATA_IFILE 4

#define GS_IFILE_SUCCESS 1
#define GS_IFILE_FAILURE 2



// geosci group types in hdf5 files:
#define GS_OBJECT_TYPE_UNKNOWN        0
#define GS_OBJECT_TYPE_METADATA_GROUP 1
#define GS_OBJECT_TYPE_IMAGE    2
#define GS_OBJECT_TYPE_VECTOR   3
#define GS_OBJECT_TYPE_VECTOR2D 4
#define GS_OBJECT_TYPE_VECTOR3D 5
#define GS_OBJECT_TYPE_TIN      6
#define GS_OBJECT_TYPE_MESH2D   7
#define GS_OBJECT_TYPE_MESH3D   8
#define GS_OBJECT_TYPE_RASTER   9
#define GS_OBJECT_TYPE_IFILE   10
#define GS_OBJECT_TYPE_FILE    11
#define GS_OBJECT_TYPE_METADATA_DATASET 12
#define GS_OBJECT_TYPE_METADATA_IFILE   13
#define GS_OBJECT_TYPE_UNDER_CONSTRUCTION -1


// geosci coordinates units:
#define GS_COORDINATES_UNKNOWN 0
#define GS_COORDINATES_DEGREES 1
#define GS_COORDINATES_METERS  2
#define GS_COORDINATES_PIXELS  3





/* complex types: */
/* needs updating based on machine defaults....*/
/* tried using the int8_t, etc types but it would not compile...??? */
/* LEP: dec 1, 2013 */
/*
typedef char complex ci16_t;
typedef short int complex ci32_t;
typedef int complex ci64_t;
typedef float complex c64_t;
typedef double complex c128_t;

*/



// the functions:
int GS_AppendMetadata(hid_t object_id, const_bstring metadata_name, 
                      const_bstring value);
int GS_FileCreateMetadata(hid_t file_id, const_bstring descriptor);
  //int GS_ImageCreateMetadata(hid_t file_id, const_bstring image_name, 
  //                        const_bstring typestring, const_bstring chanstr, 
  //                        const_bstring pixstr, const_bstring linestr, 
  //                        const_bstring descriptor);
int GS_ImageCreateMetadata(hid_t file_id, const_bstring image_name,  
                        const_bstring descriptor);
int GS_VectorCreateMetadata(hid_t file_id, const_bstring vector_name, 
                         const_bstring descriptor);

int GS_RasterCreateMetadata(hid_t image_id, const_bstring raster_name, 
                            int raster_type, int npixels, int nlines,
                            const_bstring descriptor);



  //int GS_ObjectGetType(hid_t object_id);
int GS_ObjectIsIFileByID( hid_t dataset_id );
int GS_ObjectIsIFile( hid_t id, const_bstring name );
int GS_ObjectIsImageByID( hid_t id );
int GS_ObjectIsImage(hid_t file_id, const_bstring image_name);
int GS_ObjectIsMetadataDatasetByID( hid_t dataset_id );
int GS_ObjectIsMetadataDataset( hid_t id, const_bstring name );
int GS_ObjectIsMetadataIFileByID( hid_t dataset_id );
int GS_ObjectIsMetadataIFile( hid_t id, const_bstring name );
int GS_ObjectIsRasterByID( hid_t dataset_id );
int GS_ObjectIsRaster( hid_t id, const_bstring name );
int GS_ObjectIsVectorByID( hid_t id );
int GS_ObjectIsVector( hid_t id, const_bstring name );
int GS_ObjectLock(hid_t id);
int GS_ObjectUnlock(hid_t id);
int GS_SplitHistory(const_bstring history_string, bstring front, bstring back);
int GS_UpdateMetadata( hid_t object_id, const_bstring name, bstring value);
int GS_ValidMetadataName(int object_type, const_bstring name);


hid_t GS_ConvertToHDFDatatype(int datatype);

int           GS_DatatypeIsComplex(int datatype);
bstring GS_DatatypeAsString(int datatype);

bstring GS_DatasetReport(hid_t dataset_id);
int     GS_DatasetIsWriteable(hid_t file_id, const_bstring dataset_name);
int     GS_DatasetIsWriteableByID(hid_t dataset_id);

int     GS_GroupIsWriteable(hid_t file_id, const_bstring group_name);
int     GS_GroupIsWriteableByID(hid_t group_id);
bstring GS_GroupReport(hid_t group_id);

int     GS_GroupGetType(hid_t group_id);
int     GS_GroupSetType(hid_t group_id, int typecode);

bstring GS_GroupGetDatum(hid_t group_id);
bstring GS_GroupGetProjection(hid_t group_id);
int     GS_GroupGetBounds(hid_t group_id, double *xmin, double *xmax, 
                          double *ymin, double *ymax);

bstring GS_DatasetGetDatum(hid_t dataset_id);
bstring GS_DatasetGetProjection(hid_t dataset_id);
int     GS_DatasetGetBounds(hid_t dataset_id, double *xmin, double *xmax, 
                          double *ymin, double *ymax);



int GS_ObjectGetH5Children( hid_t object_id, int *nobjs, 
                          bstring **objnames, int **objtypes);
int GS_ObjectGetGSChildren( hid_t object_id, int *nobjs, 
                          bstring **objnames, int **objtypes);

int GS_DatasetGetType(hid_t dataset_id);
  int GS_DatasetSetType(hid_t dataset_id, int typecode);

  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////






hid_t GS_ImageCreate(hid_t file_id, const_bstring image_name, 
               const_bstring image_descriptor);
int   GS_ImageClose( hid_t image_id);
hid_t GS_ImageOpen( hid_t file_id, const_bstring image_name);
int   GS_ImageDelete( hid_t file_id, const_bstring image_name);

int GS_AppendHistory(hid_t object_id, const_bstring history_line);
int GS_AbortedHistory(hid_t object_id, const_bstring history_line);

hid_t GS_RasterCreate(hid_t image_id, const_bstring raster_name, 
                   int raster_type, int npixels, int nlines,
                   const_bstring descriptor);
hid_t GS_RasterOpen( hid_t image_id, const_bstring raster_name);
int   GS_RasterClose( hid_t raster_id);
int   GS_RasterDelete( hid_t image_id, const_bstring raster_name);
bstring GS_GetRasterTypeAsString(int datatype);
int GS_RasterPixelSize(hid_t raster_id,int func, double *x_pixelsize,
                   double *y_pixelsize, bstring pixel_units);
int GS_RasterSizeInfo(hid_t raster_id, int *npixels, int *nlines);
int GS_RasterCheckWindow( hid_t file_id, const_bstring image_name, int *window);
int GS_RasterCheckWindowByID( hid_t image_id,  int *window);
int GS_CheckWindowInside( int *window_in, const int *window_enclosing );

  ///////////////////////////////////////////////////////////////////////
  //yet int GetRasterTypes(hid_t image_id, int **types, int *ntypes);
  //yet int GetRasterTypeNumbytes(int datatype );
  //yet int GetRasterTypeAsInteger(char *typestring);
int GS_RasterTypeIsInteger(int datatype);
int GS_RasterTypeIsComplex(int datatype);
int GS_CheckUniqueName(hid_t file_id, const char *thename);







    
  ///////////////////////////////////////////////////////////////////////

#ifdef OLDSTUFF

  //int GetStringAttribute( hid_t group_id, const char *name, char **value);

  //int FileReport(const char *filename, char **report_string);
int ImageReport(const char *filename, const char *image_name, 
                char **report_string);
int FreeReportString(char *str);


  //done//hid_t ImageOpen( hid_t file_id, const char *image_name);
  //done//int   ImageClose( hid_t image_id);
  //done//int   ImageDelete( hid_t file_id, const char *image_name);
  //done//hid_t ImageAdd(hid_t file_id, const char *image_name, 
  //               const char *image_descriptor,
  //               int pixels, int lines,int channels,
  //               int *channel_types, const char **channel_descriptors);
int ImageCopy( hid_t file_from, const char *image_from, 
               int *channels_from, int *window_from,
               hid_t file_to,   const char *image_to,   
               int *channels_to,   int *window_to);

int ImageCopyByID( hid_t image_from, const char *raster_name_from, int *window_from,
                   hid_t image_to,   const char *raster_name_to,   int *window_to);

int ImageImport(const char *infilename,const char *outfilename, 
                const char *outimagename);
int ImageChannelAdd(hid_t file_id, const char *cimage_name, 
                    int channels, int *channel_types, 
                    const char **channel_descriptors);
int ImageChannelDelete(hid_t file_id, const char *cimage_name, 
                       int *channels);



  //hid_t GroupOpen(hid_t gid, const char *name);
  //int GroupClose(hid_t id );
  //hid_t GroupCreate(hid_t file_id, const char *groupname);
  //int GroupDelete(hid_t file_id, const char *group_name );



  //hid_t FileCreate(const char *filename, int pixels, int lines, const int channels[]);
  //hid_t FileCreateEmpty(const char *filename);
  //hid_t FileOpen(const char *dbname, const char *access);
  //int   FileClose(hid_t file_id);
  //int   FileFlush(hid_t object_id);


  //void  Time (char *string, int format);

  //herr_t SetCacheSize(hid_t file_id, size_t cache_size);

  //hid_t  CreateAccessPlist(void);

int ImageSizeInfo(hid_t image_id, int *npixels, int *nlines, int *nchannels);

  //int FileWriteable( hid_t id);
  //char *GetFilename(hid_t id);
int CheckWindow( hid_t file_id, const char *image_name, int *window);
int CheckWindowByID( hid_t image_id,  int *window);
int CheckWindowInside( int *window_in, const int *window_enclosing );
int CheckChannels( hid_t file_id, const char *image_name, 
                   const int *channel_list, int nchans, 
                   int *is_valid_channel, int *is_writeable_channel);
int CheckChannelsValid( hid_t file_id, const char *image_name, 
                        const int *channel_list, int nchans);
int CheckChannelsWriteable( hid_t file_id, const char *image_name, 
                            const int *channel_list, int nchans);

int CheckDatatype(int datatype);
  //hid_t GetHdf5Type(int datatype);
char *GetRasterTypeAsString(int datatype);
int HDF5Type2GSPType(hid_t datatype);



void DisableHDF5ErrorReporting(void);
void EnableHDF5ErrorReporting(void);

  //int ValidID(hid_t id);
  //hid_t GetValidFileID(hid_t id);


  //int ObjectIsDataset(hid_t id, const char *name);
  //int ObjectIsDatasetByID(hid_t id);
  //int ObjectIsFile(hid_t id);
  //int ObjectIsImage(hid_t id, const char *name);
  //int ObjectIsImageByID( hid_t id );
  //int ObjectIsVector( hid_t id, const char *name );
  //int ObjectIsVectorByID( hid_t id );
  //int ObjectIsGroup(hid_t file_id, const char *name);
  //int ObjectIsGroupByID(hid_t id);
  //int ObjectIsMetadataDataset( hid_t id, const char *name );
  //int ObjectIsMetadataDatasetByID( hid_t dataset_id );
  //int ObjectIsMetadataIFileByID( hid_t dataset_id );




hid_t	RasterOpen( hid_t image_id, const char *raster_name);
hid_t	RasterClose( hid_t raster_id);
int GetRasterTypes(hid_t image_id, int **types, int *ntypes);
int GetRasterTypeNumbytes(int datatype );
int GetRasterTypeAsInteger(char *typestring);
int GS_RasterTypeIsInteger(int datatype);
int GS_RasterTypeIsComplex(int datatype);

void Progress( const char *name, double frac);


  //hid_t DatasetOpen(hid_t source, const char *name);
  //hid_t DatasetClose(hid_t id);
  //int DatasetDelete(hid_t file_id, const char *dataset_name);
  //int DatasetGetType(hid_t dataset_id);
  //int DatasetSetType(hid_t dataset_id, int typecode);



int ImageIO( hid_t file_id, const char *image_name, int func,
             int xoff, int yoff, int xsize, int ysize,
             void *buffer, int *datatypes, int *buffer_window, 
             int nchans, int *chanmap, int write_history);


//#include "generic_image.h"

double GetDoubleValue(void *buf,int index, int from_type,
                      int to_type, int reim);
int PutDoubleValue(double value, void *buf,int index, 
                   int to_type, int reim);

double complex GetDoubleComplexValue(void *buf,int index, int from_type,
                                     int to_type);
int PutDoubleComplexValue(double complex value, void *buf,int index, 
                          int to_type);




long int GetSystemMemory(void);
long int GetFreeMemory(void);

  //char *Dirname(const char *name);


  //int ObjectIsRaster( hid_t id, const char *name );
  //int ObjectIsRasterByID( hid_t datasetid );














int SpatialrefCreate(hid_t file_id, const char *object_name, 
                     const char *code, const char *wkt,
                     const char *proj4);

int SpatialrefIO(hid_t file_id, const char *object_name, int func,
                 char *code, char *wkt, char *proj4);

int SpatialrefReport(const char *filename, const char *object_name, 
                     char **report_string);


int SpatialrefCopy(hid_t from_file_id, const char *from_object_name, 
                   hid_t   to_file_id, const char   *to_object_name);

//char *GSPGetAuthorityCode(OGRSpatialReferenceH asr);

//int GetObjectType(hid_t object_id);
//int GS_GetGroupType(hid_t file_id, const_bstring group_name);
//int GS_GetGroupTypeByID(hid_t object_id);
//int GroupTypeString2Int(const char *grouptypestring);















//int CreateFileMetadata(hid_t file_id, const char *descriptor);
//int CreateImageMetadata(hid_t file_id, const char *image_name, 
//                        const char *typestring, const char *chanstr, 
//                        const char *pixstr, const char *linestr, 
//                        const char *descriptor);
//int CreateVectorMetadata(hid_t file_id, const char *vector_name, 
//                         const char *descriptor);


//int UpdateFileMetadata( hid_t file_id, const char *name, const char *value);
//int AppendFileMetadata( hid_t file_id, const char *name, const char *value);
//int ValidFileMetadataName(const char *name);
//int AppendMetadata(hid_t object_id, const char *metadata_name, const char *value);
//int UpdateMetadata( hid_t object_id, const char *name, const char *value);


//int UpdateImageMetadata( hid_t file_id, const char *image_name, 
//                         const char *name, const char *value);
//int AppendImageMetadata( hid_t file_id, const char *image_name, 
//                         const char *name, const char *value);
//int UpdateImageMetadataByID( hid_t image_id, const char *name, 
//                             const char *value);

//int UpdateRasterMetadata( hid_t file_id, const char *image_name,
//                          const char *raster_name,
//                          const char *name, const char *value);

//int ValidMetadataName(int object_type, const char *name);

int UpdateDescriptor( hid_t object_id, const char *descriptor);
  //done//int AbortedHistory(hid_t object_id, const char *history_line);
  //DONE//int AppendHistory(hid_t object_id, const char *history_line);
  //int SplitHistory(char *history_string, char **front, char **back);


int CoordinatesCreate(hid_t file_id, const char *object_name, 
                      double ULx, double ULy, double LRx, double LRy);

int CoordinatesCopy(hid_t from_file_id, const char *from_object_name, 
                   hid_t   to_file_id, const char   *to_object_name);

int CoordinatesIO(hid_t file_id, const char *object_name, int func,
                  double *ULx, double *UULy, double *ULRx, double *ULRy, 
                  int *units);
int CoordinatesReport(hid_t file_id, const char *object_name, 
                     char *report_string);


int VectorDatabaseInit(hid_t file_id, const char *vector_name);

//sqlite3 *VectorDatabaseOpen(hid_t vector_id);

int GetVectorTableDef(const char *filename, char **table_name,
                      int *geometry_type, int *dimension,
                      char ***column_types, char ***column_names);


hid_t VectorAdd(hid_t file_id, const char *vector_name, 
                const char *vector_descriptor);

//int OSRImport(OGRSpatialReferenceH asr, OGRDataSourceH hDS);

int SpatialrefImport( const char *filename, char *code, 
                      char *wkt, char *proj4 );

int DecodeSRID( const char *code, int *code_number );




















// defs for very generic geometry types for vectors
// these include 2d, 2.5-d, and on-multi and multi- versions of each
#define GSP_POINT 1
#define GSP_LINE  2
#define GSP_POLYGON 3
#define GSP_COLLECTION 4

//defs for more specific geometry types:
#define GSP_POINT2 1
#define GSP_LINE2  2
#define GSP_POLYGON2 3
#define GSP_COLLECTION2 4
#define GSP_RING2 5
#define GSP_POINT25 6
#define GSP_LINE25  7
#define GSP_POLYGON25 8
#define GSP_COLLECTION25 9

char *GetVectorTypeAsString(int datatype);
int GetVectorNumShapes(hid_t vector_id,int *num_types,int *types,
                       long int *num_per_type);
int GetVectorBounds(hid_t vector_id,int *num_types,int *types,
                    double *bounds_per_type);


  //int GetChildren( hid_t object_id, int *nobjs, 
  //                 char ***objnames, int **objtypes);
char *H5ObjectTypeAsString(int objtype);
int GetGSPChildren( hid_t object_id, int *nobjs, 
                 char ***objnames, int **objtypes);
char *GSPObjectTypeAsString(int objtype);


// for debug only:
int report_num_objects_open(void);
int count_num_objects_open(void);



  //int DatasetRename(hid_t group_id, const char *oldname, const char *newname);

  //int FileRename(const char *oldname, const char *newname);
  //int FileIsOpen(const char *filename);
  //int FileDelete(const char *filename);

  //int ObjectLock(hid_t id);
hid_t GetRasterParent(hid_t id);
int GetRasterNumber(hid_t id);
int ChannelWritabilityIO(hid_t image_id, int func, int *nchannels,
                         int *writeability);
  //int ObjectUnlock(hid_t id);

  //done//hid_t RasterCreate(hid_t image_id, const char *raster_name, 
  //                   int raster_type, int nnpixels, int nnlines,
  //                   const char *descriptor);
int RasterDelete( hid_t image_id, const char *raster_name);




hid_t PyramidCheck(hid_t image_id, int npyramids, int *xsizes, int *ysizes);
hid_t PyramidCreate(hid_t image_id);
int PyramidDelete(hid_t image_id);
hid_t PyramidExist(hid_t image_id);
hid_t PyramidCalcSizes(int npixels, int nlines, 
                      int *npyramids, int **xsizes, int **ysizes);
hid_t PyramidUpdate(hid_t image_id);
int DatetimeCompare(const char *time2, const char *time1, double *difference);

int CheckRasterWriteableByID(hid_t raster_id);




  //char *PathnameDir(const char *name);
  //char *PathnameNodir(const char *name);
  //char *PathnameJoin(const char *front, const char *back);





int ImageFilterAverage( hid_t file_from, const char *image_from, int *channels_from, 
                        int *filter_size, const char *mask, int nthreads, int memsize_gbytes,
                        hid_t file_mask, const char *image_mask, int channel_mask,
                        int *window_mask,
                        hid_t file_to,   const char *image_to,   int *channels_to);

int ImageFilterBoxcarByID( hid_t in_raster_id, 
                           int *filter_size, double *filter_coeffs,
                           const char *mask, int nthreads, int memsize_gbytes,
                           hid_t raster_mask_id,int *window_mask,
                           hid_t out_raster_id);


//int LocationCreate(hid_t file_id, const char *object_name, 
//                   GeoLocationMetadata_t *location_hdr);

//int LocationIO(hid_t file_id, const char *object_name, int func,
//               GeoLocationMetadata_t *location_hdr);

int LocationCopy(hid_t from_file_id, const char *from_object_name, 
                   hid_t   to_file_id, const char   *to_object_name);

int LocationReport(const char *filename, const char *object_name, 
                     char **report_string);








// reading files with keys and values in ascii:
void set_keyval_comment(char comment_character);
int open_keyval(char *filename, char *mode);
int close_keyval(int filehandle);
int read_keyval_int(int fileh,char *location);
double read_keyval_dbl(int fileh,char *location);
char *read_keyval_str(int fileh,char *location);
int *read_keyval_intvec(int fileh,char *location, int *n);
double *read_keyval_dblvec(int fileh,char *location, int *n);
char **read_keyval_strvec(int fileh,char *location, int *n);

/********************************************************************/
/*  formatted_read.h  7-96                                          */
/*                                                                  */
/*  by Leland Pierce    (lep@eecs.umich.edu)                        */
/*                                                                  */
/*  This is a set of subroutines for doing reading                  */
/*  of any type of data from a binary string.                       */
/*                                                                  */
/*  Data types: ASCII: int, double, string  BINARY: int, float      */
/*  scalars only.                                                   */
/*                                                                  */
/*  A pointer to each dynamically-allocated variable is             */
/*  returned.                                                       */
/*                                                                  */
/*  Usage:                                                          */
/*        #include "formatted_read.h"                               */
/*                                                                  */
/*        link with formatted_read.o                                */
/*                                                                  */
/*  =============================================================== */

/*-------------------------------------------*/
/*     PUBLIC CONSTANTS:                     */
/*-------------------------------------------*/

/* integer formats that are possible: */
#define TWOS_COMPLEMENT 1
#define SIGN_MAGNITUDE  2

/* real formats that are possible: */
#define IEEE_754             1
#define VAX_F_FLOATING       2
#define IBM_360_370_E_FORMAT 3

/* Possible machine names, for use in formattedReadInit */
#define SUN      1
#define INTEL_PC 2
#define VAX      3
#define IBM360   4
#define HP9000   5
#define APOLLO   6
#define SGI      7
#define DEC_UNIX 8
/* others? */



/*-------------------------------------------*/
/*     PUBLIC FUNCTIONS:                     */
/*-------------------------------------------*/

void formattedReadInit(int machineName);

void formattedReadRawInit(int intByteOrder[4], int   intFormat, 
                          int floatByteOrder[4], int floatFormat);

void *formattedRead(unsigned char *string, char *format);

//==============================================================
//=============end of formatted_read.h==========================
//==============================================================
//==============================================================
//==============================================================


/*-----------------------------------------------------------------*/
/*                Reading URLs as files                            */
/*-----------------------------------------------------------------*/
#include <curl/curl.h>
enum fcurl_type_e {
  CFTYPE_NONE=0,
  CFTYPE_FILE=1,
  CFTYPE_CURL=2
};

struct fcurl_data
{
  enum fcurl_type_e type;     /* type of handle */
  union {
    CURL *curl;
    FILE *file;
  } handle;                   /* handle */

  char *buffer;               /* buffer to store cached data*/
  size_t buffer_len;          /* currently allocated buffers length */
  size_t buffer_pos;          /* end of data in buffer*/
  int still_running;          /* Is background url fetch still in progress */
};

typedef struct fcurl_data URL_FILE;

URL_FILE *url_fopen (const char *url,const char *operation);
int url_fclose (URL_FILE *file);
int url_feof (URL_FILE *file);
size_t url_fread (void *ptr, size_t size, size_t nmemb, URL_FILE *file);
char * url_fgets (char *ptr, size_t size, URL_FILE *file);
void url_rewind (URL_FILE *file);
//==============================================================
//=============end of reading urls as files=====================
//==============================================================

void convert_inertial_to_fixed(double *x, double *y, double *z, 
                               double *vx, double *vy, double *vz, double gha);
double GreenwichMeanSiderealTimeDegrees(double julian_date);
double GreenwichApparentSiderealTimeDegrees(double julian_date);

//==============================================================
// reading xml files...basic stuff
int open_xml(const char *filename, char *mode);
int close_xml(int filehandle);
int read_xml_int(int fileh,char *location);
double read_xml_dbl(int fileh,char *location);
char *read_xml_str(int fileh,char *location);
int *read_xml_intvec(int fileh,char *location, int *n);
double *read_xml_dblvec(int fileh,char *location, int *n);
char **read_xml_strvec(int fileh,char *location, int *n);

//==============================================================
int polyfit(int npts, double *inx,double *iny, int ncoeffs, double *coeffs);

//==============================================================
//for reading hdf5 files that aren't geoscipy files:
hid_t open_hdf5(const char *filename, char mode);
herr_t close_hdf5(hid_t filehandle);
int read_hdf5_int(hid_t file,char *location,char *name);
double read_hdf5_dbl(hid_t file,char *location,char *name);
char *read_hdf5_str(hid_t file,char *location,char *name);
double *read_hdf5_dblvec(hid_t file,char *location,char *name, int *n);
double *read_hdf5_dblmat(hid_t file,char *location,char *name, int *n, int *m);
void read_hdf5_dataset_dimensions(hid_t file,char *location, 
                                  int *ndims, int **dims);
int hdf5_dump(int argc, const char *argv[]);

//==============================================================
void Geodetic2XYZ(double lat, double lon, double h,
                  double *x, double *y, double *z,
                  double major, double flattening);
void XYZ2Geodetic(double x, double y, double z,
                  double *lat, double *lon, double *h,
                  double major, double flattening);
//void Any2XYZ(double projx, double projy, double projheight, 
//             ProjInfo_t *projinfo,
//             double *x, double *y, double *z);
//==============================================================

//temporarily here (this is pure PCI.....)
typedef struct {
  int    pbx;     /* No. of elements computed for BETA b(X)       	*/
  int    pby;     /* No. of elements computed for BETA b(Y)       	*/
  double rbx[21]; /* Actual current BETAS b(X)		          	*/
  double rby[21]; /* Actual current BETAS b(Y)		            	*/
  double rabx[3]; /* Actual current affine BETAS b(X)	     	  	*/
  double raby[3]; /* Actual current affine BETAS b(Y)	       		*/
  double covx[21][21]; /* covariance matrix for b(X)                    */
  double covy[21][21]; /* covariance matrix for b(Y)                    */
} GEOInfo_t;


void    GCPRegression2(GEOInfo_t *geo, 
                       double *rix, double *riy, 
                       double *rox, double *roy, 
                       int ngcp, int *order, double *rmsx, double *rmsy );

void    GCPTransform(GEOInfo_t *geo, double rix, double riy, 
                     double *roxhat, double *royhat);

void    GCPTransform2(GEOInfo_t *geo, double rix, double riy, 
                     double *roxhat, double *royhat,
                     double *roxerr, double *royerr);

//==============================================================

double JulianDateOfYear(int year);
double DayOfYear(int year, int month, int day);

int GetSpatialref_UTM_WGS84(int zone,char row, char *code, 
                            char *wkt, char *proj4);





int RasterDescriptorIO(hid_t image_id, const char *raster_name, 
                       int func, char *descriptor);



int RawHeaderGetSize(hid_t file_id, const char *object_name);
int RawHeaderIO(hid_t file_id, const char *object_name, int func,
                void *hdr, int hdrlength);
int RawHeaderCreate(hid_t file_id, const char *object_name, 
                    void *hdr, int hdrlength);


//==============================================================

void    DestroyStringList( char **list );
int     CountStringList( char **list );
char    **AddStringToList( char **target_list, char *new_string );

//==============================================================







#endif
  //OLDSTUFF











#ifdef __cplusplus
}
#endif

#endif
