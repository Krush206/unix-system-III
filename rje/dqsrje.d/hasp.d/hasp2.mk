install:
	-mkdir /usr/hasp2
	-chmod 755 /usr/hasp2
	-chown rje /usr/hasp2
	-mkdir /usr/hasp2/job
	-chmod 777 /usr/hasp2/job
	-chown rje /usr/hasp2/job
	-mkdir /usr/hasp2/pool
	-chmod 777 /usr/hasp2/pool
	-chown rje /usr/hasp2/pool
	-mkdir /usr/hasp2/info
	-chmod 777 /usr/hasp2/info
	-chown rje /usr/hasp2/info
	-rm -f /usr/hasp2/hasp2qer /usr/hasp2/hasp2disp /usr/hasp2/hasp2halt
	-rm -f /usr/hasp2/hasp2init /usr/hasp2/hasp2copy /usr/hasp2/snoop1
	-rm -f /usr/hasp2/ptran /usr/hasp2/lsl /usr/hasp2/scan
	-rm -f /usr/hasp2/hasp2recv /usr/hasp2/hasp2xmit /usr/hasp2/hasp2main
	ln /usr/hasp/haspqer /usr/hasp2/hasp2qer
	ln /usr/hasp/haspdisp /usr/hasp2/hasp2disp
	ln /usr/hasp/hasphalt /usr/hasp2/hasp2halt
	ln /usr/hasp/haspinit /usr/hasp2/hasp2init
	ln /usr/hasp/haspcopy /usr/hasp2/hasp2copy
	ln /usr/hasp/snoop0 /usr/hasp2/snoop1
	ln /usr/hasp/ptran /usr/hasp2/ptran
	ln /usr/hasp/lsl /usr/hasp2/lsl
	ln /usr/hasp/scan /usr/hasp2/scan
	ln /usr/hasp/hasprecv /usr/hasp2/hasp2recv
	ln /usr/hasp/haspxmit /usr/hasp2/hasp2xmit
	ln /usr/hasp/haspmain /usr/hasp2/hasp2main
