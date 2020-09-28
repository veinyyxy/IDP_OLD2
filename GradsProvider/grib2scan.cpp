#include "grib2scan.h"

/* global variables */
struct gag2 {
	gaint discipline,parcat,parnum;     /* Parameter identifiers */
	gaint yr,mo,dy,hr,mn,sc;            /* Reference Time */
	gaint sig;                          /* Significance of Reference Time */
	gaint numdp;                        /* Number of data points */
	gaint gdt;                          /* Grid Definition Template */
	gaint pdt;                          /* Product Definition Template */
	gaint drt;                          /* Data Representation Template */
	gaint trui;                         /* Time range units indicator */
	gaint ftime;                        /* Forecast time */
	gaint lev1type,lev1sf,lev1;         /* Level 1 type, scale factor, scaled value */
	gaint lev2type,lev2sf,lev2;         /* Level 2 type, scale factor, scaled value */
	gaint enstype,enspertnum,ensderiv;  /* Ensemble metadata */
	gaint comptype;                     /* Compression type (for JPEG2000 compression) */
	gaint bmsflg;                       /* Bit Map Section flag */
};
//gaint verb=0;

// void gaprnt (gaint i, char *ch) {
// 	printf ("%s",ch);
// }

char *gxgsym(char *ch) {
	return (getenv(ch));
}