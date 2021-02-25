#
/* rjestat1.c */


/* This information is proprietary and is the property of Bell
   Telephone Laboratories, Incorporated.  Its reproduction or
   disclosure to others, either orally or in writing, is pro-
   hibited without written permission of Bell Laboratories. */


#define IDMOD rstat1

#define IDSTR "~|^`rjestat1.c 1.8 4/22/76

#include "rjestat.h"

struct lins *lin0,*linx;

char *host[] {
	"B","A","U1","U2","C","U3","NLSC",0};

main(ac,av)
char *av[];
{
	extern sigi(),sigq();
	extern status(),display();
	register struct lins *q;
	register char **p;
	register int t;
	int bits;
	long	tv,l;
	int	j;
	char	*s;
	sig14();
	signal(SIGH,0);
	prf("%q%d",pid,getpid());
	bits=0;
	for (p=av;--ac>=0;p++)
	{
		if (scan("-",*p)) dash=1;
		if (scan("+",*p)) plus=1;
		for (t=0;host[t]!=0;t++)
		{
			if (scan(host[t],*p)) bits=|(1<<t);
		};
	};
	if (bits==0) bits=(-1);
	if (plus!=0) dash=1;
	if (getuid() != geteuid()) plus=0;
	conf();
	for (q=lin0;q<linx;q++)
	{
		if (q->hst==0) continue;
		for (p=host;*p;p++)
		{
			if (scan(*p,q->hst)==0) continue;
			t=(1<<(p-host));
			if (bits&t)
			{
				bits=& ~t;
				s = q->phne;
				for(j = 0;j < 4;j++)
					while((*s) && (*s++ != ':'));
				if ((*s != 'i') || (dash == 0))
					line[nlin++]=q;
				else
					prf("%s not configured for status terminal\n",q->hst);
			}
			break;
		};
	};
	if (nlin == 0) exit(0);
	inifcn(); 
	iniopl();
	if (dash==0)
	{
		for (t=0;t<nlin;t++)
		{
			time(&rtim); 
			status(t);
		};
		exit(0);
	};
	usage("rjestat");
	inidsp(); 
	inimsg();
	for (t=0;t<nlin;t++) {
		enq(status,t,0); 
		enq(display,t,5);
	};
	prf("\nRJE to ");
	for (t=0;t<nlin;t++)
	{
		if (t>0) prf(",");
		prf("%s",line[t]->hst);
	};
	prf(":  Status terminal started.\n");
	signal(SIGI,sigi);
	signal(SIGQ,sigq);
	time(&rtim);
	for (;;)
	{
		time(&tv);
		l = tv - rtim;
		rtim = tv;
		if (l>0) tmq((int)l);
		t=xeq();
		if (t>0) sleep(t);
	};
};

int set14;

sig14()
{
	signal(14,sig14);
	if (set14) alarm(1);
};

sleep(n)
{
	if (n==0) return;
	alarm(set14=n);
	pause();
	alarm(set14=0);
};

sigi()
{
	extern cull(),backsp();
	register int t;
	signal(SIGI,sigi);
	if ((flag&FINT)==0)
	{
		flag=|FINT;
		time(&itim);
		enq(cull,0,-1);
		enq(backsp,0,-1);
	};
	if (set14)
	{
		t=alarm(1);
		if (t!=0) alarm(t);
	};
};

sigq()
{
	exit(0);
};


struct lins rjeline[NLIN];


char rjelinx[NLIX];

conf()
{
	register struct lins *q;
	register char *p;
	register int t;
	lin0=q=rjeline;
	t=rjetab("lines",q,NLIN*6,rjelinx,NLIX);
	linx=q+(t/6);
	p=rjesys();
	for (;q<linx;q++)
	{
		if (mtch(1,p,q->sys)) continue;
		if (scan(p,q->sys)!=0 && chdir(q->dir)>=0)
		{
			if ((t=creat("pool/...",0666))>=0)
			{
				close(t); 
				continue;
			};
		};
		q->hst=0;
	};
};

usage(s)
char *s;
{
	int u,*au;
	char *px,ss[512];
	register int t,d;
	register char *p;
	au=(&u);
	p="/usr/pw/usg";
	t=getuid();
	t=+2; 
	t=<<1;
	if (t>=512) return;
	prf("%q%s/%s",ss,p,s);
	if ((d=open(ss,2))>=0)
	{
		if (lseek(d,(long)t,0)<0 || read(d,au,2)!=2) u=0;
		u++;
		if (lseek(d,(long)t,0)>=0) write(d,au,2);
		close(d); 
		return;
	};
	if ((d=creat(ss,0666))<0) return;
	px=(p=ss)+512;
	time(p); 
	p=+4;
	while (p<px) *p++=0;
	ss[t]=1;
	write(d,ss,512);
	close(d);
};

/*end*/
