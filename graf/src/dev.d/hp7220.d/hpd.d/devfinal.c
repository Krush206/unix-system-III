#include <stdio.h>
#include "dev.h"
#include "../../include/dev.h"
devfinal()
{
/*
 *  devfinal restores pen and sets plotoff condition
 */
	PENSEL; sbn(0);
	PLOTOFF;
}
