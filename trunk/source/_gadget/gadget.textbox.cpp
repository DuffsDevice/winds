#include "_gadget/gadget.textbox.h"
#include "_gadget/gadget.windows.h"
#include "_type/type.system.h"

void _textbox::setFont( _font* ft ){
	if( this->font != ft )
	{ 
		this->font = ft;
		if( this->font && this->font->valid() )
			this->setHeight( max( 10 , this->font->getHeight() + 2 ) );
	}
}

void _textbox::setStrValue( string value )
{
	this->strValue = value;
	this->cursor = min( cursor , _length( value.length() + 1 ) );
	
	this->bubbleRefresh( true );
}

void _textbox::getFontPosition( _coord& x , _coord& y , _textbox* box )
{
	switch( box->getAlign() )
	{
		case _align::center:
			x = ( box->getWidth() >> 1 ) - ( ( box->font->getStringWidth( box->getStrValue() ) - 1 ) >> 1 );
			break;
		case _align::left:
			x = 3;
			break;
		case _align::right:
			x = box->getWidth() - box->font->getStringWidth( box->getStrValue() );
			break;
	}
	
	switch( box->getVAlign() )
	{
		case _valign::middle:
			y = ( ( box->getHeight() + 1 ) >> 1 )
				- ( 
					(
						(
							( box->font->getAscent() + box->font->getHeight() ) >> 1 // This is the avg height of the font
						) + 1 
					) >> 1
				);
			break;
		case _valign::top:
			y = 2;
			break;
		case _valign::bottom:
			y = box->getHeight() - box->font->getAscent();
			break;
	}
}

_callbackReturn _textbox::refreshHandler( _event event )
{
	// Receive Gadget
	_textbox* that = event.getGadget<_textbox>();

	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
	
	_length myH = that->getHeight();
	_length myW = that->getWidth();
	
	bP.drawFilledRect( 1 , 1 , myW - 2 , myH - 2 , that->bgColor );
	
	// If there is no font it doesn't make sense to paint
	if( that->font && that->font->valid() )
	{
		_coord x = 0;
		_coord y = 0;
		
		_textbox::getFontPosition( x , y , that );
		
		// Draw Text...
		bP.drawString( x , y , that->font , that->getStrValue() , that->color );
		
		if( that->cursor > 0 )
		{
			// Get String until cursor
			string str = that->getStrValue();
			str.resize( that->cursor - 1 );
			
			_length strWidthUntilCursor = that->font->getStringWidth( str );
			
			bP.drawVerticalLine( strWidthUntilCursor + x - 1 , y - 1 , that->font->getAscent() + 2 , RGB( 31 , 0 , 0 ) );
		}
	}
	
	if( !that->pressed )
		bP.drawRect( 0 , 0 , myW , myH , RGB( 13 , 16 , 23 ) );
	else
		bP.drawRect( 0 , 0 , myW , myH , RGB( 9 , 13 , 19 ) );
	
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
				//_system::errorTone();
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
			if( DSWindows::isHardwareKey( event.getKeyCode() ) 
				|| !isprint( event.getKeyCode() ) ) // Check if printable
				break;
			val.insert( that->cursor - 1 , 1 , event.getKeyCode() );
			that->cursor++;
			break;
	}
	
	that->triggerEvent( _event( onChange ) );
	
	that->setStrValue( val );
	
	return handled;
}

_callbackReturn _textbox::focusHandler( _event event )
{
	// Open the Keyboard
	if( _system::_keyboard_ )
		_system::_keyboard_->setDestination( event.getGadget() );
	
	event.getGadget<_textbox>()->pressed = true;
	
	return use_default;
}

_callbackReturn _textbox::blurHandler( _event event )
{
	_textbox* that = event.getGadget<_textbox>();
	
	if( _system::_keyboard_ )
		_system::_keyboard_->setDestination( nullptr );
	
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
	
	if( !that->font || !that->font->valid() )
		return handled;
	
	string 	str = that->getStrValue();
	_length strLen = str.length();
	_length pos = 0;
	
	_coord x = 0;
	_coord y = 0;
	
	_textbox::getFontPosition( x , y , that );
	
	// Ensure you cannot have the iterator before the first letter
	position = max( x , position );
	
	for( ; x <= position + 2 && pos <= strLen; pos++ )
		x += that->font->getCharacterWidth( str[pos] ) + 1;
	
	that->cursor = pos;
	
	that->bubbleRefresh( true );
	
	return handled;
}

_textbox::_textbox( _coord x , _coord y , _length width , string text , _style style ) :
	_gadget( _gadgetType::textbox , width , 10 , x , y , style )
	, color( RGB( 0 , 0 , 0 ) )
	, bgColor( RGB( 31 , 31 , 31 ) )
	, font ( _system::getFont() )
	, fontSize( _system::_runtimeAttributes_->defaultFontSize )
	, align( _align::left )
	, vAlign( _valign::middle )
	, strValue( text )
	, cursor( 0 )
	, pressed( false )
{
	if( this->font && this->font->valid() )
		this->setHeight( this->font->getHeight() + 2 );
	
	// Regsiter Handling Functions for events
	this->registerEventHandler( onFocus , new _staticCallback( &_textbox::focusHandler ) );
	this->registerEventHandler( onBlur , new _staticCallback( &_textbox::blurHandler ) );
	this->registerEventHandler( refresh , new _staticCallback( &_textbox::refreshHandler ) );
	this->registerEventHandler( mouseDown , new _staticCallback( &_textbox::mouseHandler ) );
	this->registerEventHandler( keyClick , new _staticCallback( &_textbox::keyHandler ) );
	this->registerEventHandler( dragStart , new _staticCallback( &_textbox::mouseHandler ) );
	this->registerEventHandler( dragging , new _staticCallback( &_textbox::mouseHandler ) );
	
	// Refresh Myself
	this->refreshBitmap();
}