I =	../../../include
LIB =	../../../lib/glib.a
SOURCE =  arcfill.c clipseg.c compwind.c dispfile.c dispprim.c  inarea.c \
	linefill.c softarc.c softtext.c utodcoord.c
HDR1 =	$(I) ../include/dev.h
HDR2 = $(HDR1) ../include/fill.h

uplib:  uplib.a  $(LIB)
	touch uplib

uplib.a::	$(HDR1) compwind.c; ./cca compwind.c
uplib.a::	$(HDR1) dispfile.c; ./cca dispfile.c
uplib.a::	$(HDR1) utodcoord.c; ./cca utodcoord.c
uplib.a::	$(HDR1) dispprim.c; ./cca dispprim.c
uplib.a::	$(HDR2) linefill.c; ./cca linefill.c
uplib.a::	$(HDR2) arcfill.c; ./cca arcfill.c
uplib.a::	$(HDR1) softarc.c; ./cca softarc.c
uplib.a::	$(HDR1) softtext.c; ./cca softtext.c
uplib.a::	$(HDR1) clipseg.c; ./cca clipseg.c
uplib.a::	$(HDR1) inarea.c; ./cca inarea.c

listings: $(HDR1) $(SOURCE)  
	ls -l  *.c *.h | pr -h "  dev.d/lib source files  "
	pr $? 
	touch listings
