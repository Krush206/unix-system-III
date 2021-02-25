dqsrje:
	cd dqsrje.d; make -f dqsrje.mk $(ARGS)
	cd dqsrje.d; make -f dqsrje.mk clobber

kmcrje:
	cd kmcrje.d; make -f kmcrje.mk $(ARGS)
	cd kmcrje.d; make -f kmcrje.mk clobber

install:
	eval make -f rje.mk `/usr/src/arglist + $(ARGS)`

clobber:
