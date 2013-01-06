#include "_gadget/gadget.radio.h"
#include "_type/type.radiogroup.h"

// Graphics
#include "_resource/BMP_Radiobuttons.h"
#include "_resource/BMP_WindowHeader.h"

const _bitmap _radio::graphic[4] = { BMP_RadioChecked() , BMP_RadioUnchecked() , BMP_RadioBorder() , BMP_RadioBorderPressed() };

_callbackReturn _radio::refreshHandler( _event event )
{
	// Receive Gadget
	_radio* that = (_radio*)event.getGadget();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
	
	bP.fill( NO_COLOR );
	
	// Content
	bP.copyTransparent( 0 , 0 , that->graphic[ that->pressed + 2 ]);
	
	bP.copyTransparent( 2 , 2 , that->graphic[ that->getIntValue() ]);
	
	return use_default;
}

_callbackReturn _radio::dragHandler( _event event )
{
	// Receive Gadget
	_radio* that = (_radio*)event.getGadget();
	
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

_callbackReturn _radio::mouseHandler( _event event )
{
	// Receive Gadget
	_radio* that = (_radio*)event.getGadget();
	
	if( event.getType() == mouseDown )
			that->pressed = true;
	else if( that->pressed && event.getType() == mouseUp )
	{
		// Ensure it is not check already
		if( !that->getIntValue() )
		{
			that->radiogroup->enableRadio( that );
		}
		that->triggerEvent( _event( onChange ) );
		that->pressed = false;
	}
	
	// Refresh
	that->bubbleRefresh( true );
	
	return handled;
}

_radio::_radio( _coord x , _coord y , _radiogroup* group , _s32 assocValue , _style style ) :
	_gadget( _gadgetType::radiobox , 9 , 9 , x , y , style )
	, pressed( false )
	, intValue( 0 )
	, radiogroup( group )
{
	if( group )
		group->addRadio( this , assocValue );
	
	// Register my handler as the default Refresh-Handler
	this->unregisterEventHandler( mouseDoubleClick );
	this->registerEventHandler( refresh , new _staticCallback( &_radio::refreshHandler ) );
	this->registerEventHandler( mouseDown , new _staticCallback( &_radio::mouseHandler ) );
	this->registerEventHandler( mouseUp , new _staticCallback( &_radio::mouseHandler ) );
	this->registerEventHandler( dragStart , new _staticCallback( &_radio::dragHandler ) );
	this->registerEventHandler( dragging , new _staticCallback( &_radio::dragHandler ) );
	
	// Refresh Me
	this->refreshBitmap();
}

_radio::~_radio()
{
	this->radiogroup->removeRadio( this );
}