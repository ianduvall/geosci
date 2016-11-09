/* GS_Time.c

   modified from code provided by PCI, inc, by Leland Pierce, Nov, 2013.

   July 7, 2014:
   modified to use bstrings

   Aug 2, 2014
   modfied for doxygen, GS_

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>

#include "bstrlib.h"
#include "gmalloc.h"
#include "H5ATTR.h"
#include "globals.h"



void      GS_getdate( bstring string );
void      GS_GetMS( bstring string );




/** \brief GS_Time get the local time and date

GS_Time() returns a formatted  character TIME
and/or DATE string obtained from the operating system.

The  time  string is based on a  24 hour clock  (e.g.,  ten past
2 p.m. would appear as 14:10).  The overall format is controlled 
by the format argument as any of 7 different time, date, or time
and date formats. 
Use number 7 for compatibility with the rest of the GeoSci C library.

\param[out] bstring string;                                  
                String that will be updated with the current date/time. 
                It must be a valid safe string when this routine is called.
\param[in] format;                                     
                Integer selector for which format of date and/or 
                time is desired. One of:
\li Integer: selected string
\li 0 :  "HH:MM:SS        "
\li 1 :  "DD-MMM-YY       "
\li 2 :  "DD-MM-YY        "
\li 3 :  "MM-DD-YY        "
\li 4 :  "HH:MM DD-MMM-YY "
\li 5 :  "HH:MM:SS:sss    "
\li 6 :  "SSSSSSSSS.sss   "
\li 7 :  "HH:MM:SS DD-MMM-YYYY"

\par Example
Get the curent data and time in GeoSci format:
\code
bstring thedatetime=bfromcstr("");
GS_Time (thedatetime, 7);
...use thedatetime...
bdestroy(thedatetime);
\endcode

\par Details
This code is derived from source provided by PCI, Inc and so may 
need to be rewritten to avoid lawsuits.

*/



void      GS_Time (bstring string, int format)
{
  bstring tstring=bfromcstr("");
  // month names of the year:
  bstring months=bfromcstr("JanFebMarAprMayJunJulAugSepOctNovDec");
  int            month;

  bstring ms=bfromcstr("");

  time_t            clock;

/* -------------------------------------------------------------------- */
/*      Get time from operating system                              */
/* -------------------------------------------------------------------- */
  GS_getdate( tstring );
  //printf("000 tstring=|%s|\n",bdata(tstring));
/* -------------------------------------------------------------------- */
/*      Determine the month encoding in 'tstring'.                  */
/* -------------------------------------------------------------------- */
  // get the substring in tstring containing the month:
  bstring themonth = bmidstr(tstring,4,3);
  // find in the months string:
  month = binstr(themonth,0,months);
  if(month>=0){
    month /=3; // convert to 0->11
    month++;   // convert to 1->12
  } else {
    //error
    month = 1;
  }//endif

/* -------------------------------------------------------------------- */
/*      Clear the output string to spaces.                              */
/* -------------------------------------------------------------------- */
      bassigncstr( string, "                    " );

/* -------------------------------------------------------------------- */
/*      Ensure a leading zero on the day.                               */
/* -------------------------------------------------------------------- */
      bstring zero=bfromcstr("0");
      if (bchar(tstring,8) == ' ' )
          breplace(tstring,8,1,zero,' ');
      bdestroy(zero);
      
/* -------------------------------------------------------------------- */
/*      Format output strings.                                          */
/* -------------------------------------------------------------------- */
      switch (format)
      {
        case 0:
          //strncpy( string, tstring+11, 8 );
          breplace(string,0,8,bmidstr(tstring,11,8),' ');
          break;

        case 1:
          //strncpy( string, tstring+8, 2 );
          breplace(string,0,2,bmidstr(tstring,8,2),' ');
          binsertch(string,2,1,'-'); //string[2] = '-';
          //strncpy( string+3, tstring+4, 3 );
          breplace(string,3,3,bmidstr(tstring,4,3),' ');
          binsertch(string,6,1,'-'); //string[6] = '-';
          //strncpy( string+7, tstring+22, 2 );
          breplace(string,7,2,bmidstr(tstring,22,2),' ');
          break;

        case 2:
          //strncpy( string, tstring+8, 2 );
          breplace(string,0,2,bmidstr(tstring,8,2),' ');
          binsertch(string,2,1,'-'); //string[2] = '-';
          //sprintf( string+3, "%2d", month );
          breplace(string,3,3,bformat("%2d", month),' ');
          if( bchar(string,3) == ' ' )
            binsertch(string,3,1,'0');
          binsertch(string,5,1,'-'); //string[5] = '-';
          //strncpy( string+6, tstring+22, 2 );
          breplace(string,6,2,bmidstr(tstring,22,2),' ');
          break;

        case 3:
          breplace(string,0,3,bformat("%2d", month ),' ');
          if( bchar(string,0) == ' ' )
            binsertch(string,0,1,'0');//string[0] = '0';
          binsertch(string,2,1,'-'); //string[2] = '-';
          //strncpy( string+3, tstring+8, 2 );
          breplace(string,3,2,bmidstr(tstring,8,2),' ');
          binsertch(string,5,1,'-'); //string[5] = '-';
          //strncpy( string+6, tstring+22, 2 );
          breplace(string,6,2,bmidstr(tstring,22,2),' ');
          break;

        case 4:
          //strncpy( string, tstring+11, 5 );
          breplace(string,0,5,bmidstr(tstring,11,5),' ');
          //strncpy( string+6, tstring+8, 2 );
          breplace(string,6,2,bmidstr(tstring,8,2),' ');
          binsertch(string,8,1,'-'); //string[8] = '-';
          //strncpy( string+9, tstring+4, 3 );
          breplace(string,9,3,bmidstr(tstring,4,3),' ');
          binsertch(string,12,1,'-'); //string[12] = '-';
          //strncpy( string+13, tstring+22, 2 );
          breplace(string,13,2,bmidstr(tstring,22,2),' ');
          break;

        case 5:
          //strncpy( string, tstring+11, 8 ); 
          breplace(string,0,8,bmidstr(tstring,11,8),' ');
          binsertch(string,8,1,':'); //string[8] = ':';
          GS_GetMS( ms );
          //strncpy( string+9, ms, 3 );
          breplace(string,9,3,ms,' ');
          break;

        case 6:
          time( &clock );
          //sprintf( string, "%9ld", clock );
          breplace(string,0,9,bformat("%9ld", clock ),' ');
          GS_GetMS( ms );
          binsertch(string,9,1,'.'); //string[9] = '.';
          //strncpy( string+10, ms, 3 );
          breplace(string,10,3,ms,' ');
          break;

        case 7:
          // date
          //printf("tstring=|%s|\n",bdata(tstring));
          //strncpy( string, tstring+11, 8 );
          breplace(string,0,8,bmidstr(tstring,11,8),' ');
          //printf("1 string=|%s|\n",bdata(string));

          // time
          //strncpy( string+9, tstring+8, 2 );
          breplace(string,9,2,bmidstr(tstring,8,2),' ');
          //printf("2 string=|%s|\n",bdata(string));
          
          binsertch(string,9+2,1,'-'); //string[9+2] = '-';
          //printf("3 string=|%s|\n",bdata(string));
          //strncpy( string+9+3, tstring+4, 3 );
          breplace(string,9+3,3,bmidstr(tstring,4,3),' ');
          //printf("4 string=|%s|\n",bdata(string));
          binsertch(string,9+6,1,'-'); //string[9+6] = '-';
          //printf("5 string=|%s|\n",bdata(string));
          //strncpy( string+9+7, tstring+20, 4 );
          breplace(string,9+7,4,bmidstr(tstring,20,4),' ');
          //printf("6 string=|%s|\n",bdata(string));

          binsertch(string,20,1,' '); //string[20] = ' ';
          //printf("7 string=|%s|\n",bdata(string));

          breplace(string,21,3,bmidstr(tstring,25,3),' ');
          
          break;

      }

      bdestroy(tstring);
      bdestroy(months);
      bdestroy(ms);

}

/************************************************************************/
/*                        GSgetdate()                        */
/*                                                      */
/*      Currently this seems to work on all systems, including the VAX!      */
/************************************************************************/


void      GS_getdate( bstring string )
{
      time_t            clock;

      time( &clock );

      time_t t = time(NULL);
      struct tm lt = {0};

      localtime_r(&t, &lt);

      //printf("The time zone is '%s'.\n", lt.tm_zone);

      //printf("GS_getdate: time=|%s|\n",ctime(&clock));

      bassignblk(string,ctime(&clock),24);
      
      bcatcstr(string," ");
      bcatcstr(string,lt.tm_zone);
      //printf("GS_getdate: time_string=|%s|\n",bdata(string));

}


/************************************************************************/
/*      Get the current milliseconds value.                        */
/************************************************************************/

void      GS_GetMS ( bstring string )
{

  struct timeval      time_value;
  int            milli_sec;

  gettimeofday( &time_value, NULL );

  milli_sec = (time_value.tv_usec + 500) / 1000.0;
  bassignformat( string, "%03d", milli_sec );

}
