#include <stdio.h>
#include "../../include/dev.h"

sbn(i) 
int i;
{
/*
 *  sbn translates integer i into single byte format
 */
	i=i & 077;
	if(i>=32) putchar(i);  else putchar(i|0100);
}
