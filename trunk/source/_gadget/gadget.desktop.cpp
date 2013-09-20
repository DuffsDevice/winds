#include "_gadget/gadget.desktop.h"
#include "_type/type.system.h"
#include "_type/type.bitmapResizer.h"
#include "_type/type.color.h"
#include "_type/type.freetypefont.h"

//_freetypefont* ft = nullptr;

_callbackReturn _desktop::refreshHandler( _event event )
{
	// Receive Gadget
	_desktop* that = event.getGadget<_desktop>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	_color c = _system::getUser()->dTC;
	c.setL( c.getL() + 22 );
	
	bP.drawVerticalGradient( 0 , 0 , bP.getWidth() , bP.getHeight() , c.getColor() , _system::getUser()->dTC );
	
	const _bitmap& wp = _system::getUser()->getWallpaper();
	
	if( wp.isValid() )
		bP.copy( 128 - ( wp.getWidth() >> 1 ) , 96 - ( wp.getHeight() >> 1 ) , wp );
	
	//bP.copy( 0 , 0 , _system::getUser()->userLogo );
	//if( ft )
	//	bP.drawString( 20 , 20 , ft , "Zelda!" , RGB( 25 , 0 , 4 ) , 30 );
	
	return use_default;
}


_desktop::_desktop( _style&& style ) :
	_fileview( SCREEN_WIDTH , SCREEN_HEIGHT - 10 , 0 , 0 , "%USERS%/" + _system::getUser()->getFoldername() + "/desktop/" , _fileviewType::symbol_big , _scrollType::prevent , _scrollType::prevent , false , style | _styleAttr::focusNoAction )
{
	//ft = new _freetypefont("/font.ttf");
	
	this->setInternalEventHandler( onDraw , make_callback( &_desktop::refreshHandler ) );
	
	// Refresh
	this->redraw();
}