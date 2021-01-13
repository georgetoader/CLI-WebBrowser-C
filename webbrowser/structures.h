#include <stdlib.h>
#include <stdio.h>
#include "TStiva.h"
#include "TCoada.h"
#include "utils.h"
#include <string.h>
#include <stdarg.h>
#include <time.h>

#ifndef __STRUCTURES__
#define __STRUCTURES__

/* WEBPAGE */
typedef struct{
    char url[21];
    unsigned long num_res;
    Resource* resources;
} Webpage;

/* TAB */
typedef struct{
	Webpage* current_page;
    TStiva back_stack;
    TStiva forward_stack;
} Tab;

/* BROWSER */
typedef struct{
	Tab* current_tab;
	TStiva tabs;
	TCoada history;
	TCoada downloads;
	TCoada finished;
} Browser;

#endif
