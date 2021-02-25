#define LSIR 0
#define LSIW 1
#define ETX 003



char ibuf[512];
char obuf[512];

char *p, *b;

int bcnt, ptop, blnk, tstop, mesgf;

char *argp;

int retx, last, ret, wcnt, xpr, bgf;
long	max,blkcnt,atol();
int	reset[3];
char bgerr[] "\n\n** output file too large. **\n\n";
char wrerr[] "\n\n\n** Write error on output file. **\n\n";
main(argc,argv) char *argv[];
{
	register char c;
	register int gotch;

	argp = argv[0];
	if (open(argp,1) != 1) exit(4);
	p = obuf;
	bcnt = blkcnt = retx = 0;
	max = atol(argv[1]);
	ret = 0;
	setjmp(reset);
	if (ret) exit(ret);
	mesgf = 1;
	for(;;) {
		blnk = tstop = 0;
		gotch = 1;
		c = getbt();
		if (mesgf)
			if (c == '@' && (*b & 0177)  > '@')
				mesgf = 0;
		for(;;) {
			if (gotch == 0)
				if (--bcnt < 0) c = getbt();
				else c = *b++ & 0177;
			gotch = 0;
			if (c > 040) {
				bltb(c);
				continue;
			}
			switch (c) {
			case ' ':
				blnk++;
				continue;
			case '\n':
			case 014:
				putbt(c);
				break;
			case 033:
				c = getbt();
				if ((c & 0100) != 0) {
					blnk += c & 077;
					continue;
				}
				if ((c & 040) != 0) {
					if ((c = -(c & 037)) == 0)
						continue;
					if ((*b &= 0177) > 040)
						bltb(c);
					else
						if (*b == 040) {
							blnk -= c;
							c = getbt();
						}
				}
			default:
				continue;
			}
		break;
		}
	}
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
				if (retx == 0) {
					ret = 2;
					logit("Partial file lost");
					longjmp(reset);
				}
				if (mesgf)
					ret = 3;
				if(last != ETX)
					logit("Partial file concatenated");
				longjmp(reset);
			}
			else
				if ((ibuf[bcnt] & 0177) == ETX) {
					last = ETX;
					if(retx++ > 0)
						logit("Complete file concatenated");
				}
				else
					last = 0;
	}
	return *b++ & 0177;
}
putbt(charp) char charp;
{
	if (obuf + 512 <= p) rflush();
	*p++ = charp;
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
			lseek(1,-512L,2);
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
			lseek(1,-512L,2);
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

bltb(charp) char charp;
{
	register char c;

	c = charp;
	for(;;) {
		if (obuf + 512 <= p) rflush();
		if (blnk) {
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
			continue;
		}
		*p++ = c;
		if (c <= 0) {
			p[-1] = *b;
			tstop--;
			c++;
			if (c < 0)
				continue;
			else {
				c = getbt();
				break;
			}
		} else {
			tstop--;
			break;
		}
	}
}

logit(msg)
char	*msg;
{
	char	ebuff[40];
	int	tvec[2];
	char	*ep;
	register	ds;
	time(tvec);
	ep = prf("%qrecv:	%t	%s\n",ebuff,tvec,msg);
	if((ds = open("log",1)) > 0) {
		lseek(ds,0L,2);
		write(ds,ebuff,ep - ebuff);
		close(ds);
	}
}
