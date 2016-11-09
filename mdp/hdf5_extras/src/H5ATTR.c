/* H5ATTR.c

   written by F. Alted 2005/09/29

   Nov 12, 2013
   Added documentation: Leland Pierce

   July 8, 2014
   Modified for bstrings

   Aug 2, 2014
   Modified for doxygen

 */


#include "H5ATTR.h"


// for herr_t
#define ERROR -1
#define SUCCESS 0




/** \brief H5ATTRset_attribute_string sets the value of a string attribute

\anchor   H5ATTRset_attribute_string
\addindex H5ATTRset_attribute_string

\fn herr_t H5ATTRset_attribute_string( hid_t obj_id,
                                   const_bstring attr_name,
                                   const_bstring attr_data,
                                   hsize_t attr_size,
                                   int cset )


\see H5ATTRget_attribute_string()

\param[in]  obj_id
                   The id of the object to which the attribute should
                   be attached. This is usually a group.
\param[in] attr_name
                   The name of the attribute to be set.
\param[in] attr_data
                   The value for the attribute, as a safe string.
\param[in] attr_size
                   The number of characters in \c attr_data.
\param[in] cset
                   The character set for the string. Valid values are:
                   H5T_CSET_ASCII and H5T_CSET_UTF8.

\returns
On success it returns SUCCESS (0), on failure it returns ERROR (-1).

\par Example
Set the value of \c descriptor to "Arizona".
\code
hid_t object_id;
bstring descriptor=bfromcstr("descriptor");
bstring arizona = bfromcstr("Arizona");
if( H5ATTRset_attribute_string( object_id, descriptor,
                                arizona,7,H5T_CSET_ASCII)
                                 == ERROR){
   printf("ERROR setting the attribute value\n");
}
bdestroy(descriptor);
bdestroy(arizona);
\endcode

\par Details
This function is normally called by GS_SetStringAttribute, which is the preferred
function for setting values of string attributes.

*/



/** \brief H5ATTRget_attribute_string gets the value of a string attribute

\anchor H5ATTRget_attribute_string
\addindex H5ATTRget_attribute_string

\fn hsize_t H5ATTRget_attribute_string( hid_t obj_id,
                                    const_bstring attr_name,
                                    bstring attr_data,
                                    int *cset )


\see H5ATTRset_attribute_string()

\param[in]  obj_id
                   The id of the object to which the attribute should
                   be attached. This is usually a group.
\param[in] attr_name
                   The name of the attribute to be set.
\param[out] attr_data
                   The value for the attribute, as a safe string.
\param[in] cset
                   The character set for the string. Valid values are:
                   H5T_CSET_ASCII and H5T_CSET_UTF8.

\returns
On success it returns the length of the string, on failure it returns ERROR (-1).

\par Example
Read the value of the \c descriptor metadata item.
\code
hid_t object_id;
bstring desc_value=bfromcstr("");
bstring descriptor = bfromcstr("descriptor");
if( H5ATTRget_attribute_string( object_id, descriptor,desc_value,
                                H5T_CSET_ASCII) == ERROR){
   printf("ERROR getting the attribute value\n");
}
bdestroy(descriptor);
...use the desc_value ....
bdestroy(desc_value);
\endcode

\par Details
This function is normally called by GS_GetStringAttribute, which is the preferred
function for getting values of string attributes.

*/

//#define DEBUG


/* original header: */
/****************************************************************************
 * NCSA HDF                                                                 *
 * Scientific Data Technologies                                             *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 * Modified versions of H5LT for getting and setting attributes for open
 * groups and leaves.
 * F. Alted 2005/09/29
 *                                                                          *
 ****************************************************************************/




/*-------------------------------------------------------------------------
 *
 * Set & get attribute functions
 *
 *-------------------------------------------------------------------------
 */

/*-------------------------------------------------------------------------
 * Function: H5ATTRset_attribute
 *
 * Purpose: Create an attribute named attr_name and attach it to the
 * object specified by the name obj_name. This supports general
 * n-dimensional types (rank > 0), but if rank == 0, an H5T_SCALAR is
 * chosen.
 *
 * Return: Success: 0, Failure: -1
 *
 * Programmer: Francesc Alted
 *
 * Date: October 18, 2006
 *
 * Comments:
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */

herr_t H5ATTRset_attribute( hid_t obj_id,
                            const_bstring attr_name,
                            hid_t type_id,
                            size_t rank,
                            hsize_t *dims,
                            const_bstring attr_data )
{
 hid_t      space_id;
 hid_t      attr_id;
 int        has_attr;
 char *attr_name2;
 char *attr_data2;

 /* Create the data space for the attribute. */
 if (rank == 0)
   space_id = H5Screate( H5S_SCALAR );
 else
   space_id = H5Screate_simple( rank, dims, NULL );

 /* Verify whether the attribute already exists */
 has_attr = H5ATTRfind_attribute( obj_id, attr_name );

 attr_name2 = (char *)GMalloc(sizeof(char)*(bstrlen(attr_name)+10));
 if(!attr_name2){
   goto out;
 }
 strcpy(attr_name2,bdata(attr_name));
 /* The attribute already exists, delete it */
 if ( has_attr == TRUE ) {
   if ( H5Adelete( obj_id, attr_name2 ) < 0 ){
     H5Sclose( space_id );
     GFree(attr_name2);
     goto out;
   }//endif
 }//endif

 /* Create and write the attribute */
 attr_id = H5Acreate( obj_id, attr_name2, type_id, space_id, H5P_DEFAULT,
                      H5P_DEFAULT );
 H5Sclose( space_id );
 GFree(attr_name2);

 attr_data2 = (char *)GMalloc(sizeof(char)*(bstrlen(attr_data)));
 if(!attr_data2){
   goto out;
 }
 strcpy(attr_data2,bdata(attr_data));
 if ( H5Awrite( attr_id, type_id, attr_data2 ) < 0 ){
   H5Aclose( attr_id );
   GFree(attr_data2);
   goto out;
 }
 GFree(attr_data2);

 // success:
 H5Aclose( attr_id );
 return SUCCESS;

 // failure:
out:
 return ERROR;
}


/*-------------------------------------------------------------------------
 * Function: H5ATTRset_attribute_string
 *
 * Purpose: Creates and writes a string attribute named attr_name and attaches
 *          it to the object specified by the name obj_name.
 *
 * Return: Success: 0, Failure: -1
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 23, 2001
 *
 * Comments: If the attribute already exists, it is overwritten
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */

herr_t H5ATTRset_attribute_string( hid_t obj_id,
                                   const_bstring attr_name,
                                   const_bstring attr_data,
                                   hsize_t attr_size,
                                   int cset )
{
 hid_t      attr_type;
 hid_t      attr_space_id;
 hid_t      attr_id;
 int        has_attr;
 char *attr_name2;

#ifdef DEBUG
 printf("H5ATTRset_attribute_string: 1: attr_name=|%s|, attr_data=|%s|, attr_size=%d\n",
        bdata(attr_name), bdata(attr_data),(int)attr_size);
#endif
/* -------------------------------------------------------------------- */
/* See if the source-id is valid                                        */
/* -------------------------------------------------------------------- */
  if(!GS_ValidID(obj_id)){
    bassignformat(error_string,"H5ATTRset_attribute_string: Invalid object ID.");
#ifdef DEBUG
    printf("H5ATTRset_attribute_string: 1.1: obj_id is invalid: %d\n",(int)obj_id);
#endif
    return ERROR;
  }// endif

#ifdef DEBUG
 printf("H5ATTRset_attribute_string: 2\n");
#endif

 /* Create the attribute */
 if ( (attr_type = H5Tcopy( H5T_C_S1 )) < 0 )
  goto out;

 if ( ( ( cset == H5T_CSET_ASCII ) || ( cset == H5T_CSET_UTF8 ) ) &&
      ( H5Tset_cset( attr_type, cset ) < 0 ) )
  goto out;

 if ( cset == H5T_CSET_ASCII )
  attr_size += 1; /* extra null term */

 if ( ( attr_size > 0 ) && ( H5Tset_size( attr_type, attr_size) < 0 ) )
  goto out;

 if ( H5Tset_strpad( attr_type, H5T_STR_NULLTERM ) < 0 )
  goto out;

 if ( (attr_space_id = H5Screate( H5S_SCALAR )) < 0 )
  goto out;

#ifdef DEBUG
 printf("H5ATTRset_attribute_string: 3\n");
#endif


 /* Verify if the attribute already exists */
 has_attr = H5ATTRfind_attribute( obj_id, attr_name );

 attr_name2 = (char*)GMalloc(sizeof(char)*(bstrlen(attr_name)+10));
 if(!attr_name2){
   goto out;
 }
 strcpy(attr_name2,bdata(attr_name));
 /* The attribute already exists, delete it */
 if ( has_attr == 1 )
 {
   if ( H5Adelete( obj_id, attr_name2 ) < 0 ) {
     GFree(attr_name2);
     goto out;
   }
 }
 GFree(attr_name2);
#ifdef DEBUG
 printf("H5ATTRset_attribute_string: 4\n");
#endif

 /* Create and write the attribute */

 char *attr_str2;
 attr_str2 = (char *)GMalloc(sizeof(char)*(bstrlen(attr_name)+bstrlen(attr_data)+10));
 if(!attr_str2) {
   goto out;
 }
 strcpy(attr_str2,bdata(attr_name));
 if ( (attr_id = H5Acreate( obj_id, attr_str2, attr_type, attr_space_id,
                            H5P_DEFAULT, H5P_DEFAULT )) < 0 ) {
   GFree(attr_str2);
   goto out;
 }
 
 strcpy(attr_str2,bdata(attr_data));
 if ( H5Awrite( attr_id, attr_type, attr_str2 ) < 0 ){
   GFree(attr_str2);
   goto out;
 }
 GFree(attr_str2);

 if ( H5Aclose( attr_id ) < 0 )
  goto out;

 if ( H5Sclose( attr_space_id ) < 0 )
  goto out;

 if ( H5Tclose(attr_type) < 0 )
  goto out;

 return SUCCESS;

out:
 return ERROR;
}


/*-------------------------------------------------------------------------
 * Function: H5ATTRget_attribute
 *
 * Purpose: Reads an attribute named attr_name with the memory type type_id
 *
 * Return: Success: 0, Failure: -1
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: September 19, 2002
 *
 * Comments:
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */

herr_t H5ATTRget_attribute( hid_t obj_id,
                            const_bstring attr_name,
                            hid_t type_id,
                            void *data )
{

 /* identifiers */
 hid_t attr_id;
 char *attr_name2;

 attr_name2= (char *)GMalloc(sizeof(char)*(bstrlen(attr_name)+10));
 if(!attr_name2) {
   goto out;
 }
 strcpy(attr_name2,bdata(attr_name));
 if ( ( attr_id = H5Aopen_by_name(obj_id, ".", attr_name2,
                                  H5P_DEFAULT, H5P_DEFAULT) ) < 0 ) {
   GFree(attr_name2);
   return ERROR;
 }
 GFree(attr_name2);

 if ( H5Aread( attr_id, type_id, data ) < 0 )
  goto out;

 if ( H5Aclose( attr_id ) < 0 )
  return ERROR;

 return SUCCESS;

out:
 H5Aclose( attr_id );
 return ERROR;
}


/*-------------------------------------------------------------------------
 * Function: H5ATTRget_attribute_string
 *
 * Purpose: Reads an string attribute named attr_name.
 *
 * Return: Success: 0, Failure: -1
 *
 * Programmer: Francesc Alted, faltet@pytables.com
 *
 * Date: February 23, 2005
 *
 * Comments:
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */

hsize_t H5ATTRget_attribute_string( hid_t obj_id,
                                    const_bstring attr_name,
                                    bstring data,
                                    int *cset )
{
 /* identifiers */
 hid_t      attr_id;
 hid_t      attr_type;
 hsize_t    type_size = 0;
 htri_t     is_vlstr = 0;
 char *local_data;
 char *attr_name2;

 bassigncstr(status_string,"");
 bassigncstr(error_string,"");


 local_data = NULL;

 attr_name2 = (char *)GMalloc(sizeof(char)*(bstrlen(attr_name)+10));
 if(!attr_name2) {
   goto out;
 }
 strcpy(attr_name2,bdata(attr_name));
 if(H5Aexists( obj_id, attr_name2 ) <= 0 ){
   bassignformat(error_string,"H5ATTRget_attribute_string: attribute: %s, does not exist\n",bdata(attr_name));
   GFree(attr_name2);
   return ERROR;
 }// endif

 if ( ( attr_id = H5Aopen_by_name(obj_id, ".", attr_name2,
                                  H5P_DEFAULT, H5P_DEFAULT) ) < 0 ) {
   GFree(attr_name2);
   return ERROR;
 }
 GFree(attr_name2);

 if ( (attr_type = H5Aget_type( attr_id )) < 0 )
  goto out;

 if ( ( cset != NULL ) && ( ( *cset = H5Tget_cset( attr_type ) ) < 0 ) )
  goto out;

 //is_vlstr = H5Tis_variable_str( attr_type );
 //if ( is_vlstr == 0 )
 //{
  /* Get the size */
  if ( (type_size = H5Tget_size( attr_type )) < 0 )
   goto out;

  /* Malloc space enough for the string, plus 1 for the trailing '\0' */
  local_data = (char *)GMalloc((unsigned long)(type_size+1));
  if(!local_data){
    goto out;
  }
  if ( H5Aread( attr_id, attr_type, local_data ) < 0 ) {
    GFree(local_data);
    goto out;
  }
  /* Set the last character to \0 in case we are dealing with space
     padded strings */
  local_data[type_size] = '\0';
  
  bassigncstr(data,local_data);
  GFree(local_data);

  //}
  //else
  //{
  // if ( H5Aread( attr_id, attr_type, data ) < 0 )
  // goto out;
  //
  // type_size = strlen( *data );
  // }

 if ( H5Tclose( attr_type ) < 0 )
  goto out;

 if ( H5Aclose( attr_id ) < 0 )
  return ERROR;

 return type_size;

out:
 H5Tclose( attr_type );
 H5Aclose( attr_id );
 if (local_data != NULL) {
   GFree(local_data);
   local_data = NULL;
 }
 return ERROR;
}



/*-------------------------------------------------------------------------
 *
 * Helper functions
 *
 *-------------------------------------------------------------------------
 */

/*-------------------------------------------------------------------------
 * Function: find_attr
 *
 * Purpose: operator function used by H5ATTRfind_attribute
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: June 21, 2001
 *
 * Comments:
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */

static herr_t find_attr( hid_t loc_id,
                         const char *name,
                         const H5A_info_t *ainfo,
                         void *op_data)
{

 /* Define a default zero value for return. This will cause the
  * iterator to continue if the palette attribute is not found yet.
  */

 int ret = 0;
 bstring attr_name = bfromcstr("");
 bstring thename = bfromcstr("");
 bassigncstr(attr_name,(char*)op_data);
 bassigncstr(thename,name);

 //bstring attr_name = bfromcstr((char*)op_data);
 //bstring thename   = bfromcstr(name);

 /* Shut the compiler up */
 loc_id=loc_id;

 /* Define a positive value for return value if the attribute was
  * found. This will cause the iterator to immediately return that
  * positive value, indicating short-circuit success
  */

 if( EQUAL( thename, attr_name ) ) {
   bdestroy(attr_name);
   bdestroy(thename);
   ret = 1;
 }

 //lep bdestroy(attr_name);
 //lep bdestroy(thename);
 return ret;
}


/*-------------------------------------------------------------------------
 * Function: H5ATTRfind_attribute
 *
 * Purpose: Inquires if an attribute named attr_name exists attached
 * to the object loc_id.
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: June 21, 2001
 *
 * Comments:
 *  The function uses H5Aiterate with the operator function find_attr
 *
 * Return:
 *  Success: The return value of the first operator that
 *              returns non-zero, or zero if all members were
 *              processed with no operator returning non-zero.
 *
 *  Failure: Negative if something goes wrong within the
 *              library, or the negative value returned by one
 *              of the operators.
 *
 *-------------------------------------------------------------------------
 */

herr_t H5ATTRfind_attribute( hid_t loc_id,
                             const_bstring attr_name )
{

 hsize_t attr_num;
 herr_t  ret;
 char *attr_name2;

 attr_num = 0;

 attr_name2 = (char *)GMalloc(sizeof(char)*(bstrlen(attr_name)+10));
 if(!attr_name2) {
   return ERROR;
 }
 strcpy(attr_name2,bdata(attr_name));
 ret = H5Aiterate( loc_id, H5_INDEX_CRT_ORDER, H5_ITER_NATIVE, &attr_num,
                   find_attr, (void *)attr_name2 );
 GFree(attr_name2);
 
 return ret;
}



/*-------------------------------------------------------------------------
 * Function: H5ATTRget_attribute_ndims
 *
 * Purpose: Gets the dimensionality of an attribute.
 *
 * Return: Success: 0, Failure: -1
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: September 4, 2001
 *
 *-------------------------------------------------------------------------
 */

herr_t H5ATTRget_type_ndims( hid_t obj_id,
                             const_bstring attr_name,
                             hid_t *type_id,
                             H5T_class_t *class_id,
                             size_t *type_size,
                             int *rank )
{
 hid_t       attr_id;
 hid_t       space_id;
 char *attr_name2;

 attr_name2=(char *)GMalloc(sizeof(char)*(bstrlen(attr_name)+10));
 if(!attr_name2){
   return ERROR;
 }
 strcpy(attr_name2,bdata(attr_name));
 /* Open the attribute. */
 if ( ( attr_id = H5Aopen_by_name(obj_id, ".", attr_name2,
                                  H5P_DEFAULT, H5P_DEFAULT) ) < 0 )
 {
   GFree(attr_name2);
  return ERROR;
 }
 GFree(attr_name2);

 /* Get an identifier for the datatype. */
 *type_id = H5Aget_type( attr_id );

 /* Get the class. */
 *class_id = H5Tget_class( *type_id );

 /* Get the size. */
 *type_size = H5Tget_size( *type_id );

 /* Get the dataspace handle */
 if ( (space_id = H5Aget_space( attr_id )) < 0 )
  goto out;

 /* Get rank */
 if ( (*rank = H5Sget_simple_extent_ndims( space_id )) < 0 )
  goto out;

 /* Terminate access to the attribute */
 if ( H5Sclose( space_id ) < 0 )
  goto out;

 /* End access to the attribute */
 if ( H5Aclose( attr_id ) )
  goto out;

 return SUCCESS;

out:
 H5Tclose( *type_id );
 H5Aclose( attr_id );
 return ERROR;
}


/*-------------------------------------------------------------------------
 * Function: H5ATTRget_dims
 *
 * Purpose: Gets information about an attribute.
 *
 * Return: Success: 0, Failure: -1
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: September 4, 2001
 *
 *-------------------------------------------------------------------------
 */

herr_t H5ATTRget_dims( hid_t obj_id,
                       const_bstring attr_name,
                       hsize_t *dims)
{
 hid_t       attr_id;
 hid_t       space_id;
 char *attr_name2;

 attr_name2 = (char *)GMalloc(sizeof(char)*(bstrlen(attr_name)+10));
 if(!attr_name2){
   return ERROR;
 }
 strcpy(attr_name2,bdata(attr_name));
 /* Open the attribute. */
 if ( ( attr_id = H5Aopen_by_name(obj_id, ".", attr_name2,
                                  H5P_DEFAULT, H5P_DEFAULT) ) < 0 )
 {
   GFree(attr_name2);
   return ERROR;
 }
 GFree(attr_name2);

  /* Get the dataspace handle */
 if ( (space_id = H5Aget_space( attr_id )) < 0 )
  goto out;

 /* Get dimensions */
 if ( H5Sget_simple_extent_dims( space_id, dims, NULL) < 0 )
  goto out;

 /* Terminate access to the dataspace */
 if ( H5Sclose( space_id ) < 0 )
  goto out;

  /* End access to the attribute */
 if ( H5Aclose( attr_id ) )
  goto out;

 return SUCCESS;

out:
 H5Aclose( attr_id );
 return ERROR;
}
