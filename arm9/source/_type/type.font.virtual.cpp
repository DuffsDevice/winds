#include <_type/type.font.virtual.h>
#include <cstdio>

bool _virtualFont::isCharSupportedInternal( _wchar ch ) const
{
	_fontHandle font = this->getActualFont( ch );
	return font && font->isCharSupported( ch );
}

_length _virtualFont::isMonospace() const
{
	_length width = 0;
	for( const _fontHandle& font : fonts ){
		_length oldWidth = width;
		if( font && oldWidth != (width = font->isMonospace()) && oldWidth )
			return 0;
	}
	return width;
}

_length _virtualFont::getHeight( _fontSize fontSize , _wchar ch ) const
{
	_fontHandle font = this->getActualFont( ch );
	return font ? font->getHeight( fontSize , ch ) : 0;
}

_length _virtualFont::getCharacterWidth( _wchar ch , _fontSize fontSize ) const
{
	_fontHandle font = this->getActualFont( ch );
	return font ? font->getCharacterWidth( ch , fontSize ) : 0;
}

_length _virtualFont::getAscent( _fontSize fontSize , _wchar ch ) const
{
	_fontHandle font = this->getActualFont( ch );
	return font ? font->getAscent( fontSize , ch ) : 0;
}

_fontHandle _virtualFont::getActualFont( _wchar& ch ) const
{
	for( const _fontRangeEntry& range : ranges )
		if( range.first.first <= ch && range.first.second >= ch )
			return fonts[range.second];
	
	for( const _pair<_wchar,_specialCharEntry>& specialChar : specialChars )
		if( specialChar.first == ch ){
			ch = specialChar.second.first;
			return fonts[specialChar.second.second];
		}
	
	return fonts[0];
}

_length _virtualFont::getLetterSpace( _fontSize fontSize , _wchar ch ) const
{
	_fontHandle font = this->getActualFont( ch );
	return font ? font->getLetterSpace( fontSize , ch ) : 0;
}

_length _virtualFont::drawCharacter( _pixelArray dest , _length bitmapWidth , _coord x , _coord y
								, _wchar character , _color color , _rect clip , _fontSize fontSize ) const
{
	_fontHandle font = this->getActualFont( character );
	return font ? font->drawCharacter( dest , bitmapWidth , x , y , character , color , clip , fontSize ) : 0;
}

bool _virtualFont::isValid() const {
	for( const _fontHandle& font : fonts ){
		if( !font || !font->isValid() )
			return false;
	}
	return true;
}