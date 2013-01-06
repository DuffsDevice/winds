#include "_type/type.h"
#include <nds/bios.h>

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

_s32 mid( _s32 a , _s32 b , _s32 c ){
	_s32 mi = min( a , min( b , c ) );
	_s32 ma = max( a , max( b , c ) );
	if( a == ma ){
		if( b == mi )
			return c;
		return b;
	}
	if( b == ma ){
		if( a == mi )
			return c;
		return a;
	}
	else{
		if( b == mi )
			return a;
		return b;
	}
}

int string2int( const char *p ){
    int x = 0;
    bool neg = false;
    if (*p == '-') {
        neg = true;
        ++p;
    }
    while (*p >= '0' && *p <= '9') {
        x = (x*10) + (*p - '0');
        ++p;
    }
    if (neg) {
        x = -x;
    }
    return x;
}

const char numbers[37] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

string int2string( _int val , _u8 zeroFill , _u8 numbersystem )
{
	char result[32]; // string which will contain the number
	int i = 31;
	while( val != 0 )
	{
		int remainder;
		swiDivMod( val , numbersystem , &val , &remainder );
		result[--i] = numbers[remainder];
	}
	
	if( zeroFill )
		while( i > 31 - zeroFill )
			result[--i] = '0';
	
	result[31] = 0; // Delimiter
	
	return &result[i];
}

int countDecimals( _s32 value , _u8 numbersystem )
{
	int i = 0;
	switch( numbersystem )
	{
		case 0:
		case 1:
			i = 1;
			break;
		case 2:
			while( value != 0 )
			{
				i++;
				value >>= 1;
			}
			break;
		case 4:
			while( value != 0 )
			{
				i++;
				value >>= 2;
			}
			break;
		case 8:
			while( value != 0 )
			{
				i++;
				value >>= 3;
			}
			break;
		case 16:
			while( value != 0 )
			{
				i++;
				value >>= 4;
			}
			break;
		case 32:
			while( value != 0 )
			{
				i++;
				value >>= 5;
			}
			break;
		default:
			while( value != 0 )
			{
				i++;
				value /= numbersystem;
			}
			break;
	}
	
	return i;
}