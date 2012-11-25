#include "_gadget/gadget.actionButton.h"
#include "_resource/BMP_ActionButtons.h"

_callbackReturn _actionButton::refreshHandler( _event event )
{
	// Receive Gadget
	_actionButton* that = event.getGadget<_actionButton>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
	
	bP.copyTransparent( 0 , 0 , that->bitmaps[ _u8( that->type ) ] );
	
	return use_default;
}

_bitmap _actionButton::bitmaps[] = { BMP_ActionBtnNext() , BMP_ActionBtnPrev() };

_actionButton::_actionButton( _actionButtonType type , _coord x , _coord y , _style style ) :
	_button( 9 , 9 , x , y , "" , style )
	, type( type )
{
	// Register handler
	this->unregisterEventHandler( mouseDoubleClick );
	this->registerEventHandler( refresh , &_actionButton::refreshHandler );
	
	// Refresh Me
	this->refreshBitmap();
}