#include "_type/type.memoryfont.h"

_memoryfont::_memoryfont( string nN , _char fc , _char lc , _u8 ht , _u8 mH , const _bit *data , const _u8 *widths , const _u16 *offsets , _u8 ms , _u8 sW ) :
	_font( nN )
	, spaceWidth( sW )
	, monospace( ms )
	, firstChar( fc )
	, lastChar( lc )
	, height( ht )
	, mainHeight( mH )
	, charData( data )
	, charWidths( widths )
	, charOffsets( offsets )
{ }

_u16 _memoryfont::getCharacterWidth( _char character , _u8 fontSize ) const {
	if( character == ' ' )
		return this->spaceWidth;
	return (this->charWidths[ character - this->firstChar ]);
}

_u16 _memoryfont::isMonospace() const {
	return this->monospace;
}

bool _memoryfont::isCharSupported( _char ch ) const {
	return ch <= this->lastChar && ch >= this->firstChar;
}

bool _memoryfont::valid() const {
	return this->charData != nullptr;
}

_u16 _memoryfont::getHeight( _u8 fontSize ) const {
	return this->height;
}

_u16 _memoryfont::getAscent( _u8 fontSize ) const {
	return this->mainHeight;
}

_u16 _memoryfont::drawCharacter( _bitmap* dest , _coord x0 , _coord y0 , _char ch , _pixel color , _rect clip , _u8 fontSize ) const 
{
	if( ch == ' ' )
		return this->spaceWidth;
	
	const _bit *data 	= this->charData + this->charOffsets[ ch - this->firstChar ];
	_length width 		= this->getCharacterWidth( ch );
	
	// no need to blit to screen
	if ( y0 > clip.getY2() ) return width;
	if ( y0 + this->height < clip.y ) return width;
	if ( x0 > clip.getX2() ) return width;
	if ( x0 + width < clip.x ) return width;
	
	// Check for transparent
	if( !RGB_GETA(color) )
		goto end;
	
	if( this->monospace && width <= this->monospace - 2 )
		x0++;
	if( this->monospace && width <= this->monospace - 4 )
		x0++;
	
	for( int x = 0; x < width ; ++x ){
		for( int y = 0; y < this->height ; y++ ){
			if( data[ y * width + x ] )
				dest->drawPixel( x0 + x , y0 + y , color );
		}
	}
	end: // Breakpoint
	if( this->monospace != 0 )
		return this->monospace;
	
	return width;
}