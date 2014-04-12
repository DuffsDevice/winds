#ifndef _WIN_MEMORYFONT_
#define _WIN_MEMORYFONT_

#include "_type/type.h"
#include "_type/type.font.h"

class _memoryFont : public _font{

	private:
		_u8 		spaceWidth;
		_u8			monospace;
		_char		firstChar;
		_char		lastChar;
		_u8			height;
		_u8			mainHeight;
		_s8			letterSpace;
		const _u16*	charData;
		const _u8*	charWidths;
		const _u16*	charOffsets;
		
	protected:
		
		//! Check if the supplied character can be displayed by the font
		bool isCharSupportedInternal( _char ch ) const {
			return ch <= this->lastChar && ch >= this->firstChar;
		}
		
	public:
		
		/**
		 * Constructor with: Data, Widths, Offsets etc...
		 * @param nM Name of the Font e.g. 'Arial'
		 * @param fc The first character specified by data
		 * @param lc The last character specified by data
		 * @param ht The Height of the Font
		 * @param mH The Main-Height of the Font
		 * @see Line 84
		 * @param data Pointer to the _bit Base, in which the font is stored
		 * @param widths Pointer to an Array of u8ts in which the widths of the characters are stored
		 * @param offsets Array offsets (at which field of 'data' starts the character 'c'? etc...)
		 * @param ms Whether the font is monospace (if yes, pass the character-width of an 'm')
		 * @param sW The Width of a Space
		 * @return void
		 */
		_memoryFont( string nN , _char fc , _char lc , _u8 ht , _u8 mH , const _u16* data , const _u8* widths , const _u16* offsets , _u8 ms , _u8 sW , _s8 letterSpace = 1 );
		
		//! Get the CharacterWidth related to a specific character
		_length getCharacterWidth( _char ch , _u8 fontSize = 0 ) const ;
		
		//! Check whether this font is monospace
		_length isMonospace() const {
			return this->monospace;
		}
		
		//! Get the Height of the font
		_length getHeight( _u8 fontSize = 0 ) const {
			return this->height;
		}
		
		//! Get the Main-Height of the font
		_length getAscent( _u8 fontSize = 0 ) const {
			return this->mainHeight;
		}
		
		//! Get Space between two letters
		_length getLetterSpace( _u8 fontSize = 0 ) const {
			return this->letterSpace;
		}
		
		//! Check if this font is valid
		bool isValid() const {
			return this->charData != nullptr;
		}
		
		//! draw a Character!
		_length drawCharacter( _pixelArray dest , _length bitmapWidth , _coord x , _coord y , _char character , _color color , _rect clip , _u8 fontSize = 0 ) const ;
		
};

#endif