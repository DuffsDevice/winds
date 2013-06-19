#include "_gadget/gadget.textbox.h"
#include "_gadget/gadget.windows.h"
#include "_type/type.system.h"
#include "_type/type.textphrases.h"

void _textbox::setFont( const _font* ft )
{
	if( this->font != ft )
	{ 
		this->font = ft;
		if( this->font && this->font->isValid() )
			this->setHeight( max( 10 , this->font->getHeight() + 2 ) );
	}
}

void _textbox::setInternalCursor( _u32 cursor )
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
		
		_s32 minValueScroll = max( fontPosition + fontWidth - this->getWidth() + _textbox::borderX - 1 , 0 );
		
		// Resize string!
		stringExtractor::strResize( cstr , this->cursor - 1 );
		fontWidth = this->font->getStringWidth( cstr );
		
		delete[] cstr;
		
		_s32 minScroll = fontPosition + fontWidth - _textbox::borderX;
		_s32 maxCursorScroll = fontPosition + fontWidth - this->getWidth() + _textbox::borderX - 1;
		
		this->scroll = min( min( minScroll , minValueScroll ) , _s32(this->scroll) );
		this->scroll = max( maxCursorScroll , _s32(this->scroll) );
	}
	
	this->bubbleRefresh( true );
}

void _textbox::removeStr( _int position , _length numChars )
{
	this->strValue.erase( position , numChars );
}
void _textbox::insertStr( _int position , string s )
{
	this->strValue.insert( position , s );
}

void _textbox::setStrValue( string value )
{
	if( this->strValue == value )
		return;
	
	this->strValue = value;
	this->setInternalCursor( min( cursor , _u32( value.length() + 1 ) ) );
}

_2s32 _textbox::getFontPosition( bool noScroll )
{
	_coord x = 0;
	_coord y = 0;
	
	switch( this->getAlign() )
	{
		case _align::center:
			x = ( this->getWidth() >> 1 ) - ( ( this->font->getStringWidth( this->strValue ) - 1 ) >> 1 );
			break;
		case _align::left:
			x = _textbox::borderX;
			break;
		case _align::right:
			x = this->getWidth() - this->font->getStringWidth( this->strValue ) - _textbox::borderX;
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
			y = _textbox::borderY;
			break;
		case _valign::bottom:
			y = this->getHeight() - ( ( this->font->getAscent() + this->font->getHeight() ) >> 1 ) - _textbox::borderY;
			break;
	}
	
	if( !noScroll )
		x -= this->scroll;
	
	return _2s32( x , y );
}

_callbackReturn _textbox::refreshHandler( _event event )
{
	// Receive Gadget
	_textbox* that = event.getGadget<_textbox>();

	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsolutePosition() ) );
	else
		bP.normalizeClippingRects();
	
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

_callbackReturn _textbox::keyHandler( _event event )
{
	_textbox* that = event.getGadget<_textbox>();
	
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
			that->triggerEvent( _event( onChange ) );
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
			that->triggerEvent( _event( onChange ) );
			break;
	}
	
	return handled;
}

_callbackReturn _textbox::focusHandler( _event event )
{
	// Set Cursor
	event.getGadget<_textbox>()->setInternalCursor( event.getType() == onFocus ? stringExtractor::strLen( event.getGadget<_textbox>()->strValue.c_str() ) + 1 : 0 );
	
	return use_default;
}

_callbackReturn _textbox::mouseHandler( _event event )
{
	_textbox* that = event.getGadget<_textbox>();
	
	_coord position = event.getPosX();
	
	if( event.getType() == dragging )
	{
		// X-Coordinate of stylus relative to this _textbox
		position -= that->getX();
	}
	
	if( !that->font || !that->font->isValid() )
		return handled;
	
	_length pos = 0;
	
	_2s32 fontPos = that->getFontPosition();
	
	pos = that->font->getNumCharsUntilWidth( position - fontPos.first + 1 , that->strValue , that->fontSize );
	
	// Ensure you cannot have the iterator before the first letter
	that->setInternalCursor( max( 1 , pos + 1 ) );
	
	return handled;
}

_textbox::_textbox( _coord x , _coord y , _length width , _length height , string text , _style&& style ) :
	_gadget( _gadgetType::textbox , width , height , x , y , style | _styleAttr::keyboardRequest | _styleAttr::draggable | _styleAttr::smallDragTrig )
	, color( RGB( 0 , 0 , 0 ) )
	, bgColor( RGB( 31 , 31 , 31 ) )
	, font ( _system::getFont() )
	, fontSize( _system::_rtA_->getDefaultFontSize() )
	, align( _align::left )
	, vAlign( _valign::middle )
	, strValue( text )
	, cursor( 0 )
	, scroll( 0 )
{
	// Regsiter Handling Functions for events
	this->setInternalEventHandler( onFocus , _staticCallback( &_textbox::focusHandler ) );
	this->setInternalEventHandler( onBlur , _staticCallback( &_textbox::focusHandler ) );
	this->setInternalEventHandler( refresh , _staticCallback( &_textbox::refreshHandler ) );
	this->setInternalEventHandler( mouseDown , _staticCallback( &_textbox::mouseHandler ) );
	this->setInternalEventHandler( keyDown , _staticCallback( &_textbox::keyHandler ) );
	this->setInternalEventHandler( keyRepeat , _staticCallback( &_textbox::keyHandler ) );
	this->setInternalEventHandler( dragging , _staticCallback( &_textbox::mouseHandler ) );
	
	// Refresh Myself
	this->refreshBitmap();
}

// C++0x I Love you! Delegating Ctors! Yeehaa...
_textbox::_textbox( _coord x , _coord y , _length width , string text , _style&& style ) :
	_textbox( x , y , width , 10 , text , (_style&&)style )
{
	if( this->font && this->font->isValid() )
		this->setHeight( this->font->getHeight() + 2 );
}