I =	../../../include
BIN =	../../../bin
LIB =	../../../lib/glib.a
SOURCE = chcknds.c getentry.c getmax.c gpsfmt.c idcmp.c \
	levcal.c maxwordlen.c position.c setkids.c setwtst.c sizefac.c\
	squish.c treebldr.c upperstr.c validid.c vcmp.c vmap.c vtoc.c 
HDR =	$(I)/setop.h $(I)/util.h $(I)/errpr.h 

vtoc:	vtoc.a vtoc.h $(I)/gsl.h $(LIB)
	cc vtoc.a $(LIB) -lm -i -s -o $(BIN)/vtoc
	chmod 755 $(BIN)/vtoc
	touch vtoc

vtoc.a::	vtoc.c $(HDR)
		./cca vtoc.c
vtoc.a::	treebldr.c vtoc.h
		./cca treebldr.c
vtoc.a::	vmap.c vtoc.h
		./cca vmap.c
vtoc.a::	levcal.c vtoc.h
		./cca levcal.c
vtoc.a::	setkids.c vtoc.h
		./cca setkids.c
vtoc.a::	sizefac.c vtoc.h
		./cca sizefac.c
vtoc.a::	squish.c vtoc.h
		./cca squish.c
vtoc.a::	gpsfmt.c vtoc.h $(I)/gsl.h
		./cca gpsfmt.c
vtoc.a::	getentry.c vtoc.h
		./cca getentry.c
vtoc.a::	position.c vtoc.h
		./cca position.c
vtoc.a::	vcmp.c vtoc.h
		./cca vcmp.c
vtoc.a::	idcmp.c vtoc.h
		./cca idcmp.c
vtoc.a::	chcknds.c vtoc.h
		./cca chcknds.c
vtoc.a::	getmax.c vtoc.h
		./cca getmax.c
vtoc.a::	setwtst.c vtoc.h $(I)/gsl.h
		./cca setwtst.c
vtoc.a::	validid.c vtoc.h
		./cca validid.c
vtoc.a::	maxwordlen.c $(I)/util.h
		./cca maxwordlen.c
vtoc.a::	upperstr.c
		./cca upperstr.c

vtoc.h:	$(HDR); touch vtoc.h

listings: $(SOURCE) vtoc.h
	ls -l *.c *.h  | pr -h "  vtoc source files  "
	print $?
	touch listings
