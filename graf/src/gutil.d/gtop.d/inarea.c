/* <: t-5 d :> */
#include <stdio.h>
#include "../../../include/gsl.h"
#include "../../../include/util.h"

inarea(x,y,arptr)
int x,y;
struct area *arptr;
{
/*
 *  inarea returns SUCCESS if x and y are within
 *  the area defined by arptr
 */
	if(x >=arptr->lx && x <= arptr->hx && 
	 y >= arptr->ly && y <= arptr->hy) return(SUCCESS);
	else return(FAIL);
}
