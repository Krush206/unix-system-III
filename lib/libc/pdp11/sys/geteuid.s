/ C library -- geteuid


/ uid = geteuid();
/  returns effective uid

.globl	_geteuid
.getuid = 24.

_geteuid:
	mov	r5,-(sp)
	mov	sp,r5
	sys	.getuid
	mov	r1,r0
	mov	(sp)+,r5
	rts	pc
