#include "_gadget/gadget.textarea.h"
#include "_type/type.gadget.helpers.h"
#include "_type/type.windows.soundbank.h"
#include "_controller/controller.font.h"

_callbackReturn _textArea::refreshHandler( _event event )
{
	// Receive Gadget
	_textArea* that = event.getGadget<_textArea>();

	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
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

void _textArea::adjustScrollToCursor()
{
	// First, check if we need to update the text; then place cursor
	this->checkUpdate();
	
	// Letter index 2 byte index
	_s32 cursorIndex = this->text.getNumBytesFromNumLetters( this->text.getCursor() );
	
	// Get Position and height of cursor
	_2s32 dimensions = this->text.getYMetricsOfLine( this->getGuiStringDimensions() , this->text.getLineContainingIndex( cursorIndex ) );
	
	// Store Current Scroll
	_int scrollValue = this->scrollBar->getValue();
	
	// Make absolute
	dimensions.first += scrollValue;
	
	//
	// Adjust scrollbar position
	//
	
	// Make sure the cursor is not above the visible area
	if( scrollValue > dimensions.first - _textArea::borderY )
		scrollValue = dimensions.first - _textArea::borderY;
	
	// Make sure the cursor does not get displayed below the visible area
	if( this->getHeight() + scrollValue < dimensions.first + dimensions.second + _textArea::borderY )
		scrollValue = dimensions.first + dimensions.second + _textArea::borderY - this->getHeight();
	
	// Set Scroll
	this->scrollBar->setValue( scrollValue , false );
	
	// 'onScroll' updates the scroll of the textarea
	this->handleEvent( onScroll );
	
	// Refresh
	this->checkRefresh();
}


void _textArea::setCursor( _optValue<_u32> newCursor )
{
	this->text.setCursorEnabled( newCursor.isValid() );
	
	// Check if needed to update things
	if( newCursor != this->text.getCursor() )
	{
		// Set Cursor
		this->text.setCursor( newCursor );
		
		// Show cursor
		this->adjustScrollToCursor();
	}
}

_callbackReturn _textArea::keyHandler( _event event )
{
	_textArea* that = event.getGadget<_textArea>();
	
	switch( event.getKeyCode() ){
		case _key::backspace:
		case _key::b:
			if( that->text.getCursor() == 0 ){
				_windowsSoundBank::play( _windowsSound::ping );
				break;
			}
			// Refresh
			that->text.remove( that->text.getCursor() - 1 , 1 );
			
			// Notify content has changed
			that->triggerEvent( onEdit );
			
			// Update size
			that->update();
		case _key::left:
		case _key::right:
			that->text.moveCursor( event.getKeyCode() == _key::right );
			that->adjustScrollToCursor();
			break;
		case _key::down:
		case _key::up:
			that->text.moveCursorByLine( that->getGuiStringDimensions() , event.getKeyCode() == _key::down );
			that->adjustScrollToCursor();
			break;
		default:
			if( _hardwareKeyPattern::isHardwareKey( event.getKeyCode() ) 
				|| ( !isprint( (_char)event.getKeyCode() ) && !iscntrl( (_char)event.getKeyCode() ) ) // Check if printable
			)
				break;
			
			// Insert glyphCode()
			that->text.insert( that->text.getCursor() , (_char)event.getKeyCode() );
			
			// Notify that content has changed
			that->triggerEvent( onEdit );
			
			// Update scrollbars etc.
			that->updateNow();
			
			// Set Cursor
			that->setCursor( that->text.getCursor() + 1 );
			break;
	}
	
	return handled;
}

_callbackReturn _textArea::generalHandler( _event event )
{
	// Read gadget
	_textArea* that = event.getGadget<_textArea>();
	
	switch( event.getType() )
	{
		case onFocus:
			event.getGadget<_textArea>()->text.setCursor( 0 );
			event.getGadget<_textArea>()->text.setCursorEnabled( true ); // Show Cursor
			that->redraw(); // refresh
			break;
		case onBlur:
			event.getGadget<_textArea>()->text.setCursorEnabled( false ); // Remove Cursor
			that->redraw(); // refresh
			break;
		case onResize:
			that->text.indicateNewDimensions();
		case onUpdate: // Adjust scrollbar parameters
		{
			reCompute:
			
			// Check, if the text needs to be update
			that->checkUpdate();
			
			_length neededHeight = that->text.getTextHeight() + that->text.getOffsetY( that->getGuiStringDimensions() );
			bool hideScrollBar = neededHeight <= that->getHeight();
			
			if( hideScrollBar != that->scrollBar->isHidden() )
			{
				if( hideScrollBar )
					that->scrollBar->hide();
				else
					that->scrollBar->show();
				goto reCompute; // as the scrollbar is now shown/hidden the text-lengths have to be recomputed
			}
			that->scrollBar->setX( that->getWidth() - 9 /* x-coord */ );
			that->scrollBar->setHeight( that->getHeight() - 2 /* height of the scrollbar */	);
			that->scrollBar->setLength( that->getHeight() /* Visible Part of the canvas */ );
			that->scrollBar->setLength2( neededHeight + _textArea::borderY * 2 /* Height of the inner canvas */ );
			break;
		}
		case onScroll:
			that->checkRefresh();
			break;
		default:
			break;
	}
	
	return use_default;
}

_callbackReturn _textArea::mouseHandler( _event event )
{
	_textArea* that = event.getGadget<_textArea>();
	
	_coord posX = event.getPosX();
	_coord posY = event.getPosY();
	
	if( event == onDragging ){
		posX -= that->getX();
		posY -= that->getY();
	}
	
	that->text.setCursorFromTouch( that->getGuiStringDimensions() , posX , posY );
	that->checkRefresh();
	
	return handled;
}

_textArea::_textArea( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string value , _style&& style ) :
	_gadget( _gadgetType::textarea , x , y , width , height , style | _style::keyboardRequest | _style::draggable | _style::smallDragThld )
	, bgColor( _color::fromRGB( 31 , 31 , 31 ) )
	, text( move(value) , _fontController::getStandardFont() , _color::black , _fontController::getStandardFontSize() )
{
	// Adjust guiString object
	this->text.setFontChangeEnabled( false );
	this->text.setFontColorChangeEnabled( false );
	this->text.setFontSizeChangeEnabled( false );
	
	this->scrollBar =
		new _scrollBar(
			width - 9 , /* x-coord */
			1 , /* y-coordinate */
			height - 2 , /* height of the scrollbar */
			height - 2 , /* Visible Part of the canvas */
			this->text.getTextHeight() , /* Height of the inner canvas */
			_dimension::vertical
			, 0 /* value */
			, _style::canNotTakeFocus | _style::notFocusable
		)
	;
	
	// Scrollbar preferences
	this->addChild( this->scrollBar );
	this->scrollBar->setInternalEventHandler( onScroll , _gadgetHelpers::eventForwardGadget(this) );
	this->scrollBar->setStep( this->text.getFont()->getHeight() + 1 );
	
	// Register update Handler...
	this->setInternalEventHandler( onUpdate , make_callback( &_textArea::generalHandler ) );
	
	// that will set the right parameters for the Scrollbar!
	this->updateNow();
	
	// Regsiter Handling Functions for events
	this->setInternalEventHandler( onFocus , make_callback( &_textArea::generalHandler ) );
	this->setInternalEventHandler( onBlur , make_callback( &_textArea::generalHandler ) );
	this->setInternalEventHandler( onResize , make_callback( &_textArea::generalHandler ) );
	this->setInternalEventHandler( onDraw , make_callback( &_textArea::refreshHandler ) );
	this->setInternalEventHandler( onMouseDown , make_callback( &_textArea::mouseHandler ) );
	this->setInternalEventHandler( onKeyDown , make_callback( &_textArea::keyHandler ) );
	this->setInternalEventHandler( onKeyRepeat , make_callback( &_textArea::keyHandler ) );
	this->setInternalEventHandler( onDragging , make_callback( &_textArea::mouseHandler ) );
	this->setInternalEventHandler( onScroll , make_callback( &_textArea::generalHandler ) );
	
	// Refresh Myself
	this->redraw();
}

_textArea::~_textArea(){
	delete this->scrollBar;
}