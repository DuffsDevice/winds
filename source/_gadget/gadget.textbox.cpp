#include "_gadget/gadget.textbox.h"
#include "_gadget/gadget.windows.h"
#include "_type/type.system.h"

void _textbox::setFont( const _font* ft )
{
	if( this->font != ft )
	{ 
		this->font = ft;
		if( this->font && this->font->valid() )
			this->setHeight( max( 10 , this->font->getHeight() + 2 ) );
	}
}

void _textbox::setInternalCursor( _u32 cursor )
{
	this->cursor = cursor;
	
	if( this->align == _align::left ) // Only available
	{
		string str = this->getStrValue();
		
		_coord fontPosition = this->getFontPosition( true ).first;
		
		_length fontWidth = this->font->getStringWidth( str ); // Get whole Length of the string
		
		_s32 minValueScroll = max( fontPosition + fontWidth - this->getWidth() + _textbox::borderX - 1 , 0 );
		
		// Choose Fastest method
		if( this->cursor > ( str.length() >> 1 ) )
			fontWidth -= this->font->getStringWidth( str.substr( this->cursor - 1 ) ); // Subtract the end of the string
		else
		{
			str.resize( max( _u32(1) , this->cursor ) - 1 );
			fontWidth = this->font->getStringWidth( str );
		}
		
		_s32 minScroll = fontPosition + fontWidth - _textbox::borderX;
		_s32 maxCursorScroll = fontPosition + fontWidth - this->getWidth() + _textbox::borderX - 1;
		
		this->scroll = min( min( minScroll , minValueScroll ) , _s32(this->scroll) );
		this->scroll = max( maxCursorScroll , _s32(this->scroll) );
	}
	
	this->bubbleRefresh( true );
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
			x = ( this->getWidth() >> 1 ) - ( ( this->font->getStringWidth( this->getStrValue() ) - 1 ) >> 1 );
			break;
		case _align::left:
			x = _textbox::borderX;
			break;
		case _align::right:
			x = this->getWidth() - this->font->getStringWidth( this->getStrValue() ) - _textbox::borderX;
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
	if( that->font && that->font->valid() )
	{
		_2s32 pos = that->getFontPosition();
		
		// Draw Text...
		bP.drawString( pos.first , pos.second , that->font , that->getStrValue() , that->color );
		
		if( that->cursor > 0 )
		{
			// Get String until cursor
			string str = that->getStrValue();
			str.resize( that->cursor - 1 );
			
			_length strWidthUntilCursor = that->font->getStringWidth( str );
			
			bP.drawVerticalLine( strWidthUntilCursor + pos.first - 1 , pos.second - 1 , that->font->getAscent() + 2 , RGB( 31 , 0 , 0 ) );
		}
	}
	
	if( !that->isPressed() )
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
			
			// Refresh
			that->strValue = val;
			that->setInternalCursor( that->cursor - 1 );
			
			that->triggerEvent( _event( onChange ) );
			break;
		case DSWindows::KEY_CARRIAGE_RETURN:
			break;
		case DSWindows::KEY_LEFT:
			if( that->cursor > 1 )
				that->setInternalCursor( that->cursor - 1 );
			break;
		case DSWindows::KEY_RIGHT:
			if( that->cursor > 0 && val.length() - that->cursor + 1 > 0 )
				that->setInternalCursor( that->cursor + 1 );
			break;
		default:
			if( 
				DSWindows::isHardwareKey( event.getKeyCode() ) 
				|| !isprint( event.getKeyCode() ) // Check if printable
			)
				break;
			val.insert( that->cursor - 1 , 1 , event.getKeyCode() );
			
			// Refresh
			that->strValue = val;
			that->setInternalCursor( that->cursor + 1 );
			
			that->triggerEvent( _event( onChange ) );
			break;
	}
	
	return handled;
}

_callbackReturn _textbox::focusHandler( _event event )
{
	// Set Cursor
	event.getGadget<_textbox>()->setInternalCursor( 1 );
	
	return use_default;
}

_callbackReturn _textbox::blurHandler( _event event )
{
	// Remove Cursor!
	event.getGadget<_textbox>()->setInternalCursor( 0 );
	
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
	
	if( !that->font || !that->font->valid() )
		return handled;
	
	string 	str = that->getStrValue();
	_length strLen = str.length();
	_length pos = 0;
	
	_2s32 fontPos = that->getFontPosition();
	
	// Ensure you cannot have the iterator before the first letter
	position = max( _coord(fontPos.first) , position );
	
	for( ; fontPos.first <= position + 2 && pos <= strLen; pos++ )
		fontPos.first += that->font->getCharacterWidth( str[pos] ) + 1;
	
	that->setInternalCursor( pos );
	
	return handled;
}

_textbox::_textbox( _coord x , _coord y , _length width , _length height , string text , _style style ) :
	_gadget( _gadgetType::textbox , width , height , x , y , style | _styleAttr::keyboardRequest | _styleAttr::draggable | _styleAttr::smallDragTrig )
	, color( RGB( 0 , 0 , 0 ) )
	, bgColor( RGB( 31 , 31 , 31 ) )
	, font ( _system::getFont() )
	, fontSize( _system::_runtimeAttributes_->defaultFontSize )
	, align( _align::left )
	, vAlign( _valign::middle )
	, strValue( text )
	, cursor( 0 )
	, scroll( 0 )
{
	// Regsiter Handling Functions for events
	this->registerEventHandler( onFocus , new _staticCallback( &_textbox::focusHandler ) );
	this->registerEventHandler( onBlur , new _staticCallback( &_textbox::blurHandler ) );
	this->registerEventHandler( refresh , new _staticCallback( &_textbox::refreshHandler ) );
	this->registerEventHandler( mouseDown , new _staticCallback( &_textbox::mouseHandler ) );
	this->registerEventHandler( keyDown , new _staticCallback( &_textbox::keyHandler ) );
	this->registerEventHandler( keyRepeat , new _staticCallback( &_textbox::keyHandler ) );
	this->registerEventHandler( dragging , new _staticCallback( &_textbox::mouseHandler ) );
	
	// Refresh Myself
	this->refreshBitmap();
}

// C++0x I Love you! Delegating Ctors! Yeehaa...
_textbox::_textbox( _coord x , _coord y , _length width , string text , _style style ) :
	_textbox( x , y , width , 10 , text , style )
{
	if( this->font && this->font->valid() )
		this->setHeight( this->font->getHeight() + 2 );
}