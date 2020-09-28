#include "gabufr.h"


#ifndef GABUFR_DEBUG
#define GABUFR_DEBUG 0
#endif

#define GABUFR_NO_PARSE_DATA 0
#define GABUFR_PARSE_DATA 1

typedef struct {
  gabufr_msg * msg; /* msg being parsed */
  gabufr_varid * descpos; /* current descriptor */
  gabufr_varid * parentpos; /* descriptor in next level out (needed
			       for NCEP use of a sequence to indicate
			       replication of next descriptor */
  unsigned char * datapos; /* position in raw data */
  gaint databits;   /* bit offset in raw data */
  gaint z;  /* current replication count in outermost level */
  gaint sub; /* current subset */
  gaint delrep; /* flag set if previous descriptor was delayed
		 replication (F=2,Y=0) */
  gabufr_val * curval; /* end of list of parsed data values */
} gabufr_parseinf;

void gabufr_close(gabufr_dset * dset) {
	gabufr_msg * msg, * nextmsg;

	for (msg = dset->msgs; msg != NULL; msg = nextmsg) {
		nextmsg = msg->next;
		gabufr_free_msg(msg);
	}

	if (dset->buf) {
		printf("should have been done already");
		free(dset->buf);
	}

	free(dset);
}

void gabufr_free_msg(gabufr_msg * msg) {
	gabufr_val * val, * nextval;
	gaint i;
	if (msg->subs) {
		for (i = 0; i < msg->subcnt; i++) {
			if (msg->subs[i]) {
				for (val = msg->subs[i]; val != NULL; val = nextval) {
					nextval = val->next;
					gabufr_free_val(val);
				}
			}
		}
		free(msg->subs);
	}
	free(msg);
}  

void gabufr_free_val(gabufr_val * val) {
	if (val->sval) {
		free(val->sval);
	}
	free(val);
}