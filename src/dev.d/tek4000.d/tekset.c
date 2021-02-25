#include	"stdio.h"
/*
	This program will reset the screen of a tektronix 4014-1.
*/

main()
{
	fprintf(stderr,"\033\014\033\073");
	exit(0);
}
