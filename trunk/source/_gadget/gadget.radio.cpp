#include "_gadget/gadget.radio.h"
#include "_type/type.singlevaluegroup.h"

// Graphics
#include "_resource/BMP_Radiobuttons.h"

const _bitmap _radio::graphic[4] = { BMP_RadioChecked() , BMP_RadioUnchecked() , BMP_RadioBorder() , BMP_RadioBorderPressed() };

_callbackReturn _radio::refreshHandler( _event event )
{
	// Receive Gadget
	_radio* that = (_radio*)event.getGadget();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsolutePosition() ) );
	else
		bP.normalizeClippingRects();
	
	bP.fill( NO_COLOR );
	
	// Content
	bP.copyTransparent( 0 , 0 , that->graphic[ that->isPressed() + 2 ]);
	
	bP.copyTransparent( 2 , 2 , that->graphic[ that->getIntValue() ]);
	
	return use_default;
}

_callbackReturn _radio::mouseHandler( _event event )
{
	// Receive Gadget
	_radio* that = event.getGadget<_radio>();
	
	// Ensure it is not check already
	if( that->group && !that->getIntValue() )
	{
		that->group->enableSelector( that );
		that->triggerEvent( _event( onChange ) );
		that->bubbleRefresh( true );
	}
	
	return handled;
}

_radio::_radio( _coord x , _coord y , _singleValueGroup<_radio>* group , _s32 assocValue , _style style ) :
	_gadget( _gadgetType::radiobox , 9 , 9 , x , y , style )
	, intValue( 0 )
	, group( nullptr ) // Will be set when the _radio will be added to the _radiogroup
{
	if( group )
		group->addSelector( this , assocValue );
	
	// Register my handler as the default Refresh-Handler
	this->registerEventHandler( refresh , new _staticCallback( &_radio::refreshHandler ) );
	this->registerEventHandler( mouseClick , new _staticCallback( &_radio::mouseHandler ) );
	this->registerEventHandler( onMouseEnter , new _gadget::eventForwardRefresh() );
	this->registerEventHandler( onMouseLeave , new _gadget::eventForwardRefresh() );
	
	// Refresh Me
	this->refreshBitmap();
}

_radio::~_radio()
{
	if( this->group )
		this->group->removeSelector( this );
}