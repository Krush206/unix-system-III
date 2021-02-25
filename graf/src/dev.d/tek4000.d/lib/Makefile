I =	../../../../include
HDR1 =	$(I) tek.h  ../include/gedstructs.h
HDR2 = $(HDR1)  ../include/termodes.h

SOURCE = addrbuf.c chplot.c circarc.c clip.c \
	getacur.c getcraw.c getline.c getpoint.c getpos.c inarea.c locacur.c\
	scaleoff.c setmode.c\
	tekalpha.c tekarc.c tekflush.c tekinit.c teklines.c tekmove.c\
	tektext.c tekxy.c unscaleoff.c xytek.c xytekl.c

tek:	tek.a
	touch tek

tek.a::	$(HDR1) teklines.c; ./cca teklines.c
tek.a::	$(HDR2) tekmove.c; ./cca tekmove.c
tek.a::	$(HDR2) tekinit.c; ./cca tekinit.c
tek.a::	$(HDR1) tektext.c; ./cca tektext.c
tek.a::	$(HDR2) tekalpha.c; ./cca tekalpha.c
tek.a::	$(HDR1) tekarc.c; ./cca tekarc.c
tek.a::	$(HDR1) chplot.c; ./cca chplot.c
tek.a::	$(HDR1) circarc.c; ./cca circarc.c
tek.a::	$(HDR2) addrbuf.c; ./cca addrbuf.c
tek.a::	$(HDR1) clip.c; ./cca clip.c
tek.a::	$(HDR2) tekflush.c; ./cca tekflush.c
tek.a::	$(HDR2) setmode.c; ./cca setmode.c
tek.a::	$(HDR2) getacur.c; ./cca getacur.c
tek.a::	$(HDR2) locacur.c; ./cca locacur.c
tek.a::	$(HDR2) getpos.c; ./cca getpos.c
tek.a::	$(HDR1) tekxy.c; ./cca tekxy.c
tek.a::	$(HDR1) xytek.c; ./cca xytek.c
tek.a::	$(HDR1) xytekl.c; ./cca xytekl.c
tek.a::	$(HDR1) inarea.c; ./cca inarea.c
tek.a:: $(HDR1) getcraw.c; ./cca getcraw
tek.a:: $(HDR1) getline.c; ./cca getline
tek.a:: $(HDR2) getpoint.c; ./cca getpoint
tek.a::	$(HDR1) scaleoff.c; ./cca scaleoff.c
tek.a::	$(HDR1) unscaleoff.c; ./cca unscaleoff.c


listings:	$(HDR2) $(SOURCE)  
	ls -l  *.c *.h | pr -h "  tek4000/lib source files  "
	print $? 
	touch listings
