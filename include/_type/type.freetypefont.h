#ifndef _WIN_T_FREETYPEFONT_
#define _WIN_T_FREETYPEFONT_

#include "_type/type.font.h"
#include "_type/type.direntry.h"
#include "type.bitmap.h"

#include "_library/truetype.h"

class _freetypefont : private _direntry , public _font
{	
	private:
		
		_u8*			cache;
		_u32			cacheSize;
		stbtt_fontinfo	fontInfo;
		
	public:
		
		_freetypefont( string path );
		
		_length drawCharacter( _pixelArray dest , _length bitmapWidth , _coord x , _coord y , _char character , _pixel color , _rect clip , _u8 fontSize = 8 ) const ;
		
		inline _length getCharacterWidth( const _char codepoint , _u8 fontSize = 8 ) const ;
		
		inline _length isMonospace() const ;
		
		_length getAscent( _u8 fontSize = 8 ) const ;
		
		_length getHeight( _u8 fontSize = 8 ) const { return fontSize; }
		
		bool valid() const ;
		
		bool isCharSupported( const _char codepoint ) const ;
		
		~_freetypefont();
	
};

#endif