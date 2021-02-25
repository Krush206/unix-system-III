CC = cc

LIB = ../../../gath/lib/librje.a

install:	directs uvacmain uvacrecv uvacxmit

install:
	cp ptrans /usr/uvac/ptrans
	-chmod 755 /usr/uvac/ptrans
	-chown rje /usr/uvac/ptrans
	cp testjob /usr/uvac/testjob
	-chmod 644 /usr/uvac/testjob
	-chown rje /usr/uvac/testjob
	-rm -f /usr/uvac/uvacqer /usr/uvac/uvacdisp /usr/uvac/uvachalt
	-rm -f /usr/uvac/uvacinit /usr/uvac/uvaccopy /usr/uvac/snoop0
	-rm -f /usr/uvac/ptran /usr/uvac/lsl /usr/uvac/scan
	ln /usr/rje/rjeqer /usr/uvac/uvacqer
	ln /usr/rje/rjedisp /usr/uvac/uvacdisp
	ln /usr/rje/rjehalt /usr/uvac/uvachalt
	ln /usr/rje/rjeinit /usr/uvac/uvacinit
	ln /usr/rje/rjecopy /usr/uvac/uvaccopy
	ln /usr/rje/snoop0 /usr/uvac/snoop0
	ln /usr/rje/ptran /usr/uvac/ptran
	ln /usr/rje/lsl /usr/uvac/lsl
	ln /usr/rje/scan /usr/uvac/scan

uvacmain:	umain.c $(LIB)
	$(CC) -I/usr/include/sys -s -O -n -o uvacmain umain.c $(LIB)
	cp uvacmain /usr/uvac/uvacmain
	-chmod 755 /usr/uvac/uvacmain
	-chown rje /usr/uvac/uvacmain

uvacrecv:	urecv.c $(LIB)
	$(CC) -I/usr/include/sys -s -O -o uvacrecv urecv.c $(LIB)
	cp uvacrecv /usr/uvac/uvacrecv
	-chmod 755 /usr/uvac/uvacrecv
	-chown rje /usr/uvac/uvacrecv

uvacxmit:	uxmit.c $(LIB)
	$(CC) -I/usr/include/sys -s -O -o uvacxmit uxmit.c $(LIB)
	cp uvacxmit /usr/uvac/uvacxmit
	-chmod 755 /usr/uvac/uvacxmit
	-chown rje /usr/uvac/uvacxmit

$(LIB):		../../../gath/lib/*.c
	cd ../../../gath/lib; make -f librje.mk librje.a

directs:
	-mkdir /usr/uvac
	-chmod 755 /usr/uvac
	-chown rje /usr/uvac
	-mkdir /usr/uvac/job
	-chmod 777 /usr/uvac/job
	-chown rje /usr/uvac/job
	-mkdir /usr/uvac/pool
	-chmod 777 /usr/uvac/pool
	-chown rje /usr/uvac/pool
	-mkdir /usr/uvac/info
	-chmod 777 /usr/uvac/info
	-chown rje /usr/uvac/info

clean:
	-rm -f uvacmain uvacrecv uvacxmit
