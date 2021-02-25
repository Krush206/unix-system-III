#include <stdio.h>
#include <a.out.h>
#include <sys/param.h>
#ifdef	vax
#include <sys/mba.h>
#endif
#include <sys/utsname.h>
#include <sys/elog.h>
#include <sys/erec.h>
#include <sys/map.h>
#include <sys/err.h>
#include <sys/iobuf.h>

struct nlist nl[] = {
#ifdef	vax

	"_err",0,0,0,0,
	"_time",0,0,0,0,
	"_hputab",0,0,0,0,
	"_hstab",0,0,0,0,
	"_httab",0,0,0,0,
	"_rftab",0,0,0,0,
	"_rktab",0,0,0,0,
	"_rltab",0,0,0,0,
	"_rptab",0,0,0,0,
	"_tmtab",0,0,0,0,
	"\0",0,0,0,0

#else

	"_err",0,0,
	"_time",0,0,
	"_hputab",0,0,
	"_hstab",0,0,
	"_httab",0,0,
	"_rftab",0,0,
	"_rktab",0,0,
	"_rltab",0,0,
	"_rptab",0,0,
	"_tmtab",0,0,
	"\0",0,0
#endif
};

time_t	time;
char	*dump;
char	*nlfile	= "/unix";
char	*tmpfile;
int	fd;
int	tfd;
int	child;
int	wflg;
struct iobuf iobuf;

struct err err;

char	*sbrk();
char	*mktemp();
int	catch();

main(argc,argv)
char **argv;
{
	register n, slot;
	register struct err *ep = &err;
	int status;
	struct iobuf *dp;

	if(argc < 2)
		errexit("Arg count\n");
	dump = *++argv;
	if(argc > 2)
		nlfile = *++argv;
	nlist(nlfile,nl);
#ifdef vax
	for(n = 0; nl[n].n_name[0]; n++)
		if(nl[n].n_type)
			nl[n].n_value -= 0x80000000;
#endif
	if(nl[0].n_type == 0 || nl[1].n_type == 0)
		errexit("Bad nlist\n");
	if((fd = open(dump,0)) < 0)
		errexit("Can't open dump file\n");
	fetch(nl[0].n_value,(char *)&err,sizeof(struct err));
	fetch(nl[1].n_value,(char *)&time,sizeof(time_t));
	if(NESLOT != ep->e_nslot)
		errexit("Number of slots in dump don't agree with sys/err.h\n");
	for(n = 1; n < 32; n++)
		signal(n,catch);
	tmpfile = mktemp("/usr/tmp/errXXXXXX");
	if((tfd = creat(tmpfile,0666)) < 0) {
		tmpfile = mktemp("/tmp/errXXXXXX");
		if((tfd = creat(tmpfile,0666)) < 0)
			errexit("Can't create tmp file \n");
	}
	slot = (struct errhdr **) ((char *)ep->e_org
		- (char *)nl[0].n_value
#ifdef vax
		- 0x80000000
#endif
		+ (char *)&err) - ep->e_ptrs;
	for(n = slot; n < ep->e_nslot; n++)
		puterec(ep->e_ptrs[n],0);
	for(n = 0; n < slot; n++)
		puterec(ep->e_ptrs[n],0);
	if(nl[2].n_value)
		{
		n = (unsigned)(nl[2].n_value +  sizeof(iobuf)*8);
		for(dp = (struct iobuf *)nl[2].n_value; dp < (struct iobuf *) n; dp++)
			getiobuf((unsigned)dp);
	}
	for (n = 3; nl[n].n_name[0]; n++)
		getiobuf(nl[n].n_value);
	close(tfd);
	if(wflg == 0)
		printf("No errors logged\n");
	else {
		if((child = fork()) == 0) {
			execl("errpt","errpt","-a",tmpfile,0);
			execl("/bin/errpt","errpt","-a",tmpfile,0);
			execl("/usr/bin/errpt","errpt","-a",tmpfile,0);
			exit(16);
		}
		if(child < 0) {
			unlink(tmpfile);
			errexit("Can't fork\n");
		}
		wait(&status);
	}
	unlink(tmpfile);
}
errexit(s1)
char *s1;
{
	fprintf(stderr,s1);
	exit(16);
}
fetch(off,buf,size)
char *buf;
unsigned off;
{
	lseek(fd,(long)off,0);
	if(read(fd,buf,size) != size)
		errexit("Read error\n");
}
getiobuf(addr)
unsigned register addr;
{
	if(addr)
		{
		fetch(addr, (char *) &iobuf, sizeof(iobuf));
		if(iobuf.io_erec)
			puterec(iobuf.io_erec,1);
	}
}
puterec(erec,unlog)
register struct errhdr *erec;
register unlog;
{
	if(erec)
		{
		erec = (struct errhdr *) ((char *)erec
		- (char *)nl[0].n_value
#ifdef vax
		- 0x80000000
#endif
		+ (char *)&err);
		if(unlog)
			{
			erec->e_type = E_BLK;
			erec->e_time = time;
			((struct eblock *)((char *)erec + sizeof(struct errhdr)))->e_bflags |= E_ERROR;
		}
		write(tfd, (char *)erec, erec->e_len);
		wflg++;
	}
}
catch()
{
	if(tmpfile)
		unlink(tmpfile);
	exit(1);
}
