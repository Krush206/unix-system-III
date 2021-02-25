/* <: t-5 d :> */
#include <stdio.h>
#include "../../../include/gsl.h"
#include "../../../include/gpl.h"
#include "../../../include/util.h"
#include "../../../include/debug.h"

xymaxmin(fpi,fpo,arptr)
FILE *fpi,*fpo;
struct area *arptr;
{	int cnt,*lptr,sz=0,init=TRUE;
	struct command cd;
/*
 *  xymaxmin reads GPS from fpi and copies to fpo and
 *  sets max and min coords into struct pointed to by arptr
 *  & returns max text size
 */
	while(getgedf(fpi,&cd) != EOF){
		if(init && cd.cmd != COMMENT){
			arptr->lx = arptr->hx = *cd.aptr;
			arptr->ly = arptr->hy = *(cd.aptr+1);
			init=FALSE;
		}
		switch(cd.cmd){
			case LINES:
			case ARCS:   
			case ALPHA:
				for(lptr=cd.aptr,cnt=cd.cnt-2;cnt>0;cnt-=2,lptr+=2){
					arptr->lx = MIN(arptr->lx,*lptr); 
	   				arptr->ly = MIN(arptr->ly,*(lptr+1)); 
	   				arptr->hx = MAX(arptr->hx,*lptr); 
	   				arptr->hy = MAX(arptr->hy,*(lptr+1)) ;
				}
				break;
			case TEXT:  textmaxmin(&cd,arptr); 
					sz=MAX(sz,cd.tsiz);
					break;
		}
		if(fpo != NULL) putgedf(fpo,&cd);
	}
	return(sz);
}
#define EXPAND(x1) ((x1>0)? x1+1 : x1 -1)

textmaxmin(cdptr,arptr)
struct command *cdptr;
struct area *arptr;
{	double x1,y1,x2,y2;
	double xhch,yhch,xvch,yvch,floor();
/*
 *  textmaxmin computes max and min for the text string
 *  in cdptr and compares with hi-lo values in arptr
 */
	gslhchar(&xhch,&yhch,cdptr->trot,.8*cdptr->tsiz);
	gslvchar(&xvch,&yvch,cdptr->trot,1.2*cdptr->tsiz);
	x1 = *cdptr->aptr; y1 = *(cdptr->aptr+1);
	x1 += xvch; y1 += yvch;  /*  decend full line to allow for descending chars  */
	x1 -= xhch/2.; y1 -= yhch/2.;  /*  to low left corner  */

	x2 = x1 + (gslslen(cdptr->tptr)) * xhch;
	y2 = y1 + (gslslen(cdptr->tptr)) * yhch;

	test(arptr,x1,y1);  test(arptr,x2,y2);
	x1 -= xvch*1.5; y1 -= yvch*1.5;
	x2 -= xvch*1.5; y2 -= yvch*1.5;
	test(arptr,x1,y1); test(arptr,x2,y2);
}
static
test(arptr,x,y)
struct area *arptr;
double x,y;
{
	x=EXPAND(x); y=EXPAND(y);
	arptr->lx = MIN(arptr->lx,(int)gslclipu((long)x));
	arptr->ly = MIN(arptr->ly,(int)gslclipu((long)y));
	arptr->hx = MAX(arptr->hx,(int)gslclipu((long)x));
	arptr->hy = MAX(arptr->hy,(int)gslclipu((long)y));
}
