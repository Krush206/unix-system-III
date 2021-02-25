/
/

/ a9 -- pdp-11 assembler pass 1

eae = 0

/ key to types

/	0	undefined
/	1	absolute
/	2	text
/	3	data
/	4	bss
/	5	flop freg,dst (movfo, = stcfd)
/	6	branch
/	7	jsr
/	10	rts
/	11	sys
/	12	movf (=ldf,stf)
/	13	double operand (mov)
/	14	flop fsrc,freg (addf)
/	15	single operand (clr)
/	16	.byte
/	17	string (.ascii, "<")
/	20	.even
/	21	.if
/	22	.endif
/	23	.globl
/	24	register
/	25	.text
/	26	.data
/	27	.bss
/	30	mul,div, etc
/	31	sob
/	32	.comm
/	33	estimated text
/	34	estimated data
/	35	jbr
/	36	jeq, jne, etc

	.data
symtab:
/ special variables

<.\0\0\0\0\0\0\0>; dotrel:02; dot:000000
<..\0\0\0\0\0\0>;	01; dotdot:000000

/ register

<r0\0\0\0\0\0\0>;	24;000000
<r1\0\0\0\0\0\0>;	24;000001
<r2\0\0\0\0\0\0>;	24;000002
<r3\0\0\0\0\0\0>;	24;000003
<r4\0\0\0\0\0\0>;	24;000004
<r5\0\0\0\0\0\0>;	24;000005
<sp\0\0\0\0\0\0>;	24;000006
<pc\0\0\0\0\0\0>;	24;000007

.if eae

/eae & switches

<csw\0\0\0\0\0>;	01;177570
<div\0\0\0\0\0>;	01;177300
<ac\0\0\0\0\0\0>;	01;177302
<mq\0\0\0\0\0\0>;	01;177304
<mul\0\0\0\0\0>;	01;177306
<sc\0\0\0\0\0\0>;	01;177310
<sr\0\0\0\0\0\0>;	01;177311
<nor\0\0\0\0\0>;	01;177312
<lsh\0\0\0\0\0>;	01;177314
<ash\0\0\0\0\0>;	01;177316

.endif

/ double operand

<mov\0\0\0\0\0>;	13;0010000
<movb\0\0\0\0>;		13;0110000
<cmp\0\0\0\0\0>;	13;0020000
<cmpb\0\0\0\0>;		13;0120000
<bit\0\0\0\0\0>;	13;0030000
<bitb\0\0\0\0>;		13;0130000
<bic\0\0\0\0\0>;	13;0040000
<bicb\0\0\0\0>;		13;0140000
<bis\0\0\0\0\0>;	13;0050000
<bisb\0\0\0\0>;		13;0150000
<add\0\0\0\0\0>;	13;0060000
<sub\0\0\0\0\0>;	13;0160000

/ branch

<br\0\0\0\0\0\0>;	06;0000400
<bne\0\0\0\0\0>;	06;0001000
<beq\0\0\0\0\0>;	06;0001400
<bge\0\0\0\0\0>;	06;0002000
<blt\0\0\0\0\0>;	06;0002400
<bgt\0\0\0\0\0>;	06;0003000
<ble\0\0\0\0\0>;	06;0003400
<bpl\0\0\0\0\0>;	06;0100000
<bmi\0\0\0\0\0>;	06;0100400
<bhi\0\0\0\0\0>;	06;0101000
<blos\0\0\0\0>;		06;0101400
<bvc\0\0\0\0\0>;	06;0102000
<bvs\0\0\0\0\0>;	06;0102400
<bhis\0\0\0\0>;		06;0103000
<bec\0\0\0\0\0>;	06;0103000
<bcc\0\0\0\0\0>;	06;0103000
<blo\0\0\0\0\0>;	06;0103400
<bcs\0\0\0\0\0>;	06;0103400
<bes\0\0\0\0\0>;	06;0103400

/ jump/branch type

<jbr\0\0\0\0\0>;	35;0000400
<jne\0\0\0\0\0>;	36;0001000
<jeq\0\0\0\0\0>;	36;0001400
<jge\0\0\0\0\0>;	36;0002000
<jlt\0\0\0\0\0>;	36;0002400
<jgt\0\0\0\0\0>;	36;0003000
<jle\0\0\0\0\0>;	36;0003400
<jpl\0\0\0\0\0>;	36;0100000
<jmi\0\0\0\0\0>;	36;0100400
<jhi\0\0\0\0\0>;	36;0101000
<jlos\0\0\0\0>;		36;0101400
<jvc\0\0\0\0\0>;	36;0102000
<jvs\0\0\0\0\0>;	36;0102400
<jhis\0\0\0\0>;		36;0103000
<jec\0\0\0\0\0>;	36;0103000
<jcc\0\0\0\0\0>;	36;0103000
<jlo\0\0\0\0\0>;	36;0103400
<jcs\0\0\0\0\0>;	36;0103400
<jes\0\0\0\0\0>;	36;0103400

/ single operand

<clr\0\0\0\0\0>;	15;0005000
<clrb\0\0\0\0>;		15;0105000
<com\0\0\0\0\0>;	15;0005100
<comb\0\0\0\0>;		15;0105100
<inc\0\0\0\0\0>;	15;0005200
<incb\0\0\0\0>;		15;0105200
<dec\0\0\0\0\0>;	15;0005300
<decb\0\0\0\0>;		15;0105300
<neg\0\0\0\0\0>;	15;0005400
<negb\0\0\0\0>;		15;0105400
<adc\0\0\0\0\0>;	15;0005500
<adcb\0\0\0\0>;		15;0105500
<sbc\0\0\0\0\0>;	15;0005600
<sbcb\0\0\0\0>;		15;0105600
<tst\0\0\0\0\0>;	15;0005700
<tstb\0\0\0\0>;		15;0105700
<ror\0\0\0\0\0>;	15;0006000
<rorb\0\0\0\0>;		15;0106000
<rol\0\0\0\0\0>;	15;0006100
<rolb\0\0\0\0>;		15;0106100
<asr\0\0\0\0\0>;	15;0006200
<asrb\0\0\0\0>;		15;0106200
<asl\0\0\0\0\0>;	15;0006300
<aslb\0\0\0\0>;		15;0106300
<jmp\0\0\0\0\0>;	15;0000100
<swab\0\0\0\0>;		15;0000300

/ jsr

<jsr\0\0\0\0\0>;	07;0004000

/ rts

<rts\0\0\0\0\0>;	010;000200

/ simple operand

<sys\0\0\0\0\0>;	011;104400

/ flag-setting

<clc\0\0\0\0\0>;	01;0000241
<clv\0\0\0\0\0>;	01;0000242
<clz\0\0\0\0\0>;	01;0000244
<cln\0\0\0\0\0>;	01;0000250
<sec\0\0\0\0\0>;	01;0000261
<sev\0\0\0\0\0>;	01;0000262
<sez\0\0\0\0\0>;	01;0000264
<sen\0\0\0\0\0>;	01;0000270

/ floating point ops

<cfcc\0\0\0\0>;		01;170000
<setf\0\0\0\0>;		01;170001
<setd\0\0\0\0>;		01;170011
<seti\0\0\0\0>;		01;170002
<setl\0\0\0\0>;		01;170012
<clrf\0\0\0\0>;		15;170400
<negf\0\0\0\0>;		15;170700
<absf\0\0\0\0>;		15;170600
<tstf\0\0\0\0>;		15;170500
<movf\0\0\0\0>;		12;172400
<movif\0\0\0>;		14;177000
<movfi\0\0\0>;		05;175400
<movof\0\0\0>;		14;177400
<movfo\0\0\0>;		05;176000
<addf\0\0\0\0>;		14;172000
<subf\0\0\0\0>;		14;173000
<mulf\0\0\0\0>;		14;171000
<divf\0\0\0\0>;		14;174400
<cmpf\0\0\0\0>;		14;173400
<modf\0\0\0\0>;		14;171400
<movie\0\0\0>;		14;176400
<movei\0\0\0>;		05;175000
<ldfps\0\0\0>;		15;170100
<stfps\0\0\0>;		15;170200
<fr0\0\0\0\0\0>;	24;000000
<fr1\0\0\0\0\0>;	24;000001
<fr2\0\0\0\0\0>;	24;000002
<fr3\0\0\0\0\0>;	24;000003
<fr4\0\0\0\0\0>;	24;000004
<fr5\0\0\0\0\0>;	24;000005

/ 11/45 operations

<als\0\0\0\0\0>;	30;072000
<alsc\0\0\0\0>;		30;073000
<mpy\0\0\0\0\0>;	30;070000
.if eae-1
<mul\0\0\0\0\0>;	30;070000
<div\0\0\0\0\0>;	30;071000
<ash\0\0\0\0\0>;	30;072000
<ashc\0\0\0\0>;		30;073000
.endif
<dvd\0\0\0\0\0>;	30;071000
<xor\0\0\0\0\0>;	07;074000
<sxt\0\0\0\0\0>;	15;006700
<mark\0\0\0\0>;		11;006400
<sob\0\0\0\0\0>;	31;077000

/ specials

<.byte\0\0\0>;		16;000000
<.even\0\0\0>;		20;000000
<.if\0\0\0\0\0>;	21;000000
<.endif\0\0>;		22;000000
<.globl\0\0>;		23;000000
<.text\0\0\0>;		25;000000
<.data\0\0\0>;		26;000000
<.bss\0\0\0\0>;		27;000000
<.comm\0\0\0>;		32;000000

ebsymtab:


start:
	sys	signal; 2; 1
	ror	r0
	bcs	1f
	sys	signal; 2; aexit
1:
	mov	(sp)+,r0
	tst	(sp)+

nextarg:
	mov	(sp),r1
	cmpb	(r1),$'-
	bne	notopt
	tst	(sp)+
	dec	r0
	cmpb	1(r1),$'u
	bne	2f
	movb	$'g,unglob
	br	nextarg
2:
	tstb	1(r1)
	bne	2f
	movb	$'g,unglob
	br	nextarg
2:
	cmpb	1(r1),$'o
	bne	notopt
	mov	(sp),r1
	mov	r1,outfp
	beq	4f
	mov	r1,ofname
1:
	tstb	(r1)+
	bne	1b
	dec	r1
	mov	r1,r2
	sub	(sp),r1
	cmp	r1,$2
	ble	4f
	cmpb	-(r2),$'c
	beq	1f
	cmpb	(r2),$'s
	bne	4f
1:
	cmpb	-(r2),$'.
	bne	4f
	mov	r1,oflen
	mov	$2,r0
	sys	write; 9f; 8f-9f
	mov	$2,r0
	sys	write
ofname:	..
oflen:	..
	mov	$2,r0
	sys	write; 7f; 1
	mov	$3,r0
	sys	exit

7:	<\n>
9:	<as: output would overwrite >; 8:

	.even
4:
	tst	(sp)+
	dec	r0
	br	nextarg

notopt:
	movb	r0,nargs
	tst	-(sp)
	mov	sp,curarg
	jsr	r5,fcreat; a.tmp1
	movb	r0,pof
	jsr	r5,fcreat; a.tmp2
	movb	r0,fbfil
	jsr	pc,setup
	jmp	go

setup:
	mov	$symtab,r1
1:
	clr	r3
	mov	$8,r2
	mov	r1,-(sp)
2:
	movb	(r1)+,r4
	beq	2f
	add	r4,r3
	swab	r3
	sob	r2,2b
2:
	clr	r2
	div	$hshsiz,r2
	ashc	$1,r2
	add	$hshtab,r3
4:
	sub	r2,r3
	cmp	r3,$hshtab
	bhi	3f
	add	$2*hshsiz,r3
3:
	tst	-(r3)
	bne	4b
	mov	(sp)+,r1
	mov	r1,(r3)
	add	$12.,r1
	cmp	r1,$ebsymtab
	blo	1b
	rts	pc

/overlay buffer
inbuf	= setup
.	=inbuf+512.

