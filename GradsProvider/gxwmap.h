/*  Copyright (C) 1988-2010 by Brian Doty and the 
    Institute of Global Environment and Society (IGES).  
    See file COPYRIGHT for more information.   */

/* Authored by B. Doty */
#ifndef GXWMAP_H
#define GXWMAP_H

#include "config.h"
#include <malloc.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "gatypes.h"
#include "gx.h"
/* #include "gxmap.h" */

gadouble gxaarw (gadouble lon, gadouble lat);
void gxdmap (struct mapopt *mopt);
int gxlamc (struct mapprj *mpj);
int gxltln (struct mapprj *mpj);
int gxmoll (struct mapprj *mpj);
int gxnste (struct mapprj *mpj);
int gxortg (struct mapprj *mpj);
void gxlamcp (gadouble rlon, gadouble rlat, gadouble *x, gadouble *y);
void gxlamcb (gadouble x, gadouble y, gadouble *rlon, gadouble *rlat);
void gxmollb (gadouble x, gadouble y, gadouble *rlon, gadouble *rlat);
void gxmollp (gadouble rlon, gadouble rlat, gadouble *x, gadouble *y);
void gxnpst (gadouble rlon, gadouble rlat, gadouble *x, gadouble *y);
void gxnrev (gadouble x, gadouble y, gadouble *rlon, gadouble *rlat);
void gxortgb (gadouble x, gadouble y, gadouble *rlon, gadouble *rlat);
void gxortgp (gadouble rlon, gadouble rlat, gadouble *x, gadouble *y);
int gxscld (struct mapprj *mpj, int xflip, int yflip);
int gxsste (struct mapprj *mpj);
void gxspst (gadouble rlon, gadouble rlat, gadouble *x, gadouble *y);
void gxsrev (gadouble x, gadouble y, gadouble *rlon, gadouble *rlat);
int gxrobi (struct mapprj *mpj);
void gxrobb (gadouble x, gadouble y, gadouble *rlon, gadouble *rlat);
void gxrobp (gadouble rlon, gadouble rlat, gadouble *x, gadouble *y);
gadouble *gxmpoly(gadouble *xy, gaint cnt, gadouble llinc, gaint *newcnt);

#endif