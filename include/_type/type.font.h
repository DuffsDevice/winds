#ifndef _WIN_FONT_
#define _WIN_FONT_

#include "_type/type.h"
#include "_type/type.rect.h"

class _bitmap;

class _font{

	private:
		
		string	 	name; //! Name
		
	protected:
		
		//! Set Font-Name
		void setName( string newName );
		
	public:
		
		//! Constructor with name
		_font( string newName );
		
		virtual ~_font(){}
		
		//! Get the CharacterWidth related to a specific character
		virtual _u16 getCharacterWidth( _char character , _u8 fontSize = 0 ) const = 0;
		
		//! Get the With of a string
		_u16 getStringWidth( string str , _u8 fontSize = 0 ) const ;
		
		//! Check whether this font is monospace
		virtual _u16 isMonospace() const = 0;
		
		//! Check if the supplied character can be displayed by the font
		virtual bool isCharSupported( _char ch ) const  = 0;
		
		//! Get the Height of the font
		virtual _u16 getHeight( _u8 fontSize = 0 ) const = 0;
		
		//! Get the Main-Height of the font, which is :
		/**
		ASC ->  	||  ||			<- H
		  - 		|||||| 	 /``\	   E
		ENT ->______||  ||___\,,|___   I
								|      GH
							 \,,/	<- T
		*/
		virtual _u16 getAscent( _u8 fontSize = 0 ) const = 0;
		
		//! Check if this font is valid
		virtual bool valid() const = 0;
		
		//! Get the Name of the Font e.g. 'Arial'
		string getName() const ;
		
		//! draw a Character!
		virtual _u16 drawCharacter( _bitmap* dest , _coord x , _coord y , _char character , _pixel color , _rect clip , _u8 fontSize = 0 ) const = 0;
		
		
		//! Receive a font, created from file
		static const _font* fromFile( string path );
};

#include "_type/type.bitmap.h"

#endif