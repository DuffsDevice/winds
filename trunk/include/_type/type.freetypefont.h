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
		
		_u16 drawCharacter( _bitmap* dest , _coord x , _coord y , _char character , _pixel color , _rect clip , _u8 fontSize = 8 ) const ;
		
		inline _u16 getCharacterWidth( const _char codepoint , _u8 fontSize = 8 ) const ;
		
		inline _u16 isMonospace() const ;
		
		_u16 getAscent( _u8 fontSize = 8 ) const ;
		
		_u16 getHeight( _u8 fontSize = 8 ) const { return fontSize; }
		
		bool valid() const ;
		
		bool isCharSupported( const _char codepoint ) const ;
		
		~_freetypefont();
	
};

#endif