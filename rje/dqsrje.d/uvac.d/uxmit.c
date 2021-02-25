#define LSIR 1
#define LSIW 0
#define MIN(a,b)  a>b?b:a
#define STX 002
#define ETB 027
#define ETX 003

char ibuf[512];
char obuf[512];
char *p, *b, *ptr;
int colcnt, bcnt, cnt, minum;

main(argc,argv) char *argv[];
{
	int jj;

	if (open(argv[0],0) != 1) exit(4);
	p = obuf;

/* b is ptr to ibuf; bcnt is bytes left in ibuf;
   p is ptr to obuf; colcnt is column position in line */

	bcnt = 0; *p++ = STX;
	for(;;)

		/* while chars in buffer process them */

		if (--bcnt >= 0) {

			/* get line length and check bounds */

			if (((colcnt = (*b++ & 0377)) < 0) || (colcnt > 132))
				exit(bad());

			/* room? if not end block, flush block, continue */

		nmode:  if ((p + colcnt) >= (obuf + 497)) {
				while (p < obuf + 497)
					*p++ = '\0';
				*p++ = ETB;
				if (jj=_flush(0,p)) exit(jj);
				else p = obuf + 1;
			}
			/* process line - may have to replenish buffer
			   if line crosses buffers */

			while (colcnt > 0) {
				if ((bcnt <= 0) && (bcnt = rdunix()) < 0)
					exit(bad());
				minum = MIN(bcnt,colcnt);
				bcnt =- minum; colcnt =- minum;

				/* process chars */

				do {
					if ((*b & 0140) == 0)
						exit(bad());
					else *p++ = *b++;
				} while(--minum > 0);
			}

			/* put record  terminator and continue */

			*p++ = '\n';
		}
		else

			/* buffer empty */

			if ((bcnt = rdunix()) <= 0) {
				if (bcnt < 0)
					exit(bad());
				if (p == obuf || p == obuf + 1)
					exit(4);
				while (p < obuf + 497)
					*p++ = '\0';
				*p++ = ETX;
				if (jj = _flush(0,p)) exit(jj);
				else exit(_fin());
			}
	exit(1);
}

_fin()
{
	if (write(LSIW,0,0) < 0) return(3);
	else return(1);
}

bad()
{
	write(LSIW,0,0);
	return(5);
}

_flush(i,eptr) int i; char *eptr;
{ register int wcnt, j;
	wcnt = eptr - obuf;
	if (wcnt > 2) {
		j = write(LSIW,obuf,wcnt);
		if (j < 0) return(3);
		else if (j == 0) return(2);
	}
	return(0);
}

rdunix()
{
	b = ibuf;
	return(read(LSIR,ibuf,512));
}

