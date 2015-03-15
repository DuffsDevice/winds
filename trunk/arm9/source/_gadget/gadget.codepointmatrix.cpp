#include <_gadget/gadget.codepointmatrix.h>
#include <_controller/controller.font.h>
#include <_type/type.windows.soundbank.h>

void _codepointMatrix::setFont( _fontHandle ft )
{
	if( this->font == ft || !ft )
		return;
	
	this->font = ft;
	
	// Inform that maybe, size should change now
	this->update();
	
	// ...and refresh!
	this->redraw();
}

void _codepointMatrix::setFontSize( _fontSize fontSize )
{
	if( this->fontSize == fontSize )
		return;
	
	// Set FontSize...
	this->fontSize = fontSize;
	
	// Inform that maybe, size should change now
	this->update();
	
	// ...and refresh!
	this->redraw();
}

void _codepointMatrix::setTileWidth( _fontSize value )
{
	if( this->tileWidth == value )
		return;
	
	this->tileWidth = value;
	
	// Inform that maybe, size should change now
	this->update();
	
	// ...and refresh!
	this->redraw();
}

void _codepointMatrix::setTileHeight( _u8 value )
{
	if( this->tileHeight == value )
		return;
	
	this->tileHeight = value;
	
	// Inform that maybe, size should change now
	this->update();
	
	// ...and refresh!
	this->redraw();
}

void _codepointMatrix::setCursor( _pos pos )
{
	if( pos == this->cursor )
		return;
	
	validatePosition( pos );
	
	_pos oldCursor = this->cursor;
	this->cursor = pos;
	
	// Draw the old and cursor tile
	this->redrawTile( oldCursor );
	this->redrawTile( this->cursor );
}

_pos _codepointMatrix::getAdjacentTile( _pos pos , bool next )
{
	if( next )
		pos.first++;
	else
		pos.first--;
	
	// Move to next line, if the tile exceeds the matrix on the right side
	while( pos.first >= this->numCols ){
		pos.first -= this->numCols;
		pos.second++;
	}
	
	// Move to previous line, if the tile exceeds the matrix on the left side
	while( pos.first < 0 ){
		pos.first += this->numCols;
		pos.second--;
	}
	
	return pos;
}

_length _codepointMatrix::getMaximumUsedTileInLine( _length lineNumber ) const
{
	_u16 maximum = 0;
	if( this->useExtended ){
		_codepointMatrixEntryEx* data = &this->data.extended[ numCols * lineNumber ];
		for( _u16 i = 0 ; i < this->numCols ; i++ )
			if( data[i].codepoint )
				maximum = i + 1;
	}
	else{
		_codepointMatrixEntry* data = &this->data.basic[ numCols * lineNumber ];
		for( _u16 i = 0 ; i < this->numCols ; i++ )
			if( data[i] )
				maximum = i + 1;
	}
	return maximum;
}

void _codepointMatrix::redrawTile( _pos pos )
{
	_rect rect = getTileMetrics( pos );
	
	// Get BitmapPort
	_bitmapPort bP = getBitmapPort(rect);
	
	_color fg;
	_color bg;
	_wchar codepoint;
	
	if( this->useExtended ){
		_codepointMatrixEntryEx entry = this->data.extended[ pos.second * this->numCols + pos.first ];
		fg = entry.foreground;
		bg = entry.background;
		codepoint = entry.codepoint;
	}
	else{
		fg = this->curForeground;
		bg = this->curBackground;
		codepoint = this->data.basic[ pos.second * this->numCols + pos.first ];
	}
	
	// Let the cursor draw the tile it is on
	if( pos == this->cursor )
		std::swap( bg , fg );
	
	bP.fill( bg );
	bP.drawChar( rect.x + ( this->tileWidth - this->font->getCharacterWidth( codepoint , this->fontSize ) ) / 2, rect.y , this->font , codepoint , fg , this->fontSize );
	
	redrawParents( rect.toRelative( -getAbsolutePosition() ) );
	
	return;
}

void _codepointMatrix::moveUpCanvas()
{
	if( this->useExtended )
	{
		for( _u16 y = 1 ; y < this->numRows ; y++ )
			for( _u16 x = 0 ; x < this->numCols ; x++ )
				this->data.extended[ y * this->numCols + x - this->numCols ] = this->data.extended[ y * this->numCols + x];
		
		// Remove Codepoints in the last line
		_codepointMatrixEntryEx* data = this->data.extended + this->numRows * this->numCols - this->numCols;
		for( _u16 x = 0 ; x < this->numCols ; x++ )
			data[x] = { 0 , this->curForeground , this->curBackground };
	}
	else{
		for( _u16 y = 1 ; y < this->numRows ; y++ )
			for( _u16 x = 0 ; x < this->numCols ; x++ )
				this->data.basic[ y * this->numCols + x - this->numCols ] = this->data.basic[ y * this->numCols + x];
		
		// Remove Codepoints in the last line
		_codepointMatrixEntry* data = this->data.basic + this->numRows * this->numCols - this->numCols;
		for( _u16 x = 0 ; x < this->numCols ; x++ )
			data[x] = 0;
	}
	
	this->redraw();
}


void _codepointMatrix::print( _wchar ch )
{
	if( this->useExtended )
		this->data.extended[ numCols * this->cursor.second + cursor.first ] = { ch , this->curForeground , this->curBackground };
	else
		this->data.basic[ numCols * this->cursor.second + cursor.first ] = ch;
	
	// Redraw
	_pos newCursor = this->cursor;
	newCursor.first++;
	
	if( newCursor.first == this->numCols || !ch || ch == (_wchar)_key::carriage_return )
	{
		newCursor.first = 0;
		
		if( newCursor.second == this->numRows - 1 )
			this->moveUpCanvas();
		else
			newCursor.second++;
	}
	
	// Move the cursor
	setCursor( newCursor );
}

void _codepointMatrix::backspace()
{
	_pos oldCursor = this->cursor;
	_pos newCursor = oldCursor;
	
	// Line Break
	if( newCursor.first == 0 ){
		if( newCursor.second > 0 )
			newCursor.first = min<_length>( this->numCols , getMaximumUsedTileInLine( --newCursor.second ) );
		else
			return;
	}
	
	this->cursor = getAdjacentTile( newCursor , false );
	this->remove();
	
	// Redraw old and new position of the cursor
	this->redrawTile( oldCursor );
	this->redrawTile( this->cursor );
}

void _codepointMatrix::remove()
{
	_pos	cur = this->cursor;
	_pos	next = cur;
	
	while( true )
	{
		_wchar	curCP = getCodepointAt( next );
		
		if( curCP == (_wchar)_key::carriage_return )
		{
			next.first = 0;
			next.second++;
			
			// Copy from next tile to this
			copyTile( cur , next );
		}
		else if( curCP )
			copyTile( cur , getAdjacentTile( cur , true ) );
		else
			break;
		
		curCP = getCodepointAt( cur );
		
		// Move forward
		if( curCP == (_wchar)_key::carriage_return ){
			next.first = 0;
			cur.second++;
		}
		else if( curCP )
			cur = getAdjacentTile( next , true );
		else
			break;
	}
	
	redraw();
}

void _codepointMatrix::moveCursor( bool increase )
{
	// Redraw
	_pos newCursor = this->cursor;
	
	if( increase )
	{
		_wchar codepoint = getCodepointAt(newCursor);
		newCursor.first++;
		
		if( newCursor.first == this->numCols || !codepoint || codepoint == (_wchar)_key::carriage_return )
		{
			if( newCursor.second == this->numRows - 1 )
				return;
			newCursor.second++;
			newCursor.first = 0;
		}
	}
	else
	{
		if( newCursor.first == 0 ){
			if( newCursor.second > 0 )
				newCursor.first = getMaximumUsedTileInLine( --newCursor.second );
			else
				return;
		}
		
		// Move left
		if( newCursor > 0 )
			newCursor.first--;
	}
	
	// Move the cursor
	setCursor( newCursor );
}

void _codepointMatrix::resizeMatrixTo( _u16 numCols , _u16 numRows )
{
	if( this->numCols == numCols && this->numRows == numRows )
		return;
	
	_u16 minCols = min<_u16>( numCols , this->numCols );
	_u16 minRows = min<_u16>( numRows , this->numRows );
	
	if( this->useExtended )
	{
		_codepointMatrixEntryEx* newMatrix = new _codepointMatrixEntryEx[numCols*numRows];
		
		// Reset
		for( _u32 i = numCols*numRows ; i-- > 0 ; )
			newMatrix[i] = _codepointMatrixEntryEx{ 0 , this->curForeground , this->curBackground };
		
		if( this->data.extended ){
			for( _u16 y = 0 ; y < minRows ; y++ )
				for( _u16 x = 0 ; x < minCols ; x++ )
					newMatrix[ y * numCols + x ] = this->data.extended[ y * this->numCols + x];
			
			delete[] this->data.extended;
		}
		
		this->data.extended = newMatrix;
	}
	else
	{
		_codepointMatrixEntry* newMatrix = new _codepointMatrixEntry[numCols*numRows];
		
		// Reset
		for( _u32 i = numCols*numRows ; i-- > 0 ; )
			newMatrix[i] = 0;
		
		if( this->data.basic ){
			for( _u16 y = 0 ; y < minRows ; y++ )
				for( _u16 x = 0 ; x < minCols ; x++ )
					newMatrix[ y * numCols + x ] = this->data.basic[ y * this->numCols + x];
			
			delete[] this->data.basic;
		}
		
		this->data.basic = newMatrix;
	}
	
	this->numCols = numCols;
	this->numRows = numRows;
}

_callbackReturn _codepointMatrix::keyHandler( _event event )
{
	_codepointMatrix* that = event.getGadget<_codepointMatrix>();
	
	switch( event.getKeyCode() ){
		case _key::backspace:
		case _key::b:
			if( !that->cursor.first && !that->cursor.second ){
				_windowsSoundBank::play( _windowsSound::ping );
				break;
			}
			that->backspace();
			
			// Trigger Handler
			that->triggerEvent( onEdit );
			break;
		case _key::carriage_return:
			that->print( (_wchar)event.getKeyCode() );
			that->triggerEvent( onEdit ); // Trigger Handler
			break;
		case _key::left:
			that->moveCursor( false );
			break;
		case _key::right:
			that->moveCursor( true );
			break;
		case _key::up:
		case _key::down:
			break;
		default:
			if( _hardwareKeyPattern::isHardwareKey( event.getKeyCode() ) ) // Check if printable
				return use_default;
			
			that->print( (_wchar)event.getKeyCode() );
			
			// Trigger Handler
			that->triggerEvent( onEdit );
			break;
	}
	
	return handled;
}


_callbackReturn _codepointMatrix::updateHandler( _event event )
{
	// Receive Gadget
	_codepointMatrix* that = event.getGadget<_codepointMatrix>();
	
	_u16 newNumCols = that->getWidth() / that->tileWidth;
	_u16 newNumRows = that->getHeight() / that->tileHeight;
	
	that->resizeMatrixTo( newNumCols , newNumRows );
	
	return handled;
}

_callbackReturn _codepointMatrix::refreshHandler( _event event )
{
	// Receive Gadget
	_codepointMatrix* that = event.getGadget<_codepointMatrix>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	// Reset to standard color
	bP.fill( that->curBackground );
	
	// If there is no font it doesn't make sense to paint
	if( !that->font || !that->font->isValid() )
		return use_default;
	
	if( that->useExtended )
		for( _u16 y = 0 ; y < that->numRows ; y++ )
			for( _u16 x = 0 ; x < that->numCols ; x++ ){
				_codepointMatrixEntryEx entry = that->data.extended[ y * that->numCols + x ];
				bP.drawFilledRect( x * that->tileWidth , y * that->tileHeight , that->tileWidth , that->tileHeight , entry.background );
				bP.drawChar( x * that->tileWidth , y * that->tileHeight , that->font , entry.codepoint , entry.foreground , that->fontSize );
			}
	else
		for( _u16 y = 0 ; y < that->numRows ; y++ )
			for( _u16 x = 0 ; x < that->numCols ; x++ )
				bP.drawChar( x * that->tileWidth , y * that->tileHeight , that->font , that->data.basic[ y * that->numCols + x ] , that->curForeground , that->fontSize );	
	
	// Draw the cursor
	that->redrawTile( that->cursor );
	
	return use_default;
}

_codepointMatrix::_codepointMatrix( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , bool extendedMode , _style&& style )
	: _gadget( _gadgetType::codepointmatrix , x , y , width , height , style | _style::keyboardRequest )
	, useExtended( extendedMode )
	, tileWidth( 6 )
	, tileHeight( 8 )
	, curForeground( _color::white )
	, curBackground( _color::black )
	, cursor( 0 , 0 )
	, font( _fontController::getStandardFont() )
	, fontSize( _fontController::getStandardFontSize() )
{
	this->data.basic = nullptr;
	
	// Update Size of tiles
	this->tileWidth = max( font->getCharacterWidth( 0x2588 , this->fontSize ) , font->getCharacterWidth( 'M' , this->fontSize ) );
	this->tileHeight = font->getHeight();
	
	this->setInternalEventHandler( onUpdate , make_callback( &_codepointMatrix::updateHandler ) );
	
	// force update of size
	this->updateNow();
	
	// Register my handlers
	this->setInternalEventHandler( onDraw , make_callback( &_codepointMatrix::refreshHandler ) );
	this->setInternalEventHandler( onKeyDown , make_callback( &_codepointMatrix::keyHandler ) );
	this->setInternalEventHandler( onKeyRepeat , make_callback( &_codepointMatrix::keyHandler ) );
	this->setInternalEventHandler( onResize , make_callback( &_codepointMatrix::updateHandler ) );
	
	// Refresh
	this->redraw();
}