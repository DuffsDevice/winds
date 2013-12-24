#include "_gadget/gadget.textbox.h"
#include "_gadget/gadget.windows.h"
#include "_type/type.system.h"
#include "_type/type.text.phrases.h"

void _textBox::setFont( const _font* ft )
{
	if( this->font != ft )
	{ 
		this->font = ft;
		if( this->font && this->font->isValid() )
			this->setHeight( max( 10 , this->font->getHeight() + 2 ) );
	}
}

void _textBox::setInternalCursor( _u32 cursor )
{
	this->cursor = cursor;
	
	if( this->align == _align::left ) // Only available
	{
		// Copy the string and..
		string str = this->strValue;
		char* cstr = new char[str.length() + 1];
		strcpy( cstr , str.c_str() );
		
		_coord fontPosition = this->getFontPosition( true ).first;
		_length fontWidth = this->font->getStringWidth( cstr ); // Get whole Length of the string
		
		_s32 minValueScroll = max( fontPosition + fontWidth - this->getWidth() + _textBox::borderX - 1 , 0 );
		
		// Resize string!
		stringExtractor::strResize( cstr , this->cursor - 1 );
		fontWidth = this->font->getStringWidth( cstr );
		
		delete[] cstr;
		
		_s32 minScroll = fontPosition + fontWidth - _textBox::borderX;
		_s32 maxCursorScroll = fontPosition + fontWidth - this->getWidth() + _textBox::borderX - 1;
		
		this->scroll = min( min( minScroll , minValueScroll ) , _s32(this->scroll) );
		this->scroll = max( maxCursorScroll , _s32(this->scroll) );
	}
	
	this->redraw();
}

void _textBox::removeStr( _int position , _length numChars )
{
	this->strValue.erase( position , numChars );
}
void _textBox::insertStr( _int position , string s )
{
	this->strValue.insert( position , s );
}

void _textBox::setStrValue( string value )
{
	if( this->strValue == value )
		return;
	
	this->strValue = value;
	this->setInternalCursor( min( cursor , _u32( value.length() + 1 ) ) );
}

_2s32 _textBox::getFontPosition( bool noScroll )
{
	_coord x = 0;
	_coord y = 0;
	
	switch( this->getAlign() )
	{
		case _align::center:
			x = ( this->getWidth() >> 1 ) - ( ( this->font->getStringWidth( this->strValue ) - 1 ) >> 1 );
			break;
		case _align::left:
			x = _textBox::borderX;
			break;
		case _align::right:
			x = this->getWidth() - this->font->getStringWidth( this->strValue ) - _textBox::borderX;
			break;
	}
	
	switch( this->getVAlign() )
	{
		case _valign::middle:
			y = ( ( this->getHeight() + 1 ) >> 1 )
				- ( 
					(
						(
							( this->font->getAscent() + this->font->getHeight() ) >> 1 // This is the avg height of the font
						) + 1 
					) >> 1
				);
			break;
		case _valign::top:
			y = _textBox::borderY;
			break;
		case _valign::bottom:
			y = this->getHeight() - ( ( this->font->getAscent() + this->font->getHeight() ) >> 1 ) - _textBox::borderY;
			break;
	}
	
	if( !noScroll )
		x -= this->scroll;
	
	return _2s32( x , y );
}

_callbackReturn _textBox::refreshHandler( _event event )
{
	// Receive Gadget
	_textBox* that = event.getGadget<_textBox>();

	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	_length myH = that->getHeight();
	_length myW = that->getWidth();
	
	bP.drawFilledRect( 1 , 1 , myW - 2 , myH - 2 , that->bgColor );
	
	// If there is no font it doesn't make sense to paint
	if( that->font && that->font->isValid() )
	{
		_2s32 pos = that->getFontPosition();
		
		// Draw Text...
		bP.drawString( pos.first , pos.second , that->font , that->strValue , that->color , that->fontSize );
		
		if( that->cursor > 0 )
		{
			// Get String until cursor
			string str = that->strValue;
			
			// Copy the string and..
			char* cstr = new char[str.length() + 1];
			strcpy( cstr , str.c_str() );
			
			// Shorten it
			stringExtractor::strResize( cstr , that->cursor - 1 );
			
			_length strWidthUntilCursor = that->font->getStringWidth( cstr );
			
			// Free the allocated copy
			delete[] cstr;
			
			bP.drawVerticalLine( strWidthUntilCursor + pos.first - 1 , pos.second - 1 , that->font->getAscent() + 2 , RGB( 31 , 0 , 0 ) );
		}
	}
	
	_callbackReturn ret = that->handleEventUser( event );
	
	if( ret == not_handled )
	{
		if( !that->isPressed() )
			bP.drawRect( 0 , 0 , myW , myH , RGB( 13 , 16 , 23 ) );
		else
			bP.drawRect( 0 , 0 , myW , myH , RGB( 9 , 13 , 19 ) );
	}
	
	return use_default;
}

_callbackReturn _textBox::keyHandler( _event event )
{
	_textBox* that = event.getGadget<_textBox>();
	
	string val = that->strValue;
	
	switch( event.getKeyCode() ){
		case DSWindows::KEY_BACKSPACE:
		case DSWindows::KEY_B:
			if( !(val.length()) || that->cursor < 2 ){
				//_system::errorTone();
				break;
			}
			that->removeStr( that->cursor - 2 );
			that->setInternalCursor( that->cursor - 1 );
			
			// Trigger Handler
			that->triggerEvent( onEdit );
			break;
		case DSWindows::KEY_CARRIAGE_RETURN:
			break;
		case DSWindows::KEY_LEFT:
			if( that->cursor > 1 )
				that->setInternalCursor( that->cursor - 1 );
			break;
		case DSWindows::KEY_RIGHT:
			if( that->cursor && stringExtractor::strLen( that->strValue.c_str() ) - that->cursor + 1 > 0 )
				that->setInternalCursor( that->cursor + 1 );
			break;
		default:
			if(
				DSWindows::isHardwareKey( event.getKeyCode() )
				|| !isprint( event.getKeyCode() ) // Check if printable
			)
				break;
			
			that->insertStr( that->cursor - 1 , string( 1 , event.getKeyCode() ) );
			that->setInternalCursor( that->cursor + 1 );
			
			// Trigger Handler
			that->triggerEvent( onEdit );
			break;
	}
	
	return handled;
}

_callbackReturn _textBox::focusHandler( _event event )
{
	// Set Cursor
	event.getGadget<_textBox>()->setInternalCursor( event == onFocus ? stringExtractor::strLen( event.getGadget<_textBox>()->strValue.c_str() ) + 1 : 0 );
	
	return use_default;
}

_callbackReturn _textBox::updateHandler( _event event )
{
	_textBox* that = event.getGadget<_textBox>();
	
	if( that->font && that->font->isValid() )
		that->setHeightIfAuto( that->font->getHeight() + 2 );
	
	return handled;
}

_callbackReturn _textBox::mouseHandler( _event event )
{
	_textBox* that = event.getGadget<_textBox>();
	
	_coord position = event.getPosX();
	
	if( event == onDragging )
	{
		// X-Coordinate of stylus relative to this _textBox
		position -= that->getX();
	}
	
	if( !that->font || !that->font->isValid() )
		return not_handled;
	
	_length pos = 0;
	
	_2s32 fontPos = that->getFontPosition();
	
	pos = that->font->getNumCharsUntilWidth( position - fontPos.first + 1 , that->strValue , that->fontSize );
	
	// Ensure you cannot have the iterator before the first letter
	that->setInternalCursor( max( 1 , pos + 1 ) );
	
	return handled;
}

_textBox::_textBox( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string text , _style&& style ) :
	_gadget( _gadgetType::textbox , x , y , width , height , style | _styleAttr::keyboardRequest | _styleAttr::draggable | _styleAttr::smallDragTrig )
	, color( RGB( 0 , 0 , 0 ) )
	, bgColor( RGB( 31 , 31 , 31 ) )
	, font ( _system::getFont() )
	, fontSize( _system::getRTA().getDefaultFontSize() )
	, align( _align::left )
	, vAlign( _valign::middle )
	, strValue( text )
	, cursor( 0 )
	, scroll( 0 )
{
	// Set update Handler
	this->setInternalEventHandler( onUpdate , make_callback( &_textBox::updateHandler ) );
	
	this->updateNow();
	
	// Regsiter Handling Functions for events
	this->setInternalEventHandler( onFocus , make_callback( &_textBox::focusHandler ) );
	this->setInternalEventHandler( onBlur , make_callback( &_textBox::focusHandler ) );
	this->setInternalEventHandler( onDraw , make_callback( &_textBox::refreshHandler ) );
	this->setInternalEventHandler( onMouseDown , make_callback( &_textBox::mouseHandler ) );
	this->setInternalEventHandler( onKeyDown , make_callback( &_textBox::keyHandler ) );
	this->setInternalEventHandler( onKeyRepeat , make_callback( &_textBox::keyHandler ) );
	this->setInternalEventHandler( onDragging , make_callback( &_textBox::mouseHandler ) );
	
	// Refresh Myself
	this->redraw();
}