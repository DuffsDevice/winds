#include "_type/type.font.h"

_font::_font( string newName ) 
	: name( newName )
{ }

_u16 _font::getStringWidth( string str , _u8 fontSize ) const {
	_u16 tempX = 0;
	for( const _char& ch : str )
		tempX += 1 + this->getCharacterWidth( ch , fontSize );
	return tempX;
}

string _font::getName() const {
	return this->name;
}