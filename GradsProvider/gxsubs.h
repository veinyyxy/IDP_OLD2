#ifndef GXSUBS_H
#define GXSUBS_H

#include "config.h"
#include <malloc.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gatypes.h"
#include "gx.h"

void gxwide (gaint wid);
void gxstyl (gaint style);
void gxcolr (gaint clr);
void gxdraw (gadouble x, gadouble y);
void bdterp (gadouble x1, gadouble y1, gadouble x2, gadouble y2,
	gadouble *x, gadouble *y);
void gxvcon (gadouble s, gadouble t, gadouble *x, gadouble *y);
void gxmove (gadouble x, gadouble y);
char *gxgnam(char *ch);
char *gxgsym(char *ch);
void gxclip (gadouble xmin, gadouble xmax, gadouble ymin, gadouble ymax);
gaint gxqclr (void);
void gxcord (gadouble *coords, gaint num, gaint level);
void gxmaskrec (gadouble xlo, gadouble xhi, gadouble ylo, gadouble yhi);
int gxmaskrq (gadouble xlo, gadouble xhi, gadouble ylo, gadouble yhi);
void gxplot (gadouble x, gadouble y, gaint ipen );
gaint gxqbck (void);
gaint gxqstl (void);
gaint gxqwid (void);
void gxmaskclear(void);
void gxfill (gadouble *xy, gaint num);
void gxrecf (gadouble xlo, gadouble xhi, gadouble ylo, gadouble yhi);
void gxproj ( void (*fproj) (gadouble s, gadouble t, gadouble *x, gadouble *y));
void gxgrid ( void (*fproj) (gadouble s, gadouble t, gadouble *x, gadouble *y));
void gxconv (gadouble s, gadouble t, gadouble *x, gadouble *y, gaint level);
void gxgrmp (gadouble s, gadouble t, gadouble *x, gadouble *y);
void gxback ( void (*fproj) (gadouble s, gadouble t, gadouble *x, gadouble *y));
void gxmark (gaint mtype, gadouble x, gadouble y, gadouble siz );
void gxscal (gadouble xmin, gadouble xmax, gadouble ymin, gadouble ymax,
	gadouble smin, gadouble smax, gadouble tmin, gadouble tmax);
void gxqrgb (gaint clr, gaint *r, gaint *g, gaint *b);
void gxrset (gaint level);
void gxxy2w (gadouble x, gadouble y, gadouble *s, gadouble *t);
void gxstrt (gadouble xmx, gadouble ymx, gaint batch, gaint hbufsz);
void gxend (void);
void gxfrme (gaint action);
void gxsfrm (void);
void gxvpag (gadouble xmax, gadouble ymax,
	gadouble smin, gadouble smax, gadouble tmin, gadouble tmax);
gaint gxacol (gaint clr, gaint red, gaint green, gaint blue );
void gxppvp (gadouble, gadouble, gadouble *, gadouble *);
gaint gxacol (gaint clr, gaint red, gaint green, gaint blue );
void gxptrn (gaint typ, gaint den, gaint ang);

#endif