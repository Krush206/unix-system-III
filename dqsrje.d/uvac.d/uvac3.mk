install:
	-mkdir /usr/uvac3
	-chmod 755 /usr/uvac3
	-chown rje /usr/uvac3
	-mkdir /usr/uvac3/job
	-chmod 777 /usr/uvac3/job
	-chown rje /usr/uvac3/job
	-mkdir /usr/uvac3/pool
	-chmod 777 /usr/uvac3/pool
	-chown rje /usr/uvac3/pool
	-mkdir /usr/uvac3/info
	-chmod 777 /usr/uvac3/info
	-chown rje /usr/uvac3/info
	-rm -f /usr/uvac3/uvac3qer /usr/uvac3/uvac3disp /usr/uvac3/uvac3halt
	-rm -f /usr/uvac3/uvac3init /usr/uvac3/uvac3copy /usr/uvac3/snoop2
	-rm -f /usr/uvac3/ptran /usr/uvac3/lsl /usr/uvac3/scan
	-rm -f /usr/uvac3/uvac3recv /usr/uvac3/uvac3xmit /usr/uvac3/uvac3main
	ln /usr/uvac/uvacqer /usr/uvac3/uvac3qer
	ln /usr/uvac/uvacdisp /usr/uvac3/uvac3disp
	ln /usr/uvac/uvachalt /usr/uvac3/uvac3halt
	ln /usr/uvac/uvacinit /usr/uvac3/uvac3init
	ln /usr/uvac/uvaccopy /usr/uvac3/uvac3copy
	ln /usr/uvac/snoop0 /usr/uvac3/snoop2
	ln /usr/uvac/ptran /usr/uvac3/ptran
	ln /usr/uvac/lsl /usr/uvac3/lsl
	ln /usr/uvac/scan /usr/uvac3/scan
	ln /usr/uvac/uvacrecv /usr/uvac3/uvac3recv
	ln /usr/uvac/uvacxmit /usr/uvac3/uvac3xmit
	ln /usr/uvac/uvacmain /usr/uvac3/uvac3main
