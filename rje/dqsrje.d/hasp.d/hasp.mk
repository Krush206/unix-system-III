CC = cc

LIB = ../../../gath/lib/librje.a

install:	directs haspmain hasprecv haspxmit

install:
	cp ptrans /usr/hasp/ptrans
	-chmod 755 /usr/hasp/ptrans
	-chown rje /usr/hasp/ptrans
	cp testjob /usr/hasp/testjob
	-chmod 644 /usr/hasp/testjob
	-chown rje /usr/hasp/testjob
	-rm -f /usr/hasp/haspqer /usr/hasp/haspdisp /usr/hasp/hasphalt
	-rm -f /usr/hasp/haspinit /usr/hasp/haspcopy /usr/hasp/snoop0
	-rm -f /usr/hasp/ptran /usr/hasp/lsl /usr/hasp/scan
	ln /usr/rje/rjeqer /usr/hasp/haspqer
	ln /usr/rje/rjedisp /usr/hasp/haspdisp
	ln /usr/rje/rjehalt /usr/hasp/hasphalt
	ln /usr/rje/rjeinit /usr/hasp/haspinit
	ln /usr/rje/rjecopy /usr/hasp/haspcopy
	ln /usr/rje/snoop0 /usr/hasp/snoop0
	ln /usr/rje/ptran /usr/hasp/ptran
	ln /usr/rje/lsl /usr/hasp/lsl
	ln /usr/rje/scan /usr/hasp/scan

haspmain:	hmain.c $(LIB)
	$(CC) -I/usr/include/sys -s -O -n -o haspmain hmain.c $(LIB)
	cp haspmain /usr/hasp/haspmain
	-chmod 755 /usr/hasp/haspmain
	-chown rje /usr/hasp/haspmain

hasprecv:	hrecv.c $(LIB)
	$(CC) -I/usr/include/sys -s -O -o hasprecv hrecv.c $(LIB)
	cp hasprecv /usr/hasp/hasprecv
	-chmod 755 /usr/hasp/hasprecv
	-chown rje /usr/hasp/hasprecv

haspxmit:	hxmit.c $(LIB)
	$(CC) -I/usr/include/sys -s -O -o haspxmit hxmit.c $(LIB)
	cp haspxmit /usr/hasp/haspxmit
	-chmod 755 /usr/hasp/haspxmit
	-chown rje /usr/hasp/haspxmit

$(LIB):		../../../gath/lib/*.c
	cd ../../../gath/lib; make -f librje.mk librje.a

directs:
	-mkdir /usr/hasp
	-chmod 755 /usr/hasp
	-chown rje /usr/hasp
	-mkdir /usr/hasp/job
	-chmod 777 /usr/hasp/job
	-chown rje /usr/hasp/job
	-mkdir /usr/hasp/pool
	-chmod 777 /usr/hasp/pool
	-chown rje /usr/hasp/pool
	-mkdir /usr/hasp/info
	-chmod 777 /usr/hasp/info
	-chown rje /usr/hasp/info

clean:
	-rm -f haspmain hasprecv haspxmit
