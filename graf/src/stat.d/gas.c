/* <:t-5 d:> */
#include "stdio.h"
#include "s.h"
#define DFLTTO POSINFIN
#define DFLTFROM 1
#define DFLTNUM 10
#define DFLTINTV 1

struct options {
	int col;	/* columns per output line */
	int num;	/* # of numbers to be generated */
	double intv;	/* interval between numbers */
	double high;	/* termination value */
	double low;	/* start value */
} op = { DFLTCOL,0,UNDEF,DFLTTO,DFLTFROM };

main(argc,argv) /*generate arithmetic series*/
int argc;
char *argv[];
{
	int ac, argf, i;
	char **av, *cp;
	double x, fmod();
	int  num=0, setnum=0;

	nodename = *argv;
	for( ac=argc-1,av=argv+1; ac>0; ac--,av++ )
		if( *(cp=av[0])=='-' && *++cp!='\0' ) {
			while( *cp!='\0' )
				switch(*cp++) {
				case 'c': SETINT(op.col,cp); break;
				case 'i': SETDOUB(op.intv,cp); break;
				case 't': SETDOUB(op.high,cp); setnum=1; break;
				case 's': SETDOUB(op.low,cp); break;
				case 'n': SETINT(op.num,cp); break;
				case ' ': case ',': break;
				default: ERRPR1(%c?,*(cp-1));
				}
			--argc;

		}

	if( !DEF(op.intv) ) op.intv = op.low<op.high ? DFLTINTV : -DFLTINTV;
	if(!op.num)
		if( setnum && op.intv!=0 ) op.num = abs((int)((op.high-op.low)/op.intv)) + 1;
		else op.num = DFLTNUM;

	for(i=0; i<op.num;) {
		x = op.low + fmod(i*op.intv,op.high-op.low+op.intv);
		PRINT(x,++i,op.col);
	}
	if(i%op.col != 0) putchar('\n');
	exit(0);
}
