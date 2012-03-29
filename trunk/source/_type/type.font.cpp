#include "_type/type.font.h"

_font::_font() 
	: name( "" ) , spaceWidth( 0 ) , monospace( 0 ) , firstChar( 0 ) , lastChar( 0 ) , height( 0 ) , mainHeight( 0 ) , charData( nullptr ) , charWidths( nullptr ) , charOffsets( nullptr )
{ }

_font::_font( string newName ) 
	: name( newName )
{ }

_font::_font( _font &font )
	: name( font.getName() ) , spaceWidth( font.getCharacterWidth(' ') ) , monospace( font.isMonospace() ) , firstChar( font.getFirstChar() ) , lastChar( font.getLastChar() ) , height( font.getHeight() ) , mainHeight( font.getMainHeight() ) , charData( font.getData() ) , charWidths( font.getCharWidths() ) , charOffsets( font.getCharOffsets() )
{ }

_font::_font( string nN , _char fc , _char lc , _u8 ht , _u8 mH , const _bit *data , const _u8 *widths , const _u16 *offsets , _u8 ms , _u8 sW )
	: name( nN ) , spaceWidth( sW ) , monospace( ms ) , firstChar( fc ) , lastChar( lc ) , height( ht ) , mainHeight( mH ) , charData( data ) , charWidths( widths ) , charOffsets( offsets )
{ }

const _u8* _font::getCharWidths() const {
	return this->charWidths;
}

const _bit* _font::getData() const {
	return this->charData;
}

const _u16* _font::getCharOffsets() const { 
	return this->charOffsets;
}

const _bit* _font::getCharacterData( _u8 character ) const {
	return (&this->charData[ this->charOffsets[ character - this->firstChar ] ] );
}

_u8 _font::getCharacterWidth( unsigned char character ) const {
	if( character == ' ' )
		return this->spaceWidth;
	return (this->charWidths[ character - this->firstChar ]);
}

_u16 _font::getStringWidth( string str ) const {
	_u16 tempX = 0;
	for( _u16 i = str.length() ; i > 0 ; )
		tempX += 1 + this->getCharacterWidth( str[--i] );
	return tempX;
}

_u8 _font::isMonospace() const {
	return this->monospace;
}

void _font::setName( string nN ){
	this->name = nN;
}

_char _font::getLastChar() const {
	return this->lastChar;
}

_char _font::getFirstChar() const {
	return this->firstChar;
}

bool _font::valid() const {
	return this->charData != nullptr;
}

_u8 _font::getHeight() const {
	return this->height;
}

_u8 _font::getMainHeight() const {
	return this->mainHeight;
}

string _font::getName() const {
	return this->name;
}