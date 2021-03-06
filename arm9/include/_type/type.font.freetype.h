#ifndef _WIN_T_FREETYPEFONT_
#define _WIN_T_FREETYPEFONT_

#include <_type/type.font.h>
#include <_type/type.direntry.h>
#include <_type/type.bitmap.h>
#include <_library/library.truetype.h>

class _freetypeFont : private _direntry , public _font
{	
	private:
		
		_u8*			cache;
		_u32			cacheSize;
		stbtt_fontinfo	fontInfo;
	
	protected:
		
		//! Check if the supplied character can be displayed by the font
		bool isCharSupportedInternal( _wchar codepoint ) const ;
		
	public:
		
		//! Ctor: Create a _font from a .ttf file
		_freetypeFont( string path );
		
		//! draw a Character!
		_length drawCharacter( _pixelArray dest , _length bitmapWidth , _coord x , _coord y
							, _wchar character , _color color , _rect clip , _fontSize fontSize = 8 ) const ITCM_CODE ;
		
		//! Get the CharacterWidth related to a specific character
		_length getCharacterWidth( _wchar codepoint , _fontSize fontSize = 8 ) const ;
		
		//! Check whether this font is monospace
		_length isMonospace() const { 
			return false;
		}
		
		//! Get the Main-Height of the font
		_length getAscent( _fontSize fontSize = 8 , _wchar ch = 'a' ) const ;
		
		//! Get the Height of the font
		_length getHeight( _fontSize fontSize = 8 , _wchar ch = 'a' ) const { return fontSize; }
		
		//! Get Space between two letters
		_length getLetterSpace( _fontSize = 8 , _wchar = 'a' ) const {
			// We return 0, because the letter space is
			// already included in the width of one character
			return 0;
		}
		
		//! Check if this font is valid
		bool isValid() const {
			return this->fontInfo.numGlyphs != 0;
		}
		
		//! Dtor
		~_freetypeFont();
	
};

#endif