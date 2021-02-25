#define LSIR 0
#define LSIW 1
#define DS 0133
#define SL 0141
#define A 0301
#define S 0342
#define T 0343
#define ESC 047
#define DLE 020
#define STX 002
#define ETB 046
#define ETX 003



char trtab[256] {	/*	Conversion tables for EBCDIC to 8-bit ASCII	*/
	  1 , 0  ,  3 ,  3 ,0234, 0  ,0206, 0  ,0227,0215,0216, 0  , 0  , 0  , 0  , 0  ,
	  3 , 0  , 0  , 0  ,0235,0205, 0  ,0207, 0  , 0  ,0222,0217, 0  ,  2 ,  3 , 0  ,
	0200,0201,0202,0203,0204, 0  ,  3 , 0  ,0210,0211,0212,0213,0214, 0  , 0  , 0  ,
	0220,0221, 0  ,0223,0224,0225,0226, 0  ,0230,0231,0232,0233, 0  , 0  ,0236, 0  ,
	  1 ,0240,0241,0242,0243,0244,0245,0246,0247,0250,0325,'.' ,'<' ,'(' ,'+' ,'|' ,
	'&' ,0251,0252,0253,0254,0255,0256,0257,0260,0261,'!' ,'$' ,'*' ,')' ,';' ,'~' ,
	'-' ,'/' ,0262,0263,0264,0265,0266,0267,0270,0271,0313,',' ,'%' ,'_' ,'>' ,'?' ,
	0272,0273,0274,0275,0276,0277,0300,0301,0302,'`' ,':' ,'#' ,'@' ,'\'' ,'=' ,'"' ,
	0303,'a' ,'b' ,'c' ,'d' ,'e' ,'f' ,'g' ,'h' ,'i' ,0304,0305,0306,0307,0310,0311,
	0312,'j' ,'k' ,'l' ,'m' ,'n' ,'o' ,'p' ,'q' ,'r' , 0  ,0314,0315,0316,0317,0320,
	0321,0345,'s' ,'t' ,'u' ,'v' ,'w' ,'x' ,'y' ,'z' ,0322,0323,0324,'[' ,0326,0327,
	0330,0331,0332,0333,0334,0335,0336,0337,0340,0341,0342,0343,0344,']' ,0346,0347,
	'{' ,'A' ,'B' ,'C' ,'D' ,'E' ,'F' ,'G' ,'H' ,'I' ,0350,0351,0352,0353,0354,0355,
	'}' ,'J' ,'K' ,'L' ,'M' ,'N' ,'O' ,'P' ,'Q' ,'R' ,0356,0357,0360,0361,0362,0363,
	'\\' ,0237,'S' ,'T' ,'U' ,'V' ,'W' ,'X' ,'Y' ,'Z' ,0364,0365,0366,0367,0370,0371,
	'0' ,'1' ,'2' ,'3' ,'4' ,'5' ,'6' ,'7' ,'8' ,'9' ,0372,0373,0374,0375,0376,0377,
	};

char jlog[]	"JES2JOBLOG";		/*jes2 header*/
char alog[]	"ISV40JOBORIGIN";	/*asp header*/

char ibuf[512];
char obuf[512];

int bcnt, ff, spx, pcnt, ptop, cc, blnk, tstop;


int xpr, bgf, wcnt, ret;
char *savp, *p, *b;
char *argp;

long	max,blkcnt,atol();
char bgerr[] "\n\n** output file too large. **\n\n";
char wrerr[] "\n\n\n** Write error on output file. **\n\n";
int	reset[3];

main(argc,argv)
int argc;
char *argv[];
{
	register char c;
	register int j, first;

	argp = argv[0];
	if (open(argp,1) != 1) exit(4);
	p = obuf;
	bcnt = ff = xpr = pcnt = bgf = blkcnt = 0;
	spx = 0;
	if(*argv[2] == 'y')
		spx++;
	max = atol(argv[1]);
	ptop = 1;
	cc = 1;
	ret = 0;
	setjmp(reset);
	if (ret) exit(ret);
	c = getbt();
	for(;;) {

		/* control character? */

		if ((c & 0300) == 0) {
			switch(c) {
			case ESC:	/* escape */
				switch ((c = getbt()) & 0377){
				case A:		/* skip */
					cc = 0;
					ff = 1;
					break;
				case S:		/* double */
					cc = 2;
					break;
				case T:		/* triple */
					cc = 3;
				}
				c = getbt();
				break;
			case DLE:		/* transparent? */
				if ((c = getbt()) == STX)
					xmode();
				continue;
			default:
				cc = 1;
				c = getbt();
				continue;
			}
		}

		/* save start of line in buffer for jes2/asp check */

		savp = p;

		/* check for hasp header */

		if (!spx && (ptop > 0) && (c == DS || c == SL)) {
			spx = 1;
			if (pcnt <= 5) {
				/* truncate file here */
				close(LSIW);
				creat(argp,644);
				blkcnt = 0;
				p = obuf;
			}
		}
		blnk = tstop = 0;
		first = 0;

		/* process characters */

		for(;;) {
			if (first)
				if (--bcnt < 0) c = getbt();
				else c = *b++;
			else
				first = 1;

			/* translate to ascii */

			if ((j = trtab[c & 0377]) > 0 && j < 4) {
				if( decode(-j) < 0)  break;
				continue;
			}
			else if (j == 0) c = '^';
			else c = j & 0377;
			for(;;) {
				if (obuf + 512 <= p) rflush();
				if (blnk == 0) {
					*p++ = c;
					tstop--;
					break;
				}
				tstop =& 7;
				if (tstop <= 0) tstop = 8;
				if ((blnk < tstop) || (tstop == 1)) {
					blnk--;
					*p++ = ' ';
					tstop--;
					continue;
				}
				blnk =- tstop;
				*p++ = '\t';
				tstop = 0;
			}
		}
	cc = 1;
	c = getbt();
	}
}
decode(l) int l;
{
	register char c;

/* c = -3 iff end of line
   c = -2 iff compression
   c = -1 iff blank
*/

	c = l;
	if (c > -2) {
		blnk++;
		return(0);
	}
	else if (c == -2) {
		c = getbt();
		c =& 077;
		blnk =+ c;
		return(0);
	}
	else if (cc <= 0)
		putbt(' ');
	do
		putbt('\n');
	while (--cc > 0);
	if (cc < 0) {
		pcnt++;
		ptop = 2;
	}
	ptop--;

	/* check for jes2/asp header */

	if (!spx && !ptop && hdr()) {
		spx = 1;
		if (pcnt <= 6) {
			close(LSIW);
			creat(argp,644);
			blkcnt = 0;
			p = obuf;
		}
	}
	return(-1);
}

getbt()
{

	ret = 0;
	if (--bcnt < 0) {
		bcnt = read(LSIR,ibuf,512);
		b = ibuf;
		if (--bcnt < 0) {
			rflush();
			ret = 1;	/* print file */
			if (xpr) ret = 2;	/* punch file */
			else if (!ff) ret = 3;	/* message file */
			longjmp(reset);
		}
	}
	return(*b++);
}

putbt(charp) char charp;
{
	if (obuf + 512 <= p) rflush();
	*p++ = charp;
}


hdr()
{
	register char *lp, *js2;
	register char *asp;
	int jflg, aflg;

	lp = savp;
	js2 = jlog;
	asp = alog;
	jflg = aflg = 1;
	while(*js2 && *asp) {
		if (obuf + 512 <= lp) lp = obuf;
		switch(*lp++) {
		case ' ':
		case '\t':
			continue;
		default:
			if ((jflg) && (*js2++ != lp[-1])) jflg = 0;
			if ((aflg) && (*asp++ != lp[-1])) aflg = 0;
			if ((jflg == 0) && (aflg == 0))
				return(0);
		}
	}
	return(1);
}

xmode()
{
	register int cnt, lcnt, mcnt;
	char *savb, *savb2;
	int first;

	xpr = 1;
	first = 0;

	for(;;) {
		if (first) {
			getbt();
			getbt();
		}
		else
			first = 1;
		savb = b;
		b =+ 80;
		if ((bcnt =- 80) <= 0) continue;
		savb2 = b;
		while(*--savb2 == 0100);
		savb2++;
		if ((cnt = savb2 - savb) < 0) cnt = 0;
		putbt(cnt);
		while (cnt > 0) {
			if ((mcnt = obuf + 512 - p) <= 0)  {
				rflush();
				mcnt = 512;
			}
			lcnt = (cnt >= mcnt) ? mcnt : cnt;
			cnt = (cnt >= mcnt) ? cnt - mcnt : 0;
			do
				*p++ = *savb++;
			while (--lcnt > 0);
		}
		first = 0;
	}
}

rflush()
{
	register char *ptr;
	register int tmp, j;
	if ((wcnt = p - obuf) <= 0) {
			p = obuf;
			return;
	}
	blkcnt++;
	if (bgf && xpr) {
		p = obuf;
		if (bgf != 2) {
			bgf = 2;
			for(tmp = 0; tmp < 256; tmp++)
				*p++ = -1;
			wcnt = 512;
			lseek(1,(long)(-512),2);
			writit();
		}
		return;
	}
	else {
		if (wcnt != 512)
			return(writit());
		if (bgf) {
			p = obuf;
			ptr = bgerr;
			while(*p++ = *ptr++);
			p[-1] = '\n';
			lseek(1,(long)(-512),2);
			return(writit());
		}
		if ((max != 0) && (blkcnt - xpr > max))
			bgf = 1;
		writit();
	}
}

writit()
{
	register char *ptr;

	p = obuf;
	if (write(LSIW,obuf,wcnt) < 0) {
		ptr = wrerr;
		while(*p++ = *ptr++);
		p[-1] = '\n';
	}
}

