#ifndef GAUTIL_H
#define GAUTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>

#include <ctype.h>
#include "grads.h"
#include "config.h"
#include "grads.h"
#include "gx.h"

gaint nxtcmd (char *cmd, char *prompt);
gaint nxrdln (char *cmd, char *prompt);
void timadd (struct dt *dtim, struct dt *dto);
void timsub (struct dt *dtim, struct dt *dto);
gadouble t2gr (gadouble *vals, struct dt *dtim);
void gr2t (gadouble *vals, gadouble gr, struct dt *dtim);
gaint timdif (struct dt *dtim1, struct dt *dtim2);
gaint qleap (gaint year);
char *adtprs (char *ch, struct dt *def, struct dt *dtim);
char *rdtprs (char *ch, struct dt *dtim);
gaint cmpwrd (char *ch1, char *ch2);
gaint cmpwrdl (char *ch1, char *ch2);
char * nxtwrd (char *ch);
gadouble liconv (gadouble *vals, gadouble v);
gadouble gr2lev (gadouble *vals, gadouble gr);
gadouble lev2gr (gadouble *vals, gadouble lev);
char *e2ens (struct gafile *pfi, gadouble e);
char *intprs (char *ch, gaint *val);
char *longprs (char *ch, long *val);
char * getdbl(char *ch, gadouble *val);
char * getflt(char *ch, gafloat *val);
char *dimprs (char *ch, struct gastat *pst, struct gafile *pfi, gaint *dim, gadouble *d, gaint type, gaint *wflag);
void lowcas (char *ch);
void uppcas (char *ch);
void getstr (char *ch1, char *ch2, gaint len);
void getwrd (char *ch1, char *ch2, gaint len);
gaint wrdlen (char *ch2);
void gamnmx (struct gagrid *pgr);
void gasmnmx (struct gastn *stn);
gaint garemb (char *ch);
gadouble *gamo32 (gaint istrt, gaint num);
gadouble *gagaus (gaint istrt, gaint num);
gadouble *gags30 (gaint istrt, gaint num);
gadouble *gags20 (gaint istrt, gaint num) ;
gadouble *gags15 (gaint istrt, gaint num);
gadouble *gagst62 (gaint istrt, gaint num);
gaint gat2ch (struct dt *dtim, gaint tinc, char *ch, gaint chlen);
gaint cmpch (char *str1, char *str2, gaint len);
void gafree (struct gastat *pst);
void gagfre (struct gagrid *pgr);
void gasfre (struct gastn *stn);
void fnmexp (char *out, char *in1, char *in2);
char *gafndt (char *fn, struct dt *dtim, struct dt *dtimi, gadouble *vals, struct gachsub *pch1st, struct gaens *ens1st, gaint t, gaint e, gaint *flag);
void gabswp (void *r, gaint cnt);
void gabswp8 (void *r, gaint cnt);
void ganbswp(char *buf, gaint cnt);
void gahswp (struct rpthdr *hdr);
gaint dayweek (struct dt *dtime);
gafloat ibm2flt(unsigned char *ibm);
gaint flt2ibm(gafloat x, unsigned char *ibm);
gafloat ieee2flt(unsigned char *ieee);
gadouble ieee2dbl(unsigned char *ieee);
gaint flt2ieee(gafloat x, unsigned char *ieee);
gadouble *cpscal (gadouble *vals, gaint lin, gaint dir, gaint dim);
gaint getvnm (struct gavar *pvar, char *mrec);
gaint getenm (struct gaens *ens, char *mrec);
gaint dequal(gadouble op1, gadouble op2, gadouble tolerance);
gadouble scaled2dbl(gaint scale_factor, gaint scale_value);
gadouble Int_Power(gadouble x, gaint y);
gaint fseeko(FILE *stream, off_t offset, gaint whence);
off_t ftello(FILE *stream);

#endif