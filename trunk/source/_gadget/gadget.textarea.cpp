#include "_gadget/gadget.textarea.h"
#include "_type/type.system.h"

_2s32 _textarea::getFontPosition( string str , bool noScroll )
{
	_coord x = 0;
	_coord y = _textarea::borderY;
	
	switch( this->getAlign() )
	{
		case _align::center:
			x = ( this->getWidth() - ( this->scrollBar->isHidden() ? 0 : 9 ) - this->text.getFont()->getStringWidth( str ) + 1 ) >> 1;
			break;
		case _align::left:
			x = _textarea::borderX;
			break;
		case _align::right:
			x = this->getWidth() - ( this->scrollBar->isHidden() ? 0 : 9 ) - this->text.getFont()->getStringWidth( str ) - _textarea::borderX;
			break;
	}
	
	if( !noScroll )
		y -= this->scrollBar->getValue();
	
	return _2s32( x , y );
}

_callbackReturn _textarea::refreshHandler( _event event )
{
	// Receive Gadget
	_textarea* that = event.getGadget<_textarea>();

	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsolutePosition() ) );
	else
		bP.normalizeClippingRects();
	
	_length myH = that->getHeight();
	_length myW = that->getWidth();
	
	bP.drawFilledRect( 1 , 1 , myW - 2 , myH - 2 , that->bgColor );
	
	_u32 lineCount = that->text.getLineCount();
	
	// If there is no font it doesn't make sense to paint
	if( lineCount )
	{
		_coord y = that->getFontPosition( "" ).second;
		
		const _font* ft = that->text.getFont();
		
		_u8 fontHeight = ft->getHeight();
		
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
	
	if( ret == not_handled )
	{
		if( !that->isPressed() )
			bP.drawRect( 0 , 0 , myW , myH , RGB( 13 , 16 , 23 ) );
		else
			bP.drawRect( 0 , 0 , myW , myH , RGB( 9 , 13 , 19 ) );
	}
	
	return use_default;
}

void _textarea::setInternalCursor( _u32 cursor )
{
	_u32 newCursor = min( cursor , this->text.getText().length() + 1 );
	newCursor = max( newCursor , _u32(1) );
	
	if( newCursor != this->cursor )
	{
		this->cursor = newCursor;
		
		_s64 requiredHeight = this->text.getLineContainingCharIndex( this->cursor - 1 ) * ( this->text.getFont()->getHeight() + 1 );
		
		this->scrollBar->setValue( max( min( requiredHeight , (_s64)this->scrollBar->getValue() ) , requiredHeight + this->text.getFont()->getHeight() - this->getHeight() + _textarea::borderY * 2 + 1 ) );
		
		this->bubbleRefresh( true );
	}
}

_callbackReturn _textarea::keyHandler( _event event )
{
	_textarea* that = event.getGadget<_textarea>();
	
	switch( event.getKeyCode() ){
		case DSWindows::KEY_BACKSPACE:
		case DSWindows::KEY_B:
			if( !(that->text.getText().length()) || that->cursor < 2 ){
				//_system::errorTone();
				break;
			}
			// Refresh
			that->text.remove( that->cursor - 2 );
			
			that->triggerEvent( _event( onChange ) );
			that->handleEvent( _event( onResize ) );
			that->setInternalCursor( that->cursor - 1 );
			break;
		case DSWindows::KEY_LEFT:
			that->setInternalCursor( that->cursor - 1 );
			break;
		case DSWindows::KEY_RIGHT:
			that->setInternalCursor( that->cursor + 1 );
			break;
		case DSWindows::KEY_DOWN:
		case DSWindows::KEY_UP:
			{
				//! Line 1 (current line)
				_u32 lineOfCursor = that->text.getLineContainingCharIndex( that->cursor - 1 );
				_u32 line2Number;
				
				if( event.getKeyCode() == DSWindows::KEY_UP )
					line2Number = max( (_u32)1 , lineOfCursor ) - 1;
				else
					line2Number = min( that->text.getLineCount() - 1 , lineOfCursor + 1 );
				
				//! Temporary font-object
				const _font* ft = that->text.getFont();
				
				if( line2Number == lineOfCursor || !ft || !ft->isValid() ) // We are at the limits of the textarea
					break; // abort
				
				//! Get X-Position of the cursor
				string line = that->text.getLineContent( lineOfCursor );
				
				//! Get starting X-Coordinate!
				_coord xPos = that->getFontPosition( line ).first;
				
				//! Cut the string at the cursor and add the size of the line to the current x-start to get the cursor position!
				line.resize( that->cursor - that->text.getLineStart( lineOfCursor ) - 1 );
				xPos += ft->getStringWidth( line ); // Add
				
				
				//! Line 2
				string line2 = that->text.getLineContent( line2Number );
				
				//! Compute Staring x_coordinate
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
			// Refresh
			that->text.insert( that->cursor - 1 , event.getKeyCode() );
			
			that->triggerEvent( _event( onChange ) );
			that->handleEvent( _event( onResize ) );
			that->setInternalCursor( that->cursor + 1 );
			break;
	}
	
	return handled;
}

_callbackReturn _textarea::generalHandler( _event event )
{
	_textarea* that = event.getGadget<_textarea>();
	
	switch( event.getType() )
	{
		case onFocus:
			event.getGadget<_textarea>()->cursor = 1;
			that->bubbleRefresh( true ); // refresh
			break;
		case onBlur:
			event.getGadget<_textarea>()->cursor = 0; // Remove Cursor
			that->bubbleRefresh( true ); // refresh
			break;
		case onResize:
		{
			_length neededHeight = ( _textarea::borderY * 2 ) + that->text.getLineCount() * ( that->text.getFont()->getHeight() + 1 );
			if( neededHeight <= that->getHeight() )
				that->scrollBar->hide();
			else
				that->scrollBar->show();
			event.getGadget<_textarea>()->text.setWidth( that->getWidth() - _textarea::borderX * 2 - ( that->scrollBar->isHidden() ? 0 : 9 ) );
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

_callbackReturn _textarea::mouseHandler( _event event )
{
	_textarea* that = event.getGadget<_textarea>();
	
	_coord posX = event.getPosX();
	_coord posY = event.getPosY();
	
	if( event.getType() == dragging )
	{
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

_textarea::_textarea( _length width , _length height , _coord x , _coord y , string value , _style&& style ) :
	_gadget( _gadgetType::textarea , width , height , x , y , style | _styleAttr::keyboardRequest | _styleAttr::draggable | _styleAttr::smallDragTrig )
	, color( RGB( 0 , 0 , 0 ) )
	, bgColor( RGB( 31 , 31 , 31 ) )
	, text( _system::getFont() , _system::_rtA_->getDefaultFontSize() , width - _textarea::borderX * 2 , value )
	, cursor( 0 )
	, align( _align::center )
{
	// Regsiter Handling Functions for events
	this->setInternalEventHandler( onFocus , _staticCallback( &_textarea::generalHandler ) );
	this->setInternalEventHandler( onBlur , _staticCallback( &_textarea::generalHandler ) );
	this->setInternalEventHandler( onResize , _staticCallback( &_textarea::generalHandler ) );
	this->setInternalEventHandler( refresh , _staticCallback( &_textarea::refreshHandler ) );
	this->setInternalEventHandler( mouseDown , _staticCallback( &_textarea::mouseHandler ) );
	this->setInternalEventHandler( keyDown , _staticCallback( &_textarea::keyHandler ) );
	this->setInternalEventHandler( keyRepeat , _staticCallback( &_textarea::keyHandler ) );
	this->setInternalEventHandler( dragging , _staticCallback( &_textarea::mouseHandler ) );
	
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
	this->scrollBar->setInternalEventHandler( onScroll , _gadget::eventForwardRefreshGadget(this) );
	this->scrollBar->setStep( this->text.getFont()->getHeight() + 1 );
	
	//! Set the right parameters for the Scrollbar
	this->handleEvent( onResize );
	
	// Refresh Myself
	this->refreshBitmap();
}

_textarea::~_textarea(){
	delete this->scrollBar;
}