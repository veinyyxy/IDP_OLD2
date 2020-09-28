#ifndef GATXT_H
#define GATXT_H

#include <stdio.h>
#include "gatypes.h"
#include "config.h"

void gatxt(char *color);
char *gatxts(char *str, char *color);
char *gatxtl(char *str, gaint level);

#endif