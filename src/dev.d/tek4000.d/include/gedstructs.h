#include <sgtty.h>
struct control{
	short change; /* TRUE when display buffer has been altered */
	short textf; /* TRUE when text is to be drawn */
	double wratio; /* window ratio: univ units per screen units */
	struct area w;  /*  window low x,y and high x,y  */
};
struct tekterm{
	int curls,curlw,curmode;
	struct sgttyb cook;
	struct sgttyb raw;
	int alphax,alphay;
};
