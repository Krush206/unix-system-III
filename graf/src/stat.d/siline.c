/* <:t-5 d:> */
#include "stdio.h"
#include "s.h"

struct options {
	double icept;	/* intercept */
	double slope;
	int col;	/* output columnation */
	int num;	/* auto generate count */
} op = { 0., 1., 5, 0 };

main(argc,argv) /* generate line given slope and intercept */
int argc;
char *argv[];
{
	FILE *fdi;
	int ac, argf, i;
	char **av, *cp;

	double x;
	char lab[MAXLABEL];

	nodename = *argv;
	for( ac=argc-1,av=argv+1; ac>0; ac--,av++ )
		if( *(cp=av[0])=='-' && *++cp!='\0' ) {
			while( *cp!='\0' )
				switch(*cp++) {
				case 'c': SETINT(op.col,cp); break;
				case 'i': SETDOUB(op.icept,cp); break;
				case 'n': SETINT(op.num,cp); break;
				case 's': SETDOUB(op.slope,cp); break;
				case ' ': case ',': break;
				default: ERRPR1(%c?,*(cp-1));
				}
			--argc;
		}


	if( argc<=1 ) argc=2, argf=0; /* no args, use stdin */
	else argf=1;

	while( --argc>0 ) {
		if( strcmp(*++argv,"-")==0 || !argf ) fdi=stdin;
		else if( **argv!='-' ) {
			if( (fdi=fopen(*argv,"r"))==NULL ) {
				ERRPR1(cannot open %s, *argv);
				continue;
			}
		} else { argc++; continue; }

		if( op.num > 0 )
			for( i=1; i<=op.num; i++ )
				PRINT(op.slope*(i-1)+op.icept,i,op.col);
		else {
			TITOUT1(fdi,lab,MAXLABEL,fitted);
			for( i=1; getdoub(fdi,&x)!=EOF; i++ )
				PRINT(op.slope*x+op.icept,i,op.col);
		}
		if( i%op.col != 1) putchar('\n');
		fclose(fdi);
	}
	exit(0);
}
