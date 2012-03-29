#ifndef _WIN_FONT_
#define _WIN_FONT_

#include <string>
#include "type.h"

using namespace std;

class _font{

	private:
		string	 	name; //! Name
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
		
		//! Default-Constructor
		_font();
	
		//! Constructor with name
		_font( string newName );
		
		//! Copy-Constructor
		_font( _font &font );
		
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
		_font( string nN , _char fc , _char lc , _u8 ht , _u8 mH , const _bit *data , const _u8 *widths , const _u16 *offsets , _u8 ms , _u8 sW );
		
		//! Get all Charwidths as an Array
		const _u8* getCharWidths() const ;
		
		//! Get the Font-Data
		const _bit *getData() const ;
		
		//! Get all character-offsets as an array
		const _u16* getCharOffsets() const ;
		
		//! Get the CharacterData related to a specific character
		const _bit *getCharacterData( _u8 character ) const ;
		
		//! Get the CharacterWidth related to a specific character
		_u8 getCharacterWidth( _char character ) const ;
		
		//! Get the With of a string
		_u16 getStringWidth( string str ) const ;
		
		//! Check whether this font is monospace
		_u8 isMonospace() const ;
		
		//! Set the Name of the font
		void setName( string nN );
		
		//! Get the last Character specified by charData
		_char getLastChar() const ;
	
		//! Get the first Character specified by charData
		_char getFirstChar() const ;
		
		//! Get the Height of the font
		_u8 getHeight() const ;
		
		//! Get the Main-Height of the font, which is :
		/**
		MAIN->  	||  ||			<- W S
		  - 		|||||| 	 /``\	   H I
		SIZE->______||  ||___\,,|___   O Z
								|      L E
							 \,,/	<- E
		*/
		_u8 getMainHeight() const ;
		
		//! Check if this font is valid
		bool valid() const ;
		
		//! Get the Name of the Font e.g. 'Arial'
		string getName() const ;
		
};

#endif