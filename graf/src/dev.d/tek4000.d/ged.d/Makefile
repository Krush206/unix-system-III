I =	../../../../include
L =	../../../../lib
BIN =	../../../../bin
LIB =	../lib
HDRS =	$(I) ged.h ../include/gedstructs.h
SOURCE = bufged.c clipu.c cmdfuncs.c cmdproc.c copymove.c deleteobj.c dispbuf.c drbox.c \
	ged.c getfile.c \
	objects.c outwindow.c points.c \
	rotarea.c scalearea.c setwindow.c text.c \
	unixesc.c window.c xymxmn.c

ged::	ged.a $(LIB)/tek.a $(L)/glib.a
	cc ged.a $(LIB)/tek.a $(L)/glib.a -lm -i -s -o $(BIN)/ged
	chmod 755 $(BIN)/ged
	touch ged

ged.a::	$(HDRS) ged.c; ./cca ged.c
ged.a::	$(HDRS) cmdproc.c; ./cca cmdproc.c
ged.a:: $(HDRS) cmdfuncs.c; ./cca cmdfuncs.c
ged.a::	$(HDRS) dispbuf.c; ./cca dispbuf.c
ged.a::	$(HDRS) setwindow.c; ./cca setwindow.c
ged.a::	$(HDRS) outwindow.c; ./cca outwindow.c
ged.a::	$(HDRS) getfile.c; ./cca getfile.c
ged.a::	$(HDRS) text.c; ./cca text.c
ged.a::	$(HDRS) drbox.c; ./cca drbox.c
ged.a::	$(HDRS) unixesc.c; ./cca unixesc.c
ged.a::	$(HDRS) objects.c; ./cca objects.c
ged.a::	$(HDRS) points.c; ./cca points.c
ged.a::	$(HDRS) deleteobj.c; ./cca deleteobj.c
ged.a:: $(HDRS) copymove.c; ./cca copymove.c
ged.a::	$(HDRS) rotarea.c; ./cca rotarea.c
ged.a::	$(HDRS) scalearea.c; ./cca scalearea.c
ged.a::	$(HDRS) bufged.c; ./cca bufged.c
ged.a::	$(HDRS) window.c; ./cca window.c
ged.a::	$(HDRS) xymxmn.c; ./cca xymxmn.c
ged.a:: $(HDRS) clipu.c; ./cca clipu.c

listings: $(HDRS) $(SOURCE)  
	ls -l *.c *.h | pr -h "  tek4000/ged.d source files  "
	print $? 
	touch listings
