/*  Copyright (C) 1988-2010 by Brian Doty and the 
    Institute of Global Environment and Society (IGES).  
    See file COPYRIGHT for more information.   */

/* qqq resolve gxdraw vs gxsdrw issue for cterp off */
/* qqq clip labels and masking outside of parea */
/* qqq error(mem) handling */

/* Authored by B. Doty */
/* Add labeling with masking 10/2008  B. Doty */
/* 
 * Include ./configure's header file
 */

#ifndef GXCNTRS_H
#define GXCNTRS_H

#include "config.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "gatypes.h"
#include "gx.h"

void gxcflw (gaint i, gaint j, gaint iside, gaint dr);
gaint gxcspl (gaint frombuf, struct gxcntr *pcntr);
int gxqclab (gadouble xpos, gadouble ypos, gadouble csize);
void gxpclab (gadouble xpos, gadouble ypos, gadouble rot, gaint ccol, struct gxcntr *pcntr);
gaint pathln (gadouble p1, gadouble p2, gadouble p3, gadouble p4);
void gxclev (gadouble *r, gaint is, gaint js, gaint ib, gaint ie, gaint jb,
	gaint je, gadouble v, char *u, struct gxcntr *pcntr);
void gxpclin (void);
void gxclab (gadouble csize, gaint flag, gaint colflg);
void gxcrel (void);
gaint gxclvert (FILE *kmlfp) ;

#endif