#include "_type/type.h"

map<_align,string> align2string = {
	{ left , "left" },
	{ center , "center" },
	{ right , "right" }
};

map<_valign,string> valign2string = {
	{ top , "top" },
	{ middle , "middle" },
	{ bottom , "bottom" }
};

map<string,_align> string2align = {
	{ "left" , left },
	{ "center" , center },
	{ "right" , right }
};

map<string,_valign> string2valign = {
	{ "top" , top },
	{ "middle" , middle },
	{ "bottom" , bottom }
};

_u8 libnds2key[12] = { 1 , 2 , 3 , 4 , 5 , 6 , 7 , 14 , 15 , 16 , 17 , 18 };