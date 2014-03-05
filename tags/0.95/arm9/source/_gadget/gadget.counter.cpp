#include "_gadget/gadget.counter.h"
#include "_gadget/gadget.button.h"
#include "_type/type.system.h"
#include "func.memory.h"

_counter::_counter( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , bool circular , _s32 value , _optValue<_s32> upperBound , _optValue<_s32> lowerBound  , _optValue<_u8> numbersystem , _style&& style ) :
	_gadget( _gadgetType::counter , x , y , width , _system::getUser().cOH , (_style&&)style )
	, circular( circular )
	, lowerBound( lowerBound.isValid() ? (_s32)lowerBound : 0 )
	, upperBound( upperBound.isValid() ? (_s32)upperBound : 99 )
	, intValue( mid( lowerBound , value , upperBound ) )
	, numbersystem( numbersystem.isValid() ? (_u8)numbersystem : 10 )
{
	this->setMinWidth( 15 );
	
	// Regsiter draw and key Handlers
	this->setInternalEventHandler( onDraw , make_callback( &_counter::refreshHandler ) );
	this->setInternalEventHandler( onKeyDown , make_callback( &_counter::keyHandler ) );
	this->setInternalEventHandler( onKeyRepeat , make_callback( &_counter::keyHandler ) );
	this->setInternalEventHandler( onBlur , make_callback( &_counter::focusHandler ) );
	this->setInternalEventHandler( onFocus , make_callback( &_counter::focusHandler ) );
	
	// Read the number of decimals we have to fill with letters
	this->refreshDecimals();
	
	//
	// Add Gadgets
	//
	
	// Create buttons for increasing and decreasing
	this->increaseHandle = new _scrollButton( this->getWidth() - 9 , 1 , 8 , 6 , _scrollButtonType::buttonTop );
	this->decreaseHandle = new _scrollButton( this->getWidth() - 9 , 7 , 8 , 8 , _scrollButtonType::buttonBottom );
	
	// Create Label to hold value
	this->valueLabel = new _label( 1 , 1 , this->getWidth() - 10 , this->getHeight() - 2 , int2string( value , this->decimals , this->numbersystem ) );
	this->valueLabel->setAlign( _align::center );
	this->valueLabel->setVAlign( _valign::middle );
	this->valueLabel->setFont( _system::getFont( "CourierNew10" ) );
	
	// Register onClick handlers to buttons
	this->increaseHandle->setUserEventHandler( onMouseClick , make_callback( this , &_counter::btnClickHandler ) );
	this->decreaseHandle->setUserEventHandler( onMouseClick , make_callback( this , &_counter::btnClickHandler ) );
	
	this->addEnhancedChild( this->increaseHandle );
	this->addEnhancedChild( this->decreaseHandle );
	this->addChild( this->valueLabel );
	
	// Refresh Me
	this->redraw();
}

_counter::~_counter()
{
	delete this->increaseHandle;
	delete this->decreaseHandle;
	delete this->valueLabel;
}

// For Key-Events
_callbackReturn _counter::keyHandler( _event event )
{
	_counter* that = event.getGadget<_counter>();
	
	if( event.getKeyCode() == _key::up )
		that->increaseInternal();
	else if( event.getKeyCode() == _key::down )
		that->decreaseInternal();
	
	return handled;
}

// For clicks onto one of the buttons
_callbackReturn _counter::btnClickHandler( _event event )
{	
	if( event.getGadget() == this->increaseHandle )
		this->increaseInternal();
	else if( event.getGadget() == this->decreaseHandle )
		this->decreaseInternal();
	
	return handled;
}

// Handles blur and focus of the colorpicker
_callbackReturn _counter::focusHandler( _event event )
{
	_counter* that = event.getGadget<_counter>();
	
	// Blur and Focus changes the style of the label
	that->valueLabel->setBgColor( _system::getRTA().getItemBackground( event == onFocus ) );
	that->valueLabel->setColor( _system::getRTA().getItemForeground( event == onFocus ) );
	
	return handled;
}

_callbackReturn _counter::refreshHandler( _event event )
{
	// Receive Gadget
	_counter* that = event.getGadget<_counter>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	bP.fill( _color::white ); // reset Color
	bP.drawRect( 0 , 0 , bP.getWidth() , bP.getHeight() , _color::fromRGB( 9 , 13 , 19 ) );
	
	return use_default;
}

void _counter::setIntValue( _s32 value )
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