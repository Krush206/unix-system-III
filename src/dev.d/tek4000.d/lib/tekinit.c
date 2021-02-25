/* <: t-5 d :> */
#include <stdio.h>
#include "tek.h"
#include "../include/termodes.h"
#include "../../../../include/errpr.h"
struct tekterm tm={UNSET,UNSET,ALPHA};

tekinit()
{
	int i;
	extern char *nodename;

	ALPHAMODE;
	for( i=0; i<=1 && gtty(++i,&tm.cook); ); /* make sure both gttys are on same fd */
	if( gtty(i,&tm.raw) ) ERRPR0(no terminal);
	tm.raw.sg_flags = 0340;
	getacur(&tm.alphax,&tm.alphay);
	tm.curls = UNSET;  tm.curlw = UNSET; tm.curmode = ALPHA;
	putchar('\n');
}
