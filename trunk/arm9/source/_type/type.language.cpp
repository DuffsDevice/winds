#include "_type/type.language.h"

_array<_language,8> id2language = {
	"ja" ,
	"en" ,
	"fr" ,
	"de" ,
	"it" ,
	"es" ,
	"zh" ,
	"--"
};

_converter<_language,_u8> language2id = {
	{ "ja" , 0 } ,
	{ "en" , 1 } ,
	{ "fr" , 2 } ,
	{ "de" , 3 } ,
	{ "it" , 4 } ,
	{ "es" , 5 } ,
	{ "zh" , 6 } ,
	{ "--" , 7 }
};