#ifndef _WIN_T_TEXTPHRASES_
#define _WIN_T_TEXTPHRASES_

#include "_type/type.h"

class _font;

namespace stringIntegrator
{
	union fontPhraseExBit
	{
		_u8 value;
		struct
		{
			bool i1 : 1;
			bool i2 : 1;
			bool i3 : 1;
			bool i4 : 1;
			bool notNull : 4;
		} PACKED;
	} PACKED ;
	
	//! Generates an excape sequence that will change the displayed font
	extern string fontChangePhrase( const _font* ft );
	
	//! Generates an excape sequence that will change the displayed fontSize in a string
	unused static string sizeChangePhrase( _u8 fontSize )
	{
		_char ret[3] = { DSWindows::STR_CHANGEFONTSIZE , (_char)fontSize , 0 };
		return ret;
	}
	
	//! Generates an excape sequence that will change the displayed font color
	unused static string colorChangePhrase( _pixel color )
	{
		_char ret[4] = { DSWindows::STR_CHANGEFONTCOLOR , _char( color >> 8 ) , _char( color & 255 ) , 0 };
		return ret;
	}
};

namespace stringExtractor
{
	/**
	 * Processes a character in a given string
	 * and modifies both fontSize, font and color
	 * if an escaped sequence is found
	 **/
	extern bool processChar( const _char*& str );
	extern bool processChar( const _char*& str , _u8& fontSize , const _font*& font );
	extern bool processChar( const _char*& str , _u8& fontSize , const _font*& font , _pixel& color );
	
	extern void strResize( _char* str , _length resultLength );
	extern _length strLen( const _char* str );
	
	//! Generates an excape sequence that will change the displayed font
	extern const _font* fontChangePhrase( const _char* str );
	
	//! Generates an excape sequence that will change the displayed fontSize in a string
	unused static inline constexpr _u8 sizeChangePhrase( const _char* str ){ return str[1]; }
	
	//! Generates an excape sequence that will change the displayed font color
	unused static constexpr _pixel colorChangePhrase( const _char* str )
	{
		return _pixel( ( str[1] << 8 ) | str[2] );
	}
};

#endif