LIB = ../../gath/lib/librje.a

hasp:		rjecommon send rjestat
		cd hasp.d; make -f hasp.mk install

hasp2:		hasp
		cd hasp.d; make -f hasp2.mk

hasp3:		hasp2
		cd hasp.d; make -f hasp3.mk

uvac:		rjecommon send rjestat
		cd uvac.d; make -f uvac.mk install

uvac2:		uvac
		cd uvac.d; make -f uvac2.mk

uvac3:		uvac2
		cd uvac.d; make -f uvac3.mk

rjestat:	$(LIB)
		cd rjestat.d; make -f rjestat.mk install

send:		$(LIB)
		cd ../../gath; make -f gath.mk install
		cp /usr/bin/gath /usr/bin/send
		-chmod 775 /usr/bin/send
		-chown bin /usr/bin/send

$(LIB):		../../gath/lib/*.c
	cd ../../gath/lib; make -f librje.mk librje.a

rjecommon:	$(LIB)
		cd rjecommon.d; make -f rjecommon.mk install

install:
		make -f rje.mk $(ARGS)

clobber:
	cd hasp.d; make -f hasp.mk clean
	cd uvac.d; make -f uvac.mk clean
	cd rjecommon.d; make -f rjecommon.mk clean
	cd rjestat.d; make -f rjestat.mk clean
	cd ../../gath; make -f gath.mk clean
	cd ../../gath/lib; make -f librje.mk clean
