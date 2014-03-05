#include "_type/type.language.h"

_toStr<_language> language2string = {
	{ _language::japanese , "japanese" } ,
	{ _language::english , "english" } ,
	{ _language::french , "french" } ,
	{ _language::german , "german" } ,
	{ _language::italian , "italian" } ,
	{ _language::spanish , "spanish" } ,
	{ _language::chinese , "chinese" } ,
	{ _language::neutral , "neutral" }
};

_fromStr<_language>	string2language = {
	{ "japanese" , _language::japanese } ,
	{ "english" , _language::english } ,
	{ "french" , _language::french } ,
	{ "german" , _language::german } ,
	{ "italian" , _language::italian } ,
	{ "spanish" , _language::spanish } ,
	{ "chinese" , _language::chinese } ,
	{ "neutral" , _language::neutral }
};