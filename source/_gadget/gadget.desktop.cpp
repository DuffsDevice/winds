#include "_gadget/gadget.desktop.h"
#include "_type/type.system.h"
#include "_type/type.bitmapResizer.h"
#include "_type/type.color.h"
//#include "_type/type.freetypefont.h"

//_freetypefont* ft = nullptr;

_callbackReturn _desktop::refreshHandler( _event event )
{
	// Receive Gadget
	_desktop* that = event.getGadget<_desktop>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().relativate( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
	
	_color c;
	c.setColor( _system::_runtimeAttributes_->user->dTC )->setL( c.getL() + 22 );
	
	bP.drawVerticalGradient( 0 , 0 , bP.getWidth() , bP.getHeight() , c.getColor() , _system::_runtimeAttributes_->user->dTC );
	
	const _bitmap& wp = _system::_runtimeAttributes_->user->getWallpaper();
	
	if( wp.isValid() )
		bP.copy( 128 - ( wp.getWidth() >> 1 ) , 96 - ( wp.getHeight() >> 1 ) , wp );
	
	//bP.copy( 0 , 0 , _system::_runtimeAttributes_->user->userLogo );
	//if( ft )
		//bP.drawString( 20 , 20 , ft , "Zelda!" , RGB( 25 , 0 , 4 ) , 30 );
	
	return use_default;
}


_desktop::_desktop( _style style ) :
	_gadget( _gadgetType::desktop , SCREEN_WIDTH , SCREEN_HEIGHT - 10 , 0 , 0 , style )
{
	this->style.canReceiveFocus = false;
	//ft = new _freetypefont("/font.ttf");
	
	this->registerEventHandler( refresh , new _staticCallback( &_desktop::refreshHandler ) );
	
	// Refresh
	this->refreshBitmap();
}