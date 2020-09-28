/*  Copyright (C) 1988-2010 by Brian Doty and the 
    Institute of Global Environment and Society (IGES).  
    See file COPYRIGHT for more information.   */

/* Authored by B. Doty */

/* 
 * Include ./configure's header file
 */

#ifndef GAEXPR_H
#define GAEXPR_H

#include "config.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "grads.h"
#include "gafunc.h"

gaint gaexpr (char *expr, struct gastat *pst);
gaint eval (gaint state, struct smem *stack, gaint *cpos);
gaint gaoper (struct smem *stack, gaint c1, gaint c2, gaint c, gaint op);
struct gagrid *gagrop (struct gagrid *pgr1, struct gagrid *pgr2,
	gaint op, gaint rel);
struct gastn *gastop (struct gastn *stn1, struct gastn *stn2,
	gaint op, gaint rel);
struct gastn *gascop (struct gastn *stn, gadouble val, gaint op, gaint swap);
struct gagrid *gagrvl (gadouble val);
char *varprs (char *ch, struct gastat *pst);
gaint gagchk (struct gagrid *pgr1, struct gagrid *pgr2, gaint dim);
struct gafile *getdfn (char *name, struct gastat *pst);
struct gafile *getdfn (char *name, struct gastat *pst);
char *stnvar (char *ch, char *vnam, struct gafile *pfi,
struct gavar *pvar, struct gastat *pst);

#endif