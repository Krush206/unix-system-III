#
char C21[] = {"@(#)c21.c 1.70 80/01/18 14:41:49"}; /* sccs ident */
/*
 * C object code improver-- second part
 */

#include "c2.h"
#include <stdio.h>
#include <ctype.h>

#define NUSE 6
int ioflag;
int biti[NUSE] = {1,2,4,8,16,32};
int bitsize[4] = {0,8,16,32}; /* index by type codes */
int pos,siz; long f; /* for bit field communication */
struct node *uses[NUSE]; /* for backwards flow analysis */
char *lastrand; /* last operand of instruction */
struct node *bflow();
struct node *bicopt();
char *findcon();

redun3(p,split) register struct node *p; int split; {
/* check for 3 addr instr which should be 2 addr */
	if (OP3==((p->subop>>4)&0xF)) {
		if (split) splitrand(p);
		if (equstr(regs[RT1],regs[RT3])
		  && (p->op==ADD || p->op==MUL || p->op==BIS || p->op==XOR)) {
			register char *t=regs[RT1]; regs[RT1]=regs[RT2]; regs[RT2]=t;
		}
		if (equstr(regs[RT2],regs[RT3])) {
			p->subop=(p->subop&0xF)|(OP2<<4); p->pop=0;
			lastrand=regs[RT2]; *regs[RT3]=0; return(1);
		}
	} return(0);
}

bmove() {
	register struct node *p, *lastp; register char *cp1,*cp2; register int r;
	refcount();
	for (p=lastp= &first; 0!=(p=p->forw); lastp=p);
	clearreg(); clearuse();
	for (p=lastp; p!= &first; p=p->back) {
	if (debug) {
		for (r=NUSE;--r>=0;) if (uses[r]) printf("%d: %s\n",r,uses[r]->code);
		printf("-\n");
	}
	if (OP3==((p->subop>>4)&0xF) && 0!=redun3(p,1)) {newcode(p); redunm++;}
	switch (p->op) {
	case LABEL: case DLABEL:
		for (r=NUSE; --r>=0;)
			if (uses[r]) p->ref=(struct node *) (((int)p->ref)|biti[r]);
		break;
	case CALLS:
		clearuse(); goto std;
	case 0:
		clearuse(); break;
	case SUB:
		if ((p->subop&0xF)!=LONG) goto std; cp1=p->code;
		if (*cp1++!='$') goto std; splitrand(p);
		if (equstr(regs[RT2],"fp") && !indexa(regs[RT1])) {/* address comp. */
			char buf[50]; cp2=buf; *cp2++='-'; 
			cp1=regs[RT1]+1; while (*cp2++= *cp1++); --cp2;
			cp1="(fp),"; while (*cp2++= *cp1++); --cp2;
			cp1=regs[RT3]; while (*cp2++= *cp1++);
			p->code=copy(buf); p->combop=T(MOVA,LONG); p->pop=0;
		} else if (*cp1++=='-' && 0<=(r=getnum(cp1))) {
			p->op=ADD; p->pop=0; *--cp1='$'; p->code=cp1;
		} goto std;
	case ADD:
		if ((p->subop&0xF)!=LONG) goto std; cp1=p->code;
		if (*cp1++!='$') goto std; splitrand(p);
		if (equstr(regs[RT2],"fp") && !indexa(regs[RT1])) {/* address comp. */
			cp2=cp1-1; cp1=regs[RT1]+1; while (*cp2++= *cp1++); --cp2;
			cp1="(fp)"; while (*cp2++= *cp1++); *--cp2=',';
			p->combop=T(MOVA,LONG); p->pop=0;
		} else if (*cp1++=='-' && 0<=(r=getnum(cp1))) {
			p->op=SUB; p->pop=0; *--cp1='$'; p->code=cp1;
		}
	case CASE:
	default: std:
		p=bflow(p); break;
	case JSB:
		if (equstr(p->code,"mcount")) {uses[0]=p; regs[0][0]= -1;}
		goto std;
	case JBR: case JMP:
		clearuse();
		if (p->subop==RET || p->subop==RSB) {uses[0]=p; regs[0][0]= -1; break;}
		/* fall through */
	case CBR:
		if (p->ref->ref!=0) for (r=NUSE;--r>=0;)
			if (biti[r] & (int)p->ref->ref) {uses[r]=p; regs[r][0]= -1;}
	case EROU: case JSW:
	case TEXT: case DATA: case BSS: case ALIGN: case WGEN: case END: ;
	}
	}
	for (p= &first; p!=0; p=p->forw)
		if (p->op==LABEL || p->op==DLABEL) p->ref=0;	/* erase our tracks */
}

rmove()
{
	register struct node *p, *lastp;
	register int r;
	int r1;

	clearreg();
	for (p=first.forw; p!=0; p = p->forw) {
	lastp=p;
	if (debug) {
		for (r=0; r<NREG; r++)
			if (regs[r][0]) {
				r1=regs[r][0];
				printf("%d: %d%d %s\n", r, r1&0xF, r1>>4, regs[r]+1);
			}
		printf("-\n");
	}
	switch (p->op) {

	case CVT:
		splitrand(p); goto mov;

	case MOV:
		splitrand(p);
		if ((r = findrand(regs[RT1],p->subop)) >= 0) {
			if (r == isreg(regs[RT2]) && p->forw->op!=CBR) {
				delnode(p); redunm++; break;
			}
		}
	mov:
		repladdr(p);
		r = isreg(regs[RT1]);
		r1 = isreg(regs[RT2]);
		dest(regs[RT2],p->subop);
		if (r >= 0)
			if (r1 >= 0) savereg(r1, regs[r]+1, p->subop);
			else savereg(r, regs[RT2], p->subop);
		else
			if (r1 >= 0) savereg(r1, regs[RT1], p->subop);
			else setcon(regs[RT1], regs[RT2], p->subop);
		break;

/* .rx,.wx */
	case MFPR:
	case COM:
	case NEG:
/* .rx,.wx or .rx,.rx,.wx */
	case ADD:
	case SUB:
	case BIC:
	case BIS:
	case XOR:
	case MUL:
	case DIV:
	case ASH:
	case MOVZ:
/* .rx,.rx,.rx,.wx */
	case EXTV:
	case EXTZV:
	case INSV:
		splitrand(p);
		repladdr(p);
		dest(lastrand,p->subop);
		if (p->op==INSV) ccloc[0]=0;
		break;

/* .mx or .wx */
	case CLR:
	case INC:
	case DEC:
		splitrand(p);
		dest(lastrand,p->subop);
		if (p->op==CLR)
			if ((r = isreg(regs[RT1])) >= 0)
				savereg(r, "$0", p->subop);
			else
				setcon("$0", regs[RT1], p->subop);
		break;

/* .rx */
	case TST:
	case PUSH:
		splitrand(p);
		lastrand=regs[RT1+1]; /* fool repladdr into doing 1 operand */
		repladdr(p);
		if (p->op==TST && equstr(lastrand=regs[RT1], ccloc+1)
		  && equtype(ccloc[0],p->subop) &&!source(lastrand)) {
			delnode(p); p = p->back; nrtst++; nchange++;
		}
		setcc(lastrand,p->subop);
		break;

/* .rx,.rx,.rx */
	case CASE:
	case MOVC3:
/* .rx,.rx */
	case MTPR:
	case CALLS:
	case CMP:
	case BIT:
		splitrand(p);
		/* fool repladdr into doing right number of operands */
		if (p->op==CASE || p->op==MOVC3) lastrand=regs[RT4];
		else lastrand=regs[RT3];
		repladdr(p);
		if (p->op==CALLS || p->op==MOVC3) clearreg();
		if (p->op==BIT) bitopt(p);
		ccloc[0]=0; break;

	case CBR:
		if (p->subop>=JBC) {/* 2 operands can be optimized */
			splitrand(p); lastrand=regs[RT3]; repladdr(p);
		}
		ccloc[0] = 0;
		break;

	case JBR:
		redunbr(p);

/* .wx,.bb */
	case SOB:

	default:
		clearreg();
	}
	}
}

char *
byondrd(p) register struct node *p; {
/* return pointer to register which is "beyond last read/modify operand" */
	if (OP2==(p->subop>>4)) return(regs[RT3]);
	switch (p->op) {
		case MFPR:
		case JSB:
		case PUSHA:
		case TST: case INC: case DEC: case PUSH:	return(regs[RT2]);
		case MTPR:
		case BIT: case CMP: case CALLS:				return(regs[RT3]);
		case CASE: case MOVC3:						return(regs[RT4]);
	}
	return(lastrand);
}

struct node *
bflow(p) register struct node *p; {
	register char *cp1,*cp2,**preg; register int r;
	int flow= -1;
	struct node *olduse;
	splitrand(p);
	if (0<=(r=isreg(lastrand)) && r<NUSE && p->op!=PUSH &&
	  uses[r]==p->forw && p->subop && equtype(regs[r][0],p->subop)) {
		if (regs[r][1]!=0) {/* send directly to destination */
			if (p->op==INC || p->op==DEC) {
				if (p->op==DEC) p->op=SUB; else p->op=ADD;
				p->subop=(OP2<<4)+(p->subop&0xF); /* use 2 now, convert to 3 later */
				p->pop=0;
				cp1=lastrand; cp2=regs[RT2]; while (*cp2++= *cp1++); /* copy reg */
				cp1=lastrand; *cp1++='$'; *cp1++='1'; *cp1=0;
			}
			cp1=regs[r]+1; cp2=lastrand;
			if (OP2==(p->subop>>4)) {/* use 3 operand form of instruction */
				p->pop=0;
				p->subop += (OP3-OP2)<<4; lastrand=cp2=regs[RT3];
			}
			while (*cp2++= *cp1++);
			if (p->op==MOVA && p->forw->op==PUSH) {
				p->op=PUSHA; *regs[RT2]=0; p->pop=0;
			} else if (p->op==MOV && p->forw->op==PUSH) {
				p->op=PUSH ; *regs[RT2]=0; p->pop=0;
			}
			delnode(p->forw);
			redun3(p,0);
			newcode(p); redunm++; flow=r;
		} else if (p->op==MOV && p->forw->op!=EXTV && p->forw->op!=EXTZV) {
			/* superfluous fetch */
			int nmatch;
			char src[20]; cp2=src; cp1=regs[RT1]; while (*cp2++= *cp1++);
			splitrand(p->forw); lastrand=byondrd(p->forw); nmatch=0;
			for (preg=regs+RT1;*preg!=lastrand;preg++) if (r==isreg(*preg)) {
				cp2= *preg; cp1=src; while (*cp2++= *cp1++); ++nmatch;
			}
			if (nmatch==1) {
				if (OP2==(p->forw->subop>>4) && equstr(src,regs[RT2])) {
					p->forw->pop=0;
					p->forw->subop += (OP3-OP2)<<4; cp1=regs[RT3];
					*cp1++='r'; *cp1++=r+'0'; *cp1=0;
				}
				delnode(p); p=p->forw;
				redun3(p,0);
				newcode(p); redunm++; flow=r;
			}
		}
	}
	/* adjust 'lastrand' past any 'read' or 'modify' operands. */
	lastrand=byondrd(p);
	/* a 'write' clobbers the register. */
	if (0<=(r=isreg(lastrand)) && r<NUSE
	|| OP2==(p->subop>>4) && 0<=(r=isreg(regs[RT2])) && r<NUSE && uses[r]==0) {
		/* writing a dead register is useless, but watch side effects */
		if (uses[r]==0) {/* no direct uses, check for use of condition codes */
			register struct node *q=p;
			while ((q=nonlab(q->forw))->combop==JBR) q=q->ref;	/* cc unused, unchanged */
			if (q->op!=CBR) {/* ... and destroyed */
				preg=regs+RT1;
				while (cp1= *preg++) {
					if (cp1==lastrand) {redunm++; delnode(p); return(p->forw);}
					if (source(cp1) || equstr(cp1,lastrand)) break;
				}
			}
		}
		flow=r;
	}
	if ((r=flow)>=0) {olduse=uses[r]; uses[r]=0; *(short *)(regs[r])=0;}
	if (p->op==BIC) {p=bicopt(p); splitrand(p); lastrand=byondrd(p);}
	if (p->op==BIS) {bixprep(p,JBSS);           lastrand=byondrd(p);}
	/* now look for 'read' or 'modify' (read & write) uses */
	preg=regs+RT1; 
	while (*(cp1= *preg++)) {
		/* check for  r  */
		if (lastrand!=cp1 && 0<=(r=isreg(cp1)) && r<NUSE && uses[r]==0) {
			uses[r]=p; cp2=regs[r]; *cp2++=p->subop;
			if (p->op==MOV || p->op==PUSH) {
				if (p->op==PUSH) cp1="-(sp)";
				else {
					cp1=regs[RT2];
					if (0<=(r=isreg(cp1)) && r<NUSE) uses[r]=olduse; /* reincarnation!! */
					/* as in  addl2 r0,r1;  movl r1,r0;  ret  */
				}
				while (*cp2++= *cp1++);
			} else *cp2++=0;
			continue;
		}
		/* check for (r),(r)+,-(r),[r] */
		do if (*cp1=='(' || *cp1=='[') {/* get register number */
			char t;
			cp2= ++cp1; while (*++cp1!=')' && *cp1!=']'); t= *cp1; *cp1=0;
			if (0<=(r=isreg(cp2)) && r<NUSE && (uses[r]==0 || uses[r]==p)) {
				uses[r]=p; regs[r][0]=(*--cp2=='[' ? OPX<<4 : OPB<<4);
			}
			*cp1=t;
		} while (*++cp1);
	}
	/* pushax or movax possibility? */
	cp1=regs[RT1];
	if (*cp1++=='$'
	  && (*cp1=='_' || *cp1=='L' || (*cp1++=='v' && *cp1=='.'))
	  && natural(regs[RT1])) {
		if (p->combop==T(MOV,LONG)) {
			if (regs[RT1][1]=='L' && 0!=(p->labno=getnum(regs[RT1]+2))) {
				cp1=p->code; while (*cp1++!=','); p->code= --cp1;
			}
			p->combop=T(MOVA,LONG); ++p->code; p->pop=0;
		} else if (p->combop==T(PUSH,LONG)) {
			p->combop=T(PUSHA,LONG); ++p->code; p->pop=0;
		} else if ((p->combop&0xFFFF)==T(ADD,U(LONG,OP3))
				 && 0<=(r=isreg(regs[RT2]))) {
			cp1=cp2=p->code; ++cp1;
			do *cp2++= *cp1; while (*cp1++!=','); cp2[-1]='[';
			do *cp2++= *cp1; while (*cp1++!=','); cp2[-1]=']';
			if (!equstr(regs[RT3],"-(sp)")) p->combop=T(MOVA,BYTE);
			else {p->combop=T(PUSHA,BYTE); *cp2=0;}
			if (uses[r]==0) {uses[r]=p; regs[r][0]=OPX<<4;}
			p->pop=0;
		}
	}
	return(p);
}

ispow2(n) register long n; {/* -1 -> no; else -> log to base 2 */
	register int log;
	if (n==0 || n&(n-1)) return(-1); log=0;
	for (;;) {n >>= 1; if (n==0) return(log); ++log; if (n== -1) return(log);}
}

bitopt(p) register struct node *p; {
	/* change "bitx $<power_of_2>,a" followed by JEQ or JNE
	/* into JBC or JBS.  watch out for I/O registers. (?)
	/* assumes that 'splitrand' has already been called.
	*/
	register char *cp1,*cp2; int b;
	cp1=regs[RT1]; cp2=regs[RT2];
	if (*cp1++!='$' || !okio(cp2) || p->forw->op!=CBR || p->forw->subop&-2 ||
		0>(b=ispow2(getnum(cp1))) ||
		p->subop!=BYTE && (source(cp2) || indexa(cp2))) return;
	if (b>=bitsize[p->subop]) {/* you dummy! */
		if (source(cp2)) {/* side effect: auto increment or decrement */
			p->pop=0;
			p->op=TST; --cp1; while (*cp1++= *cp2++);
			regs[RT2][0]=0; newcode(p);
		} else delnode(p);
		p = p->forw;
		if (p->subop==JEQ) {p->combop=JBR; p->pop=0;}
		else delnode(p);
		nchange++; nbj++; return;
	}
	if (cp1=p->forw->code) {/* destination is not an internal label */
		cp2=regs[RT3]; while (*cp2++= *cp1++);
	}
	if (b==0 && (p->subop==LONG || !indexa(regs[RT2]))) {/* JLB optimization, ala BLISS */
		cp2=regs[RT1]; cp1=regs[RT2]; while (*cp2++= *cp1++);
		cp2=regs[RT2]; cp1=regs[RT3]; while (*cp2++= *cp1++);
		*(regs[RT3])=0; p->forw->subop += JLBC-JBC;
		p->forw->pop=0;
	} else {
		cp1=regs[RT1]+1;
		if (b>9) *cp1++= b/10 +'0'; *cp1++= b%10 +'0'; *cp1=0; /* $<bit_number> */
	}
	nbj++; newcode(p); p->combop = p->forw->combop+((JBC-JEQ)<<8);
	p->labno = p->forw->labno; delnode(p->forw);
	p->pop=0;
}

isfield(n) register long n; {/* -1 -> no; else -> position of low bit */
	register int pos; register long t;
	t= ((n-1)|n) +1;
	if (n!=0 && (0==t || 0==n || 0<=ispow2(t))) {
		pos=0; while(!(n&1)) {n >>= 1; ++pos;} return(pos);
	} else return(-1);
}

bixprep(p,bix) register struct node *p; {
/* initial setup, single-bit checking for bisopt, bicopt.
/* return: 0->don't bother any more; 1->worthwhile trying
*/
	register char *cp1,*cp2;
	splitrand(p); cp1=regs[RT1]; cp2=regs[RT2];
	if (*cp1++!='$' || 0>(pos=isfield(f=getnum(cp1)))
	  || !okio(cp2) || indexa(cp2) || source(cp2) || !okio(lastrand)) return(0);
	f |= f-1; if (++f==0) siz=32-pos; else siz=ispow2(f)-pos;
	if (siz==1 && pos>5 && (p->subop>>4)==OP2 && (p->subop&0xF)!=BYTE
	  && pos<bitsize[p->subop&0xF]) {
		p->ref = insertl(p->forw); p->combop = CBR | (bix<<8);
		p->pop=0;
		p->labno = p->ref->labno;
		if (pos>9) {*cp1++= pos/10 +'0'; pos %= 10;}
		*cp1++=pos+'0'; *cp1=0; newcode(p); nbj++; return(0);
	}
	return(1);
}


struct node *
bicopt(p) register struct node *p; {
/* use field operations or MOVZ if possible.  done as part of 'bflow'.
*/
	register char *cp1,*cp2; int r;
	char src[50];
	if (!bixprep(p,JBCC)) return(p);
	if (f==0) {/* the BIC isolates low order bits */
		siz=pos; pos=0;
		if ((p->subop&0xF)==LONG && *(regs[RT2])!='$') {/* result of EXTZV is long */
			/* save source of BICL in 'src' */
			cp1=regs[RT2]; cp2=src; while (*cp2++= *cp1++);
			if (p->back->op==ASH) {/* try for more */
				splitrand(p->back); cp1=regs[RT1]; cp2=regs[RT3];
				if (*cp1++=='$' && *(regs[RT2])!='$' && !indexa(regs[RT2])
				  && 0>(f=getnum(cp1)) && equstr(src,cp2)
				  && 0<=(r=isreg(cp2)) && r<NUSE) {/* a good ASH */
					pos -= f; cp1=regs[RT2]; cp2=src; while (*cp2++= *cp1++);
					delnode(p->back);
				}
			}
			if (p->back->op==CVT || p->back->op==MOVZ) {/* greedy, aren't we? */
				splitrand(p->back); cp1=regs[RT1]; cp2=regs[RT2];
				if (equstr(src,cp2) && okio(cp1) && !indexa(cp1)
				  && 0<=(r=isreg(cp2)) && r<NUSE
				  && bitsize[p->back->subop&0xF]>=(pos+siz)
				  && bitsize[p->back->subop>>4]>=(pos+siz)) {/* good CVT */
					cp1=regs[RT1]; cp2=src; while (*cp2++= *cp1++);
					delnode(p->back);
				}
			}
			/* 'pos', 'siz' known; source of field is in 'src' */
			splitrand(p); /* retrieve destination of BICL */
			if (siz==8 && pos==0) {
				p->combop = T(MOVZ,U(BYTE,LONG));
				sprintf(line,"%s,%s",src,lastrand);
			} else {
				p->combop = T(EXTZV,LONG);
				sprintf(line,"$%d,$%d,%s,%s",pos,siz,src,lastrand);
			}
			p->pop=0;
			p->code = copy(line); nfield++; return(p);
		}/* end EXTZV possibility */
	}/* end low order bits */
/* unfortunately, INSV clears the condition codes, thus cannot be used */
/*	else {/* see if BICL2 of positive field should be INSV $0 */
/*		if (p->subop==(LONG | (OP2<<4)) && 6<=(pos+siz)) {
/*			p->combop = INSV;
/*			sprintf(line,"$0,$%d,$%d,%s",pos,siz,lastrand);
/*			p->code = copy(line); nfield++; return(p);
/*		}
/*	}
*/
	return(p);
}

jumpsw()
{
	register struct node *p, *p1;
	register t;
	int nj;

	t = 0;
	nj = 0;
	for (p=first.forw; p!=0; p = p->forw)
		p->seq = ++t;
	for (p=first.forw; p!=0; p = p1) {
		p1 = p->forw;
		if (p->op == CBR && p1->op==JBR && p->ref && p1->ref
		 && abs(p->seq - p->ref->seq) > abs(p1->seq - p1->ref->seq)) {
			if (p->ref==p1->ref)
				continue;
			p->subop = revbr[p->subop];
			p->pop=0;
			t = p1->ref;
			p1->ref = p->ref;
			p->ref = t;
			t = p1->labno;
			p1->labno = p->labno;
			p->labno = t;
			nrevbr++;
			nj++;
		}
	}
	return(nj);
}

addsob()
{
	register struct node *p, *p1, *p2, *p3;

	for (p = &first; (p1 = p->forw)!=0; p = p1) {
	if (p->combop==T(DEC,LONG) && p1->op==CBR) {
		if (abs(p->seq - p1->ref->seq) > 12) continue;
		if (p1->subop==JGE || p1->subop==JGT) {
			if (p1->subop==JGE) p->combop=SOBGEQ; else p->combop=SOBGTR;
			p->pop=0;
			p->labno = p1->labno; delnode(p1); nsob++;
		}
	} else if (p->combop==T(INC,LONG)) {
		if (p1->op==LABEL && p1->refc==1 && p1->forw->combop==T(CMP,LONG)
		  && (p2=p1->forw->forw)->combop==T(CBR,JLE)
		  && (p3=p2->ref->back)->combop==JBR && p3->ref==p1
		  && p3->forw->op==LABEL && p3->forw==p2->ref) {
			/* change	INC LAB: CMP	to	LAB: INC CMP */
			p->back->forw=p1; p1->back=p->back;
			p->forw=p1->forw; p1->forw->back=p;
			p->back=p1; p1->forw=p;
			p1=p->forw;
			/* adjust beginning value by 1 */
				p2=alloc(sizeof first); p2->combop=T(DEC,LONG);
				p2->pop=0;
				p2->forw=p3; p2->back=p3->back; p3->back->forw=p2;
				p3->back=p2; p2->code=p->code; p2->labno=0;
		}
		if (p1->combop==T(CMP,LONG) && (p2=p1->forw)->op==CBR) {
			register char *cp1,*cp2;
			splitrand(p1); if (!equstr(p->code,regs[RT1])) continue;
			if (abs(p->seq - p2->ref->seq)>12) {/* outside byte displ range */
				if (p2->subop!=JLE) continue;
				p->combop=T(ACB,LONG);
				cp2=regs[RT1]; cp1=regs[RT2]; while (*cp2++= *cp1++); /* limit */
				cp2=regs[RT2]; cp1="$1"; while (*cp2++= *cp1++); /* increment */
				cp2=regs[RT3]; cp1=p->code; while (*cp2++= *cp1++); /* index */
				p->pop=0; newcode(p);
				p->labno = p2->labno; delnode(p2); delnode(p1); nsob++;
			} else if (p2->subop==JLE || p2->subop==JLT) {
				if (p2->subop==JLE) p->combop=AOBLEQ; else p->combop=AOBLSS;
				cp2=regs[RT1]; cp1=regs[RT2]; while (*cp2++= *cp1++); /* limit */
				cp2=regs[RT2]; cp1=p->code; while (*cp2++= *cp1++); /* index */
				p->pop=0; newcode(p);
				p->labno = p2->labno; delnode(p2); delnode(p1); nsob++;
			}
		}
	}
	}
}

abs(x)
{
	return(x<0? -x: x);
}

equop(p1, p2)
register struct node *p1;
struct node *p2;
{
	register char *cp1, *cp2;

	if (p1->combop != p2->combop)
		return(0);
	if (p1->op>0 && p1->op<MOV)
		return(0);
	if (p1->op==MOVA && p1->labno!=p2->labno) return(0);
	cp1 = p1->code;
	cp2 = p2->code;
	if (cp1==0 && cp2==0)
		return(1);
	if (cp1==0 || cp2==0)
		return(0);
	while (*cp1 == *cp2++)
		if (*cp1++ == 0)
			return(1);
	return(0);
}

delnode(p) register struct node *p; {
	p->back->forw = p->forw;
	p->forw->back = p->back;
}

decref(p)
register struct node *p;
{
	if (p && --p->refc <= 0) {
		nrlab++;
		delnode(p);
	}
}

struct node *
nonlab(ap)
struct node *ap;
{
	register struct node *p;

	p = ap;
	while (p && p->op==LABEL)
		p = p->forw;
	return(p);
}

clearuse() {
	register struct node **i;
	for (i=uses+NUSE; i>uses;) *--i=0;
}

clearreg() {
	register short **i;
	for (i=regs+NREG; i>regs;) **--i=0;
	conloc[0] = 0; ccloc[0] = 0;
}

savereg(ai, s, type)
register char *s;
{
	register char *p, *sp;

	sp = p = regs[ai];
	if (source(s)) /* side effects in addressing */
		return;
	/* if any indexing, must be parameter or local */
	/* indirection (as in "*-4(fp)") is ok, however */
	*p++ = type;
	while (*p++ = *s)
		if (*s=='[' || *s++=='(' && *s!='a' && *s!='f') {*sp = 0; return;}
}

dest(s,type)
register char *s;
{
	register int i;

	source(s); /* handle addressing side effects */
	if ((i = isreg(s)) >= 0) {
		*(short *)(regs[i]) = 0; /* if register destination, that reg is a goner */
		if (DOUBLE==(type&0xF) || DOUBLE==((type>>4)&0xF))
			*(short *)(regs[i+1]) = 0; /* clobber two at once */
	}
	for (i=NREG; --i>=0;)
		if (regs[i][1]=='*' && equstr(s, regs[i]+2))
			*(short *)(regs[i]) = 0; /* previous indirection through destination is invalid */
	while ((i = findrand(s,0)) >= 0) /* previous values of destination are invalid */
		*(short *)(regs[i]) = 0;
	if (!natural(s)) {/* wild store, everything except constants vanishes */
		for (i=NREG; --i>=0;) if (regs[i][1] != '$') *(short *)(regs[i]) = 0;
		conloc[0] = 0; ccloc[0] = 0;
	} else setcc(s,type); /* natural destinations set condition codes */
}

splitrand(p) struct node *p; {
/* separate operands at commas, set up 'regs' and 'lastrand' */
register char *p1, *p2; register char **preg;
preg=regs+RT1;
if (p1=p->code) while (*p1) {
	lastrand=p2= *preg++;
	while (*p1) if (','==(*p2++= *p1++)) {--p2; break;}
	*p2=0;
}
while (preg<(regs+RT1+5)) *(*preg++)=0;
}

compat(have, want) {
register int hsrc, hdst;
if (0==(want &= 0xF)) return(1); /* anything satisfies a wildcard want */
hsrc=have&0xF; if (0==(hdst=((have>>4)&0xF)) || hdst>=OP2) hdst=hsrc;
/* last term prevents floats, doubles from satisfying a request for an int */
return(hsrc>=want && hdst>=want && !((want>=FLOAT) ^ (hdst>=FLOAT)));
}

equtype(t1,t2) {return(compat(t1,t2) && compat(t2,t1));}

findrand(as, type)
char *as;
{
	register char **i;
	for (i = regs+NREG; --i>=regs;) {
		if (**i && equstr(*i+1, as) && compat(**i,type))
			return(i-regs);
	}
	return(-1);
}

isreg(s)
register char *s;
{
	if (*s++!='r' || !isdigit(*s++)) return(-1);
	if (*s==0) return(*--s-'0');
	if (*(s-1)=='1' && isdigit(*s++) && *s==0) return(10+*--s-'0');
	return(-1);
}

check()
{
	register struct node *p, *lp;

	lp = &first;
	for (p=first.forw; p!=0; p = p->forw) {
		if (p->back != lp)
			abort(-1);
		lp = p;
	}
}

source(ap)
char *ap;
{
	register char *p1, *p2;

	p1 = ap;
	p2 = p1;
	if (*p1==0)
		return(0);
	while (*p2++ && *(p2-1)!='[');
	if (*p1=='-' && *(p1+1)=='('
	 || *p1=='*' && *(p1+1)=='-' && *(p1+2)=='('
	 || *(p2-2)=='+') {
		while (*p1 && *p1++!='r');
		if (isdigit(*p1++))
			if (isdigit(*p1)) *(short *)(regs[10+*p1-'0'])=0;
			else *(short *)(regs[*--p1-'0'])=0;
		return(1);
	}
	return(0);
}

newcode(p) struct node *p; {
	register char *p1,*p2,**preg;
	preg=regs+RT1; p2=line;
	while (*(p1= *preg++)) {while (*p2++= *p1++); *(p2-1)=',';}
	*--p2=0;
	p->code=copy(line);
}

repladdr(p)
struct node *p;
{
	register r;
	register char *p1, *p2;
	char **preg; int nrepl;

	preg=regs+RT1; nrepl=0;
	while (lastrand!=(p1= *preg++))
		if (!source(p1) && 0<=(r=findrand(p1,p->subop))) {
			*p1++='r'; if (r>9) {*p1++='1'; r -= 10;} *p1++=r+'0'; *p1=0;
			nrepl++; nsaddr++;
		}
	if (nrepl) newcode(p);
}

/* movedat()
/* {
/* 	register struct node *p1, *p2;
/* 	struct node *p3;
/* 	register seg;
/* 	struct node data;
/* 	struct node *datp;
/* 
/* 	if (first.forw == 0)
/* 		return;
/* 	datp = &data;
/* 	for (p1 = first.forw; p1!=0; p1 = p1->forw) {
/* 		if (p1->op == DATA) {
/* 			p2 = p1->forw;
/* 			while (p2 && p2->op!=TEXT)
/* 				p2 = p2->forw;
/* 			if (p2==0)
/* 				break;
/* 			p3 = p1->back;
/* 			p1->back->forw = p2->forw;
/* 			p2->forw->back = p3;
/* 			p2->forw = 0;
/* 			datp->forw = p1;
/* 			p1->back = datp;
/* 			p1 = p3;
/* 			datp = p2;
/* 		}
/* 	}
/* 	if (data.forw) {
/* 		datp->forw = first.forw;
/* 		first.forw->back = datp;
/* 		data.forw->back = &first;
/* 		first.forw = data.forw;
/* 	}
/* 	seg = -1;
/* 	for (p1 = first.forw; p1!=0; p1 = p1->forw) {
/* 		if (p1->op==TEXT||p1->op==DATA||p1->op==BSS) {
/* 			if (p1->op == seg || p1->forw&&p1->forw->op==seg) {
/* 				p1->back->forw = p1->forw;
/* 				p1->forw->back = p1->back;
/* 				p1 = p1->back;
/* 				continue;
/* 			}
/* 			seg = p1->op;
/* 		}
/* 	}
/* }
*/

redunbr(p)
register struct node *p;
{
	register struct node *p1;
	register char *ap1;
	char *ap2;

	if ((p1 = p->ref) == 0)
		return;
	p1 = nonlab(p1);
	if (p1->op==TST) {
		splitrand(p1);
		savereg(RT2, "$0", p1->subop);
	} else if (p1->op==CMP)
		splitrand(p1);
	else
		return;
	if (p1->forw->op==CBR) {
		ap1 = findcon(RT1, p1->subop);
		ap2 = findcon(RT2, p1->subop);
		p1 = p1->forw;
		if (compare(p1->subop, ap1, ap2)) {
			nredunj++;
			nchange++;
			decref(p->ref);
			p->ref = p1->ref;
			p->labno = p1->labno;
			p->ref->refc++;
		}
	} else if (p1->op==TST && equstr(regs[RT1],ccloc+1) &&
			equtype(ccloc[0],p1->subop)) {
		p1=insertl(p1->forw); decref(p->ref); p->ref=p1; 
		nrtst++; nchange++;
	}
}

char *
findcon(i, type)
{
	register char *p;
	register r;

	p = regs[i];
	if (*p=='$')
		return(p);
	if ((r = isreg(p)) >= 0 && compat(regs[r][0],type))
		return(regs[r]+1);
	if (equstr(p, conloc))
		return(conval+1);
	return(p);
}

compare(op, acp1, acp2)
char *acp1, *acp2;
{
	register char *cp1, *cp2;
	register n1;
	int n2;	int sign;

	cp1 = acp1;
	cp2 = acp2;
	if (*cp1++ != '$' || *cp2++ != '$')
		return(0);
	n1 = 0; sign=1; if (*cp2=='-') {++cp2; sign= -1;}
	while (isdigit(*cp2)) {n1 *= 10; n1 += (*cp2++ - '0')*sign;}
	n2 = n1;
	n1 = 0; sign=1; if (*cp1=='-') {++cp1; sign= -1;}
	while (isdigit(*cp1)) {n1 *= 10; n1 += (*cp1++ - '0')*sign;}
	if (*cp1=='+')
		cp1++;
	if (*cp2=='+')
		cp2++;
	do {
		if (*cp1++ != *cp2)
			return(0);
	} while (*cp2++);
	cp1 = n1;
	cp2 = n2;
	switch(op) {

	case JEQ:
		return(cp1 == cp2);
	case JNE:
		return(cp1 != cp2);
	case JLE:
		return(((int)cp1) <= ((int)cp2));
	case JGE:
		return(((int)cp1) >= ((int)cp2));
	case JLT:
		return(((int)cp1) < ((int)cp2));
	case JGT:
		return(((int)cp1) > ((int)cp2));
	case JLO:
		return(cp1 < cp2);
	case JHI:
		return(cp1 > cp2);
	case JLOS:
		return(cp1 <= cp2);
	case JHIS:
		return(cp1 >= cp2);
	}
	return(0);
}

setcon(cv, cl, type)
register char *cv, *cl;
{
	register char *p;

	if (*cv != '$')
		return;
	if (!natural(cl))
		return;
	p = conloc;
	while (*p++ = *cl++);
	p = conval;
	*p++ = type;
	while (*p++ = *cv++);
}

equstr(p1, p2)
register char *p1, *p2;
{
	do {
		if (*p1++ != *p2)
			return(0);
	} while (*p2++);
	return(1);
}

setcc(ap,type)
char *ap;
{
	register char *p, *p1;

	p = ap;
	if (!natural(p)) {
		ccloc[0] = 0;
		return;
	}
	p1 = ccloc;
	*p1++ = type;
	while (*p1++ = *p++);
}

okio(p) register char *p; {/* 0->probable I/O space address; 1->not */
	if (ioflag && (!natural(p) || 0>getnum(p))) return(0);
	return(1);
}

indexa(p) register char *p; {/* 1-> uses [r] addressing mode; 0->doesn't */
	while (*p) if (*p++=='[') return(1);
	return(0);
}

natural(p)
register char *p;
{/* 1->simple local, parameter, global, or register; 0->otherwise */
	if (*p=='*' || *p=='(' || *p=='-'&&*(p+1)=='(' || *p=='$'&&getnum(p+1))
		return(0);
	while (*p++);
	p--;
	if (*--p=='+' || *p==']' || *p==')' && *(p-2)!='a' && *(p-2)!='f')
		return(0);
	return(1);
}
