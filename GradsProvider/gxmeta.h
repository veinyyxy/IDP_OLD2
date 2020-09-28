/*  Copyright (C) 1988-2010 by Brian Doty and the 
    Institute of Global Environment and Society (IGES).  
    See file COPYRIGHT for more information.   */

/* Routines related to hardcopy (metafile) output. */

#ifndef GXMETA_H
#define GXMETA_H
 
#include "config.h"
#include <malloc.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gatypes.h"
#include "gx.h"

extern short *hbuff, *hpnt, *hend;          /* Current buffer ptrs*/
extern short *bufs[250];                    /* Buffer stuff       */
extern short *bufs2[250];                   /* Enough for 50MB    */

void hout2i (gaint cmd, gaint i1, gaint i2);
void hfull (void);
gaint gxhwri (void *buf, gaint len);
void hout1 (gaint cmd, gaint opt);
void hout2 (gaint cmd, gadouble x, gadouble y);
void hout0 (gaint cmd);
void hout4 (gaint cmd, gadouble xl, gadouble xh, gadouble yl, gadouble yh);
void gxhopt (gaint flag);
void gxhnew (gadouble xsiz, gadouble ysiz, gaint hbufsz);
void gxhend (void);
void gxhfrm (gaint iact);
void hout4i (gaint cmd, gaint i1, gaint i2, gaint i3, gaint i4);
void gxhdrw (gaint dbflg);
void hout3i (gaint cmd, gaint i1, gaint i2, gaint i3);

#endif