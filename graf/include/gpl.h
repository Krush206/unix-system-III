#define TEXLEN     256
#define MAXLINES   256
#define LINES      0
#define MOVE       1
#define TEXT       2
#define ARCS       3
#define ALPHA      4
#define COMMENT    15
#define LIMIT      32768

union cmdword{
	struct { int pntw;
	} cma;
	struct { char cmbyte1,cmbyte2;
	} cmb;
	struct fbits {
		unsigned cm  : 4;  /*  command  */
		unsigned cn  : 12; /*  count of words in cmd  */
	} cmc;
};
union styleword{
	struct { char stbyte1,stbyte2;
	} sta;
	struct sbits{
		unsigned col  : 8;  /*  color  */
		unsigned wt   : 4;  /*  weight  */
		unsigned st   : 4;  /*  style  */
	} stb;
	struct sbits1{
		unsigned  col : 8;  /*  color  */
		unsigned  fon : 8;  /*  font   */
	} stc;
};
union szorient{
	struct { char szbyte1,szbyte2;
	} sza;
	struct tbits{
		unsigned ts : 8; /*  text size  */
		unsigned tr : 8; /*  text rotation  */
	} szb;
};
struct command {
	int cmd,cnt,color,weight,style,font,trot,tsiz;
	int *aptr,array[MAXLINES * 2];
	char *tptr,text[TEXLEN];
};
