#ifndef _WIN_T_VIRTUAL_FONT_
#define _WIN_T_VIRTUAL_FONT_

#include <_type/type.h>
#include <_type/type.font.h>
#include <_type/type.font.handle.h>

typedef _pair<_wchar,_u8> _specialCharEntry;
typedef _pair<_2wchar,_u8> _fontRangeEntry;

class _virtualFont : public _font
{
	private:
		
		_vector<_fontRangeEntry>		ranges;
		_map<_wchar,_specialCharEntry>	specialChars;
		_vector<_fontHandle>			fonts;
	
	protected:
		
		//! Check if the supplied character can be displayed by the font
		bool isCharSupportedInternal( _wchar ch ) const override ;
	
	public:
		
		//! Ctor
		_virtualFont( string name , _vector<_fontRangeEntry> ranges , _vector<_fontHandle> fonts ) :
			_font( move(name) )
			, ranges( move(ranges) )
			, fonts( move(fonts) )
		{}
		
		//! Ctor
		_virtualFont( string name , _map<_wchar,_specialCharEntry> specialChars , _vector<_fontHandle> fonts ) :
			_font( move(name) )
			, specialChars( move(specialChars) )
			, fonts( move(fonts) )
		{}
		
		//! Extended Ctor
		_virtualFont( string name , _vector<_fontRangeEntry> ranges , _map<_wchar,_specialCharEntry> specialChars , _vector<_fontHandle> fonts ) :
			_font( move(name) )
			, ranges( move(ranges) )
			, specialChars( move(specialChars) )
			, fonts( move(fonts) )
		{}
		
		//! Returns the actual font that is used to draw the character
		_fontHandle getActualFont( _wchar& character ) const ;
		
		//! Returns whether all fonts in this font container are monospace with the same width
		_length isMonospace() const override;
		
		//! Returns the width of a specific character
		_length getCharacterWidth( _wchar character , _fontSize fontSize = 0 ) const override ;
		
		//! Returns the maximum ascend of all fonts in this container
		_length getAscent( _fontSize fontSize = 0 , _wchar ch = 'a'  ) const override ;
		
		//! Get the maximum Height of all fonts in this container
		_length getHeight( _fontSize fontSize = 0 , _wchar ch = 'a'  ) const override;
		
		//! Get Space between two letters
		_length getLetterSpace( _fontSize fontSize = 0 , _wchar ch = 'a'  ) const override ;
		
		//! Draw a character!
		_length drawCharacter( _pixelArray dest , _length bitmapWidth , _coord x , _coord y , _wchar character , _color color , _rect clip , _fontSize fontSize = 0 ) const override ;
		
		//! Returns whether this font is valid
		bool isValid() const ;
};

#endif