CC = cc

LIB = ../../../gath/lib/librje.a

install:	directs rjeqer rjedisp rjehalt rjeinit \
	rjecopy snoop0 ptran scan shqer compact

install:
	cp lsl /usr/rje/lsl
	-chmod 755 /usr/rje/lsl
	-chown rje /usr/rje/lsl
	cp lines /usr/rje/lines
	-chmod 644 /usr/rje/lines
	-chown rje /usr/rje/lines
	cp sys /usr/rje/sys
	-chmod 644 /usr/rje/sys
	-chown rje /usr/rje/sys

rjeqer:		rjeqer.c $(LIB)
	$(CC) -I/usr/include/sys -s -O -n -o rjeqer rjeqer.c $(LIB)
	cp rjeqer /usr/rje/rjeqer
	-chmod 4755 /usr/rje/rjeqer
	-chown rje /usr/rje/rjeqer

rjedisp:	rjedisp.c $(LIB)
	$(CC) -I/usr/include/sys -s -O -n -o rjedisp rjedisp.c $(LIB)
	cp rjedisp /usr/rje/rjedisp
	-chmod 755 /usr/rje/rjedisp
	-chown rje /usr/rje/rjedisp

rjehalt:	rjehalt.c $(LIB)
	$(CC) -I/usr/include/sys -s -O -n -o rjehalt rjehalt.c $(LIB)
	cp rjehalt /usr/rje/rjehalt
	-chmod 755 /usr/rje/rjehalt
	-chown 755 /usr/rje/rjehalt

rjeinit:	rjeinit.c $(LIB)
	$(CC) -I/usr/include/sys -s -O -n -o rjeinit rjeinit.c $(LIB)
	cp rjeinit /usr/rje/rjeinit
	-chmod 755 /usr/rje/rjeinit
	-chown rje /usr/rje/rjeinit

rjecopy:	rjecopy.c $(LIB)
	$(CC) -I/usr/include/sys -s -O -n -o rjecopy rjecopy.c $(LIB)
	cp rjecopy /usr/rje/rjecopy
	-chmod 755 /usr/rje/rjecopy
	-chown rje /usr/rje/rjecopy

snoop0:		snoop0.c $(LIB)
	$(CC) -I/usr/include/sys -s -O -n -o snoop0 snoop0.c $(LIB)
	cp snoop0 /usr/rje/snoop0
	-chmod 755 /usr/rje/snoop0
	-chown rje /usr/rje/snoop0

scan:		scan.c $(LIB)
	$(CC) -I/usr/include/sys -s -O -n -o scan scan.c $(LIB)
	cp scan /usr/rje/scan
	-chmod 755 /usr/rje/scan
	-chown rje /usr/rje/scan

ptran:		ptran.c $(LIB)
	$(CC) -I/usr/include/sys -s -O -n -o ptran ptran.c $(LIB)
	cp ptran /usr/rje/ptran
	-chmod 755 /usr/rje/ptran
	-chown rje /usr/rje/ptran

shqer:		shqer.c $(LIB)
	$(CC) -I/usr/include/sys -s -O -n -o shqer shqer.c $(LIB)
	cp shqer /usr/rje/shqer
	-chmod 744  /usr/rje/shqer
	-chown root /usr/rje/shqer

compact:	compact.c $(LIB)
	$(CC) -I/usr/include/sys -s -O -n -o compact compact.c $(LIB)
	cp compact /usr/rje/compact
	-chmod 755 /usr/rje/compact
	-chown rje /usr/rje/compact

$(LIB):		../../../gath/lib/*.c
	cd ../../../gath/lib; make -f librje.mk librje.a clean

directs:
	-mkdir /usr/rje
	-mkdir /usr/rje/sque
	-chmod 755 /usr/rje
	-chown rje /usr/rje
	-chmod 755 /usr/rje/sque
	-chown rje /usr/rje/sque

clean:
	-rm -f rjeqer rjedisp rjehalt rjeinit shqer compact
	-rm -f rjecopy snoop0 ptran scan
