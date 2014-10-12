#include <_type/type.h>
#include <nds/bios.h>
#include <stdio.h>

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


_toStr<_align> align2string = {
	{ _align::left , "left" },
	{ _align::center , "center" },
	{ _align::right , "right" }
};

_toStr<_valign> valign2string = {
	{ _valign::top , "top" },
	{ _valign::middle , "middle" },
	{ _valign::bottom , "bottom" }
};

_fromStr<_align> string2align = {
	{ "left" , _align::left },
	{ "center" , _align::center },
	{ "right" , _align::right }
};

_fromStr<_valign> string2valign = {
	{ "top" , _valign::top },
	{ "middle" , _valign::middle },
	{ "bottom" , _valign::bottom }
};

_toStr<_dimension> dimension2string = {
	{ _dimension::horizontal , "horizontal" },
	{ _dimension::vertical , "vertical" }
};

_fromStr<_dimension> string2dimension = {
	{ "horizontal" , _dimension::horizontal },
	{ "vertical" , _dimension::vertical }
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

void trim( string& str , _literal delims , bool front , bool back )
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
	_char result[34]; // string which will contain the number
	
	// Check if negative
	bool valNegative = val < 0;
	val = valNegative ? -val : val;
	
	// pos will be the current position in the array of chars
	_u8 pos = 32; // we start at 32, because we need index '33' for the ending \0 character
	
	// Reduce the numbers of zeros to write when we need an extra '-' to index the sign
	if( zeroFill && valNegative )
		zeroFill--;
	
	// We work from right to left
	do{
		int remainder;
		
		// We divide by the numbersystem
		swiDivMod( val , numbersystem , &val , &remainder );
		// -> The remainder is the number we will write
		// -> The next round we work with the divided result!
		
		result[pos] = numbers[remainder];
		
		// Step one to the left
		pos--;
	}while( val != 0 );
	
	// Check if we have to fill up with leading zeros
	if( zeroFill )
	{
		_u8 lettersWrote = 32 - pos;
		
		// Check if we havent reached the index until
		// which we have to fill up with zeros
		while( zeroFill > lettersWrote )
		{
			// Fill index with zero
			result[pos] = '0';
			
			// Step one to the left
			pos--;
			
			// Increase letters that are already written down
			lettersWrote++;
		}
	}
	
	if( valNegative )
	{
		// Fill index with minus-sign
		result[pos] = '-';
		
		// Step one to the left
		pos--;
	}
	
	result[33] = 0; // Delimiter
	
	// Return the index we wrote to last
	return &result[pos+1];
}

int countDecimals( _s32 value , _u8 numbersystem )
{
	int i = value < 0; // Start with one letter offset if we a sign
	
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

string fmt2string( _literal format , ... )
{
	if( !format )
		return "";
	
	char output[256];
	va_list args;								// Declare a va_list type variable
	va_start( args , format );					// Initialise the va_list variable with the ... after 'format'
	vsnprintf( output , 256 , format , args );	// Forward the '...' to vsnprintf
	va_end( args );								// Clean up the va_list
	
	return output;
}

string vfmt2string( _literal format , va_list args )
{
	if( !format )
		return "";
	
	char output[256];

    // Forward the '...' to vsnprintf
    vsnprintf( output , 256 , format , args );
	
	return output;
}