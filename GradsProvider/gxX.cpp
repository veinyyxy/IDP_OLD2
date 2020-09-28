#include "gxX.h"

#include "gxmeta.h"
#include "gxsubs.h"

#include "qpair.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/Xos.h>
//#include <X11/Xatom.h>
//#include <X11/keysym.h>

//#include "bitmaps.h"

QVector<lineData*> ContourData;
QVector<XFILLPOINT*> ShadeData;
QVector<XFILLPOINT*> ShadeRecData;
QVector<LineTextValue*> ContourTextValueArray; //等值线贴值数组zhaogp
QVector<WindBarbData*> WindBarbDataArray; //风标属性数组zhaogp
extern QVector<double>  vector_ContorValue;   // 等值线属性值

static gaint ncolor;

/* Following function prototype has to go here since it 
depends on X include information, which shouln't go in gx.h */

/*  gxbcol:  Assign best rgb to color number from standard colormap */
//gaint gxbcol (XStandardColormap*, XColor *);
//void set_display_screen (Display *, gaint);
//void Window_Dump(Window,FILE *);
//void Pixmap_Dump(Window, FILE *, gaint, gaint, gaint, gaint);

/* Interface for X11 Release 3  */

/* Device interface level.  Following routines need to be
interfaced to the hardware:

gxdbgn - Initialize graphics output.  Set up any hardware
scaling needed, clear the graphics display area,
etc.
gxdcol - Set hardware color.  The colors should be set up
as follows:
0 - black;   1 - white    2 - red     3 - green
4 - blue     5 - cyan     6 - magenta 7 - yellow
8 - orange   9 - purple  10 - yell-grn  11 - lt.blue
12 - ora.yell 3 - blu-grn 14 - blu-purp  15 - grey
If colors are not available then grey scales can
be used, or the call can be a no op.
gxdwid - Set hardware line weight.
gxdfrm - New frame.  If in single buffer mode, clear the active
display.  If in double buffer mode, clear the background
buffer.
gxdsgl - Initiate single buffer mode.  Normal mode of operation.
gxddbl - Initiate double buffer mode, if available.  Both the
foreground and background displays should be cleared.
gxdswp - Swap buffers when in double buffer mode.  Should take
no action if in single buffer mode.
gxdrec - Draw a color filled rectangle.
gxddrw - Draw a line using current attributes
gxdmov - Move to a new point
gxdend - Terminate graphics output.
*/

static gaint batch=0;                       /* Batch mode? */
static gaint wchose=0;                      /* Controls technique for wide lines 1= s/w 0 X server*/
static gaint lcolor,lwidth,owidth;          /* Current attributes */
static gaint grflg;                         /* Greyscale flag */
static gaint devbck;                        /* Device background */
static gadouble xscl,yscl;                  /* Window Scaling */
static gadouble xxx,yyy;                       /* Old position */
static gadouble xx,yy;				/*temp*/
static gadouble xsize, ysize;               /* User specified size */
static unsigned long cvals[276];            /* Color pixel values */
static gaint cused[276];                    /* Color is assigned */
static gaint cmach[276];                    /* Color is matched */
static gaint dblmode;                       /* single or double buffering */
static gaint width,height,depth;            /* Window dimensions */

static gaint reds[16] =   {  0,255,250,  0, 30,  0,240,230,240,160,160,  0,230,  0,130,170};
static gaint greens[16] = {  0,255, 60,220, 60,200,  0,220,130,  0,230,160,175,210,  0,170};
static gaint blues[16]  = {  0,255, 60,  0,255,200,130, 50, 40,200, 50,255, 45,140,220,170};

static gaint greys[16] = {0,255,215,140,80,110,230,170,200,50,155,95,185,125,65,177};
static gaint grrev[16] = {0,1,14,3,8,7,9,5,4,6,13,12,11,10,2,15};

/* Various arrays are kept for structures that describe displayed
widgets.  This information is kept in static arrays for
efficiency reasons -- so the arrays will tend to be in memory
together and will be much faster to scan when paging is
going on.  A linked list set up would be much easier to code
and much cleaner, but at least for now I will stick with
the pre-defined arrays.  */

static struct gobj obj[512];    /* Displayed objects */
static struct gobj obj2[512];   /* Background objects */
static gaint obnum,obnum2;        /* Current number of objects */
static struct gbtn btn[256];    /* Current buttons */
static struct gbtn btn2[256];   /* Background buttons */
static struct grbb rbb[32];     /* Current rubber-band regions */
static struct grbb rbb2[32];    /* Background rbb regions */
static struct gdmu dmu[200];    /* Current dropmenus */
static struct gdmu dmu2[200];   /* Background dropmenus */
/* static struct gdlg dlg[1];      /\* Current dialog *\/ */

static struct gevent *evbase;   /* Anchor for GrADS event queue */

/* All stuff passed to Xlib routines as args are put here in
static areas since we are not invoking Xlib routines from main*/

//static Screen *sptr;
//Display *display=(Display *)NULL;
//static gaint snum;
//static GC gc;
//static XGCValues values;
//static XEvent report;
//Window win=(Window) NULL;    /* used via extern in gagui */
//static Pixmap pmap;
//static Pixmap pmaps[200];
//static XImage *image;
//static Drawable drwbl;
//static char *window_name = "GrADS " GRADS_VERSION "";
///*char *window_name = "GrADS";*/
static char *display_name = NULL; 
static char *icon_name = "GrADS";
//static Pixmap icon_pixmap;
//static XSizeHints size_hints;
///* static XIconSize *size_list; */
static gaint argc;
static char **argv;
static char *args[4];
static char *name = "grads";
static char *dgeom = "500x400+10+10";
static char *ugeom = NULL;
//static Colormap cmap;
//static XColor cell;
static XPoint *point;
//static gaint dblmode;                     /* single or double buffering */
//static XFontStruct *font1, *font2, *font3;
//static XFontStruct *font1i, *font2i, *font3i;
//static XSetWindowAttributes xsetw;
static gaint gfont,cfont;                       /* Font in use by grads */
static gaint pfilld[200];
static gaint rstate = 1;              /* Redraw state -- when zero,
									  acceptance of X Events is
									  blocked. */
static gaint bsflg;                   /* Backing store enabled or not */
static gaint excnt;                   /* Count of exposes to skip */

void gxdwid (gaint wid){                 /* Set width     */
	gauint lw;
	lwidth=wid;
	if (wchose) return;
	lw = 0;
	if (lwidth>5) lw=2;
	if (lwidth>11) lw=3;
	if (lw != owidth) {
		//XSetLineAttributes(display, gc, lw, LineSolid, CapButt, JoinBevel);
	}
	owidth = lw;
}

void gxdcol (gaint clr) {

	//lineData *pLineData = new lineData;
	//XFILLPOINT *pPolygonData = new XFILLPOINT;
	
	//for(int i = 0; i < gcmn.shdcnt; i++)     // added by Liu Xinsheng 2011-06-16
	//{
	//	if(clr == gcmn.shdcls[i])
	//	{
	//		pPolygonData->fShadeValue = gcmn.shdlvs[i];
	//		break;
	//	}
	//}

	ncolor = clr;

	//pPolygonData->n = 0;
	//pPolygonData->nColor = clr;

	//switch(clr)
	//{
	//case 0:
	//pLineData->ColorData[0] = 0;
	//pLineData->ColorData[1] = 0;
	//pLineData->ColorData[2] = 0;
	//pPolygonData->ColorData[0] = 0;
	//pPolygonData->ColorData[1] = 0;
	//pPolygonData->ColorData[2] = 0;
	//break;
	//case 1:
	//pLineData->ColorData[0] = 1;
	//pLineData->ColorData[1] = 1;
	//pLineData->ColorData[2] = 1;
	//pPolygonData->ColorData[0] = 1;
	//pPolygonData->ColorData[1] = 1;
	//pPolygonData->ColorData[2] = 1;
	//break;
	//case 2:
	//pLineData->ColorData[0] = 250.0f/255.0f;
	//pLineData->ColorData[1] = 60.0f/255.0f;
	//pLineData->ColorData[2] = 60.0f/255.0f;
	//pPolygonData->ColorData[0] = 250.0f/255.0f;
	//pPolygonData->ColorData[1] = 60.0f/255.0f;
	//pPolygonData->ColorData[2] = 60.0f/255.0f;
	//break;
	//case 3:
	//pLineData->ColorData[0] = 0.0;
	//pLineData->ColorData[1] = 220.0f/255.0f;
	//pLineData->ColorData[2] = 0.0;
	//pPolygonData->ColorData[0] = 0.0;
	//pPolygonData->ColorData[1] = 220.0f/255.0f;
	//pPolygonData->ColorData[2] = 0.0;
	//break;
	//case 4:
	//pLineData->ColorData[0] = 30.0f/255.0f;
	//pLineData->ColorData[1] = 60.0f/255.0f;
	//pLineData->ColorData[2] = 255.0f/255.0f;

	//pPolygonData->ColorData[0] = 30.0f/255.0f;
	//pPolygonData->ColorData[1] = 60.0f/255.0f;
	//pPolygonData->ColorData[2] = 255.0f/255.0f;
	//break;
	//case 5:
	//pLineData->ColorData[0] = 0.0f/255.0f;
	//pLineData->ColorData[1] = 200.0f/255.0f;
	//pLineData->ColorData[2] = 200.0f/255.0f;

	//pPolygonData->ColorData[0] = 0.0f/255.0f;
	//pPolygonData->ColorData[1] = 200.0f/255.0f;
	//pPolygonData->ColorData[2] = 200.0f/255.0f;
	//break;
	//case 6:
	//pLineData->ColorData[0] = 240.0f/255.0f;
	//pLineData->ColorData[1] = 0.0f/255.0f;
	//pLineData->ColorData[2] = 130.0f/255.0f;

	//pPolygonData->ColorData[0] = 240.0f/255.0f;
	//pPolygonData->ColorData[1] = 0.0f/255.0f;
	//pPolygonData->ColorData[2] = 130.0f/255.0f;
	//break;
	//case 7:
	//pLineData->ColorData[0] = 230.0f/255.0f;
	//pLineData->ColorData[1] = 220.0f/255.0f;
	//pLineData->ColorData[2] = 50.0f/255.0f;

	//pPolygonData->ColorData[0] = 230.0f/255.0f;
	//pPolygonData->ColorData[1] = 220.0f/255.0f;
	//pPolygonData->ColorData[2] = 50.0f/255.0f;
	//break;
	//case 8:
	//pLineData->ColorData[0] = 240.0f/255.0f;
	//pLineData->ColorData[1] = 130.0f/255.0f;
	//pLineData->ColorData[2] = 40.0f/255.0f;

	//pPolygonData->ColorData[0] = 240.0f/255.0f;
	//pPolygonData->ColorData[1] = 130.0f/255.0f;
	//pPolygonData->ColorData[2] = 40.0f/255.0f;
	//break;
	//case 9:
	//pLineData->ColorData[0] = 160.0f/255.0f;
	//pLineData->ColorData[1] = 0.0f/255.0f;
	//pLineData->ColorData[2] = 200.0f/255.0f;

	//pPolygonData->ColorData[0] = 160.0f/255.0f;
	//pPolygonData->ColorData[1] = 0.0f/255.0f;
	//pPolygonData->ColorData[2] = 200.0f/255.0f;
	//break;
	//case 10:
	//pLineData->ColorData[0] = 160.0f/255.0f;
	//pLineData->ColorData[1] = 230.0f/255.0f;
	//pLineData->ColorData[2] = 50.0f/255.0f;

	//pPolygonData->ColorData[0] = 160.0f/255.0f;
	//pPolygonData->ColorData[1] = 230.0f/255.0f;
	//pPolygonData->ColorData[2] = 50.0f/255.0f;
	//break;
	//case 11:
	//pLineData->ColorData[0] = 0.0f/255.0f;
	//pLineData->ColorData[1] = 160.0f/255.0f;
	//pLineData->ColorData[2] = 255.0f/255.0f;

	//pPolygonData->ColorData[0] = 0.0f/255.0f;
	//pPolygonData->ColorData[1] = 160.0f/255.0f;
	//pPolygonData->ColorData[2] = 255.0f/255.0f;
	//break;

	//case 12:
	//pLineData->ColorData[0] = 230.0f/255.0f;
	//pLineData->ColorData[1] = 175.0f/255.0f;
	//pLineData->ColorData[2] = 45.0f/255.0f;

	//pPolygonData->ColorData[0] = 230.0f/255.0f;
	//pPolygonData->ColorData[1] = 175.0f/255.0f;
	//pPolygonData->ColorData[2] = 45.0f/255.0f;
	//break;
	//case 13:
	//pLineData->ColorData[0] = 0.0f/255.0f;
	//pLineData->ColorData[1] = 210.0f/255.0f;
	//pLineData->ColorData[2] = 140.0f/255.0f;

	//pPolygonData->ColorData[0] = 0.0f/255.0f;
	//pPolygonData->ColorData[1] = 210.0f/255.0f;
	//pPolygonData->ColorData[2] = 140.0f/255.0f;
	//break;
	//case 14:
	//pLineData->ColorData[0] = 130.0f/255.0f;
	//pLineData->ColorData[1] = 0.0f/255.0f;
	//pLineData->ColorData[2] = 220.0f/255.0f;

	//pPolygonData->ColorData[0] = 130.0f/255.0f;
	//pPolygonData->ColorData[1] = 0.0f/255.0f;
	//pPolygonData->ColorData[2] = 220.0f/255.0f;
	//break;
	//case 15:
	//pLineData->ColorData[0] = 170.0f/255.0f;
	//pLineData->ColorData[1] = 170.0f/255.0f;
	//pLineData->ColorData[2] = 170.0f/255.0f;

	//pPolygonData->ColorData[0] = 170.0f/255.0f;
	//pPolygonData->ColorData[1] = 170.0f/255.0f;
	//pPolygonData->ColorData[2] = 170.0f/255.0f;
	//break;
	//}
	//ContourData.push_back(pLineData);
	//ShadeData.push_back(pPolygonData);

	if (clr<0) clr=0;
	if (clr>255) clr=255;
	if (devbck) {
		if (clr==0) clr = 1;
		else if (clr==1) clr = 0;
	}
	if (clr<16 && grflg) {
		if (devbck) clr = grrev[clr];
		clr+=256; 
	}
	if (!cused[clr] && !cmach[clr]) clr=15; 
	//XSetForeground(display, gc, cvals[clr]);
	lcolor=clr;
}

void gxdmov (gadouble x, gadouble y){        /* Move to x,y   */
	//xxx = (gaint)(x*xscl+0.5);
	//yyy = height - (gaint)(y*yscl+0.5);
	//xxx = x;
	//yyy = y;

	//gxxy2w(x,y,&xx,&yy);

	xx = x;
	yy = y;

}

void gxddrw (gadouble x, gadouble y){        /* Draw to x,y   */
	//gaint i, j;
	//gaint w,h;
	//i = (gaint)(x*xscl+0.5);
	//j = height - (gaint)(y*yscl+0.5);
	//i = x;
	//j = y;

	if (xx == x && yy == y) return;

	//gxxy2w(x,y,&x,&y);
	//XDrawLine (display, drwbl, gc, xxx, yyy, i, j);
	//LineTextValue *pTextValue = ContourTextValueArray.at(ContourData.size()-1);

	if (xxx != xx || yyy != yy)
	{
		lineData *pNewLineData = new lineData;

		pNewLineData->n = 2;

		switch(ncolor)
		{
		case 0:
			pNewLineData->ColorData[0] = 0;
			pNewLineData->ColorData[1] = 0;
			pNewLineData->ColorData[2] = 0;
			break;
		case 1:
			pNewLineData->ColorData[0] = 1;
			pNewLineData->ColorData[1] = 1;
			pNewLineData->ColorData[2] = 1;
			break;
		case 2:
			pNewLineData->ColorData[0] = 250.0f/255.0f;
			pNewLineData->ColorData[1] = 60.0f/255.0f;
			pNewLineData->ColorData[2] = 60.0f/255.0f;
			break;
		case 3:
			pNewLineData->ColorData[0] = 0.0;
			pNewLineData->ColorData[1] = 220.0f/255.0f;
			pNewLineData->ColorData[2] = 0.0;
			break;
		case 4:
			pNewLineData->ColorData[0] = 30.0f/255.0f;
			pNewLineData->ColorData[1] = 60.0f/255.0f;
			pNewLineData->ColorData[2] = 255.0f/255.0f;
			break;
		case 5:
			pNewLineData->ColorData[0] = 0.0f/255.0f;
			pNewLineData->ColorData[1] = 200.0f/255.0f;
			pNewLineData->ColorData[2] = 200.0f/255.0f;
			break;
		case 6:
			pNewLineData->ColorData[0] = 240.0f/255.0f;
			pNewLineData->ColorData[1] = 0.0f/255.0f;
			pNewLineData->ColorData[2] = 130.0f/255.0f;
			break;
		case 7:
			pNewLineData->ColorData[0] = 230.0f/255.0f;
			pNewLineData->ColorData[1] = 220.0f/255.0f;
			pNewLineData->ColorData[2] = 50.0f/255.0f;
			break;
		case 8:
			pNewLineData->ColorData[0] = 240.0f/255.0f;
			pNewLineData->ColorData[1] = 130.0f/255.0f;
			pNewLineData->ColorData[2] = 40.0f/255.0f;
			break;
		case 9:
			pNewLineData->ColorData[0] = 160.0f/255.0f;
			pNewLineData->ColorData[1] = 0.0f/255.0f;
			pNewLineData->ColorData[2] = 200.0f/255.0f;
			break;
		case 10:
			pNewLineData->ColorData[0] = 160.0f/255.0f;
			pNewLineData->ColorData[1] = 230.0f/255.0f;
			pNewLineData->ColorData[2] = 50.0f/255.0f;
			break;
		case 11:
			pNewLineData->ColorData[0] = 0.0f/255.0f;
			pNewLineData->ColorData[1] = 160.0f/255.0f;
			pNewLineData->ColorData[2] = 255.0f/255.0f;
			break;
		case 12:
			pNewLineData->ColorData[0] = 230.0f/255.0f;
			pNewLineData->ColorData[1] = 175.0f/255.0f;
			pNewLineData->ColorData[2] = 45.0f/255.0f;
			break;
		case 13:
			pNewLineData->ColorData[0] = 0.0f/255.0f;
			pNewLineData->ColorData[1] = 210.0f/255.0f;
			pNewLineData->ColorData[2] = 140.0f/255.0f;
			break;
		case 14:
			pNewLineData->ColorData[0] = 130.0f/255.0f;
			pNewLineData->ColorData[1] = 0.0f/255.0f;
			pNewLineData->ColorData[2] = 220.0f/255.0f;
			break;
		case 15:
			pNewLineData->ColorData[0] = 170.0f/255.0f;
			pNewLineData->ColorData[1] = 170.0f/255.0f;
			pNewLineData->ColorData[2] = 170.0f/255.0f;
			break;
		}

		pNewLineData->arrayData.push_back(xx);
		pNewLineData->arrayData.push_back(yy);
		pNewLineData->arrayData.push_back(0.0f);

		pNewLineData->arrayData.push_back(x);
		pNewLineData->arrayData.push_back(y);
		pNewLineData->arrayData.push_back(0.0f);

		if(!vector_ContorValue.empty())
		{
			pNewLineData->nContourValue = vector_ContorValue[vector_ContorValue.size()-1];
			if (vector_ContorValue.at(0) == 99999999.0f)	//折线标示
			{
				vector_ContorValue.clear();
			}
		}
		else
			pNewLineData->nContourValue = -999999;

		ContourData.push_back(pNewLineData);
	}
	else
	{
		if (ContourData.count() == 0)
		{
			lineData *pNewLineData = new lineData;

			pNewLineData->n = 1;

			switch(ncolor)
			{
			case 0:
				pNewLineData->ColorData[0] = 0;
				pNewLineData->ColorData[1] = 0;
				pNewLineData->ColorData[2] = 0;
				break;
			case 1:
				pNewLineData->ColorData[0] = 1;
				pNewLineData->ColorData[1] = 1;
				pNewLineData->ColorData[2] = 1;
				break;
			case 2:
				pNewLineData->ColorData[0] = 250.0f/255.0f;
				pNewLineData->ColorData[1] = 60.0f/255.0f;
				pNewLineData->ColorData[2] = 60.0f/255.0f;
				break;
			case 3:
				pNewLineData->ColorData[0] = 0.0;
				pNewLineData->ColorData[1] = 220.0f/255.0f;
				pNewLineData->ColorData[2] = 0.0;
				break;
			case 4:
				pNewLineData->ColorData[0] = 30.0f/255.0f;
				pNewLineData->ColorData[1] = 60.0f/255.0f;
				pNewLineData->ColorData[2] = 255.0f/255.0f;
				break;
			case 5:
				pNewLineData->ColorData[0] = 0.0f/255.0f;
				pNewLineData->ColorData[1] = 200.0f/255.0f;
				pNewLineData->ColorData[2] = 200.0f/255.0f;
				break;
			case 6:
				pNewLineData->ColorData[0] = 240.0f/255.0f;
				pNewLineData->ColorData[1] = 0.0f/255.0f;
				pNewLineData->ColorData[2] = 130.0f/255.0f;
				break;
			case 7:
				pNewLineData->ColorData[0] = 230.0f/255.0f;
				pNewLineData->ColorData[1] = 220.0f/255.0f;
				pNewLineData->ColorData[2] = 50.0f/255.0f;
				break;
			case 8:
				pNewLineData->ColorData[0] = 240.0f/255.0f;
				pNewLineData->ColorData[1] = 130.0f/255.0f;
				pNewLineData->ColorData[2] = 40.0f/255.0f;
				break;
			case 9:
				pNewLineData->ColorData[0] = 160.0f/255.0f;
				pNewLineData->ColorData[1] = 0.0f/255.0f;
				pNewLineData->ColorData[2] = 200.0f/255.0f;
				break;
			case 10:
				pNewLineData->ColorData[0] = 160.0f/255.0f;
				pNewLineData->ColorData[1] = 230.0f/255.0f;
				pNewLineData->ColorData[2] = 50.0f/255.0f;
				break;
			case 11:
				pNewLineData->ColorData[0] = 0.0f/255.0f;
				pNewLineData->ColorData[1] = 160.0f/255.0f;
				pNewLineData->ColorData[2] = 255.0f/255.0f;
				break;
			case 12:
				pNewLineData->ColorData[0] = 230.0f/255.0f;
				pNewLineData->ColorData[1] = 175.0f/255.0f;
				pNewLineData->ColorData[2] = 45.0f/255.0f;
				break;
			case 13:
				pNewLineData->ColorData[0] = 0.0f/255.0f;
				pNewLineData->ColorData[1] = 210.0f/255.0f;
				pNewLineData->ColorData[2] = 140.0f/255.0f;
				break;
			case 14:
				pNewLineData->ColorData[0] = 130.0f/255.0f;
				pNewLineData->ColorData[1] = 0.0f/255.0f;
				pNewLineData->ColorData[2] = 220.0f/255.0f;
				break;
			case 15:
				pNewLineData->ColorData[0] = 170.0f/255.0f;
				pNewLineData->ColorData[1] = 170.0f/255.0f;
				pNewLineData->ColorData[2] = 170.0f/255.0f;
				break;
			}

			pNewLineData->arrayData.push_back(x);
			pNewLineData->arrayData.push_back(y);
			pNewLineData->arrayData.push_back(0.0f);
		}
		else
		{
			lineData *pLastLineData = ContourData.at(ContourData.size()-1);

			pLastLineData->n++;
			pLastLineData->arrayData.push_back(x);
			pLastLineData->arrayData.push_back(y);
			pLastLineData->arrayData.push_back(0.0f);
		}
	}
	
	xxx = x;
	yyy = y;
	xx = x;
	yy = y;

	//if (wchose && lwidth>5) {
	//	w = xxx - i;
	//	if (w<0) w = -1*w;
	//	h = yyy - j;
	//	if (h<0) h = -1*h;
	//	if (w<h) {
	//		//XDrawLine (display, drwbl, gc, xxx-1, yyy, i-1, j);
	//		pLineData->arrayData.push_back(xx-1);
	//		pLineData->arrayData.push_back(yy);
	//		pLineData->arrayData.push_back(0.0f);

	//		pLineData->arrayData.push_back(x-1);
	//		pLineData->arrayData.push_back(y);
	//		pLineData->arrayData.push_back(0.0f);
	//		//if (lwidth>11) XDrawLine (display, drwbl, gc, xxx+1, yyy, i+1, j);
	//		if (lwidth>11)
	//		{
	//			pLineData->arrayData.push_back(xx+1);
	//			pLineData->arrayData.push_back(yy);
	//			pLineData->arrayData.push_back(0.0f);

	//			pLineData->arrayData.push_back(x+1);
	//			pLineData->arrayData.push_back(y);
	//			pLineData->arrayData.push_back(0.0f);
	//		}
	//	} else {
	//		//XDrawLine (display, drwbl, gc, xxx, yyy-1, i, j-1);
	//		pLineData->arrayData.push_back(xx);
	//		pLineData->arrayData.push_back(yy-1);
	//		pLineData->arrayData.push_back(0.0f);

	//		pLineData->arrayData.push_back(x);
	//		pLineData->arrayData.push_back(y-1);
	//		pLineData->arrayData.push_back(0.0f);
	//		//if (lwidth>11) XDrawLine (display, drwbl, gc, xxx, yyy+1, i, j+1);
	//		if (lwidth>11)
	//		{
	//			pLineData->arrayData.push_back(xx);
	//			pLineData->arrayData.push_back(yy+1);
	//			pLineData->arrayData.push_back(0.0f);

	//			pLineData->arrayData.push_back(x);
	//			pLineData->arrayData.push_back(y+1);
	//			pLineData->arrayData.push_back(0.0f);
	//		}
	//	}
	//}
	//if (QLength(display)&&rstate) gxdeve(0);
}

//static gaint gnIndex = 1;

void gxdfil (gadouble *xy, gaint n) {
	gadouble *pt;
	gaint i;
	XPoint *pnt;

	gadouble lastX = 999999;
	gadouble lastY = 999999;
	gadouble llX = 999999;
	gadouble llY = 999999;

	gaint numPoint = 0;

#if 0
	point = (XPoint *)malloc(sizeof(XPoint)*n);
	if (point==NULL) {
		printf ("Error in polygon fill routine gxdfil: \n");
		printf ("Unable to allocate enough memory for the request\n");
		return;
	}
	pnt = point;

	pt = xy;
	for (i=0; i<n; i++) 
	{
		gadouble x;
		gadouble y;

		gxxy2w (*pt, *(pt+1), &x, &y);

		if (i == n-1)  //移除最后一个相同点
		{
			if (abs(xy[0] - xy[n*2-2]) < 0.0001 
				&& abs(xy[1] - xy[n*2-1]) < 0.0001)
				break;
		}

		if (abs(x - lastX) < 0.0001 
			&& abs(y - lastY) < 0.0001 ) {} //移除相邻的相同点
		else
		{
			if (abs(x - llX) < 0.0001 
				&& abs(y - llY) < 0.0001 ) //移除相隔的相同线
			{
				pnt--;

				lastX = llX;
				lastY = llY;

				if (numPoint >= 3)
				{
					llX = point[numPoint-3].x;
					llY = point[numPoint-3].y;
				}
				else
				{
					llX = 999999;
					llY = 999999;
				}
				
				numPoint--;
			}
			else
			{
				pnt->x = x;
				pnt->y = y;

				numPoint++;

				llX = lastX;
				llY = lastY;

				lastX = x;
				lastY = y;

				pnt++;
			}
		}

		pt+=2;
	}

#endif
	
#if 1
	if (abs(xy[0] - xy[n*2-2]) < 0.00000001 
		&& abs(xy[1] - xy[n*2-1]) < 0.00000001)
	{
		point = (XPoint *)malloc(sizeof(XPoint)*n);
		if (point==NULL) {
			printf ("Error in polygon fill routine gxdfil: \n");
			printf ("Unable to allocate enough memory for the request\n");
			return;
		}
		pnt = point;

		pt = xy;
		for (i=0; i<n; i++) 
		{
			//pnt->x = (gaint)(*pt*xscl+0.5);
			//pnt->y = height - (gaint)(*(pt+1)*yscl+0.5);
			gadouble x;
			gadouble y;
			x = *pt;
			y = *(pt+1);
			//gxxy2w(x,y,&x,&y);

			if (abs(x - lastX) < 0.00000001
				&& abs(y - lastY) < 0.00000001 )
			{
				//numPoint--;
			}
			else
			{
				pnt->x = x;
				pnt->y = y;

				numPoint++;

				lastX = x;
				lastY = y;

				pnt++;
			}

			pt+=2;
		}
	}
	else
	{
		point = (XPoint *)malloc(sizeof(XPoint)*(n+1));
		if (point==NULL) {
			printf ("Error in polygon fill routine gxdfil: \n");
			printf ("   Unable to allocate enough memory for the request\n");
			return;
		}
		pnt = point;

		pt = xy;
		for (i=0; i<n; i++) 
		{
			//pnt->x = (gaint)(*pt*xscl+0.5);
			//pnt->y = height - (gaint)(*(pt+1)*yscl+0.5);
			gadouble x;
			gadouble y;
			x = *pt;
			y = *(pt+1);
			//gxxy2w(x,y,&x,&y);

			if (abs(x - lastX) < 0.0001 
				&& abs(y - lastY) < 0.0001 )
			{
				//numPoint--;
			}
			else
			{
				pnt->x = x;
				pnt->y = y;

				numPoint++;

				lastX = x;
				lastY = y;

				pnt++;
			}

			pt+=2;
		}

		pt = xy;
		pnt->x = *pt;
		pnt->y = *(pt+1);

		numPoint++;
		//n++;
	}
#endif
	
	XFILLPOINT *pPolygonData = new XFILLPOINT;
	//gnIndex++;
	//XFILLPOINT *pPolygonData = ShadeData.at(ShadeData.size()-1);

	switch(ncolor)
	{
	case 0:
		pPolygonData->ColorData[0] = 0;
		pPolygonData->ColorData[1] = 0;
		pPolygonData->ColorData[2] = 0;
		break;
	case 1:
		pPolygonData->ColorData[0] = 1;
		pPolygonData->ColorData[1] = 1;
		pPolygonData->ColorData[2] = 1;
		break;
	case 2:
		pPolygonData->ColorData[0] = 250.0f/255.0f;
		pPolygonData->ColorData[1] = 60.0f/255.0f;
		pPolygonData->ColorData[2] = 60.0f/255.0f;
		break;
	case 3:
		pPolygonData->ColorData[0] = 0.0;
		pPolygonData->ColorData[1] = 220.0f/255.0f;
		pPolygonData->ColorData[2] = 0.0;
		break;
	case 4:
		pPolygonData->ColorData[0] = 30.0f/255.0f;
		pPolygonData->ColorData[1] = 60.0f/255.0f;
		pPolygonData->ColorData[2] = 255.0f/255.0f;
		break;
	case 5:
		pPolygonData->ColorData[0] = 0.0f/255.0f;
		pPolygonData->ColorData[1] = 200.0f/255.0f;
		pPolygonData->ColorData[2] = 200.0f/255.0f;
		break;
	case 6:
		pPolygonData->ColorData[0] = 240.0f/255.0f;
		pPolygonData->ColorData[1] = 0.0f/255.0f;
		pPolygonData->ColorData[2] = 130.0f/255.0f;
		break;
	case 7:
		pPolygonData->ColorData[0] = 230.0f/255.0f;
		pPolygonData->ColorData[1] = 220.0f/255.0f;
		pPolygonData->ColorData[2] = 50.0f/255.0f;
		break;
	case 8:
		pPolygonData->ColorData[0] = 240.0f/255.0f;
		pPolygonData->ColorData[1] = 130.0f/255.0f;
		pPolygonData->ColorData[2] = 40.0f/255.0f;
		break;
	case 9:
		pPolygonData->ColorData[0] = 160.0f/255.0f;
		pPolygonData->ColorData[1] = 0.0f/255.0f;
		pPolygonData->ColorData[2] = 200.0f/255.0f;
		break;
	case 10:
		pPolygonData->ColorData[0] = 160.0f/255.0f;
		pPolygonData->ColorData[1] = 230.0f/255.0f;
		pPolygonData->ColorData[2] = 50.0f/255.0f;
		break;
	case 11:
		pPolygonData->ColorData[0] = 0.0f/255.0f;
		pPolygonData->ColorData[1] = 160.0f/255.0f;
		pPolygonData->ColorData[2] = 255.0f/255.0f;
		break;
	case 12:
		pPolygonData->ColorData[0] = 230.0f/255.0f;
		pPolygonData->ColorData[1] = 175.0f/255.0f;
		pPolygonData->ColorData[2] = 45.0f/255.0f;
		break;
	case 13:
		pPolygonData->ColorData[0] = 0.0f/255.0f;
		pPolygonData->ColorData[1] = 210.0f/255.0f;
		pPolygonData->ColorData[2] = 140.0f/255.0f;
		break;
	case 14:
		pPolygonData->ColorData[0] = 130.0f/255.0f;
		pPolygonData->ColorData[1] = 0.0f/255.0f;
		pPolygonData->ColorData[2] = 220.0f/255.0f;
		break;
	case 15:
		pPolygonData->ColorData[0] = 170.0f/255.0f;
		pPolygonData->ColorData[1] = 170.0f/255.0f;
		pPolygonData->ColorData[2] = 170.0f/255.0f;
		break;
	}

	pPolygonData->pPoint = point;
	pPolygonData->n = numPoint;
	pPolygonData->bSpecialZ = gcmn.bSpecialZ;
	//pPolygonData->fShadeValue = gcmn.dCurVal;

	//if (abs(pPolygonData->pPoint[0].x-2.5)< 0.0001 || numPoint == 294)
	//{
	//	qDebug()<<"watch!";
	//}

	for(int i = 0; i < gcmn.shdcnt; i++)     // added by Liu Xinsheng 2011-06-16
	{
		if(ncolor == gcmn.shdcls[i])
		{
			pPolygonData->fShadeValue = gcmn.shdlvs[i];
			break;
		}
	}

	ShadeData.push_back(pPolygonData);

	return;
}

void gxdrec (gadouble x1, gadouble x2, gadouble y1, gadouble y2) {
	//gaint i1,i2,j1,j2;
	gadouble i1,i2,j1,j2;
	gadouble fXScale = 0.1;
	//i1 = (gaint)(x1*xscl+0.5);
	//j1 = height - (gaint)(y1*yscl+0.5);
	//i2 = (gaint)(x2*xscl+0.5);
	//j2 = height - (gaint)(y2*yscl+0.5);

	i1 = x1;
	j1 = y1;
	i2 = x2;
	j2 = y2;

	XPoint *pnt;

	if (ncolor == 0) return;

	XFILLPOINT *pPolygonBarData = new XFILLPOINT;

	switch(ncolor)
	{
	case 0:
		pPolygonBarData->ColorData[0] = 0;
		pPolygonBarData->ColorData[1] = 0;
		pPolygonBarData->ColorData[2] = 0;
		break;
	case 1:
		pPolygonBarData->ColorData[0] = 1;
		pPolygonBarData->ColorData[1] = 1;
		pPolygonBarData->ColorData[2] = 1;
		break;
	case 2:
		pPolygonBarData->ColorData[0] = 250.0f/255.0f;
		pPolygonBarData->ColorData[1] = 60.0f/255.0f;
		pPolygonBarData->ColorData[2] = 60.0f/255.0f;
		break;
	case 3:
		pPolygonBarData->ColorData[0] = 0.0;
		pPolygonBarData->ColorData[1] = 220.0f/255.0f;
		pPolygonBarData->ColorData[2] = 0.0;
		break;
	case 4:
		pPolygonBarData->ColorData[0] = 30.0f/255.0f;
		pPolygonBarData->ColorData[1] = 60.0f/255.0f;
		pPolygonBarData->ColorData[2] = 255.0f/255.0f;
		break;
	case 5:
		pPolygonBarData->ColorData[0] = 0.0f/255.0f;
		pPolygonBarData->ColorData[1] = 200.0f/255.0f;
		pPolygonBarData->ColorData[2] = 200.0f/255.0f;
		break;
	case 6:
		pPolygonBarData->ColorData[0] = 240.0f/255.0f;
		pPolygonBarData->ColorData[1] = 0.0f/255.0f;
		pPolygonBarData->ColorData[2] = 130.0f/255.0f;
		break;
	case 7:
		pPolygonBarData->ColorData[0] = 230.0f/255.0f;
		pPolygonBarData->ColorData[1] = 220.0f/255.0f;
		pPolygonBarData->ColorData[2] = 50.0f/255.0f;
		break;
	case 8:
		pPolygonBarData->ColorData[0] = 240.0f/255.0f;
		pPolygonBarData->ColorData[1] = 130.0f/255.0f;
		pPolygonBarData->ColorData[2] = 40.0f/255.0f;
		break;
	case 9:
		pPolygonBarData->ColorData[0] = 160.0f/255.0f;
		pPolygonBarData->ColorData[1] = 0.0f/255.0f;
		pPolygonBarData->ColorData[2] = 200.0f/255.0f;
		break;
	case 10:
		pPolygonBarData->ColorData[0] = 160.0f/255.0f;
		pPolygonBarData->ColorData[1] = 230.0f/255.0f;
		pPolygonBarData->ColorData[2] = 50.0f/255.0f;
		break;
	case 11:
		pPolygonBarData->ColorData[0] = 0.0f/255.0f;
		pPolygonBarData->ColorData[1] = 160.0f/255.0f;
		pPolygonBarData->ColorData[2] = 255.0f/255.0f;
		break;
	case 12:
		pPolygonBarData->ColorData[0] = 230.0f/255.0f;
		pPolygonBarData->ColorData[1] = 175.0f/255.0f;
		pPolygonBarData->ColorData[2] = 45.0f/255.0f;
		break;
	case 13:
		pPolygonBarData->ColorData[0] = 0.0f/255.0f;
		pPolygonBarData->ColorData[1] = 210.0f/255.0f;
		pPolygonBarData->ColorData[2] = 140.0f/255.0f;
		break;
	case 14:
		pPolygonBarData->ColorData[0] = 130.0f/255.0f;
		pPolygonBarData->ColorData[1] = 0.0f/255.0f;
		pPolygonBarData->ColorData[2] = 220.0f/255.0f;
		break;
	case 15:
		pPolygonBarData->ColorData[0] = 170.0f/255.0f;
		pPolygonBarData->ColorData[1] = 170.0f/255.0f;
		pPolygonBarData->ColorData[2] = 170.0f/255.0f;
		break;
	}

	point = (XPoint *)malloc(sizeof(XPoint)*4);
	if (point==NULL) 
	{
		printf ("Error in polygon fill routine gxdfil: \n");
		printf ("Unable to allocate enough memory for the request\n");
		return;
	}
	pnt = point;

	if (i1!=i2 && j1!=j2) 
	{
		//XFillRectangle (display, drwbl, gc, i1, j2, i2-i1, j1-j2);
		//i2 = i2 - fXScale * (x2 - x1);      // 调整横向坐标
		//i1 = i1 + fXScale * (x2 - x1); 

		pnt->x = i1;
		pnt->y = j2;
		pnt++;
		pnt->x = i2;
		pnt->y = j2;
		pnt++;
		pnt->x = i2;
		pnt->y = j1;
		pnt++;
		pnt->x = i1;
		pnt->y = j1;
		pnt++;
	}
	//else
	//{
		//XDrawLine (display, drwbl, gc, i1, j1, i2, j2);
		//vetor1_contourLine.push_back(i1);
		//vetor1_contourLine.push_back(j1);
		//vetor1_contourLine.push_back(0.0f);
		//vetor1_contourLine.push_back(i2);
		//vetor1_contourLine.push_back(j2);
		//vetor1_contourLine.push_back(0.0f);
	//}
	pPolygonBarData->pPoint = point;
	pPolygonBarData->n = 4;
	ShadeRecData.push_back(pPolygonBarData);
	
	//if (gcmn.isProfile)   // draw color card
	//{
	//	XFILLPOINT *pPolygonCardData = ShadeData.at(ShadeData.size()-1);
	//	
	//	point = (XPoint *)malloc(sizeof(XPoint)*4);
	//	if (point==NULL) 
	//	{
	//		printf ("Error in polygon fill routine gxdfil: \n");
	//		printf ("Unable to allocate enough memory for the request\n");
	//		return;
	//	}
	//	pnt = point;
	//	if (i1!=i2 && j1!=j2) 
	//	{
	//		//XFillRectangle (display, drwbl, gc, i1, j2, i2-i1, j1-j2);
	//		pnt->x = i1;
	//		pnt->y = j2;
	//		pnt++;
	//		pnt->x = i2;
	//		pnt->y = j2;
	//		pnt++;
	//		pnt->x = i2;
	//		pnt->y = j1;
	//		pnt++;
	//		pnt->x = i1;
	//		pnt->y = j1;
	//		pnt++;
	//	} 
	//	else 
	//	{
	//		//XDrawLine (display, drwbl, gc, i1, j1, i2, j2);
	//		//vetor1_contourLine.push_back(i1);
	//		//vetor1_contourLine.push_back(j1);
	//		//vetor1_contourLine.push_back(0.0f);
	//		//vetor1_contourLine.push_back(i2);
	//		//vetor1_contourLine.push_back(j2);
	//		//vetor1_contourLine.push_back(0.0f);
	//	}
	//	pPolygonCardData->pPoint = point;
	//	pPolygonCardData->n = 4;
	//}
}

/* Given x,y page location, return screen pixel location */

void gxdgcoord (gadouble x, gadouble y, gaint *i, gaint *j) {
	if (batch) {
		*i = -999;
		*j = -999;
		return;
	}
	*i = (gaint)(x*xscl+0.5);
	*j = height - (gaint)(y*yscl+0.5);
}

void gxdimg(gaint *im, gaint imin, gaint jmin, gaint isiz, gaint jsiz) {
	int i,j,col;

	if (batch) return;

	//image = XGetImage(display, drwbl, imin, jmin, isiz, jsiz, AllPlanes, XYPixmap);
	//if (image==NULL) {
	//printf ("Unable to allocate image file for gxout imap. \n");
	//return;
	//}

	for (j=0; j<jsiz; j++) {
		for (i=0; i<isiz; i++) {
			col = *(im+j*isiz+i);
			//if (col<255) XPutPixel(image,i,j,cvals[col]);
		}
	}
	//XPutImage(display, drwbl, gc, image, 0, 0, imin, jmin, isiz, jsiz);
	//XDestroyImage(image);
}

void gxdbgn (gadouble xsz, gadouble ysz) {
	/*x11*/
}

/* tell x interface that we are in batch mode */

void gxdbat (void) {
	batch = 1;
}

void gxdend (void) {
	//XFreeGC(display, gc);
	//XCloseDisplay(display);
}

/* Frame action.  Values for action are:
0 -- new frame (clear display), wait before clearing.
1 -- new frame, no wait.
2 -- New frame in double buffer mode.
7 -- new frame, but just clear graphics.  Do not clear
event queue; redraw widgets.
8 -- clear only the event queue.
9 -- flush the X request buffer */

void gxdfrm (gaint iact) {
	struct gevent *geve, *geve2;
	gaint i;
	if (iact==9) {
		//gxdeve(0);
		//XFlush(display);
		return;
	}
	if (iact==0 || iact==1 || iact==7) {
		//XSetForeground(display, gc, cvals[devbck]);
		//XFillRectangle (display, drwbl, gc, 0, 0, width, height);
		//XSetForeground(display, gc, cvals[lcolor]);
		for (i=0; i<512; i++) obj[i].type = -1;
		obnum = 0;
	}

	/* Flush X event queue.  If iact is 7, keep the event info,
	otherwise discard it. */

	if (iact==7) gxdeve(0);
	else {
		//while (XCheckMaskEvent(display, ButtonReleaseMask | ButtonPressMask | ButtonMotionMask | KeyPressMask | ExposureMask | StructureNotifyMask, &report))
		//{
		// if (report.type==ConfigureNotify) {
		//  if(width!=report.xconfigure.width||height!=report.xconfigure.height){
		//	  width = report.xconfigure.width;
		//	  height = report.xconfigure.height;
		//	  xscl = (gadouble)(width)/xsize;
		//	  yscl = (gadouble)(height)/ysize;
		//	  gxdsfn();
		//	  if (iact==8) gxdrdw();
		//  }
		// }
		//}

		/* Flush GrADS event queue */

		geve = evbase;
		while (geve) {
			geve2 = geve->forw;
			free (geve);
			geve = geve2;
		}
		evbase = NULL;
	}

	/* Reset all widgets if appropriate. */

	if (iact<7 && iact!=2) gxrswd(0);

	/* Redraw all widgets if appropriate.*/

	if (iact==7) {
		gxrdrw(0);
		//XFlush(display);
	}
}

void gxddbl (void) {
	//gaint i;
	//pmap = XCreatePixmap (display, win, width, height, depth);
	//XSync(display, 0) ; /* hoop */
	//if (pmap==(Pixmap)NULL) {
	//	printf ("Error allocating pixmap for animation mode\n");
	//	printf ("Animation mode will not be enabled\n");
	//	return;
	//}
	//dblmode = 1;
	//drwbl = pmap;
	//XSetForeground(display, gc, cvals[devbck]);
	//XFillRectangle (display, drwbl, gc, 0, 0, width, height);
	//XSetForeground(display, gc, cvals[lcolor]);
	//gxrswd(1);  /* Reset all widgets */
	//for (i=0; i<512; i++) { obj[i].type=-1; obj2[i].type = -1;}
	//obnum = 0;  obnum2 = 0;
	return;
}

void gxdsgl (void) {
	//gaint i;
	if (dblmode) {
		//gxrswd(1);
		//for (i=0; i<512; i++) { obj[i].type=-1; obj2[i].type = -1;}
		//obnum = 0;  obnum2 = 0;
		//XFreePixmap (display, pmap);
		//drwbl = win;
	}
	dblmode = 0;
	return;
}

void gxdswp (void) {
	//if (dblmode) {
	//	XCopyArea (display, pmap, win, gc, 0, 0, width, height, 0, 0);
	//}
	//XSetForeground(display, gc, cvals[devbck]);
	//XFillRectangle (display, drwbl, gc, 0, 0, width, height);
	//XSetForeground(display, gc, cvals[lcolor]);
	//gxrswd(0);
	//gxcpwd();
	//gxrswd(2);
	return;
}

/* static void qprint(XStandardColormap* bst);  */

gaint gxdacl (gaint clr, gaint red, gaint green, gaint blue) {
	//XStandardColormap best;
	//gaint screen_num = DefaultScreen(display);

	if (clr<16 || clr>255) return 1;
	if (cused[clr]) {
		//XFreeColors(display, cmap, &(cvals[clr]),1,0);
		cused[clr]=0;
	}
	//cell.red = red*256;
	//cell.blue = blue*256;
	//cell.green = green*256;
	cmach[clr] = 0;
	/*if (XAllocColor(display, cmap, &cell)) {
	cvals[clr] = cell.pixel;
	cused[clr] = 1;
	} else if (XGetStandardColormap(display, 
	RootWindow(display,screen_num), &best, XA_RGB_BEST_MAP)) {
	if (gxbcol(&best, &cell)) {
	cvals[clr] = cell.pixel;
	cmach[clr] = 1;
	printf ("Color Match.  Color number = %i\n",clr);
	} else {
	printf ("Color Matching Error.  Color number = %i\n",clr);
	}
	} else {
	printf ("Color Map Allocation Error.  Color number = %i\n",clr);
	}*/
	if (cused[clr] == 0 || cmach[clr] == 1) {
		return 0;
	} 
	return(1);
}

/* set hardware background color */

void gxdbck (gaint flg) {

	devbck = flg;
	if (devbck>1) devbck = 1;
}

void gxgrey (gaint flag) {
	grflg = flag;
}

/* Examine X event queue.  Flag tells us if we should wait for an
event.  Any GrADS events (mouse-button presses) are queued.  */

void gxdeve (gaint flag) {
	//struct gevent *geve, *geve2;
	gaint /*i,j,ii,*/rc,wflg,/*button,*/eflg,/*idm,*/rdrflg;

	if (flag && evbase) flag = 0;   /* Don't wait if an event stacked */
	wflg = flag;
	eflg = 0;
	rdrflg = 0;
	while (1) {
		if (wflg && !rdrflg) {
			//XMaskEvent(display, ButtonReleaseMask | ButtonPressMask |
			//   ButtonMotionMask | KeyPressMask | ExposureMask | StructureNotifyMask, &report);
			rc = 1;
		} else {
			//rc = XCheckMaskEvent(display, ButtonReleaseMask | ButtonPressMask |
			//   ButtonMotionMask | KeyPressMask | ExposureMask | StructureNotifyMask, &report);
		}
		if (!rc && rdrflg) {
			gxdsfn();
			gxdrdw();
			rdrflg = 0;
			continue;
		}
		if (!rc) break;

		//switch  (report.type) {
		//case Expose:
		//  if (excnt>0) excnt--;
		//  else if (!bsflg) rdrflg = 1;
		//  break;
		//case ButtonPress:
		//  geve = (struct gevent *)malloc(sizeof(struct gevent));
		//  if (geve==NULL) {
		//    printf ("Memory allocation error in event queue!!!!!!\n");
		//    eflg = 1;
		//    break;
		//  }
		//  if (evbase==NULL) evbase = geve;
		//  else {
		//    geve2 = evbase;
		//    while (geve2->forw) geve2 = geve2->forw;
		//    geve2->forw = geve;
		//  }
		//  geve->forw = NULL;
		//  i = report.xbutton.x;
		//  j = report.xbutton.y;
		//  button = report.xbutton.button;
		//  if (button==Button1) button=1;
		//  else if (button==Button2) button=2;
		//  else if (button==Button3) button=3;
		//  else if (button==Button4) button=4;
		//  else if (button==Button5) button=5;
		//  geve->mbtn = button;
		//  geve->x = xsize*((gadouble)i)/width;
		//  geve->y = ysize - ysize*((gadouble)j)/height;
		//  geve->type = 0;

		//  /* Scan to see if point-click event was on one of our
		//     widgets.  Handling depends on what is clicked on. */

		//  ii = 0;
		//  while (ii<512 && obj[ii].type>-1) {
		//    if (obj[ii].type!=0 && i>obj[ii].i1 && i<obj[ii].i2 &&
		//                           j>obj[ii].j1 && j<obj[ii].j2) {
		//      if (obj[ii].mb < 0 || obj[ii].mb == button) {
		//        if (obj[ii].type==1) gxevbn(geve,ii);
		//        else if (obj[ii].type==2) gxevrb(geve,ii,i,j);
		//        else if (obj[ii].type==3) {
		//          idm = ii;
		//          while (idm != -999) idm = gxevdm(geve,idm,i,j);
		//        }
		//        ii = 100000;           /* Exit loop */
		//      }
		//    }
		//    ii++;
		//  }
		//  wflg = 0;                  /* Check for more events, but don't
		//                                wait if there aren't any. */
		//  break;
		//case ConfigureNotify:
		//  if(width!=report.xconfigure.width||height!=report.xconfigure.height){
		//    width = report.xconfigure.width;
		//    height = report.xconfigure.height;
		//    xscl = (gadouble)(width)/xsize;
		//    yscl = (gadouble)(height)/ysize;
		//    rdrflg = 1;
		//  }
		//  break;
		//}
		if (eflg) break;
	}
	if (rdrflg) {
		gxdsfn();
		gxdrdw();
	}
}

/* Select font based on screen size */

void gxdsfn(void) {

	if (width<601 || height<421) {
		if (gfont!=1) {
			//if (font1) XSetFont (display, gc, font1->fid);
			gfont = 1;
		}
	} else if (width<1001 || height<651) {
		if (gfont!=2) {
			//if (font2) XSetFont (display, gc, font2->fid);
			gfont = 2;
		}
	} else {
		if (gfont!=3) {
			//if (font3) XSetFont (display, gc, font3->fid);
			gfont = 3;
		}
	}
}

/* Attempt to redraw when user resizes window */

void gxdrdw (void) {
	int i;
	rstate = 0;
	//XSetForeground(display, gc, cvals[devbck]);
	//XFillRectangle (display, drwbl, gc, 0, 0, width, height);
	//XSetForeground(display, gc, cvals[lcolor]);
	for (i=0; i<512; i++) obj[i].type = -1;
	obnum = 0;
	if (dblmode) {
		dblmode = 0;
		//XFreePixmap (display, pmap);
		//pmap = XCreatePixmap (display, win, width, height, depth);
		//if (pmap==(Pixmap)NULL) {
		//	printf ("Error allocating pixmap for resize operation\n");
		//	printf ("Animation mode will be disabled\n");
		//	dblmode = 0;
		//	drwbl = win;
		//	rstate = 1;
		//	return;
		//}
		//drwbl = win;
		//XSetForeground(display, gc, cvals[devbck]);
		//XFillRectangle (display, drwbl, gc, 0, 0, width, height);
		//XFillRectangle (display, pmap, gc, 0, 0, width, height);
		//XSetForeground(display, gc, cvals[lcolor]);
		for (i=0; i<512; i++) obj2[i].type = -1;
		obnum2 = 0;
		gxhdrw(1);
		gxrdrw(1);
		dblmode = 1;
		/*drwbl = pmap;*/
	}
	gxhdrw(0);
	gxrdrw(0);
	rstate = 1;
}



/* Redraw all widgets.  Flag indicates whether to redraw
foreground or background widgets. */

void gxrdrw (int flag) {
	int i;
	if (flag) {
		for (i=0; i<256; i++) {
			if (btn2[i].num>-1) gxdpbn(i, NULL, 1, 0, -1);
		}
		for (i=0; i<32; i++) {
			if (rbb2[i].num>-1) gxdrbb(i, rbb2[i].type,
				rbb2[i].xlo,rbb2[i].ylo,rbb2[i].xhi,rbb2[i].yhi,rbb2[i].mb);
		}
		for (i=0; i<200; i++) {
			if (dmu2[i].num>-1) gxdrmu(i, NULL, 1, -1);
		}
	} else {
		for (i=0; i<256; i++) {
			if (btn[i].num>-1) gxdpbn(i, NULL, 1, 0, -1);
		}
		for (i=0; i<32; i++) {
			if (rbb[i].num>-1) gxdrbb(i, rbb[i].type,
				rbb[i].xlo,rbb[i].ylo,rbb[i].xhi,rbb[i].yhi,rbb[i].mb);
		}
		for (i=0; i<200; i++) {
			if (dmu[i].num>-1) gxdrmu(i, NULL, 1, -1);
		}
	}
}

/* Routine to install stipple pixmaps to provide pattern fills for
recf and polyf.  Choices include solid, dot, line and open.
Check and line density can be controlled as well as line angle. */

void gxdptn (int typ, int den, int ang) {
	//unsigned char *bitmap_bits;
	//int bitmap_width, bitmap_height;
	//Pixmap stipple_pixmap;

	if (typ==0) {
		//bitmap_bits = open_bitmap_bits;
		//bitmap_width = open_bitmap_width;
		//bitmap_height = open_bitmap_height;
	}
	else if (typ==1) {
		//XSetFillStyle (display, gc, FillSolid);
		return;
	}
	else if (typ==2) {
		if (den==6) {
			//bitmap_bits = dot_6_bitmap_bits;
			//bitmap_width = dot_6_bitmap_width;
			//bitmap_height = dot_6_bitmap_height;
		}
		else if (den==5) {
			//bitmap_bits = dot_5_bitmap_bits;
			//bitmap_width = dot_5_bitmap_width;
			//bitmap_height = dot_5_bitmap_height;
		}
		else if (den==4) {
			//bitmap_bits = dot_4_bitmap_bits;
			//bitmap_width = dot_4_bitmap_width;
			//bitmap_height = dot_4_bitmap_height;
		}
		else if (den==3) {
			//bitmap_bits = dot_3_bitmap_bits;
			//bitmap_width = dot_3_bitmap_width;
			//bitmap_height = dot_3_bitmap_height;
		}
		else if (den==2) {
			//bitmap_bits = dot_2_bitmap_bits;
			//bitmap_width = dot_2_bitmap_width;
			//bitmap_height = dot_2_bitmap_height;
		}
		else if (den==1) {
			//bitmap_bits = dot_1_bitmap_bits;
			//bitmap_width = dot_1_bitmap_width;
			//bitmap_height = dot_1_bitmap_height;
		}
		else {
			printf ("Error in density specification: density = %d\n",den);
			return;
		}
	}
	else if (typ==3) {
		if (ang==0) {
			if (den==5) {
				//bitmap_bits = line_0_5_bitmap_bits;
				//bitmap_width = line_0_5_bitmap_width;
				//bitmap_height = line_0_5_bitmap_height;
			}
			else if (den==4) {
				//bitmap_bits = line_0_4_bitmap_bits;
				//bitmap_width = line_0_4_bitmap_width;
				//bitmap_height = line_0_4_bitmap_height;
			}
			else if (den==3) {
				//bitmap_bits = line_0_3_bitmap_bits;
				//bitmap_width = line_0_3_bitmap_width;
				//bitmap_height = line_0_3_bitmap_height;
			}
			else if (den==2) {
				//bitmap_bits = line_0_2_bitmap_bits;
				//bitmap_width = line_0_2_bitmap_width;
				//bitmap_height = line_0_2_bitmap_height;
			}
			else if (den==1) {
				//bitmap_bits = line_0_1_bitmap_bits;
				//bitmap_width = line_0_1_bitmap_width;
				//bitmap_height = line_0_1_bitmap_height;
			}
			else {
				printf ("Error in density specification: density = %d\n",den);
				return;
			}
		}
		else if (ang==30) {
			if (den==5) {
				//bitmap_bits = line_30_5_bitmap_bits;
				//bitmap_width = line_30_5_bitmap_width;
				//bitmap_height = line_30_5_bitmap_height;
			}
			else if (den==4) {
				//bitmap_bits = line_30_4_bitmap_bits;
				//bitmap_width = line_30_4_bitmap_width;
				//bitmap_height = line_30_4_bitmap_height;
			}
			else if (den==3) {
				//bitmap_bits = line_30_3_bitmap_bits;
				//bitmap_width = line_30_3_bitmap_width;
				//bitmap_height = line_30_3_bitmap_height;
			}
			else if (den==2) {
				//bitmap_bits = line_30_2_bitmap_bits;
				//bitmap_width = line_30_2_bitmap_width;
				//bitmap_height = line_30_2_bitmap_height;
			}
			else if (den==1) {
				//bitmap_bits = line_30_1_bitmap_bits;
				//bitmap_width = line_30_1_bitmap_width;
				//bitmap_height = line_30_1_bitmap_height;
			}
			else {
				printf ("Error in density specification: density = %d\n",den);
				return;
			}
		}
		else if (ang==45) {
			if (den==5) {
				//      bitmap_bits = line_45_5_bitmap_bits;
				//bitmap_width = line_45_5_bitmap_width;
				//bitmap_height = line_45_5_bitmap_height;
			}
			else if (den==4) {
				//bitmap_bits = line_45_4_bitmap_bits;
				//bitmap_width = line_45_4_bitmap_width;
				//bitmap_height = line_45_4_bitmap_height;
			}
			else if (den==3) {
				//bitmap_bits = line_45_3_bitmap_bits;
				//bitmap_width = line_45_3_bitmap_width;
				//bitmap_height = line_45_3_bitmap_height;
			}
			else if (den==2) {
				//bitmap_bits = line_45_2_bitmap_bits;
				//bitmap_width = line_45_2_bitmap_width;
				//bitmap_height = line_45_2_bitmap_height;
			}
			else if (den==1) {
				//bitmap_bits = line_45_1_bitmap_bits;
				//bitmap_width = line_45_1_bitmap_width;
				//bitmap_height = line_45_1_bitmap_height;
			}
			else {
				printf ("Error in density specification: density = %d\n",den);
				return;
			}
		}
		else if (ang==60) {
			if (den==5) {
				//bitmap_bits = line_60_5_bitmap_bits;
				//bitmap_width = line_60_5_bitmap_width;
				//bitmap_height = line_60_5_bitmap_height;
			}
			else if (den==4) {
				//bitmap_bits = line_60_4_bitmap_bits;
				//bitmap_width = line_60_4_bitmap_width;
				//bitmap_height = line_60_4_bitmap_height;
			}
			else if (den==3) {
				//bitmap_bits = line_60_3_bitmap_bits;
				//bitmap_width = line_60_3_bitmap_width;
				//bitmap_height = line_60_3_bitmap_height;
			}
			else if (den==2) {
				//bitmap_bits = line_60_2_bitmap_bits;
				//bitmap_width = line_60_2_bitmap_width;
				//bitmap_height = line_60_2_bitmap_height;
			}
			else if (den==1) {
				//bitmap_bits = line_60_1_bitmap_bits;
				//bitmap_width = line_60_1_bitmap_width;
				//bitmap_height = line_60_1_bitmap_height;
			}
			else {
				printf ("Error in density specification: density = %d\n",den);
				return;
			}
		}
		else if (ang==-30) {
			if (den==5) {
				//bitmap_bits = line_120_5_bitmap_bits;
				//bitmap_width = line_120_5_bitmap_width;
				//bitmap_height = line_120_5_bitmap_height;
			}
			else if (den==4) {
				//bitmap_bits = line_120_4_bitmap_bits;
				//bitmap_width = line_120_4_bitmap_width;
				//bitmap_height = line_120_4_bitmap_height;
			}
			else if (den==3) {
				//bitmap_bits = line_120_3_bitmap_bits;
				//bitmap_width = line_120_3_bitmap_width;
				//bitmap_height = line_120_3_bitmap_height;
			}
			else if (den==2) {
				//bitmap_bits = line_120_2_bitmap_bits;
				//bitmap_width = line_120_2_bitmap_width;
				//bitmap_height = line_120_2_bitmap_height;
			}
			else if (den==1) {
				//bitmap_bits = line_120_1_bitmap_bits;
				//bitmap_width = line_120_1_bitmap_width;
				//bitmap_height = line_120_1_bitmap_height;
			}
			else {
				printf ("Error in density specification: density = %d\n",den);
				return;
			}
		}
		else if (ang==-45) {
			if (den==5) {
				//bitmap_bits = line_135_5_bitmap_bits;
				//bitmap_width = line_135_5_bitmap_width;
				//bitmap_height = line_135_5_bitmap_height;
			}
			else if (den==4) {
				//bitmap_bits = line_135_4_bitmap_bits;
				//bitmap_width = line_135_4_bitmap_width;
				//bitmap_height = line_135_4_bitmap_height;
			}
			else if (den==3) {
				//bitmap_bits = line_135_3_bitmap_bits;
				//bitmap_width = line_135_3_bitmap_width;
				//bitmap_height = line_135_3_bitmap_height;
			}
			else if (den==2) {
				//bitmap_bits = line_135_2_bitmap_bits;
				//bitmap_width = line_135_2_bitmap_width;
				//bitmap_height = line_135_2_bitmap_height;
			}
			else if (den==1) {
				//bitmap_bits = line_135_1_bitmap_bits;
				//bitmap_width = line_135_1_bitmap_width;
				//bitmap_height = line_135_1_bitmap_height;
			}
			else {
				printf ("Error in density specification: density = %d\n",den);
				return;
			}
		}
		else if (ang==-60) {
			if (den==5) {
				//bitmap_bits = line_150_5_bitmap_bits;
				//bitmap_width = line_150_5_bitmap_width;
				//bitmap_height = line_150_5_bitmap_height;
			}
			else if (den==4) {
				//bitmap_bits = line_150_4_bitmap_bits;
				//bitmap_width = line_150_4_bitmap_width;
				//bitmap_height = line_150_4_bitmap_height;
			}
			else if (den==3) {
				//bitmap_bits = line_150_3_bitmap_bits;
				//bitmap_width = line_150_3_bitmap_width;
				//bitmap_height = line_150_3_bitmap_height;
			}
			else if (den==2) {
				//bitmap_bits = line_150_2_bitmap_bits;
				//bitmap_width = line_150_2_bitmap_width;
				//bitmap_height = line_150_2_bitmap_height;
			}
			else if (den==1) {
				//bitmap_bits = line_150_1_bitmap_bits;
				//bitmap_width = line_150_1_bitmap_width;
				//bitmap_height = line_150_1_bitmap_height;
			}
			else {
				printf ("Error in density specification: density = %d\n",den);
				return;
			}
		}
		else if (ang==90||ang==-90) {
			if (den==5) {
				//bitmap_bits = line_90_5_bitmap_bits;
				//bitmap_width = line_90_5_bitmap_width;
				//bitmap_height = line_90_5_bitmap_height;
			}
			else if (den==4) {
				//bitmap_bits = line_90_4_bitmap_bits;
				//bitmap_width = line_90_4_bitmap_width;
				//bitmap_height = line_90_4_bitmap_height;
			}
			else if (den==3) {
				//bitmap_bits = line_90_3_bitmap_bits;
				//bitmap_width = line_90_3_bitmap_width;
				//bitmap_height = line_90_3_bitmap_height;
			}
			else if (den==2) {
				//bitmap_bits = line_90_2_bitmap_bits;
				//bitmap_width = line_90_2_bitmap_width;
				//bitmap_height = line_90_2_bitmap_height;
			}
			else if (den==1) {
				//bitmap_bits = line_90_1_bitmap_bits;
				//bitmap_width = line_90_1_bitmap_width;
				//bitmap_height = line_90_1_bitmap_height;
			}
			else {
				printf ("Error in density specification: density = %d\n",den);
				return;
			}
		}
		else {
			printf ("Error in angle specification: angle = %d\n",ang);
			return;
		}
	}
	else {
		printf ("Error in fill specification: type = %d\n",typ);
		return;
	}

	if (typ>1) {
		//stipple_pixmap = XCreateBitmapFromData(display, win, (char*)bitmap_bits,bitmap_width, bitmap_height);
		//XSetStipple(display, gc, stipple_pixmap);
		//XSetFillStyle(display, gc, FillStippled);
	}
}

/* Routine to display a button widget */
/* Flags are cumbersome, sigh....
redraw -- indicates the button is being redrawn, probably
due to a resize event.  When set, the assumption
is that *pbn is NULL and is ignored
btnrel -- indicates the button is being redrawn in a new state
due to a buttonpress/buttonrelease event.
nstat  -- forces the state to go to this new setting.
Used for 'redraw button' command.  */

void gxdpbn (gaint bnum, struct gbtn *pbn, gaint redraw, gaint btnrel, gaint nstat) {
	gaint i, j, w, h, ilo, ihi, jlo, jhi, ccc, len;
	struct gbtn *gbb;
	struct gobj *pob=NULL;
	if (bnum<0 || bnum>255) return;
	if (dblmode) {
		gbb = &(btn2[bnum]);
		if (btnrel) {
			/*drwbl = win;*/
			gbb = &(btn[bnum]);
		}
	} else gbb = &(btn[bnum]);
	if (!redraw) {
		*gbb = *pbn;
		gbb->num = bnum;
	}
	if (!redraw || rstate==0) {
		if (dblmode) {
			if (obnum2>511) {
				printf ("Error: Too many widgets on screen\n");
				return;
			}
		} else {
			if (obnum>511) {
				printf ("Error: Too many widgets on screen\n");
				return;
			}
		}
		if (dblmode) {pob = &(obj2[obnum2]); obnum2++;}
		else {pob = &(obj[obnum]); obnum++;}
	}
	if (gbb->num<0) return;
	if (nstat>-1) gbb->state = nstat;
	if (redraw>1) {
		if (pbn->ch) {
			if (gbb->ch) gree((char*)gbb->ch,"f500");
			gbb->ch = pbn->ch;
			gbb->len = pbn->len;
		}
		if (redraw==3) {
			gbb->fc = pbn->fc; gbb->bc = pbn->bc;
			gbb->oc1 = pbn->oc1; gbb->oc2 = pbn->oc2;
			gbb->ftc = pbn->ftc; gbb->btc = pbn->btc;
			gbb->otc1 = pbn->otc1; gbb->otc2 = pbn->otc2;
		}
	}
	i = (gaint)(gbb->x*xscl+0.5);
	j = height - (gaint)(gbb->y*yscl+0.5);
	w = (gaint)(gbb->w*xscl+0.5);
	h = (gaint)(gbb->h*yscl+0.5);
	w = w - 2;
	h = h - 2;
	gbb->ilo = 1 + i - w/2;
	gbb->jlo = 1 + j - h/2;
	gbb->ihi = gbb->ilo + w;
	gbb->jhi = gbb->jlo + h;
	ilo = gbb->ilo;  ihi = gbb->ihi;
	jlo = gbb->jlo;  jhi = gbb->jhi;
	if (gbb->state) ccc = gbb->btc;
	else ccc = gbb->bc;
	if (ccc>-1) {
		gxdcol(ccc);
		//XFillRectangle (display, drwbl, gc, gbb->ilo, gbb->jlo, w, h);
	}
	gxdwid(gbb->thk);
	if (gbb->state) ccc = gbb->otc1;
	else ccc = gbb->oc1;
	if (ccc>-1) {
		gxdcol(ccc);
		//XDrawLine (display, drwbl, gc, ilo, jhi, ihi, jhi);
		//XDrawLine (display, drwbl, gc, ihi, jhi, ihi, jlo);
	}
	if (gbb->state) ccc = gbb->otc2;
	else ccc = gbb->oc2;
	if (ccc>-1) {
		gxdcol(ccc);
		//XDrawLine (display, drwbl, gc, ihi, jlo, ilo, jlo);
		//XDrawLine (display, drwbl, gc, ilo, jlo, ilo, jhi);
	}
	if (gbb->state) ccc = gbb->ftc;
	else ccc = gbb->fc;
	if (ccc>-1) {
		len = 0;
		while (*(gbb->ch+len)) len++;
		/*    len++;*/
		gxdcol(ccc);
		//if (gfont==1 && font1) {
		//  XSetFont (display, gc, font1->fid);
		//  w = XTextWidth(font1, gbb->ch, len);
		//  i = i - w/2;
		//  j = j + 5*font1->ascent/9;
		//}
		//if (gfont==2 && font2) {
		//  XSetFont (display, gc, font2->fid);
		//  w = XTextWidth(font2, gbb->ch, len);
		//  i = i - w/2;
		//  j = j + 5*font2->ascent/9;
		//}
		//if (gfont==3 && font3) {
		//  XSetFont (display, gc, font3->fid);
		//  w = XTextWidth(font3, gbb->ch, len);
		//  i = i - w/2;
		//  j = j + 5*font3->ascent/9;
		//}
		//XDrawString(display, drwbl, gc, i, j, gbb->ch, len);
	}
	gxdcol(lcolor);
	//if (dblmode && btnrel) drwbl = pmap;
	if (!redraw || rstate==0) {
		pob->type = 1;
		pob->mb = -1;
		pob->i1 = ilo;  pob->i2 = ihi;
		pob->j1 = jlo;  pob->j2 = jhi;
		pob->iob.btn = gbb;
	}
	//XFlush(display);
}

/* Set up a rubber-band region.  */

void gxdrbb (gaint num, gaint type,
	gadouble xlo, gadouble ylo, gadouble xhi, gadouble yhi, gaint mbc) {
		struct grbb *prb;
		struct gobj *pob;

		if (num<0 || num>31) return;
		if (xlo>=xhi) return;
		if (ylo>=yhi) return;

		if (dblmode) {
			if (obnum2>511) return;
			pob = &(obj2[obnum2]); obnum2++;
			prb = &(rbb2[num]);
		} else {
			if (obnum>511) return;
			pob = &(obj[obnum]); obnum++;
			prb = &(rbb[num]);
		}

		pob->type = 2;
		pob->mb = mbc;
		pob->i1 = (gaint)(xlo*xscl+0.5);
		pob->i2 = (gaint)(xhi*xscl+0.5);
		pob->j1 = height - (gaint)(yhi*yscl+0.5);
		pob->j2 = height - (gaint)(ylo*yscl+0.5);
		pob->iob.rbb = prb;
		prb->num = num;
		prb->xlo = xlo; prb->xhi = xhi;
		prb->ylo = ylo; prb->yhi = yhi;
		prb->type = type;
		prb->mb = mbc;
}

/* Routine to display a drop menu widget:
redraw -- indicates the button is being redrawn, probably
due to a resize event.  When set, the assumption
is that *dmu is NULL and is ignored; info for
redrawing is obtained from the existing
structure list.
nstat  -- re-defines the dropmenu.
Used for 'redraw dropmenu' command.  */

void gxdrmu (gaint mnum, struct gdmu *pmu, gaint redraw, gaint nstat) {
	gaint i, j, w, h, ilo, ihi, jlo, jhi, len, lw;
	struct gdmu *gmu;
	struct gobj *pob=NULL;

	if (mnum<0 || mnum>199) return;
	if (dblmode) gmu = &(dmu2[mnum]);
	else gmu = &(dmu[mnum]);
	if (!redraw) {
		*gmu = *pmu;
		gmu->num = mnum;
	}
	if (gmu->num<0) return;
	if (gmu->casc) return;
	if (!redraw || rstate==0) {
		if (dblmode) {
			if (obnum2>511) {
				printf ("Error: Too many widgets on screen\n");
				return;
			}
		} else {
			if (obnum>511) {
				printf ("Error: Too many widgets on screen\n");
				return;
			}
		}
		if (dblmode) {pob = &(obj2[obnum2]); obnum2++;}
		else {pob = &(obj[obnum]); obnum++;}
	}
	i = (gaint)(gmu->x*xscl+0.5);
	j = height - (gaint)(gmu->y*yscl+0.5);
	w = (gaint)(gmu->w*xscl+0.5);
	h = (gaint)(gmu->h*yscl+0.5);
	w = w - 2;
	h = h - 2;
	gmu->ilo = 1 + i - w/2;
	gmu->jlo = 1 + j - h/2;
	gmu->ihi = gmu->ilo + w;
	gmu->jhi = gmu->jlo + h;
	ilo = gmu->ilo;  ihi = gmu->ihi;
	jlo = gmu->jlo;  jhi = gmu->jhi;
	if (gmu->bc>-1) {
		gxdcol(gmu->bc);
		//XFillRectangle (display, drwbl, gc, ilo, jlo, w+1, h+1);
	}
	lw = 1;
	if (gmu->thk>5) lw = 2;
	if (gmu->thk>12) lw = 3;
	gxdwid(1);
	if (gmu->oc1>-1) {
		gxdcol(gmu->oc1);
		//XDrawLine (display, drwbl, gc, ilo, jhi, ihi, jhi);
		//if (lw>1) XDrawLine (display, drwbl, gc, ilo+1, jhi-1, ihi-1, jhi-1);
		//if (lw>2) XDrawLine (display, drwbl, gc, ilo+2, jhi-2, ihi-2, jhi-2);
		//XDrawLine (display, drwbl, gc, ihi, jhi, ihi, jlo);
		//if (lw>1) XDrawLine (display, drwbl, gc, ihi-1, jhi-1, ihi-1, jlo+1);
		//if (lw>2) XDrawLine (display, drwbl, gc, ihi-2, jhi-2, ihi-2, jlo+2);
	}
	if (gmu->oc2>-1) {
		gxdcol(gmu->oc2);
		//XDrawLine (display, drwbl, gc, ihi, jlo, ilo, jlo);
		//if (lw>1) XDrawLine (display, drwbl, gc, ihi-1, jlo+1, ilo+1, jlo+1);
		//if (lw>2) XDrawLine (display, drwbl, gc, ihi-2, jlo+2, ilo+2, jlo+2);
		//XDrawLine (display, drwbl, gc, ilo, jlo, ilo, jhi);
		//if (lw>1) XDrawLine (display, drwbl, gc, ilo+1, jlo+1, ilo+1, jhi-1);
		//if (lw>2) XDrawLine (display, drwbl, gc, ilo+2, jlo+2, ilo+2, jhi-2);
	}
	if (gmu->fc>-1) {
		len = 0;
		while (*(gmu->ch+len)) len++;
		/*    len++;*/
		gxdcol(gmu->fc);
		//if (gfont==1 && font1i) {
		//  XSetFont (display, gc, font1i->fid);
		//  w = XTextWidth(font1i, gmu->ch, len);
		//  i = ilo + font1i->ascent/2;
		//  j = j + 5*font1i->ascent/9;
		//}
		//if (gfont==2 && font2i) {
		//  XSetFont (display, gc, font2i->fid);
		//  w = XTextWidth(font2i, gmu->ch, len);
		//  i = ilo + font2i->ascent/2;
		//  j = j + 5*font2i->ascent/9;
		//}
		//   if (gfont==3 && font3i) {
		//	XSetFont (display, gc, font3i->fid);
		//	w = XTextWidth(font3i, gmu->ch, len);
		//	i = ilo + font3i->ascent/2;
		//	j = j + 5*font3i->ascent/9;
		//}
		//XDrawString(display, drwbl, gc, i, j, gmu->ch, len);
		//if (gfont==1 && font1) XSetFont (display, gc, font1->fid);
		//if (gfont==2 && font2) XSetFont (display, gc, font2->fid);
		//if (gfont==3 && font3) XSetFont (display, gc, font3->fid);
	}
	gxdcol(lcolor);
	if (!redraw || rstate==0) {
		pob->type = 3;
		pob->mb = -1;
		pob->i1 = ilo;  pob->i2 = ihi;
		pob->j1 = jlo;  pob->j2 = jhi;
		pob->iob.dmu = gmu;
	}
	//XFlush(display);
}

/* Return info on mouse button press event.  Wait if requested. */

void gxdbtn (gaint flag, gadouble *xpos, gadouble *ypos,
	gaint *mbtn, gaint *type, gaint *info, gadouble *rinfo) {
		struct gevent *geve;
		gaint i;

		if (batch) {
			*xpos = -999.9;
			*ypos = -999.9;
			return;
		}
		gxdeve(flag);
		if (evbase==NULL) {
			*xpos = -999.9;
			*ypos = -999.9;
			*mbtn = -1;
			*type = -1;
		} else {
			geve = evbase;
			*xpos = geve->x;
			*ypos = geve->y;
			*mbtn = geve->mbtn;
			*type = geve->type;
			for (i=0; i<10; i++) *(info+i) = geve->info[i];
			for (i=0; i<4; i++) *(rinfo+i) = geve->rinfo[i];
			evbase = geve->forw;       /* Take even off queue */
			free(geve);
		}

}

/* Reset all widgets; release memory as appropriate. */
/* flag = 0 resets foreground, flag = 1 resets both,
flag = 2 resets background only; for after swapping */

void gxrswd(int flag) {
	int i;

	if (flag!=2) {
		for (i=0; i<256; i++) {
			if (btn[i].num>-1 && btn[i].ch!=NULL) gree(btn[i].ch,"f501");
			btn[i].num = -1;
			btn[i].ch = NULL;
		}
		for (i=0; i<200; i++) {
			if (dmu[i].num>-1 && dmu[i].ch!=NULL) gree(dmu[i].ch,"f502");
			dmu[i].num = -1;
			dmu[i].ch = NULL;
		}
		for (i=0; i<32; i++) rbb[i].num = -1;
	}

	if (flag) {
		for (i=0; i<256; i++) {
			if (flag!=2) {
				if (btn2[i].num>-1 && btn2[i].ch!=NULL) gree(btn2[i].ch,"f503");
			}
			btn2[i].num = -1;
			btn2[i].ch = NULL;
		}
		for (i=0; i<200; i++) {
			if (flag!=2) {
				if (dmu2[i].num>-1 && dmu2[i].ch!=NULL) gree(dmu2[i].ch,"f504");
			}
			dmu2[i].num = -1;
			dmu2[i].ch = NULL;
		}
		for (i=0; i<32; i++) rbb2[i].num = -1;
	}
}

void gxdrawtextvalue(char* nText,gadouble x,gadouble y)
{
	LineTextValue *tempText = new LineTextValue;
	memcpy(tempText->nTextValue,nText,sizeof(char)*20);
	//tempText->nTextValue = nText;
	tempText->x = x;
	tempText->y = y;
	ContourTextValueArray.push_back(tempText);
}

void gxdrawwindbarb(gadouble x,gadouble y,gadouble dir, gadouble spd)
{
	WindBarbData *tempWind = new WindBarbData;
	tempWind->x = x;
	tempWind->y = y;
	tempWind->dir = dir;
	tempWind->spd = spd;
	WindBarbDataArray.push_back(tempWind);
}