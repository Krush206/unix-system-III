#define SIZEQ 200
#define QUELOG "/usr/rje/sque/log"
#define QUELOG2 "/usr/rje/sque/log2"

#include	"types.h"
#include	"stat.h"

char buf[SIZEQ];
int i,j;
struct	stat	stbuf;

main() {
	if ((i=open(QUELOG))<0)
		exit(1);
	lseek(i,0L,2);
	if (lseek(i,(long)(-SIZEQ),1) < 0)
		exit(0);
	do {
		read(i,buf,2);
		if (buf[0] == ':' && buf[1] == ':') {
			lseek(i,(long)(SIZEQ),1);
			break;
		}
	} while (lseek(i,(long)(-(SIZEQ + 2)),1) >= 0);
	lseek(i,-2L,1);
	j = creat(QUELOG2,0644);
	while(read(i,buf,SIZEQ) == SIZEQ)
		write(j,buf,SIZEQ);
	close(j); close(i);
	stat(QUELOG,&stbuf);
	unlink(QUELOG);
	link(QUELOG2,QUELOG);
	unlink(QUELOG2);
	chown(QUELOG,stbuf.st_uid,stbuf.st_gid);
}
