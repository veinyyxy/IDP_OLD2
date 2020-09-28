#ifndef GALLOC_H
#define GALLOC_H

#include <stdio.h>
#include "grads.h"
#include "config.h"

void *galloc (size_t, char *);
void gree (char *, char *);
void glook(void);
void gsee (char *);
gaint gappcn (struct gafile *pfi, gaint pdefop1, gaint pdefop2);

#endif