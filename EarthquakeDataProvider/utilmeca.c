/*	$Id: utilmeca.c 9923 2012-12-18 20:45:53Z pwessel $
 *    Copyright (c) 1996-2013 by G. Patau
 *    Distributed under the GNU Public Licence
 *    See README file for copying and redistribution conditions.
 */

//#include "gmt.h"	/* to have gmt environment */
//#include "pslib.h"	/* to have pslib environment */
#include "meca.h"
#include "utilmeca.h"
//#include "nrutil.h"

void sincos (double a, double *s, double *c)
{
	*s = sin (a);	*c = cos (a);
}

void sincosd(double x, double *s, double *c)
{
	sincos((x) * D2R,s,c);
}

double zero_360(double str)
{
	if (str >= 360.)
		str -= 360.;
	else if (str < 0.)
		str += 360.;
	return(str);
}


/*
   compute null axis dip when strike and dip are given
   for each nodal plane.
   Angles are in degrees.
*/
/* Genevieve Patau */

double null_axis_dip(double str1,double dip1,double str2,double dip2)
{
	double den;

	den = asind(sind(dip1) * sind(dip2) * sind(str1 - str2));
	if (den < 0.)
		den = -den;
	return(den);
}

/*
   Compute null axis strike when strike and dip are given
   for each nodal plane.
   Angles are in degrees.
*/

/* Genevieve Patau */
/*********************************************************************/
double null_axis_strike(double str1,double dip1,double str2,double dip2)
{
	double phn, cosphn, sinphn;
	double sd1, cd1, sd2, cd2, ss1, cs1, ss2, cs2;

	sincosd (dip1, &sd1, &cd1);
	sincosd (dip2, &sd2, &cd2);
	sincosd (str1, &ss1, &cs1);
	sincosd (str2, &ss2, &cs2);

	cosphn = sd1 * cs1 * cd2 - sd2 * cs2 * cd1;
	sinphn = sd1 * ss1 * cd2 - sd2 * ss2 * cd1;
	if (sind(str1 - str2) < 0.) {
		cosphn = -cosphn;
		sinphn = -sinphn;
	}
	phn = d_atan2d(sinphn, cosphn);
	if (phn < 0.) phn += 360.;
	return(phn);
}

/*
   Compute the vector radius for a given strike,
   equal area projection, inferior sphere.
   Strike and dip of the plane are given.
*/

/* Genevieve Patau */
/*********************************************************************/
double proj_radius(double str1,double dip1,double str)
{
	double dip, r;

	if (fabs(dip1 - 90.) < EPSIL) {
/*
		printf("\nVertical plane : strike is constant.");
		printf("\nFor ps_mechanism r == 1 for str = str1");
		printf("\n            else r == 0. is used.");
*/
		r = (fabs(str - str1) < EPSIL || fabs(str - str1 - 180) < EPSIL) ? 1. : 0.;
	}
	else {
		dip = atan(tand(dip1) * sind(str - str1));
		r = sqrt(2.) * sin(M_PI_4 - dip / 2.);
	}
	return(r);
}

double  ps_mechanism(double x0, double y0, st_me meca, double size, int rgb[3], int ergb[3], GMT_LONG outline)
{

	double x[1000], y[1000];
	double pos_NP1_NP2 = sind(meca.NP1.str - meca.NP2.str);
	double fault = (GMT_IS_ZERO (meca.NP1.rake) ? meca.NP2.rake / fabs(meca.NP2.rake) : meca.NP1.rake / fabs(meca.NP1.rake));
	double radius_size;
	double str, radius, increment;
	double si, co;

	GMT_LONG lineout = 1, i;
	GMT_LONG npoints;

	struct AXIS N_axis;

/* compute null axis strike and dip */
	N_axis.dip = null_axis_dip(meca.NP1.str, meca.NP1.dip, meca.NP2.str, meca.NP2.dip);
	if (fabs(90. - N_axis.dip) < EPSIL)
		N_axis.str = meca.NP1.str;
	else
		N_axis.str = null_axis_strike(meca.NP1.str, meca.NP1.dip, meca.NP2.str, meca.NP2.dip);

/* compute radius size of the bubble */
	radius_size = size * 0.5;

/*  argument is DIAMETER!!*/
//	ps_circle(x0, y0, radius_size*2., ergb, lineout);

	if (fabs(pos_NP1_NP2) < EPSIL) {
/* pure normal or inverse fault (null axis strike is determined
   with + or - 180 degrees. */
		/* first nodal plane part */
		i = -1;
		increment = 1.;
		str = meca.NP1.str;
		while (str <= meca.NP1.str + 180. + EPSIL) {
			i++;
			radius = proj_radius(meca.NP1.str, meca.NP1.dip, str) * radius_size;
			sincosd (str, &si, &co);
			x[i] = x0 + radius * si;
			y[i] = y0 + radius * co;
			str += increment;
		}
		if (fault < 0.) {
			/* normal fault, close first compressing part */
			str = meca.NP1.str + 180.;
			while (str >= meca.NP1.str - EPSIL) {
				i++;
				sincosd (str, &si, &co);
				x[i] = x0 + si * radius_size;
				y[i] = y0 + co * radius_size;
				str -= increment;
			}
			npoints = i + 1;
		//	ps_polygon (x, y, npoints, rgb, outline);
			i = -1;
		}
		/* second nodal plane part */
		str = meca.NP2.str;
		while (str <= meca.NP2.str + 180. + EPSIL) {
			i++;
			radius = proj_radius(meca.NP2.str, meca.NP2.dip, str) * radius_size;
			sincosd (str, &si, &co);
			x[i] = x0 + radius * si;
			y[i] = y0 + radius * co;
			str += increment;
		}
		if (fault > 0.) {
			/* inverse fault, close compressing part */
			npoints = i+1;
	//		ps_polygon(x, y, npoints, rgb, outline);
		}
		else {
			/* normal fault, close second compressing part */
			str = meca.NP2.str + 180.;
			while (str >= meca.NP2.str - EPSIL) {
				i++;
				sincosd (str, &si, &co);
				x[i] = x0 + si * radius_size;
				y[i] = y0 + co * radius_size;
				str -= increment;
			}
			npoints = i + 1;
	//		ps_polygon(x, y, npoints, rgb, outline);
		}
	}
/* pure strike-slip */
	else if ((90. - meca.NP1.dip) < EPSIL && (90. - meca.NP2.dip) < EPSIL) {
		increment = fabs(meca.NP1.rake) < EPSIL ? 1. : -1.;
		/* first compressing part */
		i = 0;
		str = meca.NP1.str;
		while (increment > 0. ? str <= meca.NP1.str + 90. : str >= meca.NP1.str - 90.) {
			sincosd (str, &si, &co);
			x[i] = x0 + si * radius_size;
			y[i] = y0 + co * radius_size;
			str += increment;
			i++;
		}
		x[i] = x0;
		y[i] = y0;
		npoints = i + 1;
	//	ps_polygon(x, y, npoints, rgb, outline);
		/* second compressing part */
		i = 0;
		str = meca.NP1.str + 180.;
		while (increment > 0. ?  str <= meca.NP1.str + 270. + EPSIL : str >= meca.NP1.str + 90. - EPSIL) {
			sincosd (str, &si, &co);
			x[i] = x0 + si * radius_size;
			y[i] = y0 + co * radius_size;
			str += increment;
			i++;
		}
		x[i] = x0;
		y[i] = y0;
		npoints = i + 1;
	//	ps_polygon(x, y, npoints, rgb, outline);
	}
	else {
/* other cases */
		/* first nodal plane till null axis */
		i = -1;
		increment = 1.;
		if (meca.NP1.str > N_axis.str)
			meca.NP1.str -= 360.;
		str = meca.NP1.str;
		while (fabs(90. - meca.NP1.dip) < EPSIL ? str <= meca.NP1.str + EPSIL : str <= N_axis.str + EPSIL) {
			i++;
			radius = proj_radius(meca.NP1.str, meca.NP1.dip, str) * radius_size;
			sincosd (str, &si, &co);
			x[i] = x0 + radius * si;
			y[i] = y0 + radius * co;
			str += increment;
		}

		/* second nodal plane from null axis */
		meca.NP2.str += (1. + fault) * 90.;
		if (meca.NP2.str >= 360.) meca.NP2.str -= 360.;
		increment = fault;
		if (fault * (meca.NP2.str - N_axis.str) < -EPSIL) meca.NP2.str += fault * 360.;
		str = fabs(90. - meca.NP2.dip) < EPSIL ? meca.NP2.str : N_axis.str;
		while (increment > 0. ? str <= meca.NP2.str + EPSIL : str >= meca.NP2.str - EPSIL) {
			i++;
			radius = proj_radius(meca.NP2.str - (1. + fault) * 90., meca.NP2.dip, str) * radius_size;
			sincosd (str, &si, &co);
			x[i] = x0 + radius * si;
			y[i] = y0 + radius * co;
			str += increment;
		}

		/* close the first compressing part */
		meca.NP1.str = zero_360(meca.NP1.str);
		meca.NP2.str = zero_360(meca.NP2.str);
		increment = pos_NP1_NP2 >= 0. ? -fault : fault;
		if (increment * (meca.NP1.str - meca.NP2.str) < - EPSIL) meca.NP1.str += increment * 360.;
		str = meca.NP2.str;
		while (increment > 0. ? str <= meca.NP1.str + EPSIL : str >= meca.NP1.str - EPSIL) {
			i++;
			sincosd (str, &si, &co);
			x[i] = x0 + si * radius_size;
			y[i] = y0 + co * radius_size;
			str += increment;
		}

		npoints = i + 1;
	//	ps_polygon(x, y, npoints, rgb, outline);

		/* first nodal plane till null axis */
		i = -1;
		meca.NP1.str = zero_360(meca.NP1.str + 180.);
		if (meca.NP1.str - N_axis.str < - EPSIL) meca.NP1.str += 360.;
		increment = -1.;
		str = meca.NP1.str;
		while (fabs(90. - meca.NP1.dip) < EPSIL ? str >= meca.NP1.str -EPSIL : str >= N_axis.str - EPSIL) {
			i++;
			radius = proj_radius(meca.NP1.str - 180., meca.NP1.dip, str) * radius_size;
			sincosd (str, &si, &co);
			x[i] = x0 + radius * si;
			y[i] = y0 + radius * co;
			str += increment;
		}

		/* second nodal plane from null axis */
		meca.NP2.str = zero_360(meca.NP2.str + 180.);
		increment = -fault;
		if (fault * (N_axis.str - meca.NP2.str) < - EPSIL) meca.NP2.str -= fault * 360.;
		str = fabs(90. - meca.NP2.dip) < EPSIL ? meca.NP2.str : N_axis.str;
		while (increment > 0. ? str <= meca.NP2.str + EPSIL : str >= meca.NP2.str - EPSIL) {
			i++;
			radius = proj_radius(meca.NP2.str - (1. - fault) * 90., meca.NP2.dip, str) * radius_size;
			sincosd (str, &si, &co);
			x[i] = x0 + radius * si;
			y[i] = y0 + radius * co;
			str += increment;
		}

		/* close the second compressing part */
		meca.NP1.str = zero_360(meca.NP1.str);
		meca.NP2.str = zero_360(meca.NP2.str);
		increment = pos_NP1_NP2 >= 0. ? -fault : fault;
		if (increment * (meca.NP1.str - meca.NP2.str) < - EPSIL) meca.NP1.str += increment * 360.;
		str = meca.NP2.str;
		while (increment > 0. ? str <= meca.NP1.str + EPSIL : str >= meca.NP1.str - EPSIL) {
			i++;
			sincosd (str, &si, &co);
			x[i] = x0 + si * radius_size;
			y[i] = y0 + co * radius_size;
			str += increment;
		}

		npoints = i + 1;
	//	ps_polygon(x, y, npoints, rgb, outline);
	}
	return(radius_size*2.);
}

/*********************************************************************/
double ps_meca(double x0,double y0,st_me meca,double size)
{
	long i;
	long npoints;

//	double proj_radius();

	double x[1000], y[1000];
	double radius_size;
	double str, radius, increment;
	double si, co;

	int no_fill[3];
	long lineout = 1;

	no_fill[0] = -1;
	no_fill[1] = -1;
	no_fill[2] = -1;


/* compute radius size of the bubble */
	radius_size = size * 0.5;

/*  argument is DIAMETER!!*/
//	ps_circle(x0, y0, radius_size*2., no_fill, lineout);

		i = -1;
		increment = 1.;
		str = meca.NP1.str;
		while (str <= meca.NP1.str + 180. + EPSIL) {
			i++;
			radius = proj_radius(meca.NP1.str, meca.NP1.dip, str) * radius_size;
			sincosd (str, &si, &co);
			x[i] = x0 + radius * si;
			y[i] = y0 + radius * co;
			str += increment;
		}
		npoints = i + 1;
	//	ps_line(x, y, npoints, 3, FALSE);

		i = -1;
		increment = 1.;
		str = meca.NP2.str;
		while (str <= meca.NP2.str + 180. + EPSIL) {
			i++;
			radius = proj_radius(meca.NP2.str, meca.NP2.dip, str) * radius_size;
			sincosd (str, &si, &co);
			x[i] = x0 + radius * si;
			y[i] = y0 + radius * co;
			str += increment;
		}
		npoints = i + 1;
	//	ps_line(x, y, npoints, 3, FALSE);
		return(radius_size*2.);
}

/*********************************************************************/

