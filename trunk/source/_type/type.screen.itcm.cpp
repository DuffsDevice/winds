#include "_type/type.screen.h"
#include <nds/arm9/background.h>
#include "_type/type.system.h"
#include <nds/arm9/trig_lut.h>

_pixelArray	_screen::getMemoryPtr() const {	return bgGetGfxPtr( this->bgId ); }
int _screen::getBgId() const {	return this->bgId; }

_coord _screen::getScrollX() const { return bgState[ this->bgId ].scrollX >> 8; }
_coord _screen::getScrollY() const { return bgState[ this->bgId ].scrollY >> 8; }

float _screen::getScaleX() const { return fixedToFloat( bgState[ this->bgId ].scaleX , 8 ); }
float _screen::getScaleY() const {	return fixedToFloat( bgState[ this->bgId ].scaleY , 8 ); }

void _screen::scrollX( _coord x )
{
	bgState[ this->bgId ].scrollX = intToFixed( x , 8 );
	bgState[ this->bgId ].dirty = true;
}

void _screen::scrollY( _coord y )
{
	bgState[ this->bgId ].scrollY = intToFixed( y , 8 );
	bgState[ this->bgId ].dirty = true;
}

void _screen::scaleX( float x )
{
	bgState[ this->bgId ].scaleX = floatToFixed( x , 8 );
	bgState[ this->bgId ].dirty = true;
}

void _screen::scaleY( float y )
{
	bgState[ this->bgId ].scaleY = floatToFixed( y , 8 );
	bgState[ this->bgId ].dirty = true;
}

void _screen::scale( float rat )
{
	bgState[ this->bgId ].scaleY = bgState[ this->bgId ].scaleX = floatToFixed( rat , 8 );
	bgState[ this->bgId ].dirty = true;
}

_screen::_screen( int bgId )
	: bgId( bgId )
{
	this->scaleX( 1 );
	this->scaleY( 1 );
	this->scrollX( 0 );
	this->scrollY( 0 );
}