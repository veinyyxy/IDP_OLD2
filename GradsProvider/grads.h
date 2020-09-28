#ifndef GRADS_H
#define GRADS_H

//////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>

#include "gatypes.h"
#include "gabufr.h"

/* Handling of missing data values. After the data I/O is done, 
grid values are tested to see if they are within a small range 
(+-value/EPSILON) of the missing value. If true, then the undef 
mask is set to 0. If false, then the grid data values are good, 
and the undef mask is set to 1. Everywhere else in the code, 
undef tests are done on the mask values, not the data. */

#define EPSILON 1e5
#define FUZZ_SCALE 1e-5

/* RPTNUM: Number of garpt blocks to allocate per memory block
BLKNUM: Max number of memory requests
A max of RPTNUM*BLKNUM stations can be held per request
Static memory usage is sizeof(pointer) * BLKNUM bytes */

#define RPTNUM 200
#define BLKNUM 5000

/*******************\
*  Data Structures  *
\*******************/

/* Pointer to data object */
union gadata {
	struct gagrid *pgr;
	struct gastn  *stn;
};

/* Date/time structure */
struct dt {
	gaint yr;
	gaint mo;
	gaint dy;
	gaint hr;
	gaint mn;
};

/* Collection structure */
struct gaclct {
	struct gaclct *forw;        /* Forward Pointer */
	struct gastn  *stn;         /* Pointer to station data */
	struct gastn  *stn2;        /* Pointer to station data */
};

/* Structure for info for the gxout "writegds" option, 
for writing data for the use of the GDS */
struct gawgds {
	char *fname;                /* File name to write */
	char *opts;                 /* User specified options */
};


/*mf 9612105 Contains global information for Mike Fiorino and Gary Love 980114 mf*/

struct gamfcmn {
	gaint cal365 ;               /* 365 (no leap year) calendar */
	gaint fullyear ;             /* 1 - must specify full year 0 old default */
	gaint warnflg;               /* warning level flag for messages */
	gaint winid;                 /* Window ID */
	gaint winx;                  /* Window X position (upper left) */
	gaint winy;                  /* Window Y position (upper left) */
	gauint winw;                 /* Window width */
	gauint winh;                 /* Window height */
	gauint winb;                 /* Window border width */

};

/* Contains information about the user interface and graphics output. */
struct gacmn {

	gadouble dmin[5],dmax[5];    /* Current absolute coordinate limits    */
	/* Grid-to-world conversion info follows */
	gadouble (*xgr2ab) (gadouble *, gadouble);
	gadouble (*ygr2ab) (gadouble *, gadouble);
	gadouble (*xab2gr) (gadouble *, gadouble);
	gadouble (*yab2gr) (gadouble *, gadouble);
	gadouble *xgrval;
	gadouble *ygrval;
	gadouble *xabval;
	gadouble *yabval;
	struct gawgds *wgds;         /* Pointer to gds output structure       */
	gaint hbufsz;                /* Metafile buffer size                  */
	gaint g2bufsz;               /* Grib2 cache buffer size               */
	gaint pass;                  /* Number of passes since last clear     */
	gaint gpass[10];             /* Number of passes for each gx type     */
	gaint loopdim;               /* Looping dimension                     */
	gaint loopflg;               /* Looping on or off                     */
	struct gafile *pfi1;         /* Pointer to first gafile in chain      */
	struct gafile *pfid;         /* Pointer to default gafile             */
	gaint fnum;                  /* File count                            */
	gaint dfnum;                 /* Default file number   */
	gaint fseq;                  /* Unique sequence num for files opened  */
	struct gadefn *pdf1;         /* Pointer to first define block         */
	struct dt tmin,tmax;
	gaint vdim[5];               /* Which dimensions vary?                */
	gaint x1ex,x2ex,y1ex,y2ex;   /* For -ex flag on fwrite */
	gaint xexflg,yexflg;         /* -ex -- are dims valid? */
	gadouble pxsize,pysize;      /* Physical page size in inches          */
	gaint orient;                /* Page orientation                      */
	gaint vpflag;                /* If 1, virtual page being used         */
	gadouble xsiz,xsiz1,xsiz2;   /* Physical plotting size in X direction */
	gadouble ysiz,ysiz1,ysiz2;   /* Physical plotting size in Y direction */
	gaint paflg;                 /* User has specified plotting area      */
	gadouble pxmin,pxmax;        /* User specified plotting area          */
	gadouble pymin,pymax;
	gaint clab;                  /* control contour labels.               */
	gaint clskip;                /* Contour label skipping                */
	char *clstr;                 /* Contour label template                */
	gadouble rainmn,rainmx;      /* control rainbow colors                */
	gaint rbflg;                 /* User rainbow colors specified         */
	gaint rbcols[256];           /* User rainbow colors                   */
	gadouble cmin,cmax,cint;     /* User specified contour limits         */
	gaint cflag;                 /* If true, user specifies contour levels*/
	gadouble clevs[256];         /* User specified contour levels         */
	gaint ccflg;                 /* If true, user specifies contour colors*/
	gaint ccols[256];            /* User specified contour colors         */
	gaint shdcls[256];           /* Shade colors after shading            */
	gadouble shdlvs[256];        /* Shade levels                          */
	gaint shdcnt;                /* Number of shdlvs, shdcls              */
	gaint cntrcnt;               /* Number of contours (after countouring)*/
	gaint cntrcols[256];         /* Contour colors (after contouring)     */
	gadouble cntrlevs[256];      /* Contour levels (after contouring)     */
	gaint ccolor,cstyle;         /* User contour/line appearance          */
	gaint cthick;                /* User gx display line thickness        */
	gaint cmark;                 /* Line marker type                      */
	gaint csmth;                 /* Contour smoothing on or off           */
	gaint cterp;                 /* Spline fit on or off                  */
	gadouble rmin,rmax,rint;     /* Axis limits for 1-D plots             */
	gadouble rmin2,rmax2,rint2;  /* Axis limits for 1-D plots             */
	gaint aflag,aflag2;          /* Keep 1D axis limits fixed             */
	gaint grflag,grstyl,grcolr;  /* Grid flag, linestyle, color           */
	gaint dignum;                /* grid value plot control (gxout=grid)  */
	gadouble digsiz;
	gaint arrflg;                /* Use already set arrow scaling         */
	gadouble arrsiz;             /* Arrow size in inches                  */
	gaint arlflg;                /* Arrow label flag */
	gadouble arrmag;             /* Vector magnitude producing arrsiz arrw*/
	gadouble ahdsiz;             /* Arrow head size.       */
	gaint hemflg;                /* -1; auto  0; nhem  1; shem */
	gaint miconn;                /* Connect line graph accross missing    */
	gaint strmden;               /* Streamline density indicator  */
	gaint mdlblnk,mdldig3;       /* Station model plot opts */
	char *prstr;                 /* Format string for gxout print */
	gaint prlnum;                /* Number of values per record */
	gaint prbnum;                /* Number of blanks to add between values */
	gaint prudef;                /* Undef printed as "undef" or value */
	gaint fgvals[50];            /* Values for grid fill */
	gaint fgcols[50];
	gaint fgcnt;
	gaint gridln;                /* Line attributes for gxout grid */
	gaint stidflg;               /* Plot station ids with values      */
	gadouble axmin,axmax,axint;  /* Overrides for X-axis labels           */
	gadouble aymin,aymax,ayint;  /* Overrides for Y-axis labels           */
	gaint axflg, ayflg;          /* Is override in effect for the axis?   */
	gaint frame;                 /* Display frame?  */
	gaint rotate;                /* Rotate plot from default orientation  */
	gaint xflip, yflip;          /* Flip X or Y axes                      */
	gaint zlog;                  /* Z coordinate in log scale */
	gaint coslat;                /* Lat coordinate scaled as cos lat */
	gaint mproj;                 /* Map projection -- used for X,Y plot   */
	/*  only.  0 = no map.                   */
	gaint mpdraw;                /* Draw map outline - 0=no               */
	gadouble mpvals[10];         /* Map projection option values.         */
	gaint mpflg;                 /* Map projection option values are set. */
	char *mpdset[8];             /* Map data set names.                   */
	gaint mpcols[256];           /* Map Color array                       */
	gaint mpstls[256];           /* Map line styles array                 */
	gaint mpthks[256];           /* Map line widths array                 */
	gaint mapcol,mapstl,mapthk;  /* Default map color, style, thickness   */
	gaint gout0;                 /* Graphics output type for stat.        */
	gaint gout1;                 /* Graphics output type for 1-D.         */
	gaint gout1a;                /* Graphics output type for 1-D.         */
	gaint gout2a;                /* Graphics output type for 2-D.         */
	gaint gout2b;                /* Graphics output type for 2-D.         */
	gaint goutstn;               /* Graphics output type for stns */
	gaint blkflg;                /* Leave certain values black when shadng*/
	gadouble blkmin, blkmax;     /* Black range */
	gaint reccol,recthk;         /* Draw Rectangle color, brdr thickness  */
	gaint lincol,linstl,linthk;  /* Draw line color, style, thickness     */
	gaint mcolor;                /* auto color (orange or grey)           */
	gaint strcol,strthk,strjst;  /* Draw string color, thckns, justifictn */
	gadouble strrot;             /* Draw string rotation */
	gadouble strhsz,strvsz;      /* Draw string hor. size, vert. size     */
	gaint anncol,annthk;         /* Draw title color, thickness           */
	gaint grflg;                 /* Grey Scale flag   */
	gaint devbck;                /* Device background */
	gaint xlcol,xlthck,ylcol,ylthck,clcol,clthck;  /* color, thickness */
	gaint xlside,ylside,ylpflg;
	gadouble xlsiz,ylsiz,clsiz,xlpos,ylpos,yllow;         /* Axis lable size */
	gadouble xlevs[50],ylevs[50]; /* User specified x/y axis labels  */
	gaint xlflg,ylflg;           /* Number of user specified labels */
	gaint xtick,ytick;           /* Number of extra tick marks      */
	gadouble xlint,ylint;        /* User specified label increment */
	char *xlstr, *ylstr;         /* user substitution string for labels */
	gaint xlab,ylab;             /* Axis label options */
	char *xlabs, *ylabs;         /* User specifies all labels */
	gaint ixlabs, iylabs;        /* Count of user labels */
	gaint tlsupp;                /* Suppress year or month of time labels */
	gaint lfc1,lfc2;             /* Linefill colors */
	gaint wxcols[5];             /* wx symbol colors */
	gaint wxopt;                 /* wx options */
	gaint tser;                  /* station time series type */
	gaint bargap;                /* Bar Gap in percent  */
	gaint barolin;               /* Bar outline flag */
	gadouble barbase;            /* Bar Base Value      */
	gaint barflg;                /* Bar flag: 1, use base value  */
	/*           0, draw from plot base */
	/*          -1, draw from plot top  */
	gaint btnfc,btnbc,btnoc,btnoc2;     /* Current button attributes */
	gaint btnftc,btnbtc,btnotc,btnotc2;
	gaint btnthk;
	gaint dlgfc,dlgbc,dlgoc;   /* Current dialog attributes */
	gaint dlgpc,dlgth,dlgnu;
	gaint drvals[15];          /* Attributes for drop menus */
	char *shpfname;            /* shapefile write file name */
	gaint shptype;             /* shapefile output type: 1=point, 2=line */
	gaint gtifflg;             /* geotiff data type: 1=float 2=double */
	char *gtifname;            /* geotiff write file name */
	char *tifname;             /* kml image  file name */
	char *kmlname;             /* kml text file name */
	gaint kmlflg;              /* kml output: 1==img, 2==contours */
	char *sdfwname;            /* netcdf/hdf write file name */
	gaint sdfwtype;            /* type of sdf output: 1=classic, 2=nc4 */
	gaint sdfwpad;             /* pad the sdf output with extra dims: 1=4D, 2=5D */
	gaint sdfprec;             /* precision (8==double, 4==float, etc.) */
	gaint sdfchunk;            /* flag to indicate whether or not to chunk */
	gaint sdfzip;              /* flag to indicate whether or not to compress */
	gaint ncwid;               /* netcdf write file id  */
	gaint xchunk;              /* size of sdfoutput file chunk in X dimension */
	gaint ychunk;              /* size of sdfoutput file chunk in Y dimension */
	gaint zchunk;              /* size of sdfoutput file chunk in Z dimension */
	gaint tchunk;              /* size of sdfoutput file chunk in T dimension */
	gaint echunk;              /* size of sdfoutput file chunk in E dimension */
	struct gaattr *attr;       /* pointer to link list of user-specified SDF attributes */
	gaint dblen;               /* total number of digits for formatting data base fields */
	gaint dbprec;              /* precision digits for formatting data base fields: %len.prec */
	FILE *ffile;               /* grads.fwrite file handle */
	FILE *sfile;               /* grads.stnwrt file handle */
	char *fwname;              /* fwrite file name */
	gaint fwenflg;             /* fwrite byte order control */
	gaint fwsqflg;             /* fwrite stream vs fortran seq */
	gaint fwappend;            /* write mode (1): append */
	gaint fwexflg;             /* fwrite exact grid dims */
	gaint grdsflg;             /* Indicate whether to put grads atrib.  */
	gaint timelabflg;          /* Indicate whether to put cur time atrib.  */
	gaint stnprintflg;         /* Indicate whether to put cur time atrib.  */
	gaint dbflg;               /* Double buffer mode flag     */
	gaint batflg;              /* Batch mode */
	gaint numgrd,relnum;       /* Number of data objects held           */
	gaint type[16];            /* Data type of each data object         */
	union gadata result[16];   /* Pointers to held data objects         */
	struct gaclct *clct[32];   /* Anchor for collection */
	gaint clctnm[32];          /* Number of items collected */
	gaint clcttp[32];          /* Varying dimension of collection */
	gaint lastgx;              /* Last gx plotted */
	gaint xdim, ydim;          /* Which dimensions on X and Y axis */
	gaint statflg;             /* stat txt output on all displays */
	gaint impflg;              /* Implied run flag */
	char *impnam;              /* Implided run script name */
	gaint impcmd;              /* Implicit run */
	gaint sig;                 /* User has signalled */
	gaint ptflg;		     /* Pattern fill flag */
	gaint ptopt;		     /* Pattern option: */
	/*		0, open  */
	/*		1, solid */
	/*		2, dot */
	/*		3, line  */
	gaint ptden;		     /* Dot or line pattern density */
	gaint ptang;		     /* Line pattern angle */
	gaint dwrnflg;             /* Issue, or not, warnings about missing or constant data */
	gadouble undef;            /* default or user-defined undef value for print and file output */
	gadouble cachesf;          /* global scale factor for netcdf4/hdf5 cache size */
	gaint fillpoly;            /* color to fill shapfile polygons, -1 for no fill */
	gaint marktype;            /* type of mark for shapefile points */
	gadouble marksize;         /* size of mark for shapefile points */

	char * str_errorinfo;		// added by Liu Xinsheng 2011-06-15
	bool isProfile;				// added by Liu Xinsheng 2011-06-15
	bool isContour;				// added by Wang Jialiang 2011-06-15
	bool isShaded;				// added by Wang Jialiang 2011-06-15
	bool isLineBar;				// added by Wang Jialiang 2011-06-15
	bool isVBS;					// added by Wang Jialiang 2011-06-15
	gadouble nCmin;				// added by Wang Jialiang 2011-06-15
	gadouble nCmax;				// added by Wang Jialiang 2011-06-15
	gadouble nCint;				// added by Wang Jialiang 2011-06-15
	gadouble dCurVal;			// added by Wang Jialiang 2012-01-10
	bool bSpecialZ;			// added by Wang Jialiang 2012-01-10

	bool error;					// added by Wang Jialiang 2011-06-29
};
extern struct gacmn gcmn;

/* Sructure for string substitution in templating -- the %ch template.  
This forms a linked list chained from pchsub1 in gafile */
struct gachsub {
	struct gachsub *forw;       /* Forward pointer */
	gaint t1;                   /* First time for this substitution */
	gaint t2;                   /* Last time.  -99 indicates open ended */
	char *ch;                   /* Substitution string */
};

/* Structure for ensemble metadata */
struct gaens {
	char name[16];             /* name of ensemble */
	gaint length;              /* length of time axis */
	struct dt tinit;           /* initial time */
	gaint gt;                  /* initial time in grid units */
	gaint grbcode[4];          /* grib2 codes */
};

/* GA status structure.  Contains necessary info about the scaling
and file structure in force.                                       */
struct gastat {
	struct gafile *pfi1;       /* Pointer to first gafile in chain      */
	struct gafile *pfid;       /* Pointer to default gafile             */
	struct gadefn *pdf1;       /* Pointer to first define block         */
	struct gaclct **pclct;     /* Pointer to the collection pointers    */
	union gadata result;       /* Result goes here                      */
	struct dt tmin,tmax;
	gadouble dmin[5],dmax[5];  /* Range of absolute dimensions          */
	gaint fnum;                /* Default file number                   */
	gaint type;                /* Result type (grid==1 or stn==0)       */
	gaint idim,jdim;           /* Varying dimensions                    */
};


/* Description of a data file.                                        */
struct gafile {
	struct gafile *pforw;      /* Forward pointer to next gafile block.
							   List is anchored within gastat.       */
	gaint fseq;                /* Unique sequence number for cache detection */
	char name[4096];           /* File name or URL                      */
	char *tempname;            /* File name of open file (differs with templates) */
	char dnam[4096];           /* Descriptor file name                  */
	char *mnam;                /* Map(index) file name */
	FILE *infile;              /* File pointer.                         */
	gadouble *pArrayValuebuf;     /* Added by Liu Xinsheng 2011-05-06*/
	bool bIsArray;                               /* Added by Liu Xinsheng 2011-05-11 判断是文件还是数组*/
	gaint type;                /* Type of file:  1 = grid
							   2 = simple station
							   3 = mapped station
							   4 = defined grid       */
	char title[512];           /* Title -- describes the file.          */
	gadouble undef;            /* Global undefined value for this file  */
	gadouble ulow,uhi;         /* Undefined limits for missing data test  */
	gafloat *sbuf;             /* Buffer for file I/O equal in length
							   to the size needed to hold
							   the largest station report            */
	gadouble *rbuf;            /* Buffer for file I/O equal in length
							   to one grid row in the file           */
	unsigned char *pbuf;       /* Same as rbuf, for unpacking           */
	char *bbuf;                /* Same as rbuf, for bit map I/O         */
	char *ubuf;                /* Same as rbuf, for undef mask          */
	gaint bswap;               /* Byte swapping needed */
	gaint dhandle;             /* libgadap file handle.                 */
	gaint dapinf[5];           /* pointer to coordinate variable indices
							   (first four elements are lon,lat,lev,time
							   fifth is station id)
							   for opendap station data only */
	gaint mtype;               /* Stn map file type                     */
	gaint *tstrt;              /* Pointer to list length dnum[3] of
							   start points of times in the file     */
	gaint *tcnt;               /* Count of stns for assctd time         */
	gaint stcnt;               /* Count of mapped stids when stn data
							   and map file is type stidmap.         */
	gaint stpos;               /* Position in map file of start of
							   stid info for map file type stidmap.  */
	FILE *mfile;               /* File pointer to stidmap file          */
	gaint dnum[5];               /* Dimension sizes for this file.        */
	gaint tlpflg;                /* Circular file flag                    */
	gaint tlpst;                 /* Start time offset in circular file    */
	gaint vnum;                  /* Number of variables.                  */
	gaint ivnum;                 /* Number of level independent variables
								 for station data file                 */
	gaint lvnum;                 /* Number of level dependent variables
								 for station data file                 */
	struct gavar *pvar1;       /* Pointer to an array of structures.
							   Each structure in the array has info
							   about the specific variable.          */
	struct gaens *ens1;          /* pointer to array of ensemble structures */
	long gsiz;                   /* Number of elements in a grid (x*y)    */
	/* This is for actual grid on disk,
	not psuedo grid (when pp in force) */
	long tsiz;                   /* Number of elements in an entire time
								 group (all variables at all levels
								 for one time).                        */
	gaint trecs;                 /* Number of records (XY grids) per time
								 group.                                */
	long fhdr;                   /* Number of bytes to ignore at file head*/
	gaint wrap;                  /* The grid globally 'wraps' in X        */
	gaint seqflg, yrflg, zrflg;  /* Format flags */
	gaint ppflag;                /* Pre-projected data in use */
	gaint pdefgnrl;              /* Keyword 'general' used instead of 'file' */
	gaint ppwrot;                /* Pre-projection wind rotation flag */
	gaint ppisiz, ppjsiz;        /* Actual size of preprojected grid */
	gadouble ppvals[20];         /* Projection constants for pre-projected
								 grids.  Values depend on projection. */
	gaint *ppi[9];               /* Pointers to offsets for pre-projected
								 grid interpolation */
	gadouble *ppf[9];            /* Pointers to interpolation constants
								 for pre-projected grids */
	gadouble *ppw;               /* Pointer to wind rotation array */
	gadouble (*gr2ab[5]) (gadouble *, gadouble);
	/* Addresses of routines to do conversion
	from grid coordinates to absolute
	coordinates for X, Y, Z.  All Date/time
	conversions handled by gr2t.          */
	gadouble (*ab2gr[5]) (gadouble *, gadouble);
	/* Addresses of routines to do conversion
	from absolute coordinates to grid
	coordinates for X,Y,Z.  All date/time
	conversions handled by t2gr.          */
	gadouble *grvals[5];         /* Pointers to conversion information for
								 grid-to-absolute conversion routines. */
	gadouble *abvals[5];         /* Pointers to conversion information for
								 absolute-to-grid conversion routines. */
	gaint linear[5];             /* Indicates if a dimension has a linear
								 grid/absolute coord transformation
								 (Time coordinate always linear).      */
	gaint dimoff[5];             /* Dimension offsets for defined grids   */
	gaint climo;                 /* Climatological Flag (defined grids)   */
	gaint cysiz;                 /* Cycle size for climo grids            */
	gaint idxflg;                /* File records are indexed; 1==grib,station 2==grib2 */
	gaint grbgrd;                /* GRIB Grid type */
	struct gaindx *pindx;        /* Index Strucure if indexed file */
	struct gaindxb *pindxb;      /* Index Strucure if off_t offsets are being used */
	gaint tmplat;                /* File name templating:
								 3==templating on E and T 
								 2==templating only on E 
								 1==templating only on T, or when 
								 ddf has 'options template', but no % in dset 
								 0==no templating  */
	gaint *fnums;                /* File number for each time */
	gaint fnumc;                 /* Current file number that is open */
	gaint fnume;                 /* Current ensemble file number that is open */
	struct gachsub *pchsub1;     /* Pointer to first %ch substitution */
	gaint errcnt;                /* Current error count */
	gaint errflg;                /* Current error flag */
	gaint ncflg;                 /* 1==netcdf  2==hdfsds */
	gaint ncid;                  /* netcdf file id */
	gaint sdid;                  /* hdf-sds file id */
	gaint h5id;                  /* hdf5 file id */
	gaint packflg;               /* Data are packed with scale and offset values */
	gaint undefattrflg;          /* Undefined values are retrieved individually  */
	char *scattr;                /* scale factor attribute name for unpacking data */
	char *ofattr;                /* offset attribute name for unpacking data */
	char *undefattr;             /* undef attribute name */
	long xyhdr;                  /* Number of bytes to ignore at head of xy grids*/
	gaint calendar;              /* Support for 365-day calendars */
	gaint pa2mb;                 /* convert pressure values in descriptor file from Pa -> mb */
	gaint bufrflg;               /* 1==dtype bufr */
	struct bufrinfo *bufrinfo;   /* x,y pairs from descriptor file */ 
	gabufr_dset *bufrdset;       /* pointer to parsed bufr data */
	struct gaattr *attr;         /* pointer to link list of attribute metadata */
	gaint nsdfdims; 
	gaint sdfdimids[100];
	gaint sdfdimsiz[100];
	gaint time_type;             /* temporary flag for SDF time handling */
	char sdfdimnam[100][129];
	long cachesize;            /* default netcdf4/hdf5 cache size */
};

/* Structure that describes a grid (requestor or descriptor block).  */
struct gagrid {
	struct gafile *pfile;        /* Address of the associated gafile
								 structure to get the data from
								 (requestor block only)               */
	gadouble *grid;              /* Address of the grid.                 */
	gaint mnum;                  /* Number of grids when a multiple
								 grid result.  Note in this case, *grid
								 points to more than one grid, with the
								 "default" result being the 1st grid  */ 
	gaint mtype;                 /* Type of multiple result grid         */
	gaint *mnums;                /* See mvals  */
	gadouble *mvals;             /* Metadata associated with a multiple
								 grid result.  What is here depends on
								 the value of mtype.                  */
	gadouble undef;              /* Undefined value for this grid.       */
	gadouble rmin,rmax;          /* Minimum/Maximum grid value
								 (rmin is set to the grid value when
								 isiz=jsiz=1.  *grid points to here.) */
	char *umask;                 /* Mask for undefined values in the grid */
	char umin,umax;              /* Min/max undefined mask values. 
								 (when isiz=jsiz=1, umin is set to the 
								 mask value and *umask points to umin) */
	gaint isiz,jsiz;             /* isiz = number of elements per row.
								 jsiz = number of rows.               */
	gaint idim,jdim;             /* Dimension of rows and columns.
								 -1 = This dimension does not vary
								 0 = X dimension (usually longitude)
								 1 = Y dimension (usually lattitude)
								 2 = Z dimension (usually pressure)
								 3 = Time
								 4 = Ensemble
								 If both dimensions are -1, then the
								 grid has one value, which will be
								 placed in rmin.                      */
	gaint iwrld, jwrld;          /* World coordinates valid?             */
	gaint dimmin[5],dimmax[5];   /* Dimension limits for each dimension
								 (X,Y,Z,T,E) in grid units.           */
	struct gavar *pvar;          /* Pointer to the structure with info
								 on this particular variable.  If
								 NULL, this grid is the result of
								 an expression evaluation where the
								 variable type is unkown.             */
	char *exprsn;                /* If grid is a 'final' result, this
								 will point to a character string that
								 contains the original expression.    */
	gaint alocf;                  /* Scaling info allocated for us only  */
	gadouble (*igrab) (gadouble *, gadouble);
	gadouble (*jgrab) (gadouble *, gadouble);
	/* Addresses of routines to perform
	grid-to-absolute coordinate
	transforms for this grid's i and j
	dimensions (unless i or j = 3).      */
	gadouble (*iabgr) (gadouble *, gadouble);
	gadouble (*jabgr) (gadouble *, gadouble);
	/* Absolute to grid conversion routines */
	gadouble *ivals, *jvals;      /* Conversion info for grid to abs      */
	gadouble *iavals, *javals;    /* Conversion info for abs to grid      */
	gaint ilinr,jlinr;            /* Indicates if linear transformation   */
	gaint toff;                   /* Indicates if T dim values are forecast offsets */
};

/* Structure that contains attribute metadata */
struct gaattr {
	struct gaattr *next;          /* Address of next attribute */
	char  varname[129];           /* Name of variable or 'global' */
	char  name[129];              /* Name of attribute -- e.g. "units" */
	char  type[129];              /* Type of attribute -- e.g. "String", "Float32", etc. */
	gaint nctype;                 /* NetCDF (or HDF) data type index value */
	gaint len;                    /* Length of this attribute */
	gaint fromddf;                /* Flag for attributes from descriptor file */
	void  *value;                 /* Attribute value -- strings may contains blanks. */
};

/* Structure that contains the x,y pairs for bufr time values */
struct bufrtimeinfo {
	gaint yrxy[2];
	gaint moxy[2];
	gaint dyxy[2]; 
	gaint hrxy[2]; 
	gaint mnxy[2];  
	gaint scxy[2];  
};

/* Structure that contains the x,y pairs for file-wide bufr variables */
struct bufrinfo {
	gaint lonxy[2];
	gaint latxy[2];
	gaint levxy[2];
	gaint stidxy[2];
	struct bufrtimeinfo base,offset;   /* structures for base and offset time values */
};

/* Structure that contains the header (coordinate) info for a gabufr_msg */
struct bufrhdr {
	double lon;
	double lat;
	double lev;
	double sec,offsec;
	char   stid[8];
	struct dt tvals,toffvals;
};

/* Structure that describes a report header in a stn file */
struct rpthdr {
	char id[8];                  /* Character station id           */
	gafloat lat;                 /* Latitude of report             */
	gafloat lon;                 /* Longitude of report            */
	gafloat t;                   /* Time in relative grid units    */
	gaint nlev;                  /* Number of levels following     */
	gaint flag;                  /* Level independent var set flag */
};

/* Structure that describes a stid info block within a stidmap file */
struct stninf {
	char stid[8];
	gaint offset;
	gaint rcnt;
};

/* Structure that describes a single report                          */
struct garpt {
	struct garpt *rpt;           /* Address of next report               */
	char stid[8];                /* Station id                           */
	gadouble lat,lon,lev,tim;    /* Location of station                  */
	gaint work;                  /* Work area                            */
	gadouble val;                /* Value of variable                    */
	char umask;                  /* Undef mask                           */
};

/* Structure that describes a collection of station reports.         */
struct gastn {
	struct garpt *rpt;           /* Address of start of link list        */
	gaint rnum;                  /* Number of reports.                   */
	struct garpt *blks[BLKNUM];  /* ptrs to memory holding rpts      */
	struct gafile *pfi;          /* Address of the associated gafile
								 structure to get the data from
								 (requestor block only)               */
	gadouble undef;              /* Undefined value for this data.       */
	gadouble smin, smax;         /* Min and Max values for this data     */
	gaint idim,jdim;             /* Varying dimensions for this data
								 -1 = This dimension does not vary
								 1 = X dimension (longitude)
								 2 = Y dimension (lattitude)
								 3 = Z dimension (pressure)
								 4 = Time                           */
	gadouble dmin[5],dmax[5];    /* Dimension limits for each dimension
								 (X,Y,Z) in world coords.
								 Non-varying dimensions can have
								 limits in this structure.           */
	gaint rflag;                 /* Get stations within specified radius in
								 degrees of fixed lat and lon         */
	gadouble radius;             /* Radius */
	gaint sflag;                 /* Get specific station  */
	char stid[8];                /* Station id to get */
	gaint tmin,tmax;             /* Grid limits of time */
	gadouble ftmin,ftmax;        /* Float-valued grid limits of time, 
								 equivalent to dmin[3],dmax[3]         */
	gadouble *tvals;             /* Pointer to conversion info for the
								 time conversion routines.            */
	struct gavar *pvar;          /* Pointer to the structure with info
								 on this particular variable.  If
								 NULL, this grid is the result of
								 an expression evaluation where the
								 variable type is unkown.             */
	struct garpt **prev;         /* Used for allocating rpt structures   */
	struct garpt *crpt;
	gaint rptcnt,blkcnt;
};

/* Structure that describes a variable in a file.  These structures
are built in arrays that are hung off of gafile structures.         */
struct gavar {
	char varnm[128];             /* Variable description.                */
	char abbrv[16];              /* Variable abbreviation.               */
	char longnm[257];            /* netcdf/hdf var name if different     */
	gadouble units[16];          /* Units indicator.                     
								 Vals 0-7 are for variable codes:
								 grib, non-float data, nc/hdf dims
								 Vals  8-11 are for grib level codes  */
	gaint offset;                /* Offset in grid elements of the start
								 of this variable within a time group
								 within this file.                    */
	gaint recoff;                /* Record (XY grid) offset of the start
								 of this variable within a time group */
	gaint ncvid;                 /* netcdf vid for this variable         */
	gaint sdvid;                 /* hdf vid for this variable            */
	gaint h5vid;                 /* hdf5 dataset id for this variable    */
	gaint levels;                /* Number of levels for this variable.
								 0 is special and indiates one grid is
								 available for the surface only.      */
	gaint dfrm;                  /* format  type indicator
								 1 - unsigned char
								 4 - int  			       */
	gaint var_t ;                /* variable t transform                 */
	gadouble scale;              /* scale factor for unpacking data      */
	gadouble add;                /* offset value for unpacking data      */
	gadouble undef;              /* undefined value                      */
	gaint vecpair;               /* Variable has a vector pair           */
	gaint isu;                   /* Variable is the u-component of a vector pair */
	gaint isdvar;                /* Variable is a valid data variable (for SDF files) */
	gaint nvardims;              /* Number of variable dimensions        */
	gaint vardimids[100];        /* Variable dimension IDs. 	       */
};

/* Structure that describes a function call.                           */
struct gafunc {
	gaint argnum;                /* Number of arguments found by fncprs  */
	char *argpnt[20];            /* Pointers to the argument strings     */
	char buff[1000];             /* Argument string buffer               */
};

/* Structure that describes a user defined function                    */
struct gaufb {
	struct gaufb *ufb;           /* Forward pointer                      */
	char name[8];                /* Function name                        */
	gaint alo,ahi;               /* Limits on number of args             */
	gaint atype[8];              /* Types of args. 0=expr,1=float,2=int,3=char */
	gaint sflg;                  /* Sequential or direct                 */
	char *fname;                 /* Name of user executable              */
	char *oname;                 /* File name for data transfer to user  */
	char *iname;                 /* File name for data transfer from user */
};

/* Structure that describes a defined grid */
struct gadefn {
	struct gadefn *pforw;        /* Linked list pointer                  */
	struct gafile *pfi;          /* File Structure containing the data   */
	char abbrv[20];              /* Abbreviation assigned to this        */
};

/* Stack to evaluate the expression.  The stack consists of an
array of structures.                                               */
struct smem {
	gaint type;        /* Entry type: -2 stn,-1 grid,1=op,2='(',3=')'    */
	union sobj {
		gaint op;        /* Operator: 0=*, 1=/, 2=+                        */
		struct gagrid *pgr; /* Operand (grid or stn)                      */
		struct gastn *stn;
	} obj;
};

/* Index structure, for when the records in a data file are indexed.
The indexing file will contain this structure at the front, followed
by the specified number of header and indexing values.  These
header and indexing values are file format specific. */

struct gaindx {
	gaint type;                   /* Indexing file type */
	gaint hinum;                  /* Number of header ints */	       
	gaint hfnum;                  /* Number of header floats */      
	gaint intnum;                 /* Number of index ints (long) */  
	gaint fltnum;                 /* Number of index floats */       
	gaint *hipnt;                 /* Pointer to header int values */ 
	gafloat *hfpnt;               /* Pointer to header float values */
	gaint *intpnt;                /* Pointer to index int values */  
	gafloat *fltpnt;              /* Pointer to index float values */
} ;
struct gaindxb {
	gaint bignum;                 /* Number of off_t values */	       
	off_t *bigpnt;                /* Pointer to off_t values */
} ;

gadouble qcachesf (void);
void gainit (void);
void gasig(gaint i);
gaint gaqsig (void);

#endif