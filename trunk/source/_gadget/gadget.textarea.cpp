#include "_gadget/gadget.textarea.h"
#include "_gadget/gadget.windows.h"
#include "_type/type.system.h"

enum
{
	startX = 3, // X-Position of the most left character
	
	startY = 1, // Start of text in y-direction
};

_callbackReturn _textarea::refreshHandler( _event event )
{
	// Receive Gadget
	_textarea* that = event.getGadget<_textarea>();

	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
	
	_length myH = that->getHeight();
	_length myW = that->getWidth();
	
	bP.drawFilledRect( 1 , 1 , myW - 2 , myH - 2 , that->bgColor );
	
	_u32 lineCount = that->text.getLineCount();
	
	// If there is no font it doesn't make sense to paint
	if( lineCount )
	{
		_coord x = ::startX;
		_coord y = ::startX;
		
		_font* ft = that->text.getFont();
		
		_u8 fontHeight = ft->getHeight();
		
		_u32 lineOfCursor = -1;
		
		if( that->cursor )
			lineOfCursor = that->text.getLineContainingCharIndex( that->cursor - 1 );
		
		for( _u32 i = 0 ; i <= lineCount ; i++ )
		{
			string val = that->text.getLineContent(i);
			
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
	
	if( !that->isPressed() )
		bP.drawRect( 0 , 0 , myW , myH , RGB( 13 , 16 , 23 ) );
	else
		bP.drawRect( 0 , 0 , myW , myH , RGB( 9 , 13 , 19 ) );
	
	return use_default;
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
			that->cursor--;
			that->triggerEvent( _event( onChange ) );
			that->bubbleRefresh( true );
			break;
		case DSWindows::KEY_LEFT:
			that->cursor > 1 && that->cursor--;
			// Refresh
			that->bubbleRefresh( true );
			break;
		case DSWindows::KEY_RIGHT:
			that->cursor > 0 && that->text.getText().length() - that->cursor + 1 > 0 && that->cursor++;
			// Refresh
			that->bubbleRefresh( true );
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
				_font* ft = that->text.getFont();
				
				if( line2Number == lineOfCursor || !ft || !ft->valid() ) // We are at the limits of the textarea
					break; // abort
				
				// Get X-Position of the cursor
				string line = that->text.getLineContent( lineOfCursor );
				
				// Cut the string at the cursor
				line.resize( that->cursor - that->text.getLineStart( lineOfCursor ) - 1 );
				
				_length xPos = ft->getStringWidth( line ); // Get its width
				
				
				//! Line 2
				string line2 = that->text.getLineContent( line2Number );
				
				_length x = 0; // Current temporary x-advance
				_u32 idx = 1; // Current iterator index
				
				for( _u8 charWidth ; idx < line2.length() ; idx++ )
				{
					charWidth = ft->getCharacterWidth( line2[idx-1] );
					if( x + charWidth > xPos )
						break;
					x += charWidth + 1;
				}
				
				that->cursor = that->text.getLineStart( line2Number ) + idx;
				
				that->bubbleRefresh( true );
			}
		default:
			if( DSWindows::isHardwareKey( event.getKeyCode() ) 
				|| ( !isprint( event.getKeyCode() ) && !iscntrl( event.getKeyCode() ) ) // Check if printable
			)
				break;
			// Refresh
			that->text.insert( that->cursor - 1 , event.getKeyCode() );
			that->cursor++;
			that->triggerEvent( _event( onChange ) );
			that->bubbleRefresh( true );
			break;
	}
	
	return handled;
}

_callbackReturn _textarea::focusHandler( _event event )
{
	event.getGadget<_textarea>()->cursor = 1;
	
	event.getGadget()->bubbleRefresh( true );
	
	return use_default;
}

_callbackReturn _textarea::blurHandler( _event event )
{
	_textarea* that = event.getGadget<_textarea>();
	
	// Remove Cursor!
	that->cursor = 0;
	
	// Refresh
	that->bubbleRefresh( true );
	
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
	_font* ft = that->text.getFont();
	
	if( !ft || !ft->valid() ) // No valid font?
		return handled; // abort
	
	_u8 ftHeight = ft->getHeight();
	
	//! Compute Line Number
	_u32 lineNumber = max( 0 , ( posY - ::startY ) / ( ftHeight + 1 ) );
	lineNumber = min( lineNumber , that->text.getLineCount() - 1 );
	
	// Get X-Position of the cursor
	string line = that->text.getLineContent( lineNumber );
	
	_length x = ::startX; // Current temporary x-advance
	_u32 idx = 0; // Current iterator index
	
	for( _u8 charWidth ; idx < line.length() ; idx++ )
	{
		charWidth = ft->getCharacterWidth( line[idx] );
		if( x + charWidth > posX )
			break;
		x += charWidth + 1;
	}
	
	that->cursor = min( that->text.getLineStart( lineNumber + 1 ) , that->text.getLineStart( lineNumber ) + idx + 1 );
	
	that->bubbleRefresh( true );
	
	return handled;
}

_textarea::_textarea( _length width , _length height , _coord x , _coord y , string value , _style style ) :
	_gadget( _gadgetType::textarea , width , height , x , y , style | _styleAttr::keyboardRequest | _styleAttr::draggable | _styleAttr::smallDragTrig )
	, color( RGB( 0 , 0 , 0 ) )
	, bgColor( RGB( 31 , 31 , 31 ) )
	, text( _system::getFont() , _system::_runtimeAttributes_->defaultFontSize , width - ::startX - 1 , value )
	, cursor( 0 )
	, align( _align::left )
{
	// Regsiter Handling Functions for events
	this->registerEventHandler( onFocus , new _staticCallback( &_textarea::focusHandler ) );
	this->registerEventHandler( onBlur , new _staticCallback( &_textarea::blurHandler ) );
	this->registerEventHandler( refresh , new _staticCallback( &_textarea::refreshHandler ) );
	this->registerEventHandler( mouseDown , new _staticCallback( &_textarea::mouseHandler ) );
	this->registerEventHandler( keyDown , new _staticCallback( &_textarea::keyHandler ) );
	this->registerEventHandler( keyRepeat , new _staticCallback( &_textarea::keyHandler ) );
	this->registerEventHandler( dragging , new _staticCallback( &_textarea::mouseHandler ) );
	
	// Refresh Myself
	this->refreshBitmap();
}