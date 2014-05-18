#include "_screen/screen.desktop.view.h"
#include "_controller/controller.registry.h"
#include "_type/type.bitmap.resizer.h"
#include "_type/type.color.h"
#include "_type/type.font.freetype.h"

_callbackReturn _desktopScreenView::refreshHandler( _event event )
{
	// Receive Gadget
	_desktopScreenView* that = event.getGadget<_desktopScreenView>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	_color desktopColor = _registryController::getUserRegistry().getDesktopColor();
	_color lightDesktopColor = _color::fromHSL( desktopColor.getH() , desktopColor.getS() , desktopColor.getL() + 10 );
	bP.drawVerticalGradient( 0 , 0 , bP.getWidth() , bP.getHeight() , lightDesktopColor , desktopColor );
	
	_constBitmap& wp = _registryController::getUserRegistry().getWallpaper();
	
	if( wp.isValid() )
		bP.copy( 128 - ( wp.getWidth() >> 1 ) , 96 - ( wp.getHeight() >> 1 ) , wp );
	
	return use_default;
}


_desktopScreenView::_desktopScreenView( _style&& style ) :
	_fileView( 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT - 10 , _registryController::getUserRegistry().getHomeFolder() + "/desktop/" , _fileViewType::symbol , false , {/* Allow All Extensions*/} , style | _style::focusNoAction )
{
	_fileView::setScrollTypeX( _scrollType::prevent );
	_fileView::setScrollTypeY( _scrollType::prevent );
	
	this->setInternalEventHandler( onDraw , make_callback( &_desktopScreenView::refreshHandler ) );
	
	// Refresh
	this->redraw();
}