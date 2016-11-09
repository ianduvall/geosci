/* GS_FileReport.c

   by Leland Pierce, nov 13, 2013

   Aug 3, 2014
   Modified for bstrings, doxygen and GS_

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"


#define ERROR -1

//#define DEBUG

herr_t GS_SummarizeGroup( hid_t g_id, const char *name, 
                          const H5L_info_t *info, void *op_data);


/*************************************************************************/
/** \brief GS_FileReport summarizes the contents of a GeoSci datafile.

\param[in] filename
                   Name of datafile to be queried. 
\param[in] report_string
                   The returned report, as a multi-line string.
                   On entry, must be a valid bstring.

\returns
Returns \c TRUE on success, \c FALSE on failure.
The \c report_string contains a multi-line string with contents like the following:\n
Filename: test123.h5 [RW], Empty File\n
 Created: 19:37:45 23-Jun-2016   , Last Update: 19:37:45 23-Jun-2016\n   
 image_aa: [Image] SIR-C Raco 102.4 SRL-1


\par Example
Obtain report on a file named "testimage.hd5".
\code
bstring report_string=bfromcstr("");
bstring filename = bfromcstr("testimage.hd5");
if(GS_FileReport(filename,report_string)){
   printf("%s",report_string);
} else {
   printf("%s\n",error_string);
}
bdestroy(filename);
bdestroy(report_string);
\endcode
*/


//#define DEBUG2

/************************************************************************/
int GS_FileReport(const_bstring filename, bstring report_string)
{
  herr_t err;
  hid_t file_id;
  hid_t root_id;
  hid_t group_id;

  bassigncstr(status_string,"");
  bassigncstr(error_string,"");

#ifdef DEBUG
  printf("GS_FileReport: 1 \n");
#endif

/* -------------------------------------------------------------------- */
/*	Open the file.	                            			*/
/* -------------------------------------------------------------------- */
  bstring access = bfromcstr("r");
  file_id = GS_FileOpen(filename,access);
  bdestroy(access);
  if( file_id < 0 ){
    bassignformat(error_string,"GS_FileReport: Could not open file: %s.\n",
            bdata(filename));
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_FileReport: 2 \n");
#endif

/* -------------------------------------------------------------------- */
/*	Open the root group.	                      			*/
/* -------------------------------------------------------------------- */
  bstring root = bfromcstr("/");
  root_id = GS_GroupOpen(file_id, root);
  bdestroy(root);
  if(root_id < 0){
    bassignformat(error_string,"GS_FileReport: Can't open the file root group.");
    GS_FileClose(file_id);
    return FALSE;
  }// endif

#ifdef DEBUG
  printf("GS_FileReport: 3 \n");
#endif

/* -------------------------------------------------------------------- */
/*	Open the file header group.                   			*/
/* -------------------------------------------------------------------- */
  group_id = GS_GroupOpen(root_id, file_metadata_name);
  if(group_id < 0){
    bassignformat(error_string,"file_report: Can't open the file _Header group.");
    GS_GroupClose(root_id);
    GS_FileClose(file_id);
    return FALSE;
  }// endif
#ifdef DEBUG
  printf("GS_FileReport: 4 \n");
#endif

/* -------------------------------------------------------------------- */
/*	Grab file-header info and print that.                           */
/* -------------------------------------------------------------------- */
  bstring descriptor_str=bfromcstr("");
  bstring descriptor = bfromcstr("descriptor");
  if(!GS_GetStringAttribute(group_id,descriptor,descriptor_str)){
    bassigncstr(descriptor_str,"...unknown...");
  }// endif
  bdestroy(descriptor);

  bstring t1 = bfromcstr("");
  bstring creation_datetime = bfromcstr("creation_datetime");
  if(!GS_GetStringAttribute(group_id,creation_datetime,t1)){
    bassigncstr(t1,"...unknown...");
  }// endif
  bdestroy(creation_datetime);

  bstring t2 = bfromcstr("");
  bstring last_update_datetime = bfromcstr("last_update_datetime");
  if(!GS_GetStringAttribute(group_id,last_update_datetime,t2)){
    bassigncstr(t2,"...unknown...");
  }// endif
  bdestroy(last_update_datetime);

  bcatcstr(report_string,"Filename: ");
  bconcat (report_string,filename);

/* -------------------------------------------------------------------- */
/* Add the Read/Write status of the dataset                             */
/* -------------------------------------------------------------------- */
  if(GS_FileIsWriteable(root_id)){
    bconcat(report_string,bsstatic(" [RW]"));
  } else {
    bconcat(report_string,bsstatic(" [RO]"));
  }//endif

  bcatcstr(report_string,", ");
  bconcat (report_string,descriptor_str);
  bcatcstr(report_string,"\n Created: ");
  bconcat (report_string,t1);
  bcatcstr(report_string,", Last Update: ");
  bconcat (report_string,t2);
  bcatcstr(report_string,"\n");
  bdestroy(descriptor_str);
  bdestroy(t1);
  bdestroy(t2);

#ifdef DEBUG
  printf("GS_FileReport: 5 report-string=%s\n",bdata(report_string));
#endif

/* -------------------------------------------------------------------- */
/*	Iterate over all the groups attached to root. 			*/
/* -------------------------------------------------------------------- */
  err =  H5Literate(root_id, H5_INDEX_NAME, H5_ITER_INC, 0, 
                    GS_SummarizeGroup, (void *)report_string);
  if(err < 0) {
    bassignformat(error_string,"GS_FileReport: Failure iterating over the groups.");
    GS_GroupClose(group_id);
    GS_GroupClose(root_id);
    GS_FileClose(file_id);
    return FALSE;
  }
#ifdef DEBUG
  printf("GS_FileReport: 6\n");
#endif

/* -------------------------------------------------------------------- */
/* at this point summarize_group has finished filling in the report     */
/* so now we just return that and we're done.                           */
/* -------------------------------------------------------------------- */



/* -------------------------------------------------------------------- */
/*	Clean up.                                			*/
/* -------------------------------------------------------------------- */
  GS_GroupClose(group_id);
  GS_GroupClose(root_id);
  GS_FileClose(file_id);
#ifdef DEBUG
  printf("GS_FileReport: 7\n");
#endif

  return TRUE;
}

/*************************************************************************/
/*! \brief GS_SummarizeGroup writes a quick summary of a group


Given a name, which is part of a given group, writes
a quick summary (if the name is a group) to the global
g_report_string.

\param[in] g_id
                  Group within which 'name' is defined.
\param[in] name
                  Name of object in that group.
\param[in] info
                 not used.
\param[in] op_data
                 Must be a valid bstring that has been casted to a (void *).

\returns
Returns FALSE (0) on success, ERROR (-1) on failure.

\par Details
This is meant to be called from within H5Literate(), so
that we can create a report for all objects in a group.
Currently we do not report on the file header group, nor on
datatypes, but only on other groups, and only if those groups
have the metadata variables: grouptype, and descriptor.

*/

/*************************************************************************/
herr_t GS_SummarizeGroup( hid_t g_id, const char *name, 
                        const H5L_info_t *info, void *op_data)
{
  hid_t group_id;
  int L;
  bstring bname=bfromcstr(name);

#ifdef DEBUG2
  printf("GS_SummarizeGroup: START, name=|%s|\n",name);
#endif
/* -------------------------------------------------------------------- */
/*  we don't want to report the existence of the file metadata group,   */
/*  so return if its that one:                                          */
/* -------------------------------------------------------------------- */
  if(EQUAL(bname,file_metadata_name)) {
    bdestroy(bname);
    return 0; 
    // (0 signals success and to continue the iteration process)
  }
  // try it without the leading "/"
  bstring headername = bfromcstr("_Header");
  if(EQUAL(bname,headername)) {
    bdestroy(bname);
    bdestroy(headername);
    return 0; 
    // (0 signals success and to continue the iteration process)
  }
  bdestroy(headername);

#ifdef DEBUG2
  printf("GS_SummarizeGroup: 1\n");
#endif

/* -------------------------------------------------------------------- */
/*	return if the g_id/name does not refer to a group:              */
/* -------------------------------------------------------------------- */
  if(!GS_ObjectIsGroup(g_id,bname)){
    // not a group, so we don't report on it.
    bdestroy(bname);
    return 0;
    // (0 signals success and to continue the iteration process)
  }// endif

#ifdef DEBUG2
  printf("GS_SummarizeGroup: 2\n");
#endif

/* -------------------------------------------------------------------- */
/*	Open the group. 	                      			*/
/* -------------------------------------------------------------------- */
  group_id = GS_GroupOpen(g_id, bname);
  if(group_id < 0){
    bstring group_name= GS_GetIDName(g_id);
    bassignformat(error_string,"summarize_group: Can't open the file group: %s.",group_name);
    bdestroy(group_name);
    bdestroy(bname);
    return ERROR;
  }// endif
#ifdef DEBUG2
  printf("GS_SummarizeGroup: 3\n");
#endif


/* -------------------------------------------------------------------- */
/*	Write report to string.	                      			*/
/* -------------------------------------------------------------------- */
  // we only write: name, group-type, group-descriptor
  bstring grouptype = bfromcstr("grouptype");
  bstring group_type = bfromcstr("");
  if(!GS_GetStringAttribute(group_id,grouptype,group_type)){
    bassigncstr(group_type,"...unknown...");
  }// endif
  bdestroy(grouptype);

  bstring descriptor = bfromcstr("descriptor");
  bstring group_descriptor=bfromcstr("");
  if(!GS_GetStringAttribute(group_id,descriptor,group_descriptor)){
    bassigncstr(group_descriptor,"...unknown...");
  }// endif
  bdestroy(descriptor);
  
  bstring report_string = (bstring)op_data;
  bcatcstr(report_string," ");
  bcatcstr(report_string,name);
  bcatcstr(report_string,": [");
  bconcat (report_string,group_type);
  bcatcstr(report_string,"] ");
  bconcat (report_string,group_descriptor);
  bcatcstr(report_string,"\n");

  bdestroy(bname);
  bdestroy(group_type);
  bdestroy(group_descriptor);
#ifdef DEBUG2
  printf("GS_SummarizeGroup: 4\n");
#endif

/* -------------------------------------------------------------------- */
/*	Close the group. 	                      			*/
/* -------------------------------------------------------------------- */
  GS_GroupClose(group_id);
     
#ifdef DEBUG2
  printf("GS_SummarizeGroup: 5\n");
#endif
     
  return 0; // (signals success and to continue the iteration process)

}

