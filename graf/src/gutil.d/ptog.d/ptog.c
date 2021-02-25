/* <: t-5 d :> */
#include <stdio.h>
#include "../../../include/util.h";
#include "../../../include/gsl.h";
#include "../../../include/gpl.h";
#include "../../../include/debug.h";
#include "../../../include/errpr.h";
#define DELTAXY    4095.
#define ROT   0
#define COL   0
#define WT   0
#define TSIZ   100
#define FONT   0
#define GETXY(fpi,x,y)  x=getw(fpi); y=getw(fpi)

char *nodename;

struct gslparm gsl ={stdout,0.,0.,0.,0.,1.,1.,0.,0.};
main(argc,argv)
	int argc;
	char *argv[]; 
{
	FILE *fpi;
	int ac,argf,i;
	char **av,*cp;
/*
 *  ptog.c converts unix____ plot files to ged___ format
 *  coords are scaled onto a -2048 to +2048 grid and centered 
 */

	nodename = *argv;
	for( ac=argc-1,av=argv+1; ac>0; ac--,av++ )
		if( *(cp=av[0])=='-' && *++cp!=EOS ) {
			for(; *cp!=EOS; --argc )
				switch(*cp++) {
				default: ERRPR1(ptog: %c?,*(cp-1));
				}
		}
	if(argc <= 1) { argc=2; argf=0; } /* no args, use stdin */
	else argf=1;

	while(argc-- > 1) {
		if( strcmp(*++argv,"-")==0 || !argf ) fpi=stdin;
		else if( **argv!='-' ) {
			if((fpi =fopen(*argv,"r")) == NULL) continue;
		} else { argc++; continue; }

		ptog(fpi,stdout);
		fclose(fpi);
	}
}
ptog(fpi,fpo)
FILE *fpi,*fpo; {
	int style=NARROW, x=0,y=0,x1,y1,x2,y2,xc,yc,r;
	char c,c1,string[TEXLEN],*ptr;
	gsl.fp=fpo;
	newparm(&gsl);
	gplinit(gsl.fp);
	while((c=getc(fpi)) != EOF) {
		if((c1 = getc(fpi)) == EOF) break;
		else ungetc(c1,fpi);
		switch(c) {
		case 'm':		/*  move  */
			GETXY(fpi,x,y);
			startlines((double)x,(double)y,ROT,COL,WT,style);
			for(c1=getc(fpi); c1=='n'; c1=getc(fpi)){
				GETXY(fpi,x,y); 	line((double)x,(double)y);
			}
			ungetc(c1,fpi);
			endlines();
			break;
		case 'l':		/*  line  */
			GETXY(fpi,x,y);  GETXY(fpi,x1,y1);
			startlines((double)x,(double)y,ROT,COL,WT,style);
			line((double)x1,(double)y1); endlines();
			break;
		case 't':		/*  text  */
			ptr=string;
			while(c == 't') {
				getstr(fpi,ptr);
				ptr += strlen(ptr);
				c = getc(fpi);
			}
			ungetc(c,fpi);
			text((double)x,(double)y,string,ROT,TSIZ,COL,FONT);
			break;
		case 'e':		/*  erase--null  */
			break;
		case 'p':		/*  point  */
			GETXY(fpi,x,y);
			text((double)x,(double)y,".",ROT,TSIZ,COL,FONT);
			break;
		case 's':		/*  setscale  */
			GETXY(fpi,x,y);  GETXY(fpi,x1,y1);
			gsl.xs=DELTAXY/(x1-x);
			gsl.ys=DELTAXY/(y1-y);
			break;
		case 'a':		/*  arc  */
			GETXY(fpi,xc,yc); GETXY(fpi,x1,y1); GETXY(fpi,x2,y2);
			arcpt ((double)xc,(double)yc,(double)x1,(double)y1
			,(double)x2,(double)y2,&x,&y);
			startarcs((double)x2,(double)y2,ROT,COL,WT,style);
			arc((double)x,(double)y);  arc((double)x1,(double)y1);  endarcs();
			break;
		case 'c':		/*  circle  */
			GETXY(fpi,x,y);
			r = getw(fpi);
			startarcs((double)x-r,(double)y,ROT,COL,WT,style);
			arc((double)x+r,(double)y);  arc((double)x-r,(double)y);  endarcs();
			break;
		case 'f':		/*  linestyle  */
			getstr(fpi,string);
			linemod(string,&style);
			break;
		}
	}
}
getstr(fpi,str)
	FILE *fpi; 
	char *str; {
	for( ; *str = getc(fpi); str++){
		if(*str == '\n')
			break;
	}
	*str = '\0';
	return;
}
