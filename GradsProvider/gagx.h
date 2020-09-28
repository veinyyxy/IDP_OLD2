/*  Copyright (C) 1988-2010 by Brian Doty and the 
    Institute of Global Environment and Society (IGES).  
    See file COPYRIGHT for more information.   */

/* Authored by B. Doty */
#ifndef GAGX_H
#define GAGX_H

#include <malloc.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "gatypes.h"

gaint gacntr (struct gacmn *pcm, gaint filflg, gaint shpflg);
void gas2d (struct gacmn *pcm, struct gagrid *pgr, gaint imap);
struct gagrid *gaflip (struct gagrid *pgr, struct gacmn *pcm);
void gacsel (gadouble rmin, gadouble rmax, gadouble *cint, gadouble *cmin, gadouble *cmax);
void gaplot (struct gacmn *pcm);
void gagexp (gadouble *g1, gaint cols, gaint rows, 
	gadouble *g2, gaint exp1, gaint exp2, char *g1u, char *g2u);
void gaglin (gadouble *g1, gaint cols, gaint rows, 
	gadouble *g2, gaint exp1, gaint exp2, char *g1u, char *g2u);
void gaselc (struct gacmn *pcm, gadouble rmin, gadouble rmax);
void gaclx (gadouble xin, gadouble yin, gadouble *xout, gadouble *yout);
void gacly (gadouble xin, gadouble yin, gadouble *xout, gadouble *yout);
void gaaclx (gadouble xin, gadouble yin, gadouble *xout, gadouble *yout);
void gaacly (gadouble xin, gadouble yin, gadouble *xout, gadouble *yout);
void galnx (gadouble xin, gadouble yin, gadouble *xout, gadouble *yout);
void galny (gadouble xin, gadouble yin, gadouble *xout, gadouble *yout);
void gaalnx (gadouble xin, gadouble yin, gadouble *xout, gadouble *yout);
void gaalny (gadouble xin, gadouble yin, gadouble *xout, gadouble *yout);
void gaconv (gadouble s, gadouble t, gadouble *x, gadouble *y);
void gaaxpl (struct gacmn *pcm, gaint idim, gaint jdim);
void gafram(struct gacmn *pcm);
void gagsav (gaint type, struct gacmn *pcm, struct gagrid *pgr);
void gastts (struct gacmn *pcm);
void gadprnt (struct gacmn *pcm);
void gaoutgds (struct gacmn *pcm);
void gafwrt (struct gacmn *pcm);
void gaaxis (gaint axis, struct gacmn *pcm, gaint dim);
void gafgrd (struct gacmn *pcm);
void gaplvl (struct gacmn *pcm);
void gagtif (struct gacmn *pcm, gaint kmlflg);
void gakml (struct gacmn *pcm);
void gashpwrt (struct gacmn *pcm);
void gafstn (struct gacmn *pcm);
void gagrph (struct gacmn *pcm, gaint barflg);
void gabarb (gadouble x, gadouble y, gadouble plen, gadouble blen, gadouble rad,
	gadouble dir, gadouble spd, gaint hemflg);
void gaarrw (gadouble x, gadouble y, gadouble ang, gadouble siz, gadouble asiz);
void gagfil (gadouble *r, gaint is, gaint js, gadouble *vs, gaint *clrs, gaint lvs, char *ru);
void gaimap (gadouble *r, gaint is, gaint js, gadouble *vs, gaint *clrs, gaint lvs, 
	char *ru, gadouble xlo, gadouble xhi, gadouble ylo, gadouble yhi);
void galfil (struct gacmn *pcm);
gaint galnch (gadouble lon, char *ch);
gaint galtch (gadouble lat, char *ch);
void gampax (struct gacmn *pcm);
void gamscl (struct gacmn *pcm);
void gapmdl (struct gacmn *pcm);
void gasmrk (struct gacmn *pcm);
void gastnwrt (struct gacmn *pcm);
void gawsym (struct gacmn *pcm);
void gascat (struct gacmn *pcm);
void gapstn (struct gacmn *pcm);
void gavect (struct gacmn *pcm, gaint brbflg);
void gas1d (struct gacmn *pcm, gadouble cmin, gadouble cmax, gaint dim, gaint rotflg, 
struct gagrid *pgr, struct gastn *stn);
void gapprf (struct gacmn *pcm);
void gatser (struct gacmn *pcm);
gaint gashdc (struct gacmn *pcm, gadouble val);
void gasmdl (struct gacmn *pcm, struct garpt *rpt, gadouble *vals, char *udefs);
void gastrm (struct gacmn *pcm);
gaint gatinc (struct gacmn *pcm, struct dt *tstrt, struct dt *tincr);
void gatmlb (struct gacmn *pcm);
void gawgdstime(FILE* outfile, gadouble *val);
void gawgdsval(FILE* outfile, gafloat *val);
void gawmap (struct gacmn *pcm, gaint iflg);
void lfint (gadouble v11, gadouble v12, gadouble v21, gadouble v22, gadouble *u, gadouble *v);
void lfout (gadouble *u, gadouble *v1, gadouble *v2, gadouble *xyb, gaint cnt, gaint rotflg);
void sys_time();
gadouble wndexit (gadouble uu, gadouble vv, gadouble x, gadouble y, gadouble rad,
	gadouble *xlo, gadouble *xhi, gadouble *ylo, gadouble *yhi);
void wxsym (gaint type, gadouble xpos, gadouble ypos, gadouble scale, gaint colr, gaint *wxcols);
void get_tm_struct(time_t t);
void wxprim (gaint part, gadouble xpos, gadouble ypos, gadouble scale);
void gagx (struct gacmn *pcm);

#endif