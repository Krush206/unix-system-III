I =	../../../include
BIN =	../../../bin
SOURCE = ptog.c linemod.c ptogarcpt.c
LIB =	../../../lib/glib.a
HDR =	$(I)/util.h $(I)/gpl.h $(I)/gsl.h $(I)/errpr.h

ptog:	ptog.a $(LIB)
	cc ptog.a $(LIB) -lm -s -o $(BIN)/ptog
	chmod 755 $(BIN)/ptog
	touch ptog

ptog.a:: $(HDR) ptog.c; ./cca ptog.c
ptog.a:: linemod.c; ./cca linemod.c
ptog.a:: ptogarcpt.c; ./cca ptogarcpt.c

listings: $(HDR) $(SOURCE)  
	ls -l *.c *.h | pr -h "  gutil.d/ptog.d source files  "
	print $? 
	touch listings
