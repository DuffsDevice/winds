#include "_gadget/gadget.textarea.h"
#include "_type/type.system.h"

_2s32 _textArea::getFontPosition( string str , bool noScroll )
{
	_coord x = 0;
	_coord y = _textArea::borderY;
	
	switch( this->getAlign() )
	{
		case _align::center:
			x = ( this->getWidth() - ( this->scrollBar->isHidden() ? 0 : 9 ) - this->text.getFont()->getStringWidth( str ) + 1 ) >> 1;
			break;
		case _align::left:
			x = _textArea::borderX;
			break;
		case _align::right:
			x = this->getWidth() - ( this->scrollBar->isHidden() ? 0 : 9 ) - this->text.getFont()->getStringWidth( str ) - _textArea::borderX;
			break;
	}
	
	if( !noScroll )
		y -= this->scrollBar->getValue();
	
	return _2s32( x , y );
}

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
	
	// Temporary line count
	_u32 lineCount = that->text.getLineCount();
	
	// If there is no font it doesn't make sense to paint
	if( lineCount )
	{
		_coord y = that->getFontPosition("").second;
		
		// Store font and fontSize
		const _font* ft = that->text.getFont();
		_u8 fontHeight = ft->getHeight();
		
		// Look, where the cursor is at
		_u32 lineOfCursor = -1;
		
		if( that->cursor )
			lineOfCursor = that->text.getLineContainingCharIndex( that->cursor - 1 );
		
		for( _u32 i = 0 ; i <= lineCount ; i++ )
		{
			string val = that->text.getLineContent(i);
			
			// X-Position
			_coord x = that->getFontPosition( val ).first;
			
			// Draw Text...
			bP.drawString( x , y , ft , val , that->color );
			
			// Check if the cursor is in that line
			if( i == lineOfCursor )
			{
				val.resize( that->cursor - that->text.getLineStart( i ) - 1 );
				
				_length strWidthUntilCursor = ft->getStringWidth( val );
				
				// Draw Cursor
				bP.drawVerticalLine( strWidthUntilCursor + x - 1 , y - 1 , ft->getAscent() + 2 , RGB( 31 , 0 , 0 ) );
			}
			
			y += fontHeight + 1;
		}
	}
	
	_callbackReturn ret = that->handleEventUser( event );
	
	if( ret == not_handled || ret == use_default )
	{
		if( !that->isPressed() )
			bP.drawRect( 0 , 0 , myW , myH , RGB( 13 , 16 , 23 ) );
		else
			bP.drawRect( 0 , 0 , myW , myH , RGB( 9 , 13 , 19 ) );
	}
	
	return use_default;
}

void _textArea::setInternalCursor( _u32 cursor )
{
	// Apply Limit
	_u32 newCursor = min( cursor , this->text.getText().length() + 1 );
	newCursor = max( newCursor , _u32(1) );
	
	// Check for need
	if( newCursor != this->cursor )
	{
		// Set Cursor
		this->cursor = newCursor;
		
		// Compute the required y position to display the text
		_int cursorY1 = this->text.getLineContainingCharIndex( this->cursor - 1 ) * ( this->text.getFont()->getHeight() + 1 );
		_int cursorY2 = cursorY1 + this->text.getFont()->getHeight() + 1;
		
		// as we have to compute the minimum scroll we need
		// and as the scroll is the difference between the displayed text and the topper border of the textarea
		// , we have to relativate the cursor Y2 to the topper border => subtract the width
		cursorY2 -= this->getHeight() - _textArea::borderY * 2;
		
		// Adjust scrollbar position
		_int scrollValue = min( cursorY1 , (_int)this->scrollBar->getValue() ); // Make sure the cursor is not above the visible area
		scrollValue = max( scrollValue , cursorY2 ); // Make sure the cursor does not get displayed below the visible area
		this->scrollBar->setValue( scrollValue , false );
		
		// Refresh
		this->redraw();
	}
}

_callbackReturn _textArea::keyHandler( _event event )
{
	_textArea* that = event.getGadget<_textArea>();
	
	switch( event.getKeyCode() ){
		case DSWindows::KEY_BACKSPACE:
		case DSWindows::KEY_B:
			if( !(that->text.getText().length()) || that->cursor < 2 ){
				//_system::errorTone();
				break;
			}
			// Refresh
			that->text.remove( that->cursor - 2 );
			
			// Notify content has changed
			that->triggerEvent( onEdit );
			
			that->update();
			//that->setInternalCursor( that->cursor - 1 );
			//break; // (!)
		case DSWindows::KEY_LEFT:
			that->setInternalCursor( that->cursor - 1 );
			break;
		case DSWindows::KEY_RIGHT:
			that->setInternalCursor( that->cursor + 1 );
			break;
		case DSWindows::KEY_DOWN:
		case DSWindows::KEY_UP:
			{
				// Line 1 (current line)
				_u32 lineOfCursor = that->text.getLineContainingCharIndex( that->cursor - 1 );
				_u32 line2Number;
				
				if( event.getKeyCode() == DSWindows::KEY_UP )
					line2Number = max( (_u32)1 , lineOfCursor ) - 1;
				else
					line2Number = min( that->text.getLineCount() - 1 , lineOfCursor + 1 );
				
				// Temporary font-object
				const _font* ft = that->text.getFont();
				
				if( line2Number == lineOfCursor || !ft || !ft->isValid() ) // We are at the limits of the textarea
					break; // abort
				
				// Get X-Position of the cursor
				string line = that->text.getLineContent( lineOfCursor );
				
				// Get starting X-Coordinate!
				_coord xPos = that->getFontPosition( line ).first;
				
				// Cut the string at the cursor and add the size of the line to the current x-start to get the cursor position!
				line.resize( that->cursor - that->text.getLineStart( lineOfCursor ) - 1 );
				xPos += ft->getStringWidth( line ); // Add
				
				
				// Line 2
				string line2 = that->text.getLineContent( line2Number );
				
				// Compute Staring x_coordinate
				_coord x = that->getFontPosition( line2 ).first; // Current temporary x-advance
				_u32 idx = 1; // Current iterator index
				
				for( _u8 charWidth ; idx < line2.length() ; idx++ )
				{
					charWidth = ft->getCharacterWidth( line2[idx-1] );
					if( x + charWidth > xPos )
						break;
					x += charWidth + 1;
				}
				
				that->setInternalCursor( that->text.getLineStart( line2Number ) + idx );
			}
		default:
			if( DSWindows::isHardwareKey( event.getKeyCode() ) 
				|| ( !isprint( event.getKeyCode() ) && !iscntrl( event.getKeyCode() ) ) // Check if printable
			)
				break;
			// Insert glyph
			that->text.insert( that->cursor - 1 , event.getKeyCode() );
			
			// Notify that content has changed
			that->triggerEvent( onEdit );
			
			// Update scrollbars etc.
			that->updateNow();
			
			// Refresh
			that->setInternalCursor( that->cursor + 1 );
			
			break;
	}
	
	return handled;
}

_callbackReturn _textArea::generalHandler( _event event )
{
	_textArea* that = event.getGadget<_textArea>();
	
	switch( event.getType() )
	{
		case onFocus:
			event.getGadget<_textArea>()->cursor = 1;
			that->redraw(); // refresh
			break;
		case onBlur:
			event.getGadget<_textArea>()->cursor = 0; // Remove Cursor
			that->redraw(); // refresh
			break;
		case onResize:
		case onUpdate: // Adjust scrollbar parameters
		{
			reCompute:
			
			that->text.setWidth( that->getWidth() - _textArea::borderX * 2 - ( that->scrollBar->isHidden() ? 0 : 9 ) );
			
			_length neededHeight = ( _textArea::borderY * 2 ) + that->text.getLineCount() * ( that->text.getFont()->getHeight() + 1 );
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
			that->scrollBar->setLength2( neededHeight /* Height of the inner canvas */ );
			break;
		}
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
	
	//! Temporary font-object
	const _font* ft = that->text.getFont();
	
	if( !ft || !ft->isValid() ) // No valid font?
		return handled; // abort
	
	_u8 ftHeight = ft->getHeight();
	
	//! Compute Line Number
	_u32 lineNumber = max( 0 , ( posY - that->getFontPosition("").second ) / ( ftHeight + 1 ) );
	lineNumber = min( lineNumber , that->text.getLineCount() - 1 );
	
	// Get X-Position of the cursor
	string line = that->text.getLineContent( lineNumber );
	
	_coord x = that->getFontPosition( line ).first; // Current temporary x-advance
	_u32 idx = 0; // Current iterator index
	
	for( _u8 charWidth ; idx < line.length() ; idx++ )
	{
		charWidth = ft->getCharacterWidth( line[idx] );
		if( x + charWidth > posX )
			break;
		x += charWidth + 1;
	}
	
	that->setInternalCursor( min( that->text.getLineStart( lineNumber + 1 ) , that->text.getLineStart( lineNumber ) + idx + 1 ) );
	
	return handled;
}

_textArea::_textArea( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string value , _style&& style ) :
	_gadget( _gadgetType::textarea , x , y , width , height , style | _styleAttr::keyboardRequest | _styleAttr::draggable | _styleAttr::smallDragTrig )
	, color( RGB( 0 , 0 , 0 ) )
	, bgColor( RGB( 31 , 31 , 31 ) )
	, text( _system::getFont() , _system::getRTA().getDefaultFontSize() , width - _textArea::borderX * 2 , value )
	, cursor( 0 )
	, align( _align::left )
{
	this->scrollBar =
		new _scrollBar(
			width - 9 , /* x-coord */
			1 , /* y-coordinate */
			height - 2 , /* height of the scrollbar */
			height - 2 , /* Visible Part of the canvas */
			this->text.getLineCount() * ( this->text.getFont()->getHeight() + 1 ) , /* Height of the inner canvas */
			_dimension::vertical
			, 0 /* value */
			, _styleAttr() | _styleAttr::canNotTakeFocus | _styleAttr::canNotReceiveFocus
		)
	;
	
	this->addChild( this->scrollBar );
	this->scrollBar->setInternalEventHandler( onScroll , _gadgetHelpers::eventForwardRefreshGadget(this) );
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
	
	// Refresh Myself
	this->redraw();
}

_textArea::~_textArea(){
	delete this->scrollBar;
}