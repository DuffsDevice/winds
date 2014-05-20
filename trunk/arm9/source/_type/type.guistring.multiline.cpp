#include "_type/type.guistring.multiline.h"
#include "_library/library.syllable.h"

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
	bool		stacksInvalidated = true;
	
	
	// Stacks for font, size, color
	_fontStack		fontStack;		// Current font
	_fontSizeStack	fontSizeStack;	// Current font size
	_fontColorStack	fontColorStack;	// Current font color
	
	// Setup stacks
	getStacksAt( 0 , fontStack , fontSizeStack , fontColorStack );
	
	
	// Stacks where the state of the original stacks at a break character are saved to
	_fontStack		savedFontStack = fontStack;
	_fontColorStack	savedColorStack = fontColorStack;
	_fontSizeStack	savedSizeStack = fontSizeStack;
	
	
	_literal	originalStr = this->c_str();
	_literal	startStr = originalStr;
	_literal	curStr = startStr;
	_literal	endStr = curStr + strlen( curStr );
	
	for( ; curStr <= endStr ; curStr++ )
	{
		// Check if the current character is an escape sequence, if yes, skip it
		if( !_multiLineGuiString::processChar( curStr , fontStack , fontSizeStack , fontColorStack ) ){
			stacksInvalidated = true;
			continue;
		}
		
		// Check if ch is a character where we possibbly could break
		if( *curStr == '\n' )
		{
			// Subtract letter spacing since we don't have it after the last character
			_length letterSpacing = fontStack.top()->getLetterSpace( fontSizeStack.top() );
			
			this->pushBackLineBreak( ( curStr - originalStr ) /* Current Index */ + 1, lineWidthUntil + lineWidthSince - letterSpacing , lineHeight );
			
			// Reset current line width since we added a line break
			lineWidthSince = lineWidthUntil = 0;
			lineHeight = fontStack.top()->getHeight( fontSizeStack.top() );
		}
		else if( fontStack.top()->isCharSupported( *curStr ) )
		// advance the current lineWidth by the width of the letter to display
		{
			_length letterWidth = fontStack.top()->getCharacterWidth( *curStr , fontSizeStack.top() );
			if( letterWidth ){
				lineHeight = max( lineHeight , fontStack.top()->getHeight( fontSizeStack.top() ) ); // Increase line height if current font is bigger that before
				letterWidth += fontStack.top()->getLetterSpace( fontSizeStack.top() );
			}
			lineWidthSince += letterWidth;
		}
		
		// If the current line width exceeds the maximum width we have available
		if( lineWidthUntil + lineWidthSince >= dimensions.width )
		{
			_s32		effectiveStartIdx	= lastBreakableIndex + 1;
			_literal	tmpStart			= startStr + effectiveStartIdx;
			_literal	tmpEnd				= startStr + ( curStr - originalStr ) /* Current Index */ + 1;
			_list<_int>	possibleLineBreaks	= SyllableParser::parseText( tmpStart , tmpEnd );
			
			// If a Syllable could be found
			if( !possibleLineBreaks.empty() )
			{
				// Recreate the parsing of the last word
				_length wordWidth = 0;
				_u32 syllableBreak = possibleLineBreaks.back();
				_literal syllableCharEnd = tmpStart + syllableBreak;
				
				// Iterate to the syllable end after whcih we brake the line
				for( _literal syllableCharCur = tmpStart ; syllableCharCur <= syllableCharEnd ; syllableCharCur++ )
				{
					// Check if the current character is an escape sequence, if yes, skip it
					if( !_multiLineGuiString::processChar( syllableCharCur , savedFontStack , savedSizeStack , savedColorStack ) )
						continue;
					
					if( savedFontStack.top()->isCharSupported( *syllableCharCur ) )
					{
						_u16 letterWidth = savedFontStack.top()->getCharacterWidth( *syllableCharCur , savedSizeStack.top() );
						if( letterWidth )
							letterWidth += savedFontStack.top()->getLetterSpace( savedSizeStack.top() );
						wordWidth += letterWidth;
					}
				}
				
				_length hyphenWidth		= savedFontStack.top()->getCharacterWidth( _multiLineGuiString::breakLineLetter , savedSizeStack.top() );
				_length letterSpacing	= savedFontStack.top()->getLetterSpace( savedSizeStack.top() );
				
				// Add Syllable Line Break
				pushBackLineBreak( effectiveStartIdx + syllableBreak + 1 , lineWidthUntil + wordWidth + hyphenWidth - letterSpacing, lineHeight , true );
				
				// Reset Last Word width
				lineWidthSince -= wordWidth;
				lineWidthUntil = 0;
				lineHeight = fontStack.top()->getHeight( fontSizeStack.top() );
				continue;
			}
			
			_length letterSpace = fontStack.top()->getLetterSpace( fontSizeStack.top() ); // Subtract letter spacing since we don't have it after the last character
			
			// In case the word is so long that it doesn't fit in one line
			// AND it does not have syllables!
			if( lastBreakableIndex >= 0 )
				pushBackLineBreak( lastBreakableIndex + 1 , lineWidthUntil - letterSpace , lineHeight ); // +1 : We want to break behind the delimiter
			else{
				pushBackLineBreak( ( curStr - originalStr ) + 1 , lineWidthUntil + lineWidthSince - letterSpace , lineHeight );
				lineWidthSince = 0;
			}
			
			// Reset current line width since we added a line break
			lineWidthUntil = 0;
			lineHeight = fontStack.top()->getHeight( fontSizeStack.top() );
			lastBreakableIndex = -1;
		}
		
		// Remember that we have a possible break here
		if( SyllableParser::charIsOf( *curStr , breakChars ) )
		{
			lastBreakableIndex = ( curStr - originalStr ); // <= Current Index
			lineWidthUntil += lineWidthSince;
			lineWidthSince = 0;
			
			// Save font stacks
			if( stacksInvalidated ){
				savedFontStack = fontStack;
				savedColorStack = fontColorStack;
				savedSizeStack = fontSizeStack;
				stacksInvalidated = false;
			}
		}
	}
	
	// Subtract letter spacing since we don't have it after the last character
	_length letterSpace = lineWidthSince + lineWidthUntil > 0 ? fontStack.top()->getLetterSpace( fontSizeStack.top() ) : 0;
	
	this->pushBackLineBreak( ( curStr - originalStr ) /* Current Index */ , lineWidthSince + lineWidthUntil - letterSpace , lineHeight , false ); // Push end
	
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
	
	// Iteration variables
	_literal textOrigin		= this->c_str();
	_literal textCursor		= this->displayCursor ? textOrigin + this->getNumBytesFromNumLetters( this->cursor ) : nullptr;
	
	// Stacks
	_fontStack		curFont; // Current font
	_fontSizeStack	curSize; // Current font size
	_fontColorStack	curColor; // Current font color
	
	// Setup stacks
	getStacksAt( 0 , curFont , curSize , curColor );
	
	
	// Determine Y start position
	_coord currentYPos = this->getOffsetY( dimensions ) + dimensions.y;
	
	_lineStart	startObjNextLine = this->lineStarts.front();
	_literal	startPtrNextLine = textOrigin + startObjNextLine.charIndex;
	_lineStart	startObjCurLine; // These will be overwritten in the first two lines inside the loop
	_literal	startPtrCurLine;
	
	//
	// Iterate over whole text
	//
	for( _u32 lineNumber = 0 ; lineNumber < lineCount ; lineNumber++ )
	{
		// Make the old 'next-line' the current one
		startObjCurLine		= move(startObjNextLine);
		startPtrCurLine		= move(startPtrNextLine);
		
		// Fetch the new lines properties
		startObjNextLine	= this->lineStarts[lineNumber+1];
		startPtrNextLine	= textOrigin + startObjNextLine.charIndex;
		
		_length curLineWidth = startObjNextLine.lineWidth;		// The width of the current line is saved in the next lines attributes
		_length curLineHeight = startObjNextLine.lineHeight;	// Same Here
		
		// Determine x-coordinate of the line start
		_coord currentX;
		switch( this->align ){
			case _align::left:		currentX = offsetX;	break;
			case _align::center:	currentX = ( twiceXcenter - curLineWidth ) >> 1; break;
			case _align::right:
			default:				currentX = maxX - curLineWidth; break;
		}
		
		// Iterate to to the cursor position
		for( _literal curPtr = startPtrCurLine ; curPtr < startPtrNextLine ; curPtr++ )
		{
			if( !_multiLineGuiString::processChar( curPtr , curFont , curSize , curColor ) )
				continue;
			
			// Display Cursor?
			if( curPtr == textCursor )
				port.drawVerticalLine( currentX - 1 , currentYPos , curFont.top()->getHeight( curSize.top() ) , _color::red );
			
			// Draw letter
			_length charWidth = port.drawChar( currentX , currentYPos , curFont.top() , *curPtr , curColor.top() , curSize.top() );
			
			if( charWidth ){
				charWidth += curFont.top()->getLetterSpace( curSize.top() );
				currentX += charWidth; // Advance X-Position
			}
		}
		
		// Add Hyphen
		if( startObjNextLine.isSyllableBreak )
			port.drawChar( currentX , currentYPos , curFont.top() , _multiLineGuiString::breakLineLetter , curColor.top() , curSize.top() );
		
		// Advance Y-Position
		currentYPos += curLineHeight;
	}
}



_2s32 _multiLineGuiString::getXMetricsOfLetter( _rect dimensions ,_letterNum letterNumber ) const
{
	// Setup character positions
	_lineNumber lineNumber		= getLineContainingIndex( getNumBytesFromNumLetters( letterNumber ) );
	_literal	textOrigin		= this->c_str();
	_u32		lineStart		= getLineStart( lineNumber );
	_literal	curPtr			= textOrigin + lineStart;
	_literal	endPtr			= textOrigin + getLineEnd( lineNumber );
	
	
	// Setup Stacks
	_fontStack		curFont; // Current X-advance
	_fontSizeStack	curSize; // Current font size
	_fontColorStack	curColor; // Current font color
	
	
	// Get Stacks and subtract the number of letters until that line start
	letterNumber -= this->getStacksAt( lineStart , curFont , curSize , curColor );
	
	
	// Determine x-coordinate of the line start
	_coord currentX = this->getXMetricsOfLine( dimensions , lineNumber ).first;
	
	
	// Iterate to to the letter we are loocking for
	while( curPtr <= endPtr && letterNumber > 0 )
	{
		if( _multiLineGuiString::processChar( curPtr , curFont , curSize , curColor ) )
		{
			// Compute the width of the current character
			_u32 charWidth = this->font->getCharacterWidth( *curPtr );
			
			if( !--letterNumber )
				return _2s32( currentX + charWidth , charWidth );
			
			// Advance current width
			if( charWidth ){
				charWidth += this->font->getLetterSpace( this->fontSize );
				currentX += charWidth;
			}
		}
		
		// Go to next character
		curPtr++;
	}
	
	return _2s32( currentX , 0 );
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



string _multiLineGuiString::getLineContent( _u32 lineNo ) const
{
	if( lineNo >= this->getLineCount() )
		return "";
	
	_u32 start = this->lineStarts[lineNo].charIndex;
	_lineStart end = this->lineStarts[lineNo + 1];
	
	string s = this->substr(
		start // startIndex
		, end.charIndex - start // length
	);
	if( end.isSyllableBreak )
		s += "-";
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
	if( !lineCount ){
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
	_u32 lineNumber = 0;
	cursorY -= offsetY;
	while( lineNumber < lineCount - 1 ){
		_length lineHeight = this->lineStarts[lineNumber+1].lineHeight;
		if( cursorY < lineHeight )
			break;
		cursorY -= lineHeight;
		lineNumber++;
	}
	
	// Setup character positions
	_literal	textOrigin		= this->c_str();	
	_u32		lineStart		= getLineStart( lineNumber );
	_literal	curPtr			= textOrigin + lineStart;
	_literal	lineEndPtr		= textOrigin + getLineEnd( lineNumber );
	
	
	// Setup Stacks
	_fontStack		curFont; // Current X-advance
	_fontSizeStack	curSize; // Current font size
	_fontColorStack	curColor; // Current font color
	
	
	// Counter for letters
	_u32		numLetters = _guiString::getStacksAt( lineStart , curFont , curSize , curColor );
	
	
	// Determine x-coordinate of the line start
	_coord currentX = this->getXMetricsOfLine( dimensions , lineNumber );
	
	
	// Iterate to to the cursor position
	while( curPtr < lineEndPtr && cursorX > currentX )
	{
		if( _guiString::processChar( curPtr , curFont , curSize , curColor ) )
		{
			// Compute the width of the current character
			_u32 charWidth = this->font->getCharacterWidth( *curPtr );
			
			// Advance current width
			if( charWidth ){
				charWidth += this->font->getLetterSpace( this->fontSize );
				currentX += charWidth;
			}
			
			numLetters++;
		}
		
		// Go to next character
		curPtr++;
	}
	
	this->setCursor( numLetters );
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



_2s32 _multiLineGuiString::getYMetricsOfLine( _rect dimensions , _u32 lineNumber ) const
{
	_coord currentY = this->getOffsetY( dimensions ) + dimensions.y;
	_u32 maxLine = min( lineNumber , this->getLineCount() );
	_u32 curLine = 0;
	for( ; curLine < maxLine ; curLine++ )
		currentY += this->lineStarts[curLine+1].lineHeight;
	return _2s32( currentY , this->lineStarts[curLine+1].lineHeight );
}



_lineNumber _multiLineGuiString::moveCursorByLine( _rect dimensions , bool increase )
{
	_s32 curCursorX = this->tmpCursorX;
	
	if( curCursorX < 0 ){
		_2s32	xMetrics	= this->getXMetricsOfLetter( dimensions , this->cursor );
		this->tmpCursorX	= curCursorX = xMetrics.first + xMetrics.second;
	}
	
	_u32		cursor		= this->getNumBytesFromNumLetters( this->cursor );
	_lineNumber	curLine		= this->getLineContainingIndex( cursor );
	_lineNumber	newLine		= mid( 0 , curLine + ( increase ? 1 : -1 ) , this->getLineCount() - 1 );
	_2s32		yPos		= this->getYMetricsOfLine( dimensions , newLine );
	
	// Set New Cursor
	this->setCursorFromTouch( dimensions , tmpCursorX , yPos.first + 1 );
	this->tmpCursorX		= curCursorX;
	
	return this->cursor;
}



_2s32 _multiLineGuiString::getXMetricsOfLine( _rect dimensions , _lineNumber lineNumber ) const
{
	if( lineNumber >= this->getLineCount() )
		return _2s32(0);
	
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
	
	return _2s32( lineX , lineWidth );
}