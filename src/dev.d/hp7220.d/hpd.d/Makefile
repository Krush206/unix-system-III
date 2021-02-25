I =	../../../../include
L =	../../../../lib
BIN = ../../../../bin
UPLIB = ../../uplib
LOLIB = ../../lolib
SOURCE =	devarc.c devbuff.c devcursor.c devfinal.c devinit.c devfill.c \
		devline.c devtext.c mba.c mbn.c mbp.c newcolor.c sbn.c

HDR1 =	$(I) ../../include/dev.h dev.h

hp:	dev.c dev.a $(UPLIB)/uplib.a $(LOLIB)/lolib.a  $(L)/glib.a
	cc dev.c $(UPLIB)/uplib.a dev.a $(LOLIB)/lolib.a  $(L)/glib.a -lm -i -s -o $(BIN)/hpd
	chmod 755 $(BIN)/hpd
	touch hp

dev.a::	$(HDR1) devinit.c; ./cca devinit.c
dev.a:: $(HDR1) devbuff.c; ./cca devbuff.c
dev.a::	$(HDR1) devfinal.c; ./cca devfinal.c
dev.a::	$(HDR1) devcursor.c; ./cca devcursor.c
dev.a::	$(HDR1) devarc.c; ./cca devarc.c
dev.a::	$(HDR1) devline.c; ./cca devline.c
dev.a::	$(HDR1) devtext.c; ./cca devtext.c
dev.a::	$(HDR1) newcolor.c; ./cca newcolor.c
dev.a::	$(HDR1) mbp.c; ./cca mbp.c
dev.a::	$(HDR1) mba.c; ./cca mba.c
dev.a::	$(HDR1) mbn.c; ./cca mbn.c
dev.a::	$(HDR1) sbn.c; ./cca sbn.c
dev.a:: $(HDR1) devfill.c; ./cca devfill.c

listings: $(HDR1) $(SOURCE)  
	ls -l  *.c *.h | pr -h "  dev.d/hp.d source files  "
	pr $? 
	touch listings
