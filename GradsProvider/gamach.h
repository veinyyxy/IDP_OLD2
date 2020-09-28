#ifndef GAMACH_H
#define GAMACH_H

#include <stdio.h>
#include <math.h>
#include "gatypes.h"
#include "gaddes.h"
#include "config.h"

gaint gagby (unsigned char *ch, gaint ioff, gaint ilen);
gaint gagbb (unsigned char *ch, gaint ioff, gaint ilen);
void gapby (gaint ival, unsigned char *ch, gaint ioff, gaint ilen);
void gapbb (gaint ival, unsigned char *ch, gaint ioff, gaint ilen);

#endif