/*  Copyright (C) 1988-2010 by Brian Doty and the 
    Institute of Global Environment and Society (IGES).  
    See file COPYRIGHT for more information.   */

/* Authored by B. Doty and Jennifer Adams */

#ifndef GAIO_H
#define GAIO_H

#include "config.h"
#include <malloc.h>

#include "grads.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if     defined(WIN32)
#ifndef FOUND_C99_ISXX
#undef  isnan
#undef  isinf
#endif
#endif

#if     !defined(isnan) && !defined(HAVE_C99_ISNAN)
#define isnan(val)      (0)
#endif
#if     !defined(isinf) && !defined(HAVE_C99_ISINF)
#define isinf(val)      (0)
#endif


void gaiomg ();
gaint gaopnc (struct gafile *pfil, gaint tflag, gaint eflag);
gaint gaggrd (struct gagrid *pgrid);
gaint gagdef (void);
gaint gagpre (void);
void clicyc (gaint *ti);
gaint gagrow (gadouble *gr, char *gru, gaint *d);
gaint gancsetup (void);
gaint gancgrid (gadouble *gr, char *gru, gaint id, gaint jd);
gaint garrow (gaint x, gaint y, gaint z, gaint t, gaint e, 
	gaint len, gadouble *gr, char *gru, gaint toff);
off_t gafcorlf (gaint x, gaint y, gaint z, gaint t, gaint e);
gaint garead (off_t fpos, gaint len, gadouble *gr, char *gru);
gaint ccsread (off_t fpos, gaint len, gadouble *gr, char * gru);                         // added by Liu Xinsheng 2011-05-06
gaint gaprow (gaint x, gaint y, gaint z, gaint t, gaint e, gaint tt, 
	gaint len, gadouble *gr, char *gru);
gaint gancrow (gaint x, gaint y, gaint z, gaint t, gaint e, gaint len, gadouble *gr, char *gru);
gaint gahrow (gaint x, gaint y, gaint z, gaint t, gaint e, gaint len, gadouble *gr, char *gru);
gaint gah5row (gaint x, gaint y, gaint z, gaint t, gaint e, gaint len, gadouble *gr, char *gru);
gaint gairow (gaint x, gaint y, gaint z, gaint t, gaint e, gaint offset, gaint len, 
	gadouble *gr, char *gru);
gaint gaird (off_t fpos, gaint cstrt, gaint clen, gaint xsiz, gaint ysiz, gaint blen);
gaint gaopfn(gaint t, gaint e, gaint *ee, gaint *oflg, struct gafile *pfi);
gaint gaclosenc (struct gafile *pfi);
gaint gaclosehdf (struct gafile *pfi);
gaint gacloseh5 (struct gafile *pfi);
gaint gaoph5 (struct gafile *pfil, gaint tflag, gaint eflag);
gaint gaophdf (struct gafile *pfil, gaint tflag, gaint eflag);
gaint gagstn (struct gastn *stn);
void gacstn (char *hdr, char *rdat, gaint siz, gaint sizhdr);
void gagcst (gaint siz, char *ch);
gaint gaglvs (gaint tim, struct rpthdr *hdr, struct gastn *stn);
gaint garstn (gaint siz, char *val, off_t fpos);
struct garpt *gaarpt (struct gastn *stn);
gaint gasstn (off_t fpos);
gaint ncpattrs(gaint, char *, char *, gaint, gaint, char *);
void prntwrap(char *, char *, char *);
// struct g2buff * g2read (off_t seek, g2int ifld, gaint z, gaint t, gaint e);
// struct g2buff * g2check (gaint z, gaint t, gaint e);
// void g2clear (void);

#endif