		/ UNIX/TS - Bell Laboratories /

/ Low core
.data

br4 = 200
br5 = 240
br6 = 300
br7 = 340

.globl	start, dump, call, _utsname
.globl	trap, trap4, trap10, trap14, trap24, trap240, trap250

. = 0^.
	jmp	start		/ Initial startup
. = 4^.
	trap4;	br7+0.		/ Bus error
. = 10^.
	trap10;	br7+1.		/ Illegal instruction
. = 14^.
	trap14;	br7+2.		/ Bpt-trace trap
. = 20^.
	trap;	br7+3.		/ Iot trap
. = 24^.
	trap24;	br7+4.		/ Power failure
. = 30^.
	trap;	br7+5.		/ Emulator trap
. = 34^.
	trap;	br7+6.		/ System entry
. = 40^.
	_utsname;		/ Name of system
	br .
. = 44^.
dump:				/ Memory dump
.globl	dump_ht
	jmp	*$dump_ht
. = 50^.
	.+2;	br .		/ Trap catcher
. = 54^.
	.+2;	br .
. = 60^.
	klin;	br4+0.
	klou;	br4+0.
	.+2;	br .
	.+2;	br .
. = 100^.
	clio;	br6+0.
. = 104^.
	clio;	br6+1.
	.+2;	br .
. = 114^.
	trap;	br7+10.		/ Memory system error
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
. = 224^.
	htio;	br5+0.
	.+2;	br .
	.+2;	br .
. = 240^.
	trap240;	br7+7.		/ Programmed interrupt
. = 244^.
	trap;	br7+8.		/ Floating point
. = 250^.
	trap250;		/ Segmentation violation
	br7+9.
. = 254^.
	hpio;	br5+0.
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .
	.+2;	br .

. = 1000^.

/ Interface code to C

.globl	_clock
clio:
	jsr	r0,call; _clock

.globl	_klrint, _klxint
klin:
	jsr	r0,call; _klrint
klou:
	jsr	r0,call; _klxint

.globl	_hpintr
hpio:
	jsr	r0,call; _hpintr

.globl	_htintr
htio:
	jsr	r0,call; _htintr

