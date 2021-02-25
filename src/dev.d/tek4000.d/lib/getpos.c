/* <: t-5 d :> */
#include <stdio.h>
#include "../../../../include/util.h"
#include "tek.h"

getpos(tkptr)
struct tekaddr *tkptr;
{
/*
 *  getpos gets and sets values of 4 byte tek addresses
 */
	char s[5];

	if( read(0,s,5)==5 ) {
		tkptr->ch=s[0] & 0177;
		tkptr->xh=s[1] & 0177;
		tkptr->xl=s[2] & 0177;
		tkptr->yh=s[3] & 0177;
		tkptr->yl=s[4] & 0177;
		return(SUCCESS);
	} else return(FAIL);
}
