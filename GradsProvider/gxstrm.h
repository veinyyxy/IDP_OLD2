#ifndef GXSTRM_H
#define GXSTRM_H

#include "config.h"
#include <malloc.h>

#include <stdio.h>
#include <math.h>
#include "gatypes.h"
#include "gx.h"
#include "gxsubs.h"

/* routines in gxstrm:  gxstrm (do streamlines) */

void gxstrm (gadouble *, gadouble *, gadouble *, gaint, gaint, char *, char *, char *,
	gaint, gadouble *, gaint *, gaint, gaint);
void strmar (gadouble, gadouble, gadouble, gadouble);
gaint gxshdc (gadouble *, gaint *, gaint, gadouble);

#endif