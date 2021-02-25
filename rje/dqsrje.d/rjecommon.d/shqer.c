#define USR 19
#define QLEN 200
#define QUEDIR "/usr/rje/sque"
#define PFILE "/usr/rje/sque/pfile"
#define QUELOG	"log"
#include	"types.h"
#include	"stat.h"
#include	"ustat.h"

struct	stat	sb;

struct	ustat	usb;
char *sh, *p2;
char buf[QLEN+4], *ddv;
int ds;
long	siz;
char fspce[2];
int tout,status;
int	pf;
char	pid[10];

char	timez[12];
char	*envir[4];
int cont();

struct qdat {
	char *jnb;	/*job number*/
	char *jnm;	/*job name*/
	char *pnm;	/*programmer name*/
	char *fnm;	/*file name to execl*/
	char *shf;	/*full path to execl*/
	char *dir;	/*directory of execl file*/
	char *unm;	/*login name from usr=*/
	char *qfl;	/*standard input file*/
	char *ldir;	/*login directory*/
	char sbf[250];
};

main(argc,argv)
int argc;
char **argv;
{

	struct qdat q;
	register char *p, *p1;
	register int pse;
	int	pri;
	long	min,atol();
	char	*minp;
	char c;

	signal(1,1);
	signal(2,1);
	if (fork() != 0)
		exit(0);
	strcpy(timez,"TZ=");
	strcpy(timez+3,getenv("TZ"));
	if (argc > 1)
		pse = atoi(argv[1]);
	else
		pse = 180;
	if (pse < 15)
		pse = 15;
	if (argc > 2)
		pri = atoi(argv[2]);
	else
		pri = 0;
	buf[0] = buf[1] = ':';
	prf("%q%d\n",pid,getpid());
	pf = creat(PFILE,0644);
	write(pf,pid,strlen(pid));
	close(pf);
	chdir(QUEDIR);
	signal(14,cont);
	while((ds = open(QUELOG,2)) < 0) {
		alarm(300);
		pause();
	}
	for(;;) {
		while(read(ds,&buf[2],QLEN) > 0) {
			if (buf[2] != '/') continue;
			p = &buf[2];
			p1 = q.shf = q.sbf;
			while ((*p1++ = *p++) != ':');
			p1[-1] = '\0';
			sh = p1; q.fnm = p1;
			while(*--sh != '/');
			p2 = sh;
			sh++;
			while(*p1++ = *sh++);
			q.dir = p1;
			sh = q.shf;
			do
				*p1++ = *sh;
			while(sh++ != p2);
			*p1++ = '\0';
			q.qfl = p1;
			while ((*p1++ = *p++) != ':');
			p1[-1] = '\0';
			q.jnm = p1;
			while((*p1++ = *p++) != ':');
			if (*q.jnm == ':')
				p1++;
			p1[-1] = '\0';
			q.pnm = p1;
			while((*p1++ = *p++) != ':');
			if (*q.pnm == ':')
				p1++;
			p1[-1] = '\0';
			q.jnb = p1;
			while((*p1++ = *p++) != ':');
			if (*q.jnb == ':')
				p1++;
			p1[-1] = '\0';
			q.unm = p1;
			strcpy(p1,"LOGNAME=");
			p1 += 8;
			while((*p1++ = *p++) != ':');
			if (q.unm[8] == ':')
				p1++;
			p1[-1] = '\0';
			q.ldir = p1;
			strcpy(p1,"HOME=");
			p1 += 5;
			while((*p1++ = *p++) != ':');
			p1[-1] = '\0';
			minp = p1;
			while((*p1++ = *p++) != ':');
			if (*minp == ':')
				p1++;
			p1[-1] = '\0';
			buf[QLEN-1] = '\n';
			close(0);
			if (open(q.qfl,0) != 0) {
				lseek(ds,(long)(-QLEN),1);
				write(ds,buf,QLEN);
				continue;
			}
			unlink(q.qfl);
			if (fork() == 0) {
				if (stat(q.shf,&sb) < 0)
					exit(1);
				close(1);
				if (open("/dev/null",1) != 1)
					exit(1);
				close(2);
				dup(1);
				min = atol(minp);
				ddv = sb.st_dev;
				siz = sb.st_size >> 9;
				fspce[0] = '?';
				if (ustat(ddv,&usb) >= 0)
					if (usb.f_tfree < (siz+min) || usb.f_tinode < 50)
						fspce[0] = '1';
					else
						fspce[0] = '0';
				nice(pri);
				setuid(USR);
				envir[0] = timez;
				envir[1] = q.unm;
				envir[2] = q.ldir;
				envir[3] = 0;
				chdir(q.dir);
			  execle(q.shf,q.fnm,fspce,q.jnm,q.pnm,q.jnb,q.unm+8,0,envir);
			  execle("/bin/sh","sh",q.shf,fspce,q.jnm,q.pnm,q.jnb,q.unm+8,0,envir);
				exit(1);
			}
			lseek(ds,(long)(-QLEN),1);
			write(ds,buf,QLEN);
			alarm(pse);
			while(wait(&status) > 0);
			tout = alarm(0);
			if (tout > 10) {
				alarm(tout);
				pause();
			}
		}
		alarm(pse);
		pause();
	}
}
cont() {
	signal(14,cont);
}
