#include <stdio.h>
#include "dev.h"
#include "../../include/dev.h"
devcursor(x,y)
int x,y;
{
/*
 *  devcursor moves pen to device address given by x,y
 */
	MOVEPEN; mbp(x,y); TERMINATOR;
}
