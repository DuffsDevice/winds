#include "_gadget/gadget.desktop.h"
#include "_type/type.system.h"
#include "_type/type.bitmap.resizer.h"
#include "_type/type.color.h"
#include "_type/type.font.freetype.h"

_callbackReturn _desktop::refreshHandler( _event event )
{
	// Receive Gadget
	_desktop* that = event.getGadget<_desktop>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	_color c = _system::getUser().dTC;
	c.setL( c.getL() + 22 );
	
	bP.drawVerticalGradient( 0 , 0 , bP.getWidth() , bP.getHeight() , c.getColor() , _system::getUser().dTC );
	
	_constBitmap& wp = _system::getUser().getWallpaper();
	
	if( wp.isValid() )
		bP.copy( 128 - ( wp.getWidth() >> 1 ) , 96 - ( wp.getHeight() >> 1 ) , wp );
	
	return use_default;
}


_desktop::_desktop( _style&& style ) :
	_fileView( 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT - 10 , "%USERS%/" + _system::getUser().getFoldername() + "/desktop/" , _fileViewType::symbol_big , {} , _scrollType::prevent , _scrollType::prevent , false , style | _styleAttr::focusNoAction )
{
	//ft = new _freetypeFont("/font.ttf");
	
	this->setInternalEventHandler( onDraw , make_callback( &_desktop::refreshHandler ) );
	
	// Refresh
	this->redraw();
}