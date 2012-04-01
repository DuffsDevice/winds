#include "_gadget/gadget.desktop.h"
#include "_type/type.system.h"
#include <nds/arm9/console.h>

_gadgetEventReturnType _desktop::refreshHandler( _gadgetEvent event )
{
	// Receive Gadget
	_desktop* that = (_desktop*)event.getGadget();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().isBubbleRefresh() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteDimensions() ) );
	else
		bP.resetClippingRects();
	
	bP.fill( _system_->_runtimeAttributes_->wallpaperBgColor );
	
	if(  _system_->_runtimeAttributes_->wallpaper != nullptr )
	{
		switch( _system_->_runtimeAttributes_->wallpaperView ){
			case WALLPAPER_ORIG:{
				_bitmap* wp = _system_->_runtimeAttributes_->wallpaper;
				bP.copy( 128 - ( wp->getWidth() >> 1 ) , 86 - ( wp->getHeight() >> 1 ) , wp );
				break;
			}
			default:
				break;
		}
	}
	
	return use_default;
}

_desktop::_desktop( _gadgetStyle style ) :
	_gadget( _gadgetType::desktop , SCREEN_WIDTH , SCREEN_HEIGHT - 10 , 0 , 0 , style )
{
	this->registerEventHandler( refresh , &_desktop::refreshHandler );
	// Refresh
	this->refreshBitmap();
}