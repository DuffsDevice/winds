#include "_gadget/gadget.counter.h"
#include "_gadget/gadget.button.h"
#include "_type/type.system.h"
#include "func.memory.h"

_callbackReturn _counter::changeHandler( _event event )
{
	// Blur and Focus changes the style of the label
	if( event.getType() == onFocus )
	{
		_counter* that = event.getGadget<_counter>();
		
		that->valueLabel->setBgColor( RGB255( 10 , 36 , 106 ) );
		that->valueLabel->setColor( COLOR_WHITE );
		return handled;
	}
	if( event.getType() == onBlur )
	{
		_counter* that = event.getGadget<_counter>();
		
		that->valueLabel->setBgColor( COLOR_WHITE );
		that->valueLabel->setColor( COLOR_BLACK );
		return handled;
	}
	
	// For Key-Events
	if( event.getType() == keyDown )
	{
		_counter* that = event.getGadget<_counter>();
		
		if( event.getKeyCode() == DSWindows::KEY_UP )
			that->increase();
		else if( event.getKeyCode() == DSWindows::KEY_DOWN )
			that->decrease();
		
		that->handleEvent( _event( onChange ) );
		
		return handled;
	}
	
	_scrollButton* that = event.getGadget<_scrollButton>();
	_counter* cntr = ((_counter*)that->parent);
	
	if( that == cntr->increaseHandle )
		cntr->increase();
	else if( that == cntr->decreaseHandle )
		cntr->decrease();
	
	cntr->handleEvent( _event( onChange ) );
	
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

_counter::_counter( _coord x , _coord y , _length width , bool circular , _s32 value , _s32 upperBound , _s32 lowerBound , _u8 numbersystem , _style style ) :
	_gadget( _gadgetType::counter , max( width , _length(15) ) , 16 , x , y , style )
	, circular( circular )
	, intValue( value )
	, lowerBound( lowerBound )
	, upperBound( upperBound )
	, numbersystem( numbersystem )
{
	// Read the number of decimals we have to fill with letters
	refreshDecimals();
	
	this->increaseHandle = new _scrollButton( 8 , 6 , this->dimensions.width - 9 , 1 , _scrollButtonType::buttonTop , _styleAttr() | _styleAttr::mouseClickRepeat );
	this->decreaseHandle = new _scrollButton( 8 , 8 , this->dimensions.width - 9 , 7 , _scrollButtonType::buttonBottom , _styleAttr() | _styleAttr::mouseClickRepeat );
	
	this->valueLabel = new _label( this->getWidth() - 9 , this->getHeight() - 2 , 1 , 1 , int2string( value , this->decimals , this->numbersystem ) );
	this->valueLabel->setAlign( _align::center );
	this->valueLabel->setVAlign( _valign::middle );
	this->valueLabel->setFont( _system::getFont( "CourierNew10" ) );
	
	this->increaseHandle->registerEventHandler( onAction , new _staticCallback( &_counter::changeHandler ) );
	this->decreaseHandle->registerEventHandler( onAction , new _staticCallback( &_counter::changeHandler ) );
	
	//! Refresh - Handler
	this->registerEventHandler( refresh , new _staticCallback( &_counter::refreshHandler ) );
	this->registerEventHandler( keyDown , new _staticCallback( &_counter::changeHandler ) );
	this->registerEventHandler( onBlur , new _staticCallback( &_counter::changeHandler ) );
	this->registerEventHandler( onFocus , new _staticCallback( &_counter::changeHandler ) );
	
	this->addEnhancedChild( this->increaseHandle );
	this->addEnhancedChild( this->decreaseHandle );
	this->addChild( this->valueLabel );
	
	// Refresh Me
	this->refreshBitmap();
}

_counter::_counter( _coord x , _coord y , _length width , bool circular , _s32 value , _style style ) :
	_counter( x , y , width , circular , value , 99 , 0 , 10 , style )
{ }

_counter::~_counter()
{
	if( this->increaseHandle )
		delete this->increaseHandle;
	if( this->decreaseHandle )
		delete this->decreaseHandle;
	if( this->valueLabel )
		delete this->valueLabel;
}

void _counter::setIntValue( int value )
{
	if( value > this->upperBound )
		value = this->circular ? this->lowerBound : this->upperBound;
	else if( value < this->lowerBound )
		value = this->circular ? this->upperBound : this->lowerBound;
	
	if( value != this->intValue )
	{
		this->intValue = value;
		this->valueLabel->setStrValue( int2string( value , this->decimals , this->numbersystem ) );
	}
}