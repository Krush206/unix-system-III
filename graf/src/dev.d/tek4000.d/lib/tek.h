#include "../../../../include/gsl.h"
#include "../../../../include/gpl.h"
#include "../../../../include/debug.h"
#define UNSET      -1
#define BUFSIZ	512
#define BOLDWIDTH 2.5

struct tekaddr {
	int yh,yl,xh,xl;
	int ch;
};

#include "../include/gedstructs.h"
