/* scan */

main(ac,av)
int	ac;
char	*av[];
{
	int	t,ds;
	long	c,atol();
	char	bf[120];
	if (ac<2) {
		write(2,"usage: scan log [ count ]\n",26);
		exit(0);
	}
	if ((ds=open(av[1],0)) < 0) {
		write(2,av[1],strlen(av[1]));
		write(2,": cannot open\n",14);
		exit(1);
	}
	if (ac<3)
		lseek(ds,0L,2);
	else {
		c = atol(av[2]);
		if(c <= 0)
			lseek(ds,0L,0);
		else
			lseek(ds,-c,2);
	}
	for (;;)
	{
		t=read(ds,bf,120);
		if(t < 0)
			exit(1);
		if (t==0) {
			sleep(2); 
			continue;
		};
		if (t>0) write(1,bf,t);
	};
};
