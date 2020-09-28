/*  Copyright (C) 1988-2010 by Brian Doty and the 
    Institute of Global Environment and Society (IGES).  
    See file COPYRIGHT for more information.   */

/* Authored by B. Doty */

/* Plot character strings using the Hershey fonts */
#ifndef GXCHPL_H
#define GXCHPL_H

#include "config.h"
#include <malloc.h>

#include <stdio.h>
#include <math.h>
#include "gatypes.h"
#include "gx.h"

extern char *fch[10];     /* Pointers to font data once it is read in */
extern gaint *foff[10];   /* Pointers to character offsets */
extern gaint *flen[10];   /* Pointers to character lengths */

void gxchpl (char *chrs, int len, gadouble x, gadouble y, gadouble height, gadouble width, gadouble angle);
char * gxchgc (int ch, int fn, int *cnt);
int gxchrd (int fn);
gaint gxchln (char *chrs, gaint len, gadouble width, gadouble *w);
void gxchii (void);

#endif