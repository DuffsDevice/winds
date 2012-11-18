#include "_gadget/gadget.textbox.h"
#include "_gadget/gadget.windows.h"
#include "_type/type.system.h"

_callbackReturn _textbox::refreshHandler( _event event )
{
	// Receive Gadget
	_textbox* that = event.getGadget<_textbox>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
	
	// If there is no font it doesn't make sense to paint
	if( !that->font || !that->font->valid() )
		return use_default;
	
	_length myH = that->bitmap->getHeight();
	_length myW = that->bitmap->getWidth();
	
	bP.drawFilledRect( 1 , 1 , myW - 2 , myH - 2 , that->bgColor );
	
	_coord x = 0;
	_coord y = 0;
	
	switch( that->getAlign() )
	{
		case _align::center:
			x = ( myW >> 1 ) - ( ( that->font->getStringWidth( that->getStrValue() ) - 1 ) >> 1 );
			break;
		case _align::left:
		case _align::optimize:
			x = 2;
			break;
		case _align::right:
			x = myW - that->font->getStringWidth( that->getStrValue() );
			break;
	}
	
	switch( that->getVAlign() )
	{
		case _valign::middle:
			y = ( ( myH + 1 ) >> 1 ) - ( ( that->font->getAscent() + 1 ) >> 1 );
			break;
		case _valign::top:
			y = 2;
			break;
		case _valign::bottom:
			y = myH - that->font->getAscent();
			break;
	}
	
	// Draw Text...
	bP.drawString( x , y , that->font , that->getStrValue() , that->color );
	
	if( !that->pressed )
		bP.drawRect( 0 , 0 , myW , myH , RGB( 13 , 16 , 23 ) );
	else
		bP.drawRect( 0 , 0 , myW , myH , RGB( 9 , 13 , 19 ) );
	
	if( that->cursor )
		bP.drawVerticalLine( that->font->getStringWidth( that->getStrValue().substr( 0 , that->cursor-1 ) ) + 1 , y - 1 , that->font->getAscent() + 2 , RGB( 31 , 0 , 0 ) );
	
	return use_default;
}

_callbackReturn _textbox::keyHandler( _event event )
{
	_textbox* that = event.getGadget<_textbox>();
	
	string val = that->getStrValue();
	
	switch( event.getKeyCode() ){
		case DSWindows::KEY_BACKSPACE:
		case DSWindows::KEY_B:
			if( !(val.length()) || that->cursor < 2 ){
				//_system_->errorTone();
				break;
			}
			val = val.erase( that->cursor - 2 , 1 );
			that->cursor--;
			break;
		case DSWindows::KEY_CARRIAGE_RETURN:
			break;
		case DSWindows::KEY_LEFT:
			that->cursor-1 && that->cursor--;
			break;
		case DSWindows::KEY_RIGHT:
			val.length() - that->cursor + 1 > 0 && that->cursor++;
			break;
		default:
			if( DSWindows::isHardwareKey( event.getKeyCode() ) )
				break;
			val.insert( that->cursor - 1 , 1 , event.getKeyCode() );
			that->cursor++;
			break;
	}
	
	that->setStrValue( val );
	
	return handled;
}

_callbackReturn _textbox::focusHandler( _event event )
{
	// Open the Keyboard
	if( _system_->_keyboard_ )
		_system_->_keyboard_->setDestination( event.getGadget() );
	
	event.getGadget<_textbox>()->pressed = true;
	
	return use_default;
}

_callbackReturn _textbox::blurHandler( _event event )
{
	_textbox* that = event.getGadget<_textbox>();
	
	if( _system_->_keyboard_ )
		_system_->_keyboard_->setDestination( nullptr );
	
	// Remove Cursor!
	that->cursor = 0;
	that->pressed = false;
	
	// Refresh
	that->bubbleRefresh( true );
	
	return use_default;
}

_callbackReturn _textbox::mouseHandler( _event event )
{
	_textbox* that = event.getGadget<_textbox>();
	
	if( event.getType() == dragStart )
		return handled;
	
	_coord position = event.getPosX();
	
	if( event.getType() == dragging )
		position -= that->getX();
	
	position = max( 0 , (int)position );
	
	string 	str = that->getStrValue();
	_length strLen = str.length();
	_length pos = 0;
	
	for( _coord l = 0 ; l < position + 1 && pos <= strLen; pos++ )
		l += that->font->getCharacterWidth( str[pos] ) + 1;
	
	that->cursor = pos;
	
	that->bubbleRefresh( true );
	
	return handled;
}

_textbox::_textbox( _coord x , _coord y , _length width , string text , _style style ) :
	_label( width , 10 , x , y , text , style )
	, cursor( 0 )
	, pressed( false )
{
	// Set Label
	this->setVAlign( _valign::middle );
	this->setAlign( _align::left );
	
	_label::setBgColor( RGB( 31 , 31 , 31 ) );
	_label::setColor( RGB( 3 , 3 , 3 ) );
	
	// Regsiter Handling Functions for events
	this->unregisterEventHandler( mouseDoubleClick );
	this->registerEventHandler( onFocus , &_textbox::focusHandler );
	this->registerEventHandler( onBlur , &_textbox::blurHandler );
	this->registerEventHandler( refresh , &_textbox::refreshHandler );
	this->registerEventHandler( mouseDown , &_textbox::mouseHandler );
	this->registerEventHandler( keyDown , &_textbox::keyHandler );
	this->registerEventHandler( dragStart , &_textbox::mouseHandler );
	this->registerEventHandler( dragging , &_textbox::mouseHandler );
	
	// Refresh Myself
	this->refreshBitmap();
}