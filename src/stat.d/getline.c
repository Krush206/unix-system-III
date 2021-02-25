/* <: t-5 d :> */
#include "stdio.h"
#define ESCAPE(c,x) ( (c=x)!='\\' ? c : (x,c=x))

char *getline(fp,bufs,size) /* put next line from fp into buf */
FILE *fp;
char *bufs;
int size;
{
	char c, *bufe=bufs+size;

	while( ESCAPE(c,getc(fp))!=EOF )
		if( c!='\n' ) {
			if( bufs<bufe-1 ) *bufs++ = c;
		} else {
			*bufs = '\0';
			return(bufs);
		}
	return((char *) EOF);
}
