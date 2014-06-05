#include "_gadget/gadget.textbox.h"
#include "_type/type.windows.soundbank.h"
#include "_controller/controller.font.h"

_rect _textBox::getGuiStringDimensions() const {
	_length width = this->hasFocus() ? this->text.getPreferredTextWidth() : ( this->getWidth() - 4 );
	return _rect( 2 - this->scroll , 0 , width , this->getHeight() );
}

bool _textBox::makeSureCursorIsVisible()
{
	_s32 newScroll = this->scroll;
	
	if( this->text.isCursorEnabled() )
	{
		// Determine new scroll
		_u32 cursorPosition = this->text.getXMetricsOfLetter( this->getGuiStringDimensions() , this->text.getCursor() ).first - 1;
		_s32 minimumScroll = newScroll + ( cursorPosition - this->getWidth() + 4 );
		_s32 maximumScroll = newScroll + cursorPosition - 1;
		
		// This value controlles, that if the text is longer than the textbox, the text fills the textbox
		_s32 maximumScroll2 = this->text.getPreferredTextWidth() - this->getWidth() + 4;
		
		newScroll = min( max( 0 , maximumScroll2 ) , newScroll );
		newScroll = max( minimumScroll , newScroll );
		newScroll = min( maximumScroll , newScroll );
	}
	else
		newScroll = 0;
	
	if( newScroll != this->scroll ){
		this->scroll = newScroll;
		this->redraw();
		return true;
	}
	
	return false;
}

void _textBox::setInternalCursor( _u32 newCursor , bool enabled )
{
	this->text.setCursorEnabled( enabled );
	
	// Check if needed to update things
	if( enabled && newCursor != this->text.getCursor() )
		this->text.setCursor( newCursor ); // Set Cursor
	
	if( this->makeSureCursorIsVisible() )
		return;
	
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
			if( !that->makeSureCursorIsVisible() )
				that->checkRefresh();
			break;
		case _key::up:
			that->setInternalCursor( 0 , true );
			break;
		case _key::down:
			that->setInternalCursor( that->text.getNumLetters() , true );
			break;
		default:
			if(
				_hardwareKeyPattern::isHardwareKey( event.getKeyCode() )
				|| !isprint( (_char)event.getKeyCode() ) // Check if printable
			)
				break;
			
			that->insertStr( that->text.getCursor() , string( 1 , (_char)event.getKeyCode() ) );
			that->text.moveCursor( true );
			if( !that->makeSureCursorIsVisible() )
				that->redraw();
			
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
	that->handleEvent( onResize );
	that->setInternalCursor( cursor , event == onFocus );
	that->checkRefresh();
	
	return use_default;
}

_callbackReturn _textBox::resizeHandler( _event event )
{
	event.getGadget<_textBox>()->text.indicateNewDimensions();
	return handled;
}

_callbackReturn _textBox::updateHandler( _event event )
{
	_textBox* that = event.getGadget<_textBox>();
	
	// Check, if the _guiString object needs to be updated and do so
	that->checkUpdate();
	
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
	if( !that->makeSureCursorIsVisible() )
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
	this->text.setEllipsis( 0 );
	
	// Set update Handler & update
	this->setInternalEventHandler( onUpdate , make_callback( &_textBox::updateHandler ) );
	this->updateNow();
	
	// Regsiter Handling Functions for events
	this->setInternalEventHandler( onResize , make_callback( &_textBox::resizeHandler ) );
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