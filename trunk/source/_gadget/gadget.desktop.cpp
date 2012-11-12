#include "_gadget/gadget.desktop.h"
#include "_type/type.system.h"
#include "_type/type.freetypefont.h"

_freetypefont* ft = nullptr;

_callbackReturn _desktop::refreshHandler( _event event )
{
	// Receive Gadget
	_desktop* that = event.getGadget<_desktop>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
	
	bP.fill( _system_->_runtimeAttributes_->user->getIntAttr( "desktopColor" ) );
	
	if(  _system_->_runtimeAttributes_->wallpaper != nullptr )
	{
		switch( _system_->_runtimeAttributes_->wallpaperView ){
			case WALLPAPER_ORIG:{
				_bitmap* wp = _system_->_runtimeAttributes_->wallpaper;
				bP.copy( 128 - ( wp->getWidth() >> 1 ) , 96 - ( wp->getHeight() >> 1 ) , wp );
				break;
			}
			default:
				break;
		}
	}
	
	//bP.copy( 0 , 0 , _system_->_runtimeAttributes_->user->userLogo );
	//if( ft )
		//bP.drawString( 20 , 20 , ft , "Zelda!" , RGB( 25 , 0 , 4 ) , 30 );
	
	return use_default;
}


_desktop::_desktop( _style style ) :
	_gadget( _gadgetType::desktop , SCREEN_WIDTH , SCREEN_HEIGHT , 0 , 0 , style )
{
	this->style.canReceiveFocus = false;
	//ft = new _freetypefont("/font.ttf");
	
	this->registerEventHandler( refresh , &_desktop::refreshHandler );
	
	// Refresh
	this->refreshBitmap();
}