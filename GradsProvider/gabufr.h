#ifndef GABUFR_H
#define GABUFR_H

#define GABUFR_X_BITS 6
#define GABUFR_Y_BITS 8

#define GABUFR_TBL_SIZE   ((1 << GABUFR_X_BITS) * (1 << GABUFR_Y_BITS))

#define GABUFR_NUM_TYPE 0
#define GABUFR_STR_TYPE 1

#define GABUFR_DEF 0
#define GABUFR_UNDEF 1


#include <stdio.h>
#include <stdlib.h>
#include "gatypes.h"
#include "config.h"

typedef struct {
	gaint scale;
	gaint offset;
	gaint width;
	gaint datatype; /* flag to indicate numerical or string data */
	char *description;
} gabufr_varinf;

typedef struct gabufr_val_struct {
	struct gabufr_val_struct * next;
	gaint x;      /* BUFR ID (F,X,Y) */
	gaint y;      /* BUFR ID (F,X,Y) */
	gaint z;      /* replication offset (vert. level), if present, or -1 */
	char undef;   /* set to GABUFR_UNDEF if packed data was all ones */
	gadouble val; /* data value when datatype is NUM, or DBL_MIN otherwise */
	char *sval;   /* data value when datatype is STR, or NULL otherwise*/
} gabufr_val;

typedef struct {
	gaint bufr_edition;
	gaint master_tbl_num;
	gaint master_tbl_version;
	gaint local_tbl_version;
} gabufr_tbl_inf;

typedef struct gabufr_msg_struct {
  struct gabufr_msg_struct * next;
  gaint year;            /* base time for entire message */
  gaint month;
  gaint day;
  gaint hour;
  gaint min;
  gaint subcnt;          /* number of subsets */
  gabufr_val ** subs;    /* array of linked lists, with size nsub; 
			    one linked list per subset in message */
  gaint fileindex;       /* index of message in file, just for reference */
  gaint is_new_tbl;      /* if 0, message contains data, otherwise it's a
		            replacement BUFR table */
  /* remainder for use during parsing */
  unsigned char *section0;
  unsigned char *section1;
  unsigned char *section3;
  unsigned char *section4;
  unsigned char *end;
  gabufr_tbl_inf tbl_inf;
} gabufr_msg;

typedef struct {
  gabufr_msg * msgs; /* linked list of decoded messages 
			(some may be missing if parsing failed */
  gaint msgcnt;      /* number of messages in file */
  /* remainder for use during parsing */
  void * buf; 
  gaint len;
} gabufr_dset;

#define GABUFR_OK 0
#define GABUFR_ERR 1

//gaint gabufr_valid_varid(gaint f, gaint x, gaint y);

typedef struct gabufr_varid_struct {
	struct gabufr_varid_struct * next;
	gaint f;
	gaint x;
	gaint y;
} gabufr_varid;

void gabufr_close(gabufr_dset * dset);
void gabufr_free_msg(gabufr_msg * msg);
void gabufr_free_val(gabufr_val * val);

#endif