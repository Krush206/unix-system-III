#define LOG "log"
#define SLOG "slog"
#define STX	002
#define ETX	003
#define IRS	036
#include	"types.h"
#include	"stat.h"
#include	"ustat.h"

char trt[256] {	/*	Conversion tables for 8-bit ASCII to EBCDIC	*/
	 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,
	 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,
	 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,
	 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,
	0x40,0x5A,0x7F,0x7B,0x5B,0x6C,0x50,0x7D,
	0x4D,0x5D,0x5C,0x4E,0x6B,0x60,0x4B,0x61,
	0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,
	0xF8,0xF9,0x7A,0x5E,0x4C,0x7E,0x6E,0x6F,
	0x7C,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,
	0xC8,0xC9,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,
	0xD7,0xD8,0xD9,0xE2,0xE3,0xE4,0xE5,0xE6,
	0xE7,0xE8,0xE9,0xAD,0xE0,0xBD,0x9A,0x6D,
	0x79,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,
	0xC8,0xC9,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,
	0xD7,0xD8,0xD9,0xE2,0xE3,0xE4,0xE5,0xE6,
	0xE7,0xE8,0xE9,0xC0,0x4F,0xD0,0x5F, 0  ,
	0x20,0x21,0x22,0x23,0x24,0x15,0x06,0x17,
	0x28,0x29,0x2A,0x2B,0x2C,0x09,0x0A,0x1B,
	0x30,0x31,0x1A,0x33,0x34,0x35,0x36,0x08,
	0x38,0x39,0x3A,0x3B,0x04,0x14,0x3E,0xE1,
	0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,
	0x49,0x51,0x52,0x53,0x54,0x55,0x56,0x57,
	0x58,0x59,0x62,0x63,0x64,0x65,0x66,0x67,
	0x68,0x69,0x70,0x71,0x72,0x73,0x74,0x75,
	0x76,0x77,0x78,0x80,0x8A,0x8B,0x8C,0x8D,
	0x8E,0x8F,0x90,0x6A,0x9B,0x9C,0x9D,0x9E,
	0x9F,0xA0,0xAA,0xAB,0xAC,0x4A,0xAE,0xAF,
	0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,
	0xB8,0xB9,0xBA,0xBB,0xBC,0xA1,0xBE,0xBF,
	0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,0xDA,0xDB,
	0xDC,0xDD,0xDE,0xDF,0xEA,0xEB,0xEC,0xED,
	0xEE,0xEF,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF,
	};

int acu;
char	*signon = "signon";
int	nsgnon;
char	sgnbfa[80],sgnbfe[82];
char *xm();
int *order();
char who[10],dqs[20],home[30],pfld[50];
char strx[200];
int hpid,lk;
char xbeg[4],xend[4];
char lockf[20],statf[20],stopf[20],deadf[20],mainf[20],dispf[20];

struct ustat	usb;

main(ac,av)
char *av[];
{
	long	min,atol();
	int rbt,t,lds,sds,pd[2],tv[2];
	struct stat sb;
	char *p,*q,bf[40],phone[20];
	char	*minp,*max,*burst,*save,*inhib,*rem;
	close(0);
	umask(0);	/* Reset umask for file creation */
	strcpy(xbeg,"000");
	strcpy(xend,"000");
	if (alias(av[0],who)<0)
	{
		if (ac<2 || alias(av[1],who)<0)
		{
			prf("%q%s?\n",strx,av[0]);
			writes(1,strx); 
			exit(1);
		};
	};
	cat(lockf,who,"lock");
	cat(statf,who,"stat");
	cat(stopf,who,"stop");
	cat(deadf,who,"dead");
	cat(mainf,who,"main");
	cat(dispf,who,"disp");
	for (t=1;t<15;t++) signal(t,1);
	close(2); 
	close(3); 
	close(4);
	rbt=hpid=lk=0;
	if (ac>=1 && *av[0]=='+')
	{
		rbt=1; 
		while (wait(&t)!=(-1));
	};
	if (dmov(1,0)<0 && open("/dev/tty8",1)!=0 && open("/dev/tty",1)!=0)
		fail("can't grab tty");
	prf("%qcan't locate %s",bf,who);
	if (lookup(who,home,strx,pfld)<0 || chdir(home)<0 || stat(".",&sb)<0) fail(bf);
	cat(dqs,"/dev/",strx);
	t = sb.st_uid;
	if ((sb.st_uid != geteuid()) && setuid(sb.st_uid)<0) fail("wrong uid");
	if (stat(stopf,&sb)>=0)
	{
		unlink(stopf);
		if (rbt) exit(0);
	};
	if (rbt) sleep(60);
	if (pipe(pd)<0) fail("can't create pipe");
	p = pfld;
	minp = p;
	p = delim(p,':');
	max = p;
	p = delim(p,':');
	burst = p;
	p = delim(p,':');
	save = p;
	p = delim(p,':');
	inhib = p;
	p = delim(p,':');
	strcpy(phone,p);
	if (!rbt) {
		if(*phone) {
			strcat(phone,"-<");
			dup(1); 
			dup(1); 
			dup(1);
			if((acu=open("/dev/dn0",1))<0)
				fail("dn open error");
			if (write(acu,phone,strlen(phone))<0)
				fail("dn write error");
			close(3); 
			close(4); 
			close(5);
			close(acu);
			writes(0,"Number dialed and dn closed.\n");
		}
	}
	prf("%qcan't open %s",bf,dqs);
	sleep(8);
	if (open(dqs,2)!=3) fail(bf);
	min = atol(minp);
	if (stat(".",&sb)>=0 && ustat(sb.st_dev,&usb)>=0)
	{
		if (usb.f_tfree<100 || usb.f_tinode<10)
			fail("file system exhausted");
		if (usb.f_tfree<(min + min/2) || usb.f_tinode<100)
		{
			prf("%q%c\n\n%s: warning - only %l blocks, %l inodes free\n\n%c",
			strx,7,who,usb.f_tfree,usb.f_tinode,7);
			if (!rbt) writes(0,strx);
		};
	};
	if (!rbt) {
		unlink(SLOG);
		link(LOG,SLOG);
		unlink(LOG);
	}
	if ((lds=open("log",1))>=0) lseek(lds,0L,2);
	else lds=creat("log",0645);
	if (lds>=0)
	{
		if ((sds=open(statf,0))>=0)
		{
			if ((t=read(sds,bf,30))<0) t=0;
			bf[t]=0; 
			close(sds);
			for (p=q=bf;t=(*p);p++)
			{
				if (t<'0'||t>'9') break;
			};
			if (t==':' && p==(bf+3))
			{
				*p++=0; 
				strcpy(xbeg,bf); 
				strcpy(xend,bf);
			};
			while (*p && *p!='\n') p++;
			if (*p++) while (*q++=(*p++));
			if (q>bf+1)
			{
				*(q-1)='\n'; 
				*q=0; 
				writes(lds,bf);
			};
		};
		time(tv);
		p=ctime(tv)+11; 
		p[8]='\n';
		write(lds,p,9); 
		close(lds);
	};
	lock(); 
	requeue();
	if ((sds=creat(statf,0644))<0) fail("can't create stat file");
	if (write(sds,xend,3)!=3) fail("can't rewrite stat file");
	close(sds); 
	unlock();
	if (open(statf,2)!=4) fail("can't reopen stat file");
	/*  signon remote terminal  */
	if ((sds = open(signon,0)) > 0) {
		t = read(sds,sgnbfa,80);
		nsgnon = t+2;
		p = &sgnbfe[nsgnon];
		*--p = ETX;
		while(t-- > 0) {
			if (sgnbfa[t] == '\n')
				*--p = IRS;
			else
				*--p = trt[(unsigned)sgnbfa[t]];
		}
		*--p = STX;
		if (!rbt) {
			if (write(3,sgnbfe,nsgnon) != nsgnon)
				fail("phone connect not made");
			write(3,0,0);
		}
		else {
			while (write(3,sgnbfe,nsgnon) != nsgnon) {
				if(stat(stopf,&sb) >= 0) {
					unlink(stopf);
					exit(0);
				}
				sleep(60);
			}
			write(3,0,0);
		}
	}
	for (t=1;(hpid=fork())==(-1) && t<20;t++) sleep(t);
	if (hpid==(-1)) fail("can't fork (main)");
	if (hpid==0)
	{
		unlink(deadf);
		if (dmov(3,0)>=0 && dmov(pd[1],3)>=0 && dmov(3,1)>=0 && dmov(4,2)>=0)
		{
			sleep(5); 
			if(*inhib == '\0')
				inhib = "n";
			if(*minp == '\0')
				minp = "0";
			if(*max == '\0')
				max = "0";
			if(*burst == '\0')
				burst = "n";
			execl(mainf,xbeg,who,"main",inhib,minp,max,burst,0);
		};
		creat(deadf,0644); 
		exit(1);
	};
	lseek(4,0L,2); 
	prf("%q:%d\n",bf,hpid); 
	writes(4,bf);
	t=(rbt? 0:fork());
	if (t==(-1)) fail("can't fork (disp)");
	if (t!=0) exit(0);
	if (dmov(0,3)>=0 && dmov(pd[0],0)>=0 && dmov(3,1)>=0)
	{
		close(2); 
		close(4); 
		if(*inhib == '\0')
			inhib = "n";
		if(*minp == '\0')
			minp = "0";
		if(*save == '\0')
			save = "n";
		execl(dispf,xbeg,who,"disp",home,inhib,minp,save,0);
	};
	creat(stopf,0);
	prf("%q%c\n\n%s: couldn't execute %s\n\n%c",strx,7,who,dispf,7);
	writes(1,strx);
	while ((t=read(0,bf,40))>0) write(1,bf,t);
	exit(1);
};

fail(s)
char *s;
{
	if (hpid>0)  creat(stopf,0); 
	unlock();
	prf("%q%c\n\n%sinit failed -- %s\n\n%c",strx,7,who,s,7);
	writes(0,strx);
	exit(1);
};

dmov(d0,d1)
{
	close(d1);
	if (dup(d0)==d1) {
		close(d0); 
		return (0);
	};
	return (-1);
};

writes(ds,s)
char *s;
{
	return (write(ds,s,strlen(s)));
};

#define ENFILE 23

lock()
{
	extern int errno;
	struct	stat	sb1,sb2;
	int t,sf,ds;
	for (sf=t=0;(ds=creat(lockf,0))<0;t++)
	{
		if (errno==ENFILE) unlink(lockf);
		if (sf==0 && stat(lockf,&sb1)>=0) sf=t+1;
		if (t<sf+10) {
			sleep(2); 
			continue;
		};
		if (sf==0) fail("can't set lock");
		if (stat(lockf,&sb2)>=0 && sb1.st_mtime == sb2.st_mtime )
			unlink(lockf);
		sf=t=0;
	};
	close(ds); 
	lk=1;
};

unlock()
{
	if (lk) {
		lk=0; 
		unlink(lockf);
	};
};

struct dent {
	int in; 
	char fn[14];
};

requeue()
{
	struct dent *bx;
	register struct dent *bp;
	register char *p,*q;
	int s,t,ds,*sp,*sp0,*sx,ss[1000];
	char x0[8],x1[8],bf[512];
	char inf0[30],inf1[30];
	sp=ss; 
	sx=ss+1000;
	if ((ds=open(".",0))<0) fail("can't open dir");
	while ((t=read(ds,bf,512))!=0)
	{
		if (t<0 || (t&017)) fail("can't read dir");
		bx=(bf+t);
		for (bp=bf;bp<bx;bp++)
		{
			if (bp->in==0) continue;
			p=bp->fn; 
			q="xmit";
			while (*q!=0 && *p++==(*q)) q++;
			if (*q!=0) continue;
			for (s=t=0;t<3;t++)
			{
				if (*p<'0' || *p>'9') break;
				s=(s*10)+((*p++)-'0');
			};
			if (t!=3 || *p!=0) continue;
			if (sp<sx) *sp++=s; 
			else fail("too many xmit files");
		};
	};
	close(ds);
	if (sp==ss) return;
	sp0=sp=order(ss,sx=sp);
	strcpy(xbeg,xm(s=(*sp))+4);
	for (;;)
	{
		if (++sp>=sx) sp=ss;
		if (sp==sp0) break;
		if (++s>999) s=0;
		if (s==(*sp)) continue;
		strcpy(x0,xm(s)); 
		strcpy(x1,xm(*sp));
		if (link(x1,x0)<0 || unlink(x1)<0) fail("can't relink xmit file");
		cat(inf0,"info/logx",x0+4);
		cat(inf1,"info/logx",x1+4);
		unlink(inf0); 
		link(inf1,inf0); 
		unlink(inf1);
	};
	strcpy(xend,xm(++s)+4);
};
char *xm(s)
{
	static char xms[8];
	char *p,*q; 
	int t;
	prf("%q%d",strx,s); 
	p=strx; 
	t=strlen(p);
	if (t<0 || t>3) fail("bad xmit seq no");
	strcpy(xms,"xmit");
	q=xms+4;
	while (t<3) {
		*q++='0'; 
		t++;
	};
	strcpy(q,p); 
	return (xms);
};

delim(q,d)
char	*q,d;
{
	register	char	*p,c;
	p = q;
	c = d;
	while((*p) && *p != c)p++;
	if(*p)
		*p++ = 0;
	return(p);
}
