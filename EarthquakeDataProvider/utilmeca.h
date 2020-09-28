/*	$Id: utilmeca.h 9923 2012-12-18 20:45:53Z pwessel $
 *    Copyright (c) 1996-2013 by G. Patau
 *    Distributed under the GNU Public Licence
 *    See README file for copying and redistribution conditions.
 */
#pragma  once
#define GMT_CONV_LIMIT	1.0e-8	/* Fairly tight convergence limit or "close to zero" limit */
#define GMT_SMALL	1.0e-4	/* Needed when results aren't exactly zero but close */
#define GMT_MIN_MEMINC	2048		/* E.g., 16 kb of 8-byte doubles */
#define GMT_MAX_MEMINC	67108864

#define M_PI 3.14159265
#define GMT_IS_ZERO(x) (fabs (x) < GMT_CONV_LIMIT)
#define D2R (M_PI/180.0)
#define R2D (180.0 / M_PI)
//#define sincos(x,s,c) alpha_sincos (x, s, c)
#define sind(x) sin((x) * D2R)
#define cosd(x) cos((x) * D2R)
#define tand(x) tan((x) * D2R)
#define asind(x) (asin(x) * R2D)
#define acosd(x) (acos(x) * R2D)
#define atand(x) (atan(x) * R2D)
#define atan2d(y,x) (atan2(y,x) * R2D)
#define copysign(x,y) ((y) < 0.0 ? -fabs(x) : fabs(x))
/* Safe versions of the degree-based trig */

#define d_acosd(x) (fabs(x) >= 1.0 ? ((x) < 0.0 ? 180.0 : 0.0) : acosd(x))
#define d_asind(x) (fabs(x) >= 1.0 ? copysign (90.0, (x)) : asind(x))
#define d_atan2d(y,x) ((x) == 0.0 && (y) == 0.0 ? 0.0 : atan2d(y,x))
#define min(x,y) ( x<y ? x:y)
#define max(x,y) ( x>y ? x:y)
typedef long GMT_LONG;


void sincos (double a, double *s, double *c);
void sincosd(double x, double *s, double *c);
double zero_360(double str);
double null_axis_dip(double str1,double dip1,double str2,double dip2);
double null_axis_strike(double str1,double dip1,double str2,double dip2);
double proj_radius(double str1,double dip1,double str);
double ps_meca(double x0,double y0,st_me meca,double size);
double  ps_mechanism(double x0, double y0, st_me meca, double size, int rgb[3], int ergb[3], GMT_LONG outline);
//#define sincosd(x,s,c) sincos((x) * D2R,s,c)

