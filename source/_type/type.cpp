#include "_type/type.h"

_length SCREEN_WIDTH = 256;
_length SCREEN_HEIGHT = 192;

void* operator new(size_t size)		{	return malloc(size);	}
void operator delete(void *p)		{	free(p);				}
void* operator new[](size_t size)	{	return malloc(size);	}
void operator delete[](void *p)		{	free(p);				}

map<_align,string> align2string = {
	{ _align::left , "left" },
	{ _align::center , "center" },
	{ _align::right , "right" }
};

map<_valign,string> valign2string = {
	{ _valign::top , "top" },
	{ _valign::middle , "middle" },
	{ _valign::bottom , "bottom" }
};

map<string,_align> string2align = {
	{ "left" , _align::left },
	{ "center" , _align::center },
	{ "right" , _align::right }
};

map<string,_valign> string2valign = {
	{ "top" , _valign::top },
	{ "middle" , _valign::middle },
	{ "bottom" , _valign::bottom }
};

_char libnds2key[12] = { 1 , 2 , 3 , 4 , 5 , 6 , 7 , 14 , 15 , 16 , 17 , 18 };

bool DSWindows::isHardwareKey( _char key )
{
	return ( ( key > 0 && key < 8 ) || ( key > 13 && key < 19 ) );
}

map<_gadgetType,string> gadgetType2string = {
	{ _gadgetType::button , "button" },
	{ _gadgetType::label , "label" },
	{ _gadgetType::checkbox , "checkbox" },
	{ _gadgetType::radiobox , "radiobox" },
	{ _gadgetType::textbox , "textbox" },
	{ _gadgetType::selectbox , "selectbox" },
	{ _gadgetType::selectitem , "selectitem" },
	{ _gadgetType::progressbar , "progressbar" },
	{ _gadgetType::keyboard , "keyboard" },
	{ _gadgetType::desktop , "desktop" },
	{ _gadgetType::fileobject , "fileobject" },
	{ _gadgetType::imagegadget , "imagegadget" },
	{ _gadgetType::scrollarea , "scrollarea" },
	{ _gadgetType::scrollbutton , "scrollbutton" },
	{ _gadgetType::scrollbar , "scrollbar" },
	{ _gadgetType::window , "window" },
	{ _gadgetType::screen , "screen" },
	{ _gadgetType::contextmenu , "contextmenu" },
	{ _gadgetType::_plain , "_plain" }
};

void trim( string& str )
{
	size_t endpos = str.find_last_not_of(" \n\r\t");
	if( string::npos != endpos )
	{
		str = str.substr( 0, endpos+1 );
	}
	size_t startpos = str.find_first_not_of(" \n\r\t");
	if( string::npos != startpos )
	{
		str = str.substr( startpos );
	}
}