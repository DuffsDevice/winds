#include "_gadget/gadget.checkbox.h"

// Graphics
#include "_resource/BMP_Checkboxes.h"
#include "_resource/BMP_WindowHeader.h"

const _bitmap _checkbox::graphic[3] = { BMP_CheckboxUnchecked() , BMP_CheckboxChecked() , BMP_CheckboxSemi() };

_callbackReturn _checkbox::refreshHandler( _event event )
{
	// Receive Gadget
	_checkbox* that = event.getGadget<_checkbox>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsolutePosition() ) );
	else
		bP.normalizeClippingRects();
	
	bP.fill( NO_COLOR );
	
	//Border
	if( that->isPressed() )
		bP.drawRect( 0 	, 0 , 9 , 9 , RGB( 4 , 11 , 15 ) );
	else
		bP.drawRect( 0 	, 0 , 9 , 9 , 54829 );
	
	// Content
	bP.copy( 1 , 1 , that->graphic[ mid( 0 , that->getIntValue() , 2 ) ]);
	
	return use_default;
}



_callbackReturn _checkbox::mouseHandler( _event event )
{
	// Receive Gadget
	_checkbox* that = event.getGadget<_checkbox>();
	
	// change Value
	that->setIntValue( ! bool( that->getIntValue() ) );
	that->triggerEvent( _event( onChange ) );
	
	// Refresh
	that->bubbleRefresh( true );
	
	return handled;
}



_checkbox::_checkbox( _coord x , _coord y , _style style ) :
	_gadget( _gadgetType::checkbox , 40 , 9 , x , y , style )
	, intValue( 0 )
{
	// Register my handler as the default Refresh-Handler
	this->registerEventHandler( refresh , new _staticCallback( &_checkbox::refreshHandler ) );
	this->registerEventHandler( mouseClick , new _staticCallback( &_checkbox::mouseHandler ) );
	this->registerEventHandler( onMouseEnter , new _gadget::eventForwardRefresh() );
	this->registerEventHandler( onMouseLeave , new _gadget::eventForwardRefresh() );
	
	// Refresh Me
	this->refreshBitmap();
}

_checkbox::~_checkbox(){
	
}