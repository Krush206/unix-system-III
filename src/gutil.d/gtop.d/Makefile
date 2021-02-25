BIN =	../../../bin
I =	../../../include
LIB =	../../../lib/glib.a
PLOTLIB = /usr/lib/libplot.a
SOURCE = arcrad.c gtop.c inarea.c plotarc.c setspace.c windowon.c
HDR0 =	$(I)/gsl.h
HDR1 =	$(HDR0) $(I)/util.h $(I)/errpr.h $(I)/setop.h $(I)/gpl.h

gtop:	gtop.a $(LIB)
	cc gtop.a $(PLOTLIB) $(LIB) -lm -s -o $(BIN)/gtop
	chmod 755 $(BIN)/gtop
	touch gtop

gtop.a::	$(HDR1) gtop.c; ./cca gtop.c
gtop.a::	$(HDR1) windowon.c; ./cca windowon.c
gtop.a::	plotarc.c; ./cca plotarc.c
gtop.a::	setspace.c; ./cca setspace.c
gtop.a::	arcrad.c; ./cca arcrad.c
gtop.a::	$(HDR0) inarea.c; ./cca inarea.c

listings:	$(HDR1) $(SOURCE)  
	ls -l *.c *.h | pr -h "  gutil.d/gtop.d source files  "
	print $? 
	touch listings
