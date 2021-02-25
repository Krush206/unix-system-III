/* rjecopy */


/* This information is proprietary and is the property of Bell
   Telephone Laboratories, Incorporated.  Its reproduction or
   disclosure to others, either orally or in writing, is pro-
   hibited without written permission of Bell Laboratories. */


rjecopy() {
	return ("~|^`rjecopy.c 1.1 1/23/76");
};

char bf[512];

main(ac,av)
char *av[];
{
	char *f0,*f1,*p;
	register int t,d0,d1;
	d0=(*av[1]-'0');
	d1=(*av[2]-'0');
	f0=av[3];
	f1=av[4];
	while ((t=read(d0,bf,512))>0)
	{
		if (write(d1,bf,t)!=t) break;
	};
	if (t==0)
	{
		unlink(f0); 
		exit(0);
	};
	chmod(f1,0645);
	if ((t=creat(f1,0454))>=0) lseek(d1,0L,0);
	chmod(f1,0454);
	p=prf("%q\n\n** Read/write error during copy - original is %s **\n\n",bf,f0);
	write(d1,bf,p-bf);
	exit(1);
};
/*end*/
