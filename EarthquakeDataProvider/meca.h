/*	$Id: meca.h 9923 2012-12-18 20:45:53Z pwessel $
 *    Copyright (c) 1996-2013 by G. Patau
 *    Distributed under the GNU Public Licence
 *    See README file for copying and redistribution conditions.
 */

//#ifdef MECA_H
//#define MECA_H
#pragma once
#include <stdio.h>
#include <math.h>

#define EPSIL 0.0001

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef M_PI_4
#define M_PI_4          0.78539816339744830962
#endif
#ifndef M_PI_2
#define M_PI_2          1.57079632679489661923
#endif

#define squared(x) ((x) * (x))
#define  GMT_LONG long

struct AXIS {
    double str;
    double dip;
    double val;
    GMT_LONG e;
};
/* val in 10**e dynes-cm */
struct MOMENT
{
	double mant;
	GMT_LONG exponent;
};

struct nodal_plane
{
	double str;
	double dip;
	double rake;
};

struct MECHANISM
{
	struct nodal_plane NP1;
	struct nodal_plane NP2;
	struct MOMENT moment;
	double magms;
};

/*
struct MOMENT {
public:
	MOMENT(double m,long ex){mant=m;exponent=ex;}
	MOMENT(){};
public:
double mant;
GMT_LONG exponent;
};

class nodal_plane {
public:
	nodal_plane(double s,double d,double r){str = s;dip = d;rake = r;}
	nodal_plane(){};
public:
    double str;
    double dip;
    double rake;
}; 

class MECHANISM {
public:
	MECHANISM(nodal_plane np1,nodal_plane np2,MOMENT m,double mag)
	{
	  NP1 = np1;
	  NP2 = np2;
	  moment = m;
	  magms = mag;
	}
	MECHANISM() {};
public:

    struct nodal_plane NP1;
    struct nodal_plane NP2;
    struct MOMENT moment;
    double magms;
};
*/

/*
class FOCALSLOLUTION
{
public:
	FOCALSLOLUTION(float str1,float dip1,float rake1,float str2,float dip2,float rake2,float MW);
protected:
private:
};
*/

struct M_TENSOR {
    GMT_LONG expo;
    double f[6];
};
/* mrr mtt mff mrt mrf mtf in 10**expo dynes-cm */

typedef struct MOMENT st_mo;
typedef struct MECHANISM st_me;

double datan2(double y,double x);
//double zero_360(double str);
void dc_to_axe(st_me meca,struct AXIS *T,struct AXIS *N,struct AXIS *P);

//#endif