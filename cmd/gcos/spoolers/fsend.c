#include	<stdio.h>

/*
 *  fsend -- spooler to send file to GCOS
 */

#define	SPTYPE	GCOSA
#define FIRSTCHAR 'N'-1
#define	NTEMP	4
char	Xfname[NTEMP][25];
char	Xchar[NTEMP] = {'d', 't', 'c', 'l'};
char	*dfname = Xfname[0];
char	*tfname = Xfname[1];
char	*cfname = Xfname[2];
char	*lfname = Xfname[3];
char	*NAME;
#define	BF	'B'
#define	GCATSW	0
#define	MAXCOPY	204800L

#include	"spool.c"

int	namef	= 0;
char	fname[13] = "pipe.end"  /* gcos file name */;
char	*fmode;

main(agc, agv)
int agc;
char *agv[];
{
	int i;
	char c;

	argc = agc;    argv = agv;
	setup();
	remotsw = TOSS;		/*default is toss out online output*/
#ifndef NMAIL
	wantmail = 1;		/*default is send mail.*/
#endif
	grade = '1';
	fmode = "a";

	while (argc>1 && (arg = argv[1])[0]=='-') {
	    if(!comopt(c = arg[1]))
		switch (c) {

		case 'a':
			fmode = "a";
			break;

		case 'b':
			fmode = "b";
			break;

		case 'u':
			arg = getarg('u');
			i = 0;
			while((gcosid[i] = arg[i]))
				if(++i > 12) {
					gcosid[i] = '\0';
					break;
				}
			uidf = 1;
			break;

		case 'x':
			archsw++;
			break;

		default:
			err("Unrecognized option: %s", arg);
			break;
		}
		if(c == 'f'){
			i = 0;
			while((fname[i] = mailfile[i]))
				if(++i > 12) {
					fname[i] = '\0';
					break;
				}
			namef = 1;
		}
		argc--;
		argv++;
	}

	gcos1();		/*setup initial GCOS cards.*/
	card('L',"$	select	cc/unixsupport/access");

	filargs();		/*process file arguments.*/

	gcos2();		/*put out final cards, and spool job.*/
}

nuact(file, temp)
char *file, *temp;
{
	int c, i, j;

	if(namef)
		namef = 0;
	else {
		i = 0;
		j = 0;
		while((c = file[i++]) != '\0') {
			if(c == '/') { j = 0; continue; }
			if(j >= 12) break;
			fname[j++] = c;
		}
		fname[j++] = '\0';
	}
	if(gcdebug)
		card('L', "$	select	cc/test/fsend.1");
	else
		card('L', "$	select	cc/unixsupport/fsend.1");
	card('L', " ignore errs");
	card('L', "fcreat %s/%s,blocks/%d,unlimited/,read,modify/%s/",
			gcosid, fname, gsize, gcosid);
	card('L', "fcreat %s/%s,blocks/1,unlimited/,read,modify/%s/",
			gcosid, fname, gcosid);
	if(gcdebug)
		card('L', "$	select	cc/test/fsend.2");
	else
		card('L', "$	select	cc/unixsupport/fsend.2");
	sprintf(cardbuf, "%s %s/%s %ld", fmode, gcosid, fname, usize);
	card('L', cardbuf);
	card('L', "$	data	i*,ncksum");
}

archive()
{
	card('L', "$      select  cc/unixsupport/arch" );
	card('L', "s   %-12s %-12s", gcosid, fname);
}
