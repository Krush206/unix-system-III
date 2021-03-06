static char	sccsid[] = "@(#)sbrk.c	4.1";

extern  char	end;
static	char	*_end = &end;

char *
sbrk(incr)
register incr; {
	register char *ptr;

	ptr = _end;
	brk(_end + incr);
	return (ptr);
}

brk(val)
register char *val; {
	register char *ptr;

	if (val > _end) {
		ptr = _end;
		while (ptr < val)
			*ptr++ = 0;
	}
	_end = val;
	return (0);
}
