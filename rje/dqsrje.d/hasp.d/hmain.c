#

/*	haspmain	*/

# include	"types.h"
# include	"stat.h"
# define	DQS	0
# define	NEXCP	5

# define	ACQ	'A'
# define	BAD	'B'
# define	ERR	'E'
# define	EXC	'X'
# define	FRK	'F'
# define	HST	'S'
# define	OPN	'O'
# define	SPA	'D'
# define	TMP	'T'
# define	UNX	'U'

struct	stat	ibuf;
char	hdead[12];
char	hstop[12];
char	hmesg[12];
char	hxmit[12];
char	hrecv[12];
char	hsoff[12];
char	buf[512];
char	mbf[40];
char	xmit[]	"xmit000\0";
char	tmpf[]	"tmp0\0\0\0\0\0\0\0";
char	rfile[]	"resp\0";
char	info[]	"info/logx000\0";
char	*file,*bufsize;

long	tout,now,min,atol();
int	rrdy,dne,rcnt,mesgf,chks,nexc,dev,prntonly;
char	*max,*burst;
char	lexc[NEXCP];


struct {
	int	q_len;
	char	q_buff[29];
} qx {
	29,
	0002,	0141,	0134,	0133,	0344,	0347,	0360,	0100,
	0100,	0035,	0177,	0325,	0036,	0141,	0134,	0133,
	0304,	0321,	0361,	0140,	0371,	0371,	0371,	0371,
	0035,	0174,	0325,	0036,	0003,
};

struct {
	int	s_len;
	char	s_buff[12];
} soffo {
	12,
	0002,	0141,	0134,	0342,	0311,	0307,	0325,	0326,
	0306,	0306,	0036,	0003,
};

struct {
	int	e_len;
	char	e_buff[1];
} eot {
	1,
	0067,
};

main(argc,argv)
int	argc;
char	*argv[];
{
	register	c;
	strcpy(xmit+4,argv[0]);
	winit(argv[1]);
	min = atol(argv[4]);
	max = argv[5];
	burst = argv[6];
	stat(".",&ibuf);
	dev = ibuf.st_dev;
	nexc = 0;
	getmp();
	time(&tout);
	ckrdy();
	while(stat(hstop,&ibuf) == -1) {
		if((rrdy > 1) && (*argv[3] != 'i')) {
			time(&now);
			if(now > tout) {
				c = write(DQS,qx.q_buff,qx.q_len);
				if(c <= 0)
					excp((c == 0)? ACQ : ERR);
				else
					if(write(DQS,0,0) == -1)
						excp(ERR);
					else {
						nexc = 0;
						time(&now);
						tout = now + 600;
					}
				recv();
				continue;
			}
		}
		if(!send())
			recv();
	}
	halt();
	recv();
}

recv()
{
	register	c;
	ckrdy();
	if(rrdy < 2) {
		excp(SPA);
		write(DQS,0,0);
		sleep(10);
		return;
	}
	rcnt = nexc;
	do {
		if(read(DQS,buf,1) > 0) {
			while((c = spawn(hrecv,tmpf,max,burst)) == 4) {
				excp(TMP);
				getmp();
			}
			if(c > 4)
				down();
			nexc = 0;
			if(prntonly)
				c = 3;
			else
				c =+ 2;
			if(c == 5)
				resp(tmpf);
			post(c,tmpf);
			getmp();
			ckrdy();
			return;
		}
	} while(--rcnt > 0);
	if(dne)
		halt2();
}

halt()
{
	if(stat(hsoff,&ibuf) == -1)
		halt2();
	write(DQS,soffo.s_buff,soffo.s_len);
	write(DQS,0,0);
	write(DQS,eot.e_buff,eot.e_len);
	write(DQS,0,0);
	unlink(hsoff);
	dne = 1;
}

halt2()
{
	post(0,0);
	unlink(hstop);
	unlink(tmpf);
	dead();
}

down()
{
	excp(UNX);
	down1();
}

down1()
{
	lseek(2,0L,2);
	write(2,lexc,nexc);
	dead();
}

dead()
{
	exit(creat(hdead,0644));
}

char	*names[]	{
		&hdead,
		&hstop,
		&hmesg,
		&hxmit,
		&hrecv,
		&hsoff,
};

winit(who)
char	*who;
{
	char	*p;
	char	**np;
	if(strcmp(who,"nlsc") == 0) {
		bufsize = "400";
		prntonly = 1;
	} else {
		bufsize = "512";
		prntonly = 0;
	}
	p = "dead\0stop\0mesg\0xmit\0recv\0soff\0";
	for(np = names;*np ;np++) {
		strcpy(*np,who);
		strcat(*np,p);
		p =+ 5;
	}
}

post(code,name)
int	code;
char	*name;
{
	name = move(name,code);
	buf[0] = code + '0';
	buf[1] = '\0';
	if(name) {
		strcat(buf,name);
		strcat(buf,"\n");
	}
	if(write(1,buf,strlen(buf)) == -1)
		down();
}

char	*mv2[]	{
	0,
	"zmit\0",
	"badf\0",
	"prt\0",
	"pch\0",
	"msg\0",
	0,
};

move(s,c)
char	*s;
int	c;
{
	register	char	*p,*q;
	if((c > 6) || (s == 0))
		return(s);
	strcpy(mbf,s);
	q = mbf;
	if((p = mv2[c]) != 0)
	while(*p)
		*q++ = *p++;
	if(link(s,mbf) == -1)
		return(s);
	unlink(s);
	return(mbf);
}

spawn(prog,arg0,arg1,arg2)
char	*prog,*arg0,*arg1,*arg2;
{
	register	pid,cpid;
	int		status;
	while((pid = fork()) == -1) {
		excp(FRK);
		sleep(6);
	}
	if(pid == 0) { /* child */
		close(1);
		close(2);
		execl(prog,arg0,arg1,arg2,0);
		exit(0);
	}
	if((cpid = wait(&status)) == -1)
		down();
	if(cpid != pid) {
		kill(9,pid);
		down();
	}
	if(status & 0377)
		down();
	return((status >> 8) & 0377);
}

mktmp()
{
	register	char	*p,*q,c;
	p = tmpf;
	while(*p++);
	q = --p;
	while((c = *--p) <= '9') {
		if((*p = ++c) <= '9')
			return;
		*p = '0';
	}
	p++;
	*p++ = '1';
	do
		*p++ = '0';
	while(p <= q);
}

getmp()
{
	register	c;
	do
		mktmp();
	while(stat(tmpf,&ibuf) != -1);
	if((c = creat(tmpf,0644)) == -1)
		down();
	close(c);
}

resp(name)
char	*name;
{
	register	i,o,c;
	if((i = open(name,0)) == -1)
		return;
	if((o = open(rfile,1)) == -1) {
		if((o = creat(rfile,0645)) == -1) {
			close(i);
			return;
		}
	}
	else
		lseek(o,0L,2);
	while((c = read(i,buf,512)) > 0)
		write(o,buf,c);
	write(o,"\n",1);
	fstat(o,&ibuf);
	close(o);
	if(ibuf.st_size > 32767)
		unlink(rfile);
	close(i);
}

struct {
	long		s_tfree;
	unsigned	s_tinode;
	char		s_fname[6];
	char		s_fpack[6];
} ubuf;

ckrdy()
{
	ustat(dev,&ubuf);
	rrdy = 0;
	if(ubuf.s_tinode >= 50) {
		if(ubuf.s_tfree > min)
			rrdy++;
		if(ubuf.s_tfree > (min + min/5))
			rrdy++;
	}
}

excp(code)
char	code;
{
	lexc[nexc++] = code;
	if((nexc >= NEXCP) || (code == ERR)) {
		if(code != UNX)
			unlink(tmpf);
		down1();
	}
}

send()
{
	register	c;
	chks = 2;
	do {
		if(!mesgf) {
			if(--chks < 0)
				return(0);
			if((rrdy) && (stat(hmesg,&ibuf) != -1) && (ibuf.st_mode & 0444)) {
				mesgf = 1;
				file = hmesg;
				break;
			}
		}
		mesgf = 0;
		if(--chks < 0)
			return(0);
		lseek(2,0L,0);
		if(read(2,buf,3) != 3) {
			excp(HST);
			return(0);
		}
		buf[3] = '\0';
		file = xmit;
	} while(strcmp(buf,xmit+4) == 0);
	c = spawn(hxmit,file,bufsize,0);
	if(c == 1) {
		nexc = 0;
		if(!mesgf)
			post(1,file);
		return(next());
	}
	if(c > 5)
		down();
	switch(c) {
		case 2 :
			excp(ACQ);
			return(0);
		case 3 :
			if(!mesgf)
				post(6,file);
			excp(ERR);
			return(0);
		case 4 :
			excp(OPN);
			if(!mesgf) {
				unlink(xmit);
				strcpy(info+9,xmit+4);
				unlink(info);
			}
			break;
		case 5 :
			excp(BAD);
			post(2,file);
	}
	return(next());
}

next()
{
	register	char	*p,*q,c;
	if(mesgf) {
		unlink(hmesg);
		return(0);
	}
	p = xmit+4;
	q = xmit+7;
	while(q > p) {
		c = *--q;
		*q = ++c;
		if(c <= '9')
			return(1);
		*q = '0';
	}
	return(1);
}
