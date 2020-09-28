//////////////////////////////////////////////////////////////////////////
//		gaddes.h
//////////////////////////////////////////////////////////////////////////

#ifndef GADDES_H
#define GADDES_H

#include <malloc.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "grads.h"
#include "config.h"

struct gafile *getpfi (void);
gaint gaddes (char *name, struct gafile *pfi, gaint mflag);
gaint ccsddes (char **pstrCTL, struct gafile *pfi, gaint mflag) ;   // added by Liu Xinsheng 2011-05-05
gaint ccsdeflev (char *ch, char *rec, struct gafile *pfi, gaint dim, char **pstrCTL, int *nRow);  // added by Liu Xinsheng 2011-05-05

gaint ddfattr (char *ch, struct gafile *pfi);
gaint deflin (char *ch, struct gafile *pfi, gaint dim, gaint flag);
gaint deflev (char *ch, char *rec, struct gafile *pfi, gaint dim);

struct gaattr *parseattr (char *ch);
int strncasecmp(char *s1, char *s2, register int n);
void ll2lc (gadouble *vals, gadouble grdlat, gadouble grdlon, gadouble *grdi, gadouble *grdj, gadouble *wrot);
void ll2rotll( gadouble *vals, gadouble grdlat, gadouble grdlon, gadouble *grdi, gadouble *grdj,  gadouble *wrot );
void ll2eg (gaint im, gaint jm, gadouble *vals,  gadouble grdlon, gadouble grdlat, gadouble *grdi, gadouble *grdj, gadouble *alpha);
void ll2pse (gaint im, gaint jm, gadouble *vals, gadouble lon, gadouble lat, gadouble *grdi, gadouble *grdj);
void ll2ops(gadouble *vals, gadouble lni, gadouble lti, gadouble *grdi, gadouble *grdj);
void w3fb04 (gadouble alat, gadouble along, gadouble xmeshl, gadouble orient, gadouble *xi, gadouble *xj);
void frepfi (struct gafile *pfi, gaint flag);

#endif