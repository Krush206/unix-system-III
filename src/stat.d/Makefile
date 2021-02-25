I =	../../include
MAINS = abs.c af.c bar.c bucket.c ceil.c cor.c cusum.c exp.c floor.c\
	gamma.c gas.c hilo.c hist.c label.c\
	list.c log.c lreg.c mean.c mod.c\
	pair.c pie.c plot.c point.c power.c prime.c prod.c qsort.c rand.c\
	rank.c root.c round.c siline.c subset.c sin.c title.c total.c var.c
BASIC =	s.h s.a
GRAPHICS = plot.h $(I)/gsl.h $(LIB)/glib.a
HDR =	ae.h title.h plot.h s.h 
LIB =	../../lib
LIBS =	ctos.c doubcmp.c filbuf.c getdoub.c getint.c\
	getlabel.c getline.c getx0y0.c minmax.c pow1.c scale.c
LIBAE =	aeeval.c aeparse.c
BIN =	../../bin
LSDIR = ls -l *.h *.c | pr -h "  stat source files  "

stat all: $(MAINS) $(BASIC) $(GRAPHICS)
	make BIN=$(BIN) `echo $(MAINS) | sed "s/\.c//g"`; touch stat

abs:	abs.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
af:	af.c $(BASIC) ae.h
	BIN=$(BIN) ./cco $@; touch $@
bar:	bar.c $(BASIC) $(GRAPHICS)
	BIN=$(BIN) ./cco $@; touch $@
bucket:	bucket.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
ceil:	ceil.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
cor:	cor.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
cusum:	cusum.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
exp:	exp.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
floor:	floor.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
gamma:	gamma.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
gas:	gas.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
hilo:	hilo.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
hist:	hist.c $(BASIC) $(GRAPHICS)
	BIN=$(BIN) ./cco $@; touch $@
label:	label.c $(BASIC) $(GRAPHICS)
	BIN=$(BIN) ./cco $@; touch $@
list:	list.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
log:	log.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
lreg:	lreg.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
mean:	mean.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
mod:	mod.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
pair:	pair.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
pie:	pie.c $(BASIC) $(GRAPHICS)
	BIN=$(BIN) ./cco $@; touch $@
plot:	plot.c $(BASIC) $(GRAPHICS)
	BIN=$(BIN) ./cco $@; touch $@
point:	point.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
power:	power.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
prime:	prime.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
prod:	prod.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
qsort:	qsort.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
rand:	rand.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
rank:	rank.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
root:	root.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
round:	round.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
siline:	siline.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
sin:	sin.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
subset:	subset.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
title:	title.c $(BASIC) $(GRAPHICS)
	BIN=$(BIN) ./cco $@; touch $@
total:	total.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@
var:	var.c $(BASIC)
	BIN=$(BIN) ./cco $@; touch $@

s.h:	$(I) title.h
	touch s.h

s.a::	s.h; ./cca $(LIBAE)
s.a::	$(I)/errpr.h; ./cca filbuf.c pow1.c
s.a::	$(LIBS) $(LIBAE); ./cca $?

listings:	$(MAINS) $(HDR) $(LIBS) $(LIBAE)
	$(LSDIR)
	print $?
	touch listings

$(GRAPHICS):
		@:
