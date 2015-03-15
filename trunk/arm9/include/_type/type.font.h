#ifndef _WIN_FONT_
#define _WIN_FONT_

#include <_type/type.h>
#include <_type/type.rect.h>
#include <_type/type.color.h>
#include <_type/type.uniqueptr.h>

class _font
{
	private:
		
		string	 	name; //! Name
		string	 	path; //! Path to the True-type font file
		
	protected:
		
		//! Set Font-Name
		void setName( string newName ){
			this->name = newName;
		}
		
		//! Check if the supplied character can be displayed by the font
		virtual bool isCharSupportedInternal( _wchar ch ) const = 0;
		
	public:
		
		//! Constructor with name
		_font( string newName ) :
			name( newName )
		{ }
		
		//! Virtual dtor
		virtual ~_font(){}
		
		//! Get the characterWidth related to a specific character
		virtual _length getCharacterWidth( _wchar character , _fontSize fontSize = 0 ) const = 0;
		
		//! Get the With of a string
		_length getStringWidth( _literal str , _fontSize fontSize = 0 ) const ;
		_length getStringWidth( _wliteral str , _fontSize fontSize = 0 ) const ;
		_length getStringWidth( const string& str , _fontSize fontSize = 0 ) const {
			return getStringWidth( str.c_str() , fontSize );
		}
		_length getStringWidth( const wstring& str , _fontSize fontSize = 0 ) const ;
		
		//! Check whether this font is monospace
		virtual _length isMonospace() const = 0;
		
		//! Check if the supplied character can be displayed by the font
		inline bool isCharSupported( _wchar ch ) const {
			return ch == ' ' || ch == '\n' || this->isCharSupportedInternal( ch );
		}
		
		//! Get the Height of the font
		virtual _length getHeight( _fontSize fontSize = 0 , _wchar ch = 'a'  ) const = 0;
		
		//! Get the Main-Height of the font, which is :
		/**
		ASC ->  	||  ||			<- H
		  - 		|||||| 	 /``\	   E
		ENT ->______||  ||___\,,|___   I
								|      GH
							 \,,/	<- T
		*/
		virtual _length getAscent( _fontSize fontSize = 0 , _wchar ch = 'a'  ) const = 0;
		
		//! Check if this font is valid
		virtual bool isValid() const = 0;
		
		//! Get the Name of the Font e.g. 'Arial'
		const string& getName() const {
			return this->name;
		}
		
		//! Get Space between two letters
		virtual _length getLetterSpace( _fontSize fontSize = 0 , _wchar ch = 'a' ) const = 0;
		
		//! Draw a character!
		virtual _length drawCharacter( _pixelArray dest , _length bitmapWidth , _coord x , _coord y , _wchar character , _color color , _rect clip , _fontSize fontSize = 0 ) const = 0;
		
		//! Load a font from the given path
		static _uniquePtr<_font> fromFile( const string& path );
};

using _fontPtr = const _font*;

#endif