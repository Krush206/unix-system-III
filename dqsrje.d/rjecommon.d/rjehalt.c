#
/* rjehalt */


/* This information is proprietary and is the property of Bell
   Telephone Laboratories, Incorporated.  Its reproduction or
   disclosure to others, either orally or in writing, is pro-
   hibited without written permission of Bell Laboratories. */


rjehalt() {
	return ("~|^`rjehalt.c 1.4 4/23/76");
};

#include	"types.h"
#include	"stat.h"
#include	"stdio.h"

char who[10];

char stopf[20],deadf[20],soff[20];
char	where[30],strx[30],phone[50];

main(ac,av)
int	ac;
char	*av[];
{
	struct	stat	sb;
	register	i;
	register	char	*p;
	if (alias(av[0],who)<0)
	{
		fprintf(stderr,"%s?\n",av[0]); 
		exit(1);
	};
	if (lookup(who,where,strx,phone)<0 || chdir(where)<0)
	{
		fprintf(stderr,"Cannot locate %s.\n",who); 
		exit(1);
	};
	cat(stopf,who,"stop");
	cat(deadf,who,"dead");
	cat(soff,who,"soff");
	p = phone;
	for(i = 0;i < 5;i++)
		while(*p && (*p++ != ':'));
	if ((*p != '\0') && (stat(soff,&sb)<0) && (creat(soff,0)<0))
	{
		fprintf(stderr,"Permission denied.\n"); 
		exit(1);
	};
	if (stat(stopf,&sb)<0 && creat(stopf,0)<0)
	{
		fprintf(stderr,"Permission denied.\n"); 
		exit(1);
	};
	while (stat(deadf,&sb)<0) sleep(1);
	exit(0);
};
