# C library -- creat

# file = creat(string, mode);
# file == -1 if error

	.set	creat,8
.globl	_creat
.globl	cerror

_creat:
	.word	0x0000
	chmk	$creat
	bcc 	noerror
	jmp 	cerror
noerror:
	ret
