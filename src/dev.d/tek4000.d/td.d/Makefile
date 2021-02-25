I =	../../../../include
L =	../../../../lib
BIN = ../../../../bin
UPLIB = ../../uplib
LOLIB = ../../lolib
SOURCE =	devarc.c devcursor.c devfinal.c devinit.c devline.c \
		devtext.c xytek.c xytekl.c devfill.c devbuff.c

HDR1 =	$(I) ../../include/dev.h dev.h

dev:	dev.c dev.a $(UPLIB)/uplib.a $(LOLIB)/lolib.a  $(L)/glib.a
	cc dev.c $(UPLIB)/uplib.a dev.a $(LOLIB)/lolib.a  $(L)/glib.a -lm -i -s -o $(BIN)/td
	chmod 755 $(BIN)/td
	touch td

dev.a::	$(HDR1) devinit.c; ./cca devinit.c
dev.a::	$(HDR1) devfinal.c; ./cca devfinal.c
dev.a::	$(HDR1) devcursor.c; ./cca devcursor.c
dev.a::	$(HDR1) devarc.c; ./cca devarc.c
dev.a::	$(HDR1) devline.c; ./cca devline.c
dev.a::	$(HDR1) devtext.c; ./cca devtext.c
dev.a::	$(HDR1) xytek.c; ./cca xytek.c
dev.a::	$(HDR1) xytekl.c; ./cca xytekl.c
dev.a:: $(HDR1) devfill.c; ./cca devfill.c
dev.a:: $(HDR1) devbuff.c; ./cca devbuff.c

listings: $(HDR1) $(SOURCE)  
	ls -l  *.c *.h | pr -h "  dev.d/tek4000.d/td.d source files  "
	pr $? 
	touch listings
