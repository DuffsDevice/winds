#include "_type/type.h"
#include <nds/bios.h>

_length SCREEN_WIDTH = 256;
_length SCREEN_HEIGHT = 192;

void* operator new(size_t size)		{	return malloc(size);	}
void operator delete(void *p)		{	free(p);				}
void* operator new[](size_t size)	{	return malloc(size);	}
void operator delete[](void *p)		{	free(p);				}

_map<_align,string> align2string = {
	{ _align::left , "left" },
	{ _align::center , "center" },
	{ _align::right , "right" }
};

_map<_valign,string> valign2string = {
	{ _valign::top , "top" },
	{ _valign::middle , "middle" },
	{ _valign::bottom , "bottom" }
};

_map<string,_align> string2align = {
	{ "left" , _align::left },
	{ "center" , _align::center },
	{ "right" , _align::right }
};

_map<string,_valign> string2valign = {
	{ "top" , _valign::top },
	{ "middle" , _valign::middle },
	{ "bottom" , _valign::bottom }
};

// Casts for _margin and _padding
_border::operator _padding(){ return static_cast<_padding>(*this); }
_border::operator _margin(){ return static_cast<_margin>(*this); }


// Implementations
namespace DSWindows
{
	_char libnds2key[12] = { 1 , 2 , 3 , 4 , 5 , 6 , 7 , 14 , 15 , 16 , 17 , 18 };
	
	bool isHardwareKey( _char key )
	{
		return ( ( key > 0 && key < 8 ) || ( key > 13 && key < 19 ) );
	}
}

void trim( string& str , const char* delims , bool front , bool back )
{
	size_t endpos = back ? str.find_last_not_of(delims) : string::npos;
	if( string::npos != endpos )
	{
		str = str.substr( 0, endpos+1 );
	}
	size_t startpos = front ? str.find_first_not_of(delims) : string::npos;
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

int string2int( const _char *p ){
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

const _char numbers[37] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

string int2string( _int val , _u8 zeroFill , _u8 numbersystem )
{
	_char result[32]; // string which will contain the number
	int i = 31;
	
	do{
		int remainder;
		swiDivMod( val , numbersystem , &val , &remainder );
		result[--i] = numbers[remainder];
	}while( val );
	
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