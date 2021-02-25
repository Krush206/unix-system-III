#
/* rjestat3.c */


/* This information is proprietary and is the property of Bell
   Telephone Laboratories, Incorporated.  Its reproduction or
   disclosure to others, either orally or in writing, is pro-
   hibited without written permission of Bell Laboratories. */


#define IDMOD rstat3

#define IDSTR "~|^`rjestat3.c 1.6 2/5/76

#include	"rjestat.h"
#include	"sys/param.h"
#include	"sys/stat.h"

#define LNONE 0
#define LITEM 1
#define LNACQ 2
#define LEXCP 3
#define LSTMP 4
#define LBOOT 5
#define LDLOW 6

#define TELSE 0
#define TNORM 1
#define TDOWN 2
#define TBOOT 3
#define TFAIL 4
#define TINHB 5
#define TDLOW 6

int slst[NLIN];

status(z)
{
	register char *p,*s;
	register int t;
	char *p0,bf[512];
	int ds,typ,tb[3];
	struct	stat	sb;
	if (flag&(FINT|FBKS|FDSP))
	{
		flag=|FSTA;
		enq(status,z,4); 
		return;
	};
	flag=& ~FSTA;
	if (dash==0)
	{
		prf("%q%s/status",bf,line[z]->dir);
		ds=open(bf,0);
		*bf='\n';
		t=read(ds,bf+1,511);
		close (ds);
		if (t>0) write(1,bf,t+1);
	};
	getlog(z,tb,3);
	typ=TELSE;
	for (t=0;t<3;t++)
	{
		if (tb[t]==LITEM) {
			typ=TNORM; 
			break;
		};
		if (tb[t]==LNACQ) typ=TDOWN;
		if (tb[t]==LDLOW) typ=TDLOW;
	};
	prf("%q/dev/%s",bf,line[z]->dev);
	if (stat(bf,&sb)<0 || (0600&sb.st_mode)!=0600) typ=TINHB;
	else {
		if ((rtim - sb.st_atime)>150) typ=TBOOT;
	};
	if (typ==TBOOT && reboot(z)<0) typ=TFAIL;
	if (typ==TNORM && slst[z]==TNORM) goto X;
	switch (typ)
	{
	case TNORM: 
		s="%qRJE to %s is operating normally"; 
		break;
	case TDOWN: 
		s="%q%s is not responding to RJE"; 
		break;
	case TBOOT: 
		s="%qRJE to %s was not active - reboot attempted"; 
		break;
	case TFAIL: 
		s="%qRJE to %s is not active - reboot failed"; 
		break;
	case TINHB: 
		s="%qRJE to %s has been inhibited by UNIX operator"; 
		break;
	case TDLOW: 
		s="%qRJE to %s is impaired by lack of disk space"; 
		break;
	default: 
		s="%qRJE to %s is not operating normally - reason unknown";
	};
	for (p=s;*p;p++);
	p0=getopl((p-s)+20);
	p=prf("%q\n%t ",p0,&rtim);
	p=prf(s,p,line[z]->hst);
	p=prf("%q.\n",p);
	putopl(p-p0);
	if (dash==0 && (t=bklog(z,tb))>0)
	{
		p0=getopl(50);
		p=prf("%q (%d files queued since %t)\n",p0,t,tb);
		putopl(p-p0);
	};
X: 
	slst[z]=typ;
	enq(status,z,60);
};

getlog(z,v,n)
int v[];
{
	register char *p,*q;
	register int *r;
	int m,ds,typ,*vx;
	char *p0,*px,b[120];
	vx=v+(n-1);
	m=10*n+40;
	if (m<0 || m>120) m=120;
	for (r=v;r<=vx;r++) *r=LNONE;
	prf("%q%s/log",b,line[z]->dir);
	if ((ds=open(b,0))<0) return;
	lseek(ds,(long)(-m),2);
	p0=0;
	if (read(ds,b,m)!=m)
	{
		lseek(ds,0L,0);
		p0=b;
		m=read(ds,b,m);
	};
	px=(p=b)+m;
	close(ds);
	while (p<px)
	{
		if (*p++!='\n') continue;
		if (p0==0) {
			p0=p; 
			continue;
		};
		*(p-1)=0;
		typ=LEXCP;
		for (q=p-2;q>=p0;q--)
		{
			if (*q<'A' || *q>'Z') {
				typ=LNONE; 
				break;
			};
		};
		if (*p0>='0' && *p0<='9') typ=LITEM;
		if (scan("AAA",p0)) typ=LNACQ;
		if (scan("DDD",p0)) typ=LDLOW;
		if (scan(":..:",p0)) typ=LSTMP;
		if (scan("boot",p0)) typ=LBOOT;
		p0=p;
		for (r=v;r<vx;r++) *r=(*(r+1));
		*vx=typ;
	};
};

reboot(z)
{
	register int t;
	register char *p;
	int x;
	struct	stat	sb;
	char b[40];
	prf("%q%s/log",b,line[z]->dir);
	if (stat(b,&sb)>=0 && (rtim - sb.st_mtime) < 120) return (-1);
	if ((t=open(b,1))>=0 || (t=creat(b,0645))>=0)
	{
		p=prf("%q%sboot\n",b,line[z]->pfx);
		lseek(t,0L,2); 
		write(t,b,p-b); 
		close(t);
	};
	if (fork()!=0)
	{
		x=(-1);
		while (wait(&x)!=(-1));
		return (x>>8);
	};
	if (dash)
	{
		if ((t=fork())!=0) exit(t==(-1)? 1:0);
		close(0); 
		close(1);
	};
	for (t=2;t<15;t++) close(t);
	prf("%q%s/%sinit",b,line[z]->dir,line[z]->pfx);
	execl(b,b,0);
	exit(1);
};

struct dent {
	int in; 
	char fn[DIRSIZ];
};

bklog(z,v)
long	*v;
{
	register struct dent *p,*p0;
	register int t;
	struct dent bf[512/(sizeof(*p))];
	int n,ds;
	struct	stat	sb;
	n=0;
	*v = rtim;
	if (chdir(line[z]->dir)<0 || (ds=open(".",0))<0) return (-1);
	while ((t=read(ds,bf,512))>0)
	{
		p0=(p=bf)+(t/(sizeof(*p)));
		for (;p<p0;p++)
		{
			if (p->in==0) continue;
			if (!mtch(4,"xmit",p->fn)) continue;
			if (stat(p->fn,&sb)<0) continue;
			if ((4&sb.st_mode)==4) n++;
			if ((*v - sb.st_mtime) > 0)
				*v = sb.st_mtime;
		};
	};
	close(ds);
	return (n);
};

/*end*/
