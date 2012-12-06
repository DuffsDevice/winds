#include "_gadget/gadget.counter.h"
#include "_gadget/gadget.button.h"
#include "_type/type.system.h"
#include "func.memory.h"

_callbackReturn _counter::changeHandler( _event event )
{
	_scrollButton* that = event.getGadget<_scrollButton>();
	_counter* cntr = ((_counter*)that->parent);
	
	if( that == cntr->increaseHandle )
		cntr->increase();
	else if( that == cntr->decreaseHandle )
		cntr->decrease();
	
	return handled;
}

_callbackReturn _counter::refreshHandler( _event event )
{
	// Receive Gadget
	_counter* that = event.getGadget<_counter>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
	
	bP.fill( COLOR_WHITE );
	
	bP.drawRect( 0 , 0 , bP.getWidth() , bP.getHeight() , RGB( 9 , 13 , 19 ) );
	
	return use_default;
}

_counter::_counter( _coord x , _coord y , _length width , bool circular , _s32 value , _s32 upperBound , _s32 lowerBound , _style style ) :
	_gadget( _gadgetType::counter , max( width , _length(15) ) , 16 , x , y , style )
	, circular( circular )
	, intValue( value )
	, lowerBound( lowerBound )
	, upperBound( upperBound )
{
	this->increaseHandle = new _scrollButton( 8 , 6 , this->dimensions.width - 9 , 1 , _scrollButtonType::buttonTop );
	this->decreaseHandle = new _scrollButton( 8 , 8 , this->dimensions.width - 9 , 7 , _scrollButtonType::buttonBottom );
	this->valueLabel = new _label( this->getWidth() - 9 , this->getHeight() - 2 , 0 , 1 , int2string( value ) );
	this->valueLabel->setAlign( _align::center );
	this->valueLabel->setVAlign( _valign::middle );
	this->valueLabel->setFont( _system::getFont( "CourierNew10" ) );
	
	this->increaseHandle->registerEventHandler( onAction , &_counter::changeHandler );
	this->decreaseHandle->registerEventHandler( onAction , &_counter::changeHandler );
	
	//! Refresh - Handler
	this->registerEventHandler( refresh , &_counter::refreshHandler );
	
	this->addEnhancedChild( this->increaseHandle );
	this->addEnhancedChild( this->decreaseHandle );
	this->addChild( this->valueLabel );
	
	// Refresh Me
	this->refreshBitmap();
}

_counter::_counter( _coord x , _coord y , _length width , bool circular , _s32 value , _style style ) :
	_counter( x , y , width , circular , value , circular ? 1000 : 2147483647 , circular ? 0 : -2147483647 , style )
{ }


void _counter::setIntValue( int value )
{
	if( value > this->upperBound )
		value = this->circular ? this->lowerBound : this->upperBound;
	else if( value < this->lowerBound )
		value = this->circular ? this->upperBound : this->lowerBound;
	
	if( value != this->intValue )
	{
		this->intValue = value;
		this->valueLabel->setStrValue( int2string( value ) );
	}
}