#ifndef _WIN_FONT_
#define _WIN_FONT_

#include "_type/type.h"
#include "_type/type.rect.h"

class _font{

	private:
		
		string	 	name; //! Name
		
	protected:
		
		//! Set Font-Name
		void setName( string newName ){
			this->name = newName;
		}
		
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
		
		_length getNumCharsUntilWidth( _length width , const _char* str , _u8 fontSize = 0 ) const ;
		_length getNumCharsUntilWidth( _length width , string str , _u8 fontSize = 0 ) const 
		{
			return getNumCharsUntilWidth( width , str.c_str() , fontSize );
		}
		
		//! Check whether this font is monospace
		virtual _length isMonospace() const = 0;
		
		//! Check if the supplied character can be displayed by the font
		virtual bool isCharSupported( _char ch ) const  = 0;
		
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
		virtual bool valid() const = 0;
		
		//! Get the Name of the Font e.g. 'Arial'
		string getName() const {
			return this->name;
		}
		
		//! draw a character!
		virtual _length drawCharacter( _pixelArray dest , _length bitmapWidth , _coord x , _coord y , _char character , _pixel color , _rect clip , _u8 fontSize = 0 ) const = 0;
		
		
		//! Receive a font, created from file
		static const _font* fromFile( string path );

};

#endif