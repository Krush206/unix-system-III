USR = /usr
DD = TMP

CC = cc
CCFLAGS = -O

LD = -ld
LDFLAGS = -x

LIB = ../../../gath/lib/librje.a

.c.o:
	$(CC) -c $(CCFLAGS) $<
	$(LD) -r $(LDFLAGS) $*.o
	mv a.out $*.o

#------
# rjestat:	Create rjestat in local directory.
#------
rjestat:	rjestat0.o rjestat1.o rjestat2.o rjestat3.o \
		rjestat4.o rjestat5.o rjestat6.o
	$(CC) -s -O -n -o rjestat rjestat0.o rjestat1.o rjestat2.o \
		rjestat3.o rjestat4.o rjestat5.o \
		rjestat6.o $(LIB)

install:	rjestat
	cp rjestat /usr/bin/rjestat
	-chmod 4755 /usr/bin/rjestat
	-chown rje /usr/bin/rjestat

$(LIB):		../../../gath/lib/*.c
	cd ../../../gath/lib; make -f librje.mk librje.a

#------
# clean: Remove all .o files.
#------
clean:
		-rm -f rjestat0.o rjestat1.o rjestat2.o rjestat3.o
		-rm -f rjestat4.o rjestat5.o rjestat6.o rjestat

#------
# Dependencies for individual .o files.
#------
rjestat0.o:	rjestat0.c
rjestat1.o:	rjestat1.c rjestat.h
rjestat2.o:	rjestat2.c rjestat.h
rjestat3.o:	rjestat3.c rjestat.h /usr/include/sys/param.h /usr/include/sys/stat.h
rjestat4.o:	rjestat4.c rjestat.h /usr/include/sys/types.h /usr/include/sys/stat.h
rjestat5.o:	rjestat5.c rjestat.h /usr/include/sys/types.h /usr/include/sys/stat.h
rjestat6.o:	rjestat6.c rjestat.h
