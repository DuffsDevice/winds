#ifndef _WIN_FONT_
#define _WIN_FONT_

#include "_type/type.h"
#include "_type/type.rect.h"
#include "_type/type.color.h"

class _font{

	private:
		
		string	 	name; //! Name
		
	protected:
		
		//! Set Font-Name
		void setName( string newName ){
			this->name = newName;
		}
		
		//! Check if the supplied character can be displayed by the font
		virtual bool isCharSupportedInternal( _char ch ) const = 0;
		
	public:
		
		//! Constructor with name
		_font( string newName ) :
			name( newName )
		{ }
		
		//! Virtual dtor
		virtual ~_font(){}
		
		//! Get the characterWidth related to a specific character
		virtual _length getCharacterWidth( _char character , _u8 fontSize = 0 ) const = 0;
		
		//! Get the With of a string
		_length getStringWidth( const char* str , _u8 fontSize = 0 ) const ;
		_length getStringWidth( string str , _u8 fontSize = 0 ) const 
		{
			return getStringWidth( str.c_str() , fontSize );
		}
		
		//! Check whether this font is monospace
		virtual _length isMonospace() const = 0;
		
		//! Check if the supplied character can be displayed by the font
		inline bool isCharSupported( _char ch ) const {
			return ch == ' ' || ch == '\n' || this->isCharSupportedInternal( ch );
		}
		
		//! Get the Height of the font
		virtual _length getHeight( _u8 fontSize = 0 ) const = 0;
		
		//! Get the Main-Height of the font, which is :
		/**
		ASC ->  	||  ||			<- H
		  - 		|||||| 	 /``\	   E
		ENT ->______||  ||___\,,|___   I
								|      GH
							 \,,/	<- T
		*/
		virtual _length getAscent( _u8 fontSize = 0 ) const = 0;
		
		//! Check if this font is valid
		virtual bool isValid() const = 0;
		
		//! Get the Name of the Font e.g. 'Arial'
		const string& getName() const {
			return this->name;
		}
		
		//! Get Space between two letters
		virtual _length getLetterSpace( _u8 fontSize = 0 ) const = 0;
		
		//! draw a character!
		virtual _length drawCharacter( _pixelArray dest , _length bitmapWidth , _coord x , _coord y , _char character , _color color , _rect clip , _u8 fontSize = 0 ) const = 0;
		
		//! Returns a font, created from a file
		static const _font* fromFile( string path );

};

typedef const _font* _fontPtr;

#endif