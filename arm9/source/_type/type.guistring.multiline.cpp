#include <_type/type.guistring.multiline.h>
#include <_controller/controller.localization.h>

void _multiLineGuiString::update( _rect dimensions , _u32 startIndex )
{
	// Reset Line position after startIndex
	this->lineStarts.erase(
		remove_if( this->lineStarts.begin() , this->lineStarts.end() , [startIndex]( _lineStart value )->bool{ return value.charIndex >= startIndex; } )
		, this->lineStarts.end()
	);
	
	if( !this->font || !this->font->isValid() )
		return;
	
	// Characters after which to break
	static _literal breakChars = "\n !\"#&'*+,-./:;<=>?@`|~";
	
	// Push start Index if not existing
	if( this->lineStarts.empty() )
		this->pushBackLineBreak( 0 , 0 , 0 , false );
	
	_length		lineWidthSince = 0;										// Width in pixels of the letters since the last break index
	_length		lineWidthUntil = 0;										// Width in pixels of the letters until the last break index
	_length		lineHeight = this->font->getHeight( this->fontSize );	// Height in pixels of the current line
	_s32		lastBreakableIndex = -1;
	
	
	wstring::iterator	iter = this->begin();
	wstring::iterator	end = this->end();
	
	for( ; iter < end ; iter++ )
	{
		// Check if ch is a character where we possibbly could break
		if( *iter == '\n' )
		{
			// Subtract letter spacing since we don't have it after the last character
			_length letterSpacing = this->font->getLetterSpace( this->fontSize );
			
			this->pushBackLineBreak( ( iter - begin() ) /* Current Index */ + 1 , lineWidthUntil + lineWidthSince - letterSpacing , lineHeight );
			
			// Reset current line width since we added a line break
			lineWidthSince = lineWidthUntil = 0;
			lineHeight = this->font->getHeight( this->fontSize );
		}
		else if( this->font->isCharSupported( *iter ) )
		// advance the current lineWidth by the width of the letter to display
		{
			_length letterWidth = this->font->getCharacterWidth( *iter , this->fontSize );
			
			if( letterWidth ){
				lineHeight = max( lineHeight , this->font->getHeight( this->fontSize ) ); // Increase line height if current font is bigger that before
				letterWidth += this->font->getLetterSpace( this->fontSize );
			}
			
			lineWidthSince += letterWidth;
		}
		
		// If the current line width exceeds the maximum width we have available
		if( lineWidthUntil + lineWidthSince >= dimensions.width )
		{
			_u32		effectiveStartIdx	= lastBreakableIndex + 1;
			wstring::iterator	tmpStart	= get( effectiveStartIdx );
			wstring::iterator	tmpEnd		= iter /* Current Index */ + 1u;
			_list<_int>	possibleLineBreaks	= _localizationController::getHyphenator().parseText( this->c_str() + tmpStart.getIndex() , this->c_str() + tmpEnd.getIndex() );
			
			// If a Syllable could be found
			if( !possibleLineBreaks.empty() )
			{
				// Recreate the parsing of the last word
				_length wordWidth = 0;
				_u32 syllableBreak = possibleLineBreaks.back();
				wstring::iterator syllableCharEnd = tmpStart + syllableBreak;
				
				// Iterate to the syllable end after whcih we brake the line
				for( wstring::iterator syllableCharCur = tmpStart ; syllableCharCur <= syllableCharEnd ; syllableCharCur++ )
				{
					if( this->font->isCharSupported( *syllableCharCur ) )
					{
						_u16 letterWidth = this->font->getCharacterWidth( *syllableCharCur , this->fontSize );
						if( letterWidth )
							letterWidth += this->font->getLetterSpace( this->fontSize );
						wordWidth += letterWidth;
					}
				}
				
				_length hyphenWidth		= this->font->getCharacterWidth( _multiLineGuiString::breakLineLetter , this->fontSize );
				_length letterSpacing	= this->font->getLetterSpace( this->fontSize );
				
				// Add Syllable Line Break
				pushBackLineBreak( effectiveStartIdx + syllableBreak + 1 , lineWidthUntil + wordWidth + hyphenWidth - letterSpacing, lineHeight , true );
				
				// Reset Last Word width
				lineWidthSince -= wordWidth;
				lineWidthUntil = 0;
				lineHeight = this->font->getHeight( this->fontSize );
				continue;
			}
			
			_length letterSpace = this->font->getLetterSpace( this->fontSize ); // Subtract letter spacing since we don't have it after the last character
			
			// In case the word is so long that it doesn't fit in one line
			// AND it does not have syllables!
			if( lastBreakableIndex >= 0 )
				pushBackLineBreak( lastBreakableIndex + 1 , lineWidthUntil - letterSpace , lineHeight ); // +1 : We want to break behind the delimiter
			else{
				pushBackLineBreak( ( iter - begin() ) + 1 , lineWidthUntil + lineWidthSince - letterSpace , lineHeight );
				lineWidthSince = 0;
			}
			
			// Reset current line width since we added a line break
			lineWidthUntil = 0;
			lineHeight = this->font->getHeight( this->fontSize );
			lastBreakableIndex = -1;
		}
		
		// Remember that we have a possible break here
		if( _localizationController::getHyphenator().charIsOf( *iter , breakChars ) )
		{
			lastBreakableIndex = ( iter - begin() ); // <= Current Index
			lineWidthUntil += lineWidthSince;
			lineWidthSince = 0;
		}
	}
	
	// Subtract letter spacing since we don't have it after the last character
	_length letterSpace = lineWidthSince + lineWidthUntil > 0 ? this->font->getLetterSpace( this->fontSize ) : 0;
	
	this->pushBackLineBreak( ( iter - begin() ) /* Current Index */ , lineWidthSince + lineWidthUntil - letterSpace , lineHeight , false ); // Push end
	
	this->needsUpdateFlag = true;
}


void _multiLineGuiString::drawTo( _rect dimensions , _bitmapPort& port )
{
	if( this->needsUpdateFlag )
		this->update( dimensions );
	
	// Buffer frequently used variables
	_u32 lineCount = this->getLineCount(); // Number of lines in the gui string
	if( !lineCount )
		return;
	
	_coord offsetX = dimensions.x;
	_length maxX = offsetX + dimensions.width;
	_coord twiceXcenter = maxX + offsetX;
	
	wstring::iterator cursor = this->displayCursor ? get( this->cursor ) : wstring::iterator( wstring::npos , *this );
	
	
	// Determine Y start position
	_coord currentYPos = this->getOffsetY( dimensions ) + dimensions.y;
	
	// Neccessary things for iteration
	_lineStart			dataNextLine = this->lineStarts.front();
	wstring::iterator	iterNextLine = get( dataNextLine.charIndex );
	
	//
	// Iterate over whole text
	//
	for( _u32 lineNumber = 0 ; lineNumber < lineCount ; lineNumber++ )
	{
		// Make the old 'next-line' the current one
		wstring::iterator iterLineStart	= iterNextLine;
		
		// Fetch the new lines properties
		dataNextLine	= this->lineStarts[lineNumber+1];
		iterNextLine	= get( dataNextLine.charIndex );
		
		_length lineWidth = dataNextLine.lineWidth;		// The width of the current line is saved in the next lines data field
		_length lineHeight = dataNextLine.lineHeight;	// Same Here
		
		// Determine x-coordinate of the line start
		_coord currentX;
		switch( this->align ){
			case _align::left:		currentX = offsetX;	break;
			case _align::center:	currentX = ( twiceXcenter - lineWidth ) >> 1; break;
			case _align::right:
			default:				currentX = maxX - lineWidth; break;
		}
		
		// Iterate to to the cursor position
		for( wstring::iterator iter = iterLineStart ; iter < iterNextLine ; iter++ )
		{
			// Display Cursor?
			if( iter == cursor )
				port.drawVerticalLine( currentX - 1 , currentYPos , this->font->getHeight( this->fontSize ) , _color::red );
			
			// Draw letter
			_length charWidth = port.drawChar( currentX , currentYPos , this->font , *iter , this->fontColor , this->fontSize );
			
			if( charWidth ){
				charWidth += this->font->getLetterSpace( this->fontSize );
				currentX += charWidth; // Advance X-Position
			}
		}
		
		// Display Cursor behind the last character
		if( lineNumber == lineCount - 1 && cursor == end() )
			port.drawVerticalLine( currentX - 1 , currentYPos , this->font->getHeight( this->fontSize ) , _color::red );
		
		// Add Hyphen?
		if( dataNextLine.isSyllableBreak )
			port.drawChar( currentX , currentYPos , this->font , _multiLineGuiString::breakLineLetter , this->fontColor , this->fontSize );
		
		// Advance Y-Position
		currentYPos += lineHeight;
	}
}



_2s16 _multiLineGuiString::getXMetricsOfLetter( _rect dimensions ,_letterNum letterNumber ) const
{
	// Setup character positions
	_lineNumber 		lineNumber	= getLineContainingIndex( letterNumber );
	_u32				lineStart	= getLineStart( lineNumber );
	wstring::iterator	iter		= get( lineStart );
	wstring::iterator	end			= get( getLineEnd( lineNumber ) );
	
	// Make index relative to the current line
	letterNumber -= lineStart;
	
	// Determine x-coordinate of the line start
	_coord currentX = this->getXMetricsOfLine( dimensions , lineNumber ).first;
	
	
	// Iterate to to the letter we are loocking for
	while( iter <= end && letterNumber > 0 )
	{
		// Compute the width of the current character
		_u32 charWidth = this->font->getCharacterWidth( *iter );
		
		if( --letterNumber <= 0 )
			return _2s16( currentX + charWidth , charWidth );
		
		// Advance current width
		if( charWidth ){
			charWidth += this->font->getLetterSpace( this->fontSize );
			currentX += charWidth;
		}
		
		// Go to next character
		iter++;
	}
	
	return _2s16( currentX , 0 );
}



_length _multiLineGuiString::getTextHeight() const
{
	_u32 lineCount = this->getLineCount(); // Number of lines in the gui string
	
	// Compute the sum of all lines and their heights
	_length overAllHeight = 0;
	
	for( _u32 lineNumber = 0 ; lineNumber < lineCount ; lineNumber++ )
		overAllHeight += this->lineStarts[lineNumber+1].lineHeight;
	
	return overAllHeight;
}



wstring _multiLineGuiString::getLineContent( _u32 lineNo ) const
{
	if( lineNo >= this->getLineCount() )
		return "";
	
	_u32		start = this->lineStarts[lineNo].charIndex;
	_lineStart	end = this->lineStarts[lineNo + 1];
	
	wstring s = this->substr(
		start // startIndex
		, end.charIndex - start // length
	);
	
	if( end.isSyllableBreak )
		s.push_back(L'-');
	
	return s;
}



void _multiLineGuiString::pushBackLineBreak( _u32 charIndex , _length lineWidth , _length maxLineHeight , bool dueToHyphenation ){
	_lineStart start;
	start.charIndex = charIndex;
	start.lineHeight = maxLineHeight;
	start.lineWidth = lineWidth;
	start.isSyllableBreak = dueToHyphenation;
	this->lineStarts.emplace_back( move(start) );
}



void _multiLineGuiString::setCursorFromTouch( _rect dimensions , _coord cursorX , _coord cursorY )
{
	_u32 lineCount = this->getLineCount(); // Number of lines in the gui string
	
	// necessary?
	if( lineCount == 0 ){
		this->setCursor( 0 );
		return;
	}
	
	
	// Make relative
	cursorX -= dimensions.x;
	cursorY -= dimensions.y;
	
	// Clip
	if( cursorX < 0 ) cursorX = 0;
	if( cursorY < 0 ) cursorY = 0;
	
	// Determine Y start position of text
	_coord offsetY = this->getOffsetY( dimensions );
	
	
	// Compute line number
	_u32 lineNumber = 1;
	cursorY -= offsetY;
	while( lineNumber < lineCount )
	{
		_length lineHeight = this->lineStarts[lineNumber].lineHeight;
		
		if( cursorY < lineHeight )
			break;
		
		cursorY -= lineHeight;
		lineNumber++;
	}
	
	lineNumber--;
	
	// Setup character positions
	_u32				lineStart	= getLineStart( lineNumber );
	wstring::iterator	iter		= get( lineStart );
	wstring::iterator	end			= get( getLineEnd( lineNumber ) );
	
	
	// Determine x-coordinate of the line start
	_coord currentX = this->getXMetricsOfLine( dimensions , lineNumber );
	
	
	// Iterate to to the cursor position
	while( iter < end && cursorX > currentX )
	{
		// Compute the width of the current character
		_u32 charWidth = this->font->getCharacterWidth( *iter );
		
		// Advance current width
		if( charWidth ){
			charWidth += this->font->getLetterSpace( this->fontSize );
			currentX += charWidth;
		}
		
		// Go to next codepoint
		iter++;
	}
	
	this->setCursor( iter - begin() );
}



_u32 _multiLineGuiString::getLineContainingIndex( _u32 index ) const
{
	return std::count_if(
		this->lineStarts.begin()
		, this->lineStarts.end()
		, [index]( _lineStart start )->bool{
			return start.charIndex <= index;
		}
	) - 1;
}



_2s16 _multiLineGuiString::getYMetricsOfLine( _rect dimensions , _u32 lineNumber ) const
{
	_coord currentY = this->getOffsetY( dimensions ) + dimensions.y;
	_u32 maxLine = min( lineNumber , this->getLineCount() );
	_u32 curLine = 0;
	
	for( ; curLine < maxLine ; curLine++ )
		currentY += this->lineStarts[curLine+1].lineHeight;
	
	return _2s16( currentY , this->lineStarts[curLine+1].lineHeight );
}



_lineNumber _multiLineGuiString::moveCursorByLine( _rect dimensions , bool increase )
{
	_s32 curCursorX = this->tmpCursorX;
	
	if( curCursorX < 0 ){
		_2s16	xMetrics	= this->getXMetricsOfLetter( dimensions , this->cursor );
		this->tmpCursorX	= curCursorX = xMetrics.first + xMetrics.second;
	}
	
	_lineNumber	curLine		= this->getLineContainingIndex( this->cursor );
	_lineNumber	newLine		= mid( 0 , curLine + ( increase ? 1 : -1 ) , this->getLineCount() - 1 );
	_2s16		yPos		= this->getYMetricsOfLine( dimensions , newLine );
	
	// Set New Cursor
	this->setCursorFromTouch( dimensions , tmpCursorX , yPos.first + 1 );
	this->tmpCursorX		= curCursorX;
	
	return this->cursor;
}



_2s16 _multiLineGuiString::getXMetricsOfLine( _rect dimensions , _lineNumber lineNumber ) const
{
	if( lineNumber >= this->getLineCount() )
		return _2s16(0);
	
	// Useful variables
	_length guiStringWidth = dimensions.width;
	_length lineWidth = this->lineStarts[lineNumber+1].lineWidth; // The width of the current line is saved in the next lines attributes
	
	
	// Determine x-coordinate of the line start
	_coord lineX;
	switch( this->align ){
		case _align::left:		lineX = 0;	break;
		case _align::center:	lineX = ( guiStringWidth - lineWidth ) >> 1; break;
		case _align::right:
		default:				lineX = guiStringWidth - lineWidth; break;
	}
	
	return _2s16( lineX , lineWidth );
}