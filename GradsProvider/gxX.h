/*  Copyright (C) 1988-2010 by Brian Doty and the 
    Institute of Global Environment and Society (IGES).  
    See file COPYRIGHT for more information.   */

#ifndef GXX_H
#define GXX_H


#include "config.h"

#include "gatypes.h"
#include "gx.h"
/*#include "QDataArray.h"*/
#include "galloc.h"

#include "bitmaps.h"
#include "QVector.h"
#include "qdebug.h"
//#include "QBitmapText.h"


struct XPoint{
	gafloat x, y;
};

struct XFILLPOINT{
	float ColorData[3];
	XPoint * pPoint;
	double fShadeValue;
	int nColor;
	int n;
	bool bSpecialZ;
};

struct lineData
{
	float ColorData[3];
	QVector<float> arrayData;
	int n;
	double nContourValue;
};

struct LineTextValue
{
	char nTextValue[20];
	gadouble x;
	gadouble y;
};

struct WindBarbData
{
	gadouble dir;
	gadouble spd;
	gadouble x;
	gadouble y;
};

void gxdwid (gaint wid);
void gxdcol (gaint clr);
void gxdmov (gadouble x, gadouble y);
void gxddrw (gadouble x, gadouble y);
void gxdfil (gadouble *xy, gaint n);
void gxdrec (gadouble x1, gadouble x2, gadouble y1, gadouble y2);
void gxdgcoord (gadouble x, gadouble y, gaint *i, gaint *j);
void gxdimg(gaint *im, gaint imin, gaint jmin, gaint isiz, gaint jsiz);
void gxdbgn (gadouble xsz, gadouble ysz);
void gxdbat (void);
void gxdend (void);
void gxdfrm (gaint iact);
void gxddbl (void);
void gxdsgl (void);
void gxdswp (void);
gaint gxdacl (gaint clr, gaint red, gaint green, gaint blue);
void gxdbck (gaint flg);
void gxgrey (gaint flag);
void gxdeve (gaint flag);
void gxdsfn(void);
void gxdrdw (void);
void gxdrawtextvalue(char* nText,gadouble x,gadouble y);
void gxdrawwindbarb(gadouble x,gadouble y,gadouble dir, gadouble spd);
void gxrdrw (int flag);
void gxdptn (int typ, int den, int ang);
void gxdpbn (gaint bnum, struct gbtn *pbn, gaint redraw, gaint btnrel, gaint nstat);
void gxdrbb (gaint num, gaint type,
	gadouble xlo, gadouble ylo, gadouble xhi, gadouble yhi, gaint mbc);
void gxdrmu (gaint mnum, struct gdmu *pmu, gaint redraw, gaint nstat);
void gxdbtn (gaint, gadouble *, gadouble *, gaint *, gaint *, gaint *, gadouble *);
void gxrswd(int flag);

#endif