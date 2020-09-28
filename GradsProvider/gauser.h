#ifndef GAUSER_H
#define GAUSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "grads.h"
#include "gx.h"
#include "galloc.h"
#include "config.h"
#include "gxsubs.h"
//#include "netcdf.h"


//////////////////////////////////////////////////////////////////////////
void gaprnt (gaint level, char *msg);
void set_nc_cache(size_t newsize);
gaint gacmd (char *com, struct gacmn *pcm, gaint exflg);
gaint gadef (char *cmd, struct gacmn *pcm, gaint impf);
void gacln (struct gacmn *pcm, gaint flg);
void mygreta(char *path);
gaint ccsopen(char **pname,struct gacmn *pcm);    // added by Liu Xinsheng  2011-05-05
gaint  gaopen (char *name, struct gacmn *pcm);
gaint gadspl (char *cmd, struct gacmn *pcm);
struct gastat *getpst (struct gacmn *pcm);
gaint gapars (char *cmd, struct gastat *pst, struct gacmn *pcm);
void gagrel (struct gacmn *pcm);
gaint gaset (char *cmd, char *com, struct gacmn *pcm);
gaint gaqury (char *, char *, struct gacmn *);
gaint gaqdef (char *, struct gacmn *, gaint);
gaint prntgaattr (struct gafile *, char *, gaint, gaint);
gaint gamodf (char *cmd, struct gacmn *pcm);
char *gagsdo (char *cmd, gaint *rc);
gaint gacoll (char *cmd, struct gacmn *pcm);
gaint gadraw (char *cmd, struct gacmn *pcm);

#endif