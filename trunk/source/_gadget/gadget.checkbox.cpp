#include "_gadget/gadget.checkbox.h"

// Graphics
#include "_resource/BMP_Checkboxes.h"
#include "_resource/BMP_WindowHeader.h"

const _bitmap _checkbox::graphic[3] = { BMP_CheckboxUnchecked() , BMP_CheckboxChecked() , BMP_CheckboxSemi() };

_gadgetEventReturnType _checkbox::refreshHandler( _gadgetEvent event )
{
	// Receive Gadget
	_checkbox* that = (_checkbox*)event.getGadget();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().hasClippingRects() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteDimensions() ) );
	else
		bP.resetClippingRects();
	
	bP.fill( NO_COLOR );
	
	//Border
	if( that->pressed )
		bP.drawRect( 0 	, 0 , 9 , 9 , RGB( 4 , 11 , 15 ) );
	else
		bP.drawRect( 0 	, 0 , 9 , 9 , 54829 );
	
	// Content
	bP.copy( 1 , 1 , &that->graphic[ mid( 0 , that->getIntValue() , 2 ) ]);
	
	return use_default;
}

_gadgetEventReturnType _checkbox::dragHandler( _gadgetEvent event )
{
	// Receive Gadget
	_checkbox* that = (_checkbox*)event.getGadget();
	
	if( event.getType() == "dragStart" )
		return handled;
	
	else if( event.getType() == "dragging" )
	{	
		if( !that->getAbsoluteDimensions().contains( event.getArgs().getPosX() , event.getArgs().getPosY() ) )
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

_gadgetEventReturnType _checkbox::mouseHandler( _gadgetEvent event )
{
	// Receive Gadget
	_checkbox* that = (_checkbox*)event.getGadget();
	
	if( event.getType() == "mouseDown" )
			that->pressed = true;
	else if( that->pressed && event.getType() == "mouseUp" )
	{
		that->setIntValue( ! bool(that->getIntValue()) );
		that->triggerEvent( _gadgetEvent( "change" ) );
		that->pressed = false;
	}
	
	// Refresh
	that->bubbleRefresh( true );
	
	return handled;
}

_checkbox::_checkbox( _coord x , _coord y , _gadgetStyle style ) :
	_gadget( _gadgetType::checkbox , 40 , 9 , x , y , style )
	, _interface_input( "" )
	, pressed( false )
{
	this->setIntValue(0);
	
	// Register my handler as the default Refresh-Handler
	this->unregisterEventHandler( "mouseDoubleClick" );
	this->registerEventHandler( "refresh" , &_checkbox::refreshHandler );
	this->registerEventHandler( "mouseDown" , &_checkbox::mouseHandler );
	this->registerEventHandler( "mouseUp" , &_checkbox::mouseHandler );
	this->registerEventHandler( "dragStart" , &_checkbox::dragHandler );
	this->registerEventHandler( "dragging" , &_checkbox::dragHandler );
	
	// Refresh Me
	this->refreshBitmap();
}

_checkbox::~_checkbox(){
	
}