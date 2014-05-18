#include "_gadget/gadget.textbox.h"
#include "_type/type.windows.soundbank.h"
#include "_controller/controller.font.h"

_rect _textBox::getGuiStringDimensions() const {
	return _rect( 2 - this->scroll , 0 , this->text.getPreferredTextWidth() , this->getHeight() );
}

void _textBox::setInternalCursor( _u32 newCursor , bool enabled )
{
	this->text.setCursorEnabled( enabled );
	
	// Check if needed to update things
	if( newCursor != this->text.getCursor() )
	{
		// Set Cursor
		this->text.setCursor( newCursor );
		
		//if( this->align == _align::left ) // Only available
		//{
		//	// Copy the string and..
		//	string str = this->text;
		//	char* cstr = new char[str.length() + 1];
		//	strcpy( cstr , str.c_str() );
		//	
		//	_coord fontPosition = this->getFontPosition( true ).first;
		//	_length fontWidth = this->font->getStringWidth( cstr ); // Get whole Length of the string
		//	
		//	_s32 minValueScroll = max( fontPosition + fontWidth - this->getWidth() + _textBox::borderX - 1 , 0 );
		//	
		//	// Resize string!
		//	stringExtractor::strResize( cstr , newCursor - 1 );
		//	fontWidth = this->font->getStringWidth( cstr );
		//	
		//	delete[] cstr;
		//	
		//	_s32 minScroll = fontPosition + fontWidth - _textBox::borderX;
		//	_s32 maxCursorScroll = fontPosition + fontWidth - this->getWidth() + _textBox::borderX - 1;
		//	
		//	this->scroll = min( min( minScroll , minValueScroll ) , _s32(this->scroll) );
		//	this->scroll = max( maxCursorScroll , _s32(this->scroll) );
		//}
	}
	
	this->redraw();
}

void _textBox::removeStr( _int position , _length numChars ){
	this->text.erase( position , numChars );
}

void _textBox::insertStr( _int position , string s ){
	this->text.insert( position , move(s) );
}

_callbackReturn _textBox::refreshHandler( _event event )
{
	// Receive Gadget
	_textBox* that = event.getGadget<_textBox>();

	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	// Cache dimensions
	_length myH = that->getHeight();
	_length myW = that->getWidth();
	
	// Draw Background
	bP.fill( that->bgColor );
	
	// Draw Text
	that->text.drawTo( that->getGuiStringDimensions() , bP );
	
	_callbackReturn ret = that->handleEventUser( move(event) );
	
	if( ret == not_handled || ret == use_default )
	{
		if( !that->isPressed() )
			bP.drawRect( 0 , 0 , myW , myH , _color::fromRGB( 13 , 16 , 23 ) );
		else
			bP.drawRect( 0 , 0 , myW , myH , _color::fromRGB( 9 , 13 , 19 ) );
	}
	
	return use_default;
}

_callbackReturn _textBox::keyHandler( _event event )
{
	_textBox* that = event.getGadget<_textBox>();
	
	string val = that->text;
	
	switch( event.getKeyCode() ){
		case _key::backspace:
		case _key::b:
			if( that->text.getCursor() < 1 ){
				_windowsSoundBank::play( _windowsSound::ping );
				break;
			}
			that->removeStr( that->text.getCursor() - 1 );
			that->setCursor( that->text.getCursor() - 1 );
			
			// Trigger Handler
			that->triggerEvent( onEdit );
			break;
		case _key::carriage_return:
			break;
		case _key::left:
		case _key::right:
			that->text.moveCursor( event.getKeyCode() == _key::right );
			that->checkRefresh();
			break;
		default:
			if(
				_hardwareKeyPattern::isHardwareKey( event.getKeyCode() )
				|| !isprint( (_char)event.getKeyCode() ) // Check if printable
			)
				break;
			
			that->insertStr( that->text.getCursor() , string( 1 , (_char)event.getKeyCode() ) );
			that->setCursor( that->text.getCursor() + 1 );
			
			// Trigger Handler
			that->triggerEvent( onEdit );
			break;
	}
	
	return handled;
}

_callbackReturn _textBox::focusHandler( _event event )
{
	_textBox* that = event.getGadget<_textBox>();
	
	// Set Cursor
	_u32 cursor = event == onFocus ? that->text.getNumLetters() : 0;
	that->setInternalCursor( cursor , event == onFocus );
	
	return use_default;
}

_callbackReturn _textBox::updateHandler( _event event )
{
	_textBox* that = event.getGadget<_textBox>();
	
	// Set Height of textbox depending on the text height
	that->setHeightIfAuto( that->text.getTextHeight() + 2 );
	
	if( that->hasAutoHeight() )
		that->text.indicateNewDimensions(); // Request an update
	
	// Check, if the _guiString object needs to be updated and do so
	that->checkUpdate();
	
	return handled;
}

_callbackReturn _textBox::mouseHandler( _event event )
{
	_textBox* that = event.getGadget<_textBox>();
	
	_coord position = event.getPosX();
	
	if( event == onDragging )
		position -= that->getX(); // X-Coordinate of stylus relative to this _textBox
	
	that->text.setCursorFromTouch( that->getGuiStringDimensions() , position , 0 );
	that->checkRefresh();
	
	return handled;
}

_textBox::_textBox( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string value , _style&& style ) :
	_gadget( _gadgetType::textbox , x , y , width , height , style | _style::keyboardRequest | _style::draggable | _style::smallDragThld )
	, bgColor( _color::fromRGB( 31 , 31 , 31 ) )
	, scroll( 0 )
	, text( move(value) , _fontController::getStandardFont() , _color::fromRGB( 0 , 0 , 0 ) , _fontController::getStandardFontSize() )
{
	// Adjust guiString object
	this->text.setFontChangeEnabled( false );
	this->text.setFontColorChangeEnabled( false );
	this->text.setFontSizeChangeEnabled( false );
	
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