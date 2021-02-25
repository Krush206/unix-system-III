VPMC = vpmc

ARGS = -m

install:	rjeproto

rjeproto:	rjeproto.r
	$(VPMC) $(ARGS) -o rjeproto rjeproto.r
	cp rjeproto /etc/rjeproto
	-chmod 644 /etc/rjeproto
	-chown rje /etc/rjeproto

clean:
	-rm -f rjeproto
