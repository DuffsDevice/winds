#ifndef _WIN_MEMORYFONT_
#define _WIN_MEMORYFONT_

#include "_type/type.h"
#include "_type/type.font.h"

using namespace std;

class _memoryfont : public _font{

	private:
		_u8 		spaceWidth;
		_u8			monospace;
		_char		firstChar;
		_char		lastChar;
		_u8			height;
		_u8			mainHeight;
		const _bit*	charData;
		const _u8*	charWidths;
		const _u16*	charOffsets;
		
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
		**/
		_memoryfont( string nN , _char fc , _char lc , _u8 ht , _u8 mH , const _bit *data , const _u8 *widths , const _u16 *offsets , _u8 ms , _u8 sW );
		
		//! Get the CharacterWidth related to a specific character
		_u16 getCharacterWidth( _char ch ) const ;
		
		//! Get the With of a string
		_u16 getStringWidth( string str ) const ;
		
		//! Check whether this font is monospace
		_u16 isMonospace() const ;
		
		//! Check if the supplied character can be displayed by the font
		bool isCharSupported( _char ch ) const ;
		
		//! Get the Height of the font
		_u16 getHeight() const ;
		
		//! Get the Main-Height of the font
		_u16 getMainHeight() const ;
		
		//! Check if this font is valid
		bool valid() const ;
		
		//! draw a Character!
		_u16 drawCharacter( _bitmap* dest , _coord x , _coord y , _char character , _pixel color , _rect clip ) const ;
		
};

#endif