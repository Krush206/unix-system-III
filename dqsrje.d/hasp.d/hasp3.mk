install:
	-mkdir /usr/hasp3
	-chmod 755 /usr/hasp3
	-chown rje /usr/hasp3
	-mkdir /usr/hasp3/job
	-chmod 777 /usr/hasp3/job
	-chown rje /usr/hasp3/job
	-mkdir /usr/hasp3/pool
	-chmod 777 /usr/hasp3/pool
	-chown rje /usr/hasp3/pool
	-mkdir /usr/hasp3/info
	-chmod 777 /usr/hasp3/info
	-chown rje /usr/hasp3/info
	-rm -f /usr/hasp3/hasp3qer /usr/hasp3/hasp3disp /usr/hasp3/hasp3halt
	-rm -f /usr/hasp3/hasp3init /usr/hasp3/hasp3copy /usr/hasp3/snoop2
	-rm -f /usr/hasp3/ptran /usr/hasp3/lsl /usr/hasp3/scan
	-rm -f /usr/hasp3/hasp3recv /usr/hasp3/hasp3xmit /usr/hasp3/hasp3main
	ln /usr/hasp/haspqer /usr/hasp3/hasp3qer
	ln /usr/hasp/haspdisp /usr/hasp3/hasp3disp
	ln /usr/hasp/hasphalt /usr/hasp3/hasp3halt
	ln /usr/hasp/haspinit /usr/hasp3/hasp3init
	ln /usr/hasp/haspcopy /usr/hasp3/hasp3copy
	ln /usr/hasp/snoop0 /usr/hasp3/snoop2
	ln /usr/hasp/ptran /usr/hasp3/ptran
	ln /usr/hasp/lsl /usr/hasp3/lsl
	ln /usr/hasp/scan /usr/hasp3/scan
	ln /usr/hasp/hasprecv /usr/hasp3/hasp3recv
	ln /usr/hasp/haspxmit /usr/hasp3/hasp3xmit
	ln /usr/hasp/haspmain /usr/hasp3/hasp3main
