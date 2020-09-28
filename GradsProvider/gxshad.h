/*  Copyright (C) 1988-2010 by Brian Doty and the 
    Institute of Global Environment and Society (IGES).  
    See file COPYRIGHT for more information.   */

/* Authored by B. Doty */
#ifndef GXSHAD_H
#define GXSHAD_H

#include "config.h"

#include <stdio.h>
#include <math.h>
#include "gatypes.h"
#include "gx.h"

gaint gxsflw (gaint i, gaint j, gaint iside);
gaint putxy (gadouble x, gadouble y);
void shdcmp (void);
gaint shdmax(void);
gaint spathl (gadouble p1, gadouble p2, gadouble p3, gadouble p4);
gaint undcol (gaint i, gaint j);
void gxshad (gadouble *r, gaint is, gaint js, gadouble *vs, gaint *clrs, gaint lvs, char *u);

#endif