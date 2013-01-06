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
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
	
	bP.fill( NO_COLOR );
	
	//Border
	if( that->pressed )
		bP.drawRect( 0 	, 0 , 9 , 9 , RGB( 4 , 11 , 15 ) );
	else
		bP.drawRect( 0 	, 0 , 9 , 9 , 54829 );
	
	// Content
	bP.copy( 1 , 1 , that->graphic[ mid( 0 , that->getIntValue() , 2 ) ]);
	
	return use_default;
}

_callbackReturn _checkbox::dragHandler( _event event )
{
	// Receive Gadget
	_checkbox* that = event.getGadget<_checkbox>();
	
	if( event.getType() == dragStart )
		return handled;
	
	else if( event.getType() == dragging )
	{	
		if( !that->getAbsoluteDimensions().contains( event.getPosX() , event.getPosY() ) )
		{
			// I'm not pressed anymore!
			that->pressed = false;
			
			// Refresh my parents
			that->bubbleRefresh( true );
		}
		return handled;
	}
	
	return not_handled;
}

_callbackReturn _checkbox::mouseHandler( _event event )
{
	// Receive Gadget
	_checkbox* that = event.getGadget<_checkbox>();
	
	if( event.getType() == mouseDown )
			that->pressed = true;
	else if( that->pressed && event.getType() == mouseUp )
	{
		that->setIntValue( ! bool(that->getIntValue()) );
		that->triggerEvent( _event( onChange ) );
		that->pressed = false;
	}
	
	// Refresh
	that->bubbleRefresh( true );
	
	return handled;
}

_checkbox::_checkbox( _coord x , _coord y , _style style ) :
	_gadget( _gadgetType::checkbox , 40 , 9 , x , y , style )
	, pressed( false )
	, intValue( 0 )
{
	// Register my handler as the default Refresh-Handler
	this->unregisterEventHandler( mouseDoubleClick );
	this->registerEventHandler( refresh , new _staticCallback( &_checkbox::refreshHandler ) );
	this->registerEventHandler( mouseDown , new _staticCallback( &_checkbox::mouseHandler ) );
	this->registerEventHandler( mouseUp , new _staticCallback( &_checkbox::mouseHandler ) );
	this->registerEventHandler( dragStart , new _staticCallback( &_checkbox::dragHandler ) );
	this->registerEventHandler( dragging , new _staticCallback( &_checkbox::dragHandler ) );
	
	// Refresh Me
	this->refreshBitmap();
}

_checkbox::~_checkbox(){
	
}