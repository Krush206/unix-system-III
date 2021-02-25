#
/* rjeqer */
/* setuid=rje */


/* This information is proprietary and is the property of Bell
   Telephone Laboratories, Incorporated.  Its reproduction or
   disclosure to others, either orally or in writing, is pro-
   hibited without written permission of Bell Laboratories. */


rjeqer() {
	return ("~|^`rjeqer.c 1.5 4/23/76");
};

#include	"types.h"
#include	"stat.h"

char who[8] ;

char statf[40],lockf[40];

char *fil,*ofil,*dir;

char *nseq,*xmt;

long	siz;

struct {
	long		s_tfree;
	unsigned	s_tinode;
	char		s_fname[6];
	char		s_fpack[6];
} usb;
int		dev;
long	min,atol();

main(ac,av)
int ac;
char *av[];
{
	struct	stat	sb;
	int c,qid,sds;
	char bf[60];
	register int t;
	register char *p,*q;
	nseq="001";
	xmt="xmit000";
	if (ac!=4)
	{
		prf("Argument count! (598)\n"); 
		exit(1);
	};
	fil=av[1];
	p = dir = av[2];
	while(*++p);
	while(*--p != '/');
	q = who;
	while(*q++ = *++p);
	stat(dir,&sb);
	dev=sb.st_dev;
	qid = sb.st_uid;
	prf("%q%s/%sstat",statf,dir,who);
	prf("%q%s/%slock",lockf,dir,who);
	if (stat(fil,&sb)<0)
	{
		prf("Cannot access %s. (598)\n",fil); 
		exit(1);
	};
	if ((0555&sb.st_mode)!=0444)
	{
		prf("Mode of %s improper. (598)\n",fil); 
		exit(1);
	};
	if ((sb.st_uid == qid) && (getuid() != qid))
	{
		prf("Violates %s protection. (598)\n",who); 
		exit(1);
	};
	signal(1,1);
	signal(2,1);
	signal(3,1);
	siz = sb.st_size >> 9;
	min = atol(av[3]);
	if (sb.st_dev!=dev)
	{
		ofil=fil; 
		fil=0; 
		fil=copy(ofil);
		if (unlink(ofil)!=(-1)) ofil=0;
	};
	if (ustat(dev,&usb)>=0 && usb.s_tfree<(min + min/5) && usb.s_tfree<(siz+min))
	{
		prf("Cannot queue - only %L blocks free on file system ",usb.s_tfree+siz);
		for (p=dir;*p;prc(*p++))
		{
			if (p>dir && *p=='/') break;
		};
		prf(". (597)\n");
		cleanup(1);
	};
	for (;;)
	{
		if (lock()<0)
		{
			err("enduring lockout"); 
			continue;
		};
		close(0);
		if ((sds=open(statf,2))<0 || read(sds,xmt+4,3)!=3)
		{
			err("stat file error"); 
			continue;
		};
		p=xmt+7; 
		*p=0;
		c=1;
		for (q=nseq+2;q>=nseq;q--)
		{
			t=(*--p);
			if (t<'0' || t>'9') break;
			t=+c; 
			c=0;
			if (t>'9') {
				t='0'; 
				c=1;
			};
			*q=t;
		};
		if (q>=nseq)
		{
			err("stat file corrupted"); 
			continue;
		};
		prf("%q%s/%s",bf,dir,xmt);
		if (link(fil,bf)<0)
		{
			err("cannot link into xmit slot"); 
			continue;
		};
		break;
	};
	lseek(sds,0L,0);
	write(sds,nseq,3);
	unlock();
	prf("%q%s/info/logx%s",bf,dir,xmt+4);
	if ((t=creat(bf,0644))>=0)
	{
		p=prf("%q%s|%d|%s|%s\n",bf,
			getenv("LOGNAME","**RJE**"),getuid(),getenv("HOME","/"),getenv("LOGTTY","/dev/ttyxx"));
		write(t,bf,p-bf);
	};
	prf("Queued as %s/%s.\n",dir,xmt);
	cleanup(0);
};

copy(s)
char *s;
{
	static char f[50];
	int ids,ods;
	char bf[512];
	register int t;
	register char *p;
	if ((ids=open(s,0))<0)
	{
		prf("Cannot open %s for copy. (598)\n",s); 
		exit(1);
	};
	prf("%q%s/pool/qtm%d",f,dir,getpid());
	for (;;)
	{
		if (ustat(dev,&usb)>=0)
		{
			if (usb.s_tfree<(siz+min) || usb.s_tinode<50)
			{
				err("not enough space to copy"); 
				continue;
			};
		};
		lseek(ids,0L,0);
		if ((t=creat(p=f,0444))<0)
		{
			unlink(p);
			if ((t=creat(p,0444))<0) unlink(p);
		};
		if ((ods=t)<0)
		{
			err("cannot create copy"); 
			continue;
		};
		p=bf;
		while ((t=read(ids,p,512))>0)
		{
			if (write(ods,p,t)!=t) break;
		};
		close(ids);
		close(ods);
		if (t!=0)
		{
			unlink(f); 
			err("read/write error during copy"); 
			continue;
		};
		break;
	};
	chown(f,getuid(),0);
	return (f);
};

err(s)
char *s;
{
	int x,abrt();
	register int t;
	register char *p;
	unlock();
	prf("%sqer: %s. Try again? (599)  ",who,s);
	signal(1,abrt);
	signal(2,abrt);
	t=0; 
	p=(&x);
	if (read(2,p,1)==1)
	{
		t=0137&(*p);
		while (*p!='\n' && read(2,p,1)==1);
	};
	signal(1,1);
	signal(2,1);
	if (t!='Y') cleanup(1);
};

abrt()
{
	signal(1,1); 
	signal(2,1); 
	cleanup(1);
};

cleanup(c)
{
	setuid(getuid());
	if (fil) unlink(fil);
	if (ofil) unlink(ofil);
	exit(c);
};

#define ENFILE 23

int lox;

lock()
{
	extern int errno;
	struct	stat	sb1,sb2;
	register int i,t,sf;
	for (sf=t=0;(i=creat(lockf,0))<0;t++)
	{
		if (errno==ENFILE) unlink(lockf);
		if (sf==0 && stat(lockf,&sb2)>=0) sf=t+1;
		if (t<sf+20) {
			sleep(2); 
			continue;
		};
		if (sf != 0 && stat(lockf,&sb1) >= 0 && sb1.st_mtime == sb2.st_mtime)
		{
			unlink(lockf); 
			sf=t=0; 
			continue;
		};
		return (-1);
	};
	close(i); 
	lox=1;
	return (0);
};

unlock()
{
	if (lox) {
		unlink(lockf); 
		lox=0;
	};
};

char *prfq;

prf(s,a)
char *s;
{
	register int c,*aa;
	register char *p;
	p=s;
	aa=(&a);
	while (c=(*p++))
	{
		if (c!='%') {
			prc(c); 
			continue;
		};
		if ((c=(*p++))==0) break;
		if (c=='c') prc(*aa++);
		if (c=='d') prd(*aa++);
		if (c=='L') {
			prL(*((long *)aa));
			aa++;
			aa++;
		}
		if (c=='o') pro(*aa++);
		if (c=='s') prs(*aa++);
		if (c=='t') prt(*aa++);
		if (c=='q') prfq=(*aa++);
	};
	if (p=prfq) *p=prfq=0;
	return (p);
};

prc(c)
{
	register char *q;
	if (q=prfq)
	{
		*q++=c; 
		prfq=q; 
		return;
	};
	write(2,&c,1);
};

prL(n)
long	n;
{
	register long m,k;
	m = n;
	if (k=m/10)
		prL(k);
	prc('0' + (int)(m%10));
};

prd(n)
{
	register int m,k;
	if ((m=n)<0) {
		prc('-'); 
		m=(-m);
	};
	if (k=m/10) prd(k);
	prc('0'+m%10);
};

pro(n)
{
	register int k;
	if (k=017777&(n>>3)) pro(k);
	prc('0'+(7&n));
};

prs(s)
char *s;
{
	register int i;
	register char *p;
	p=s;
	while (i=(*p++)) prc(i);
};

prt(v)
int v[];
{
	register char *p;
	p=ctime(v)+11;
	*(p+8)=0;
	prs(p);
};

