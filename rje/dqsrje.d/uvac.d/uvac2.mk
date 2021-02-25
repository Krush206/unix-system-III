install:
	-mkdir /usr/uvac2
	-chmod 755 /usr/uvac2
	-chown rje /usr/uvac2
	-mkdir /usr/uvac2/job
	-chmod 777 /usr/uvac2/job
	-chown rje /usr/uvac2/job
	-mkdir /usr/uvac2/pool
	-chmod 777 /usr/uvac2/pool
	-chown rje /usr/uvac2/pool
	-mkdir /usr/uvac2/info
	-chmod 777 /usr/uvac2/info
	-chown rje /usr/uvac2/info
	-rm -f /usr/uvac2/uvac2qer /usr/uvac2/uvac2disp /usr/uvac2/uvac2halt
	-rm -f /usr/uvac2/uvac2init /usr/uvac2/uvac2copy /usr/uvac2/snoop1
	-rm -f /usr/uvac2/ptran /usr/uvac2/lsl /usr/uvac2/scan
	-rm -f /usr/uvac2/uvac2recv /usr/uvac2/uvac2xmit /usr/uvac2/uvac2main
	ln /usr/uvac/uvacqer /usr/uvac2/uvac2qer
	ln /usr/uvac/uvacdisp /usr/uvac2/uvac2disp
	ln /usr/uvac/uvachalt /usr/uvac2/uvac2halt
	ln /usr/uvac/uvacinit /usr/uvac2/uvac2init
	ln /usr/uvac/uvaccopy /usr/uvac2/uvac2copy
	ln /usr/uvac/snoop0 /usr/uvac2/snoop1
	ln /usr/uvac/ptran /usr/uvac2/ptran
	ln /usr/uvac/lsl /usr/uvac2/lsl
	ln /usr/uvac/scan /usr/uvac2/scan
	ln /usr/uvac/uvacrecv /usr/uvac2/uvac2recv
	ln /usr/uvac/uvacxmit /usr/uvac2/uvac2xmit
	ln /usr/uvac/uvacmain /usr/uvac2/uvac2main
