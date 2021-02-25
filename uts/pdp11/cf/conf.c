/*
 *  Configuration information
 */


#define	DL11_0 1
#define	RP06_0 1
#define	TU16_0 1
#define	MEMORY_0 1
#define	TTY_0 1
#define	ERRLOG_0 1

#define	NBUF	20
#define	NINODE	40
#define	NFILE	40
#define	NMOUNT	2
#define	CMAPSIZ	50
#define	SMAPSIZ	50
#define	NCALL	20
#define	NPROC	25
#define	NTEXT	15
#define	NCLIST	20
#define	NSABUF	3
#define	POWER	0
#define	MAXUP	25
#define	NHBUF	64

#include	"sys/param.h"
#include	"sys/io.h"
#include	"sys/space.h"

extern nodev(), nulldev();
extern klopen(), klclose(), klread(), klwrite(), klioctl();
extern hpopen(), hpclose(), hpread(), hpwrite(), hpstrategy(), hptab;
extern htopen(), htclose(), htread(), htwrite(), htstrategy(), httab;
extern mmread(), mmwrite();
extern syopen(), syread(), sywrite(), syioctl();
extern erropen(), errclose(), errread();

int	(*bdevsw[])()
{
/* 0*/	&hpopen,	&hpclose,	&hpstrategy,	&hptab,
/* 1*/	&htopen,	&htclose,	&htstrategy,	&httab,
};

int	(*cdevsw[])()
{
/* 0*/	&klopen,	&klclose,	&klread,	&klwrite,	&klioctl,
/* 1*/	&nodev, 	&nodev, 	&nodev, 	&nodev, 	&nodev,
/* 2*/	&nulldev,	&nulldev,	&mmread,	&mmwrite,	&nodev, 
/* 3*/	&nodev, 	&nodev, 	&nodev, 	&nodev, 	&nodev,
/* 4*/	&nodev, 	&nodev, 	&nodev, 	&nodev, 	&nodev,
/* 5*/	&nodev, 	&nodev, 	&nodev, 	&nodev, 	&nodev,
/* 6*/	&htopen,	&htclose,	&htread,	&htwrite,	&nodev, 
/* 7*/	&hpopen,	&hpclose,	&hpread,	&hpwrite,	&nodev, 
/* 8*/	&nodev, 	&nodev, 	&nodev, 	&nodev, 	&nodev,
/* 9*/	&nodev, 	&nodev, 	&nodev, 	&nodev, 	&nodev,
/*10*/	&nodev, 	&nodev, 	&nodev, 	&nodev, 	&nodev,
/*11*/	&nodev, 	&nodev, 	&nodev, 	&nodev, 	&nodev,
/*12*/	&nodev, 	&nodev, 	&nodev, 	&nodev, 	&nodev,
/*13*/	&syopen,	&nulldev,	&syread,	&sywrite,	&syioctl,
/*14*/	&nodev, 	&nodev, 	&nodev, 	&nodev, 	&nodev,
/*15*/	&nodev, 	&nodev, 	&nodev, 	&nodev, 	&nodev,
/*16*/	&nodev, 	&nodev, 	&nodev, 	&nodev, 	&nodev,
/*17*/	&nodev, 	&nodev, 	&nodev, 	&nodev, 	&nodev,
/*18*/	&nodev, 	&nodev, 	&nodev, 	&nodev, 	&nodev,
/*19*/	&nodev, 	&nodev, 	&nodev, 	&nodev, 	&nodev,
/*20*/	&erropen,	&errclose,	&errread,	&nodev, 	&nodev, 
};

int	bdevcnt	2;
int	cdevcnt	21;

dev_t	rootdev	{(0<<8)|0};
dev_t	pipedev	{(0<<8)|0};
dev_t	dumpdev	{(1<<8)|0};
dev_t	swapdev	{(0<<8)|0};
daddr_t	swplo	7000;
int	nswap	2500;


int	hp_addr[]
{
	0176700,
};
int	hp_cnt 8;

int	ht_addr[]
{
	0172440,
};
int	ht_cnt 4;

int	kl_addr[]
{
	0177560,
};
int	kl_cnt 1;
struct	tty	kl_tty[1];




int	(*pwr_clr[])() = 
{
	(int (*)())0
};

int	(*dev_init[])() = 
{
	(int (*)())0
};
