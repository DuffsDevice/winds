#ifndef _WIN_T_TEXTPHRASES_
#define _WIN_T_TEXTPHRASES_

#include "_type/type.h"
#include "_type/type.color.h"

// Excape characters used in fonts to prepare special actions
enum class _escapeChar : _char{
	changeFont = 22,
	changeFontColor = 23,
	changeFontSize = 24,
};

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
		} PACKED ;
	} PACKED ;
	
	//! Generates an excape sequence that will change the displayed font
	extern string fontChangePhrase( const _font* ft );
	
	//! Generates an excape sequence that will change the displayed fontSize in a string
	unused static string sizeChangePhrase( _u8 fontSize ){
		_char chars[] = { (_char)_escapeChar::changeFontSize , (_char)fontSize , 0 };
		return chars;
	}
	
	//! Generates an excape sequence that will change the displayed font color
	unused static string colorChangePhrase( _pixel color ){
		_char chars[] = { (_char)_escapeChar::changeFontColor , _char( color >> 8 ) , _char( color & 255 ) , 0 };
		return chars;
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
	extern bool processChar( const _char*& str , _u8& fontSize , const _font*& font , _color& color );
	
	extern void strResize( _char* str , _length resultLength );
	extern _length strLen( const _char* str );
	
	//! Generates an excape sequence that will change the displayed font
	extern const _font* fontChangePhrase( const _char* str );
	
	//! Generates an excape sequence that will change the displayed fontSize in a string
	unused static inline constexpr _u8 sizeChangePhrase( const _char* str ){ return str[1]; }
	
	//! Generates an excape sequence that will change the displayed font color
	unused static inline constexpr _color colorChangePhrase( const _char* str ){
		return _color( ( str[1] << 8 ) | str[2] );
	}
};

#endif