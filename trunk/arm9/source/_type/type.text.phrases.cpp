#include "_type/type.text.phrases.h"
#include "_type/type.font.h"
#include "_type/type.system.h"

bool stringExtractor::processChar( const _char*& str , _u8& fontSize , const _font*& font )
{
	switch( (_escapeChar)*str )
	{
		case _escapeChar::changeFont:
		{
			const _font* ft = stringExtractor::fontChangePhrase( str );
			if( ft && ft->isValid() )
				font = ft;
			for( int i = 5 ; str[1] != 0 && i-- ; str++ );
			break;
		}
		case _escapeChar::changeFontColor:
			for( int i = 2 ; str[1] != 0 && i-- ; str++ );
			break;
		case _escapeChar::changeFontSize:
			fontSize = stringExtractor::sizeChangePhrase( str );
			if( str[1] )
				str++;
			break;
		default:
			return true;
	}
	
	return false;
}

bool stringExtractor::processChar( const _char*& str , _u8& fontSize , const _font*& font , _pixel& color )
{
	switch( (_escapeChar)*str )
	{
		case _escapeChar::changeFont:
		{
			const _font* ft = stringExtractor::fontChangePhrase( str );
			if( ft && ft->isValid() )
				font = ft;
			for( int i = 5 ; str[1] != 0 && i-- ; str++ );
			break;
		}
		case _escapeChar::changeFontColor:
			color = stringExtractor::colorChangePhrase( str );
			RGB_SETA( color , true );
			for( int i = 2 ; str[1] != 0 && i-- ; str++ );
			break;
		case _escapeChar::changeFontSize:
			fontSize = stringExtractor::sizeChangePhrase( str );
			if( str[1] )
				str++;
			break;
		default:
			return true;
	}
	
	return false;
}

bool stringExtractor::processChar( const _char*& str )
{
	switch( (_escapeChar)*str )
	{
		case _escapeChar::changeFont:
			for( int i = 5 ; str[1] != 0 && i-- ; str++ );
			break;
		case _escapeChar::changeFontColor:
			for( int i = 2 ; str[1] != 0 && i-- ; str++ );
			break;
		case _escapeChar::changeFontSize:
			str[1] && str++;
			break;
		default:
			return true;
	}
	
	return false;
}

void stringExtractor::strResize( _char* str , _length resultLength )
{
	if( !str || !*str )
		return;
	
	while( resultLength > 0 && *str )
	{
		bool process = stringExtractor::processChar( (const char*&)str );
		
		if( process )
			resultLength--;
		
		str++;
	}
	
	*str = 0;
}

_length stringExtractor::strLen( const _char* str )
{
	if( !str || !*str )
		return 0;
	
	_length resultLength = 0;
	
	do
	{		
		if( stringExtractor::processChar( (const char*&)str ) )
			resultLength++;
	}while( *++str );
	
	return resultLength;
}

string stringIntegrator::fontChangePhrase( const _font* ft ) 
{
	_u32 i = (_u32)ft;
	_u8 i1 = i >> 24;
	_u8 i2 = i >> 16 & 255;
	_u8 i3 = i >> 8 & 255;
	_u8 i4 = i & 255;
	
	fontPhraseExBit b;
	b.i1 = i1 & 1;
	b.i2 = i2 & 1;
	b.i3 = i3 & 1;
	b.i4 = i4 & 1;
	b.notNull = 1;
	
	_char ret[7] = {	
		(_char) _escapeChar::changeFont
		, _char( b.value ) // pack all four least significant bits into one 'char'
		, _char( i1 | 1 ) , _char( i2 | 1 )	, _char( i3 | 1 ) , _char( i4 | 1 ) // Or with 1 to prevent the the terminating '\0' char
		, 0
	};
	return ret;
}

const _font* stringExtractor::fontChangePhrase( const _char* str )
{
	if( !str[1] || !str[2] || !str[3] || !str[4] || !str[5] )
		return 0;
	stringIntegrator::fontPhraseExBit b;
	b.value = str[1];
	_u32 i1 = str[2] ^ (!b.i1);
	_u32 i2 = str[3] ^ (!b.i2);
	_u32 i3 = str[4] ^ (!b.i3);
	_u32 i4 = str[5] ^ (!b.i4);
	return reinterpret_cast<const _font*>( ( i1 << 24 ) | ( i2 << 16 ) | ( i3 << 8 ) | i4 );
}