#include "_type/type.guistring.h"
#include "_library/library.syllable.h"

namespace{
	//! Structure that simplifies the serialization of a font pointer
	union escapeFontHelper
	{
		_u8 value;
		struct
		{
			bool i1 : 1;
			bool i2 : 1;
			bool i3 : 1;
			bool i4 : 1;
			bool notNull : 4;
		} PACKED ;
	} PACKED ;
}


_letterNum _guiString::getNumLetters() const
{
	_literal value = this->c_str();
	_literal end = value + this->length();
	_u32 numLetters = 0;
	
	while( value < end )
	{
		switch( (escapeChar)*value )
		{
			case escapeChar::sizeRestore:
			case escapeChar::colorRestore:
			case escapeChar::fontRestore:
				value+=1; break;
			case escapeChar::sizeChange:
				value+=2; break;
			case escapeChar::colorChange:
				value+=3; break;
			case escapeChar::fontChange:
				value+=6; break;
			default:
				++numLetters;
				++value;
		}
	}
	
	return numLetters;
}


_u32 _guiString::getNumBytesFromNumLetters( _letterNum letterNum , _letterNum startIndex ) const
{
	if( startIndex >= this->length() )
		return 0;
	
	_literal start = this->c_str() + startIndex;
	_literal end = start + this->length() - startIndex;
	_literal value = start;
	
	while( value < end && letterNum > 0 )
	{
		switch( (escapeChar)*value )
		{
			case escapeChar::sizeRestore:
			case escapeChar::colorRestore:
			case escapeChar::fontRestore:
				value+=1; break;
			case escapeChar::sizeChange:
				value+=2; break;
			case escapeChar::colorChange:
				value+=3; break;
			case escapeChar::fontChange:
				value+=6; break;
			default:
				--letterNum;
				++value;
		}
	}
	
	return value - start;
}



void _guiString::pushBackLineBreak( _u32 charIndex , _length lineWidth , _length maxLineHeight , bool dueToHyphenation ){
	_lineStart start;
	start.charIndex = charIndex;
	start.lineHeight = maxLineHeight;
	start.lineWidth = lineWidth;
	start.isSyllableBreak = dueToHyphenation;
	this->lineStarts.emplace_back( move(start) );
}



void _guiString::remove( _letterNum letterIndex , _letterNum letterCount )
{
	if( !letterCount )
		return;
	
	_u32 startIndex = this->getNumBytesFromNumLetters( letterIndex );
	_u32 numBytes = this->getNumBytesFromNumLetters( letterCount , startIndex );
	
	this->erase( startIndex , numBytes );
	this->update( startIndex );
}



void _guiString::update( _u32 startIndex )
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
	fontStack.push( this->font );
	fontSizeStack.push( this->fontSize );
	fontColorStack.push( this->fontColor );
	
	
	// Stacks where the state of the original stacks at a break character are saved to
	_fontStack		savedFontStack = fontStack;
	_fontColorStack	savedColorStack = fontColorStack;
	_fontSizeStack	savedSizeStack = fontSizeStack;
	
	
	_literal	originalStr = this->c_str();
	_literal	startStr = originalStr;
	_literal	curStr = startStr;
	_literal	endStr = curStr + strlen( curStr );
	
	if( !this->allowLineBreak )
		goto end;
	
	for( ; curStr <= endStr ; curStr++ )
	{
		// Check if the current character is an escape sequence, if yes, skip it
		if( !_guiString::processChar( curStr , fontStack , fontSizeStack , fontColorStack ) ){
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
			_u16 letterWidth = fontStack.top()->getCharacterWidth( *curStr , fontSizeStack.top() );
			if( letterWidth ){
				lineHeight = max( lineHeight , fontStack.top()->getHeight( fontSizeStack.top() ) ); // Increase line height if current font is bigger that before
				letterWidth += fontStack.top()->getLetterSpace( fontSizeStack.top() );
			}
			lineWidthSince += letterWidth;
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
		
		// If the current line width exceeds the maximum width we have available
		if( lineWidthUntil + lineWidthSince >= this->dimensions.width )
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
					if( !_guiString::processChar( syllableCharCur , savedFontStack , savedSizeStack , savedColorStack ) )
						continue;
					
					if( savedFontStack.top()->isCharSupported( *syllableCharCur ) )
					{
						_u16 letterWidth = savedFontStack.top()->getCharacterWidth( *syllableCharCur , savedSizeStack.top() );
						if( letterWidth )
							letterWidth += savedFontStack.top()->getLetterSpace( savedSizeStack.top() );
						wordWidth += letterWidth;
					}
				}
				
				_length hyphenWidth		= savedFontStack.top()->getCharacterWidth( _guiString::breakLineLetter , savedSizeStack.top() );
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
	}
	
	// Jump Label
	end:
	
	// Subtract letter spacing since we don't have it after the last character
	_length letterSpace = lineWidthSince + lineWidthUntil > 0 ? fontStack.top()->getLetterSpace( fontSizeStack.top() ) : 0;
	
	this->pushBackLineBreak( ( curStr - originalStr ) /* Current Index */ , lineWidthSince + lineWidthUntil - letterSpace , lineHeight , false ); // Push end
	
	this->needsRefreshFlag = true;
}



void _guiString::drawTo( _bitmapPort& port )
{
	if( this->needsUpdateFlag )
		this->update();
	
	// Buffer frequently used variables
	_u32 lineCount = this->getLineCount(); // Number of lines in the gui string
	if( !lineCount )
		return;
	_coord offsetX = this->dimensions.x;
	_length maxX = offsetX + this->dimensions.width;
	_coord twiceXcenter = maxX + offsetX;
	
	// Iteration variables
	_literal textOrigin		= this->c_str();	
	_literal textCursor		= this->displayCursor ? textOrigin + this->getNumBytesFromNumLetters( this->cursor ) : nullptr;
	
	// Stacks
	_fontStack		curFont; // Current font
	_fontSizeStack	curSize; // Current font size
	_fontColorStack	curColor; // Current font color
	// Setup stacks
	curFont.push( this->font );
	curSize.push( this->fontSize );
	curColor.push( this->fontColor );
	
	
	// Determine Y start position
	_coord currentYPos = this->getOffsetY() + this->dimensions.y;
	
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
			default:				currentX = maxX - startObjNextLine.lineWidth; break;
		}
		
		// Iterate to to the cursor position
		for( _literal curPtr = startPtrCurLine ; curPtr < startPtrNextLine ; curPtr++ )
		{
			if( !_guiString::processChar( curPtr , curFont , curSize , curColor ) )
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
			port.drawChar( currentX , currentYPos , curFont.top() , _guiString::breakLineLetter , curColor.top() , curSize.top() );
		
		// Advance Y-Position
		currentYPos += curLineHeight;
	}
}



bool _guiString::processChar( _literal& str , _fontStack& fontStack , _fontSizeStack& fontSizeStack , _fontColorStack& fontColorStack ) const
{
	_literal origStr = str;
	
	switch( (escapeChar)*str )
	{
		case escapeChar::fontChange:
		{
			if( str[1] ) str++; else break;
			if( str[1] ) str++; else break;
			if( str[1] ) str++; else break;
			if( str[1] ) str++; else break;
			if( str[1] ) str++; else break;
			
			// Check if feature enabled
			if( !this->allowFontChange )
				break;
			
			// Recreate the font pointer
			escapeFontHelper helper = { .value = origStr[1] };
			_u32 i1 = _u8( origStr[2] ) ^ ( !helper.i1 );
			_u32 i2 = _u8( origStr[3] ) ^ ( !helper.i2 );
			_u32 i3 = _u8( origStr[4] ) ^ ( !helper.i3 );
			_u32 i4 = _u8( origStr[5] ) ^ ( !helper.i4 );
			
			_fontPtr font = reinterpret_cast<_fontPtr>( ( i1 << 24 ) | ( i2 << 16 ) | ( i3 << 8 ) | i4 );
			
			// Check if the font pointer is valid
			if( font && font->isValid() )
				fontStack.push( font );
			else
				fontStack.push( fontStack.top() );
			break;
		}
		case escapeChar::colorChange:
		{
			if( str[1] ) str++; else break;
			if( str[1] ) str++; else break;
			
			// Check if feature enabled
			if( !this->allowColorChange )
				break;
			
			_pixel col = ( _u16(origStr[1]) << 8 ) | _u16(origStr[2]);
			fontColorStack.push( col );
			
			break;
		}
		case escapeChar::sizeChange:
		{
			if( str[1] ) str++; else break;
			
			// Check if feature enabled
			if( !this->allowSizeChange )
				break;
			
			fontSizeStack.push( origStr[1] );
			break;
		}
		case escapeChar::sizeRestore:
			if( fontSizeStack.size() > 1 && this->allowSizeChange )
				fontSizeStack.pop();
			break;
		case escapeChar::fontRestore:
			if( fontStack.size() > 1 && this->allowFontChange )
				fontStack.pop();
			break;
		case escapeChar::colorRestore:
			if( fontColorStack.size() > 1 && this->allowColorChange )
				fontColorStack.pop();
			break;
		default:
			return true;
	}
	
	return false;
}

string _guiString::fontChange( _fontPtr ft )
{
	_u32 i = (_u32)ft;
	_u8 i1 = i >> 24;
	_u8 i2 = i >> 16 & 255;
	_u8 i3 = i >> 8 & 255;
	_u8 i4 = i & 255;
	
	escapeFontHelper b;
	b.i1 = i1 & 1;
	b.i2 = i2 & 1;
	b.i3 = i3 & 1;
	b.i4 = i4 & 1;
	b.notNull = 1;
	
	_char ret[7] = {
		(_char) escapeChar::fontChange
		, _char( b.value ) // pack all four least significant bits into one 'char'
		, _char( i1 | 1 ) , _char( i2 | 1 )	, _char( i3 | 1 ) , _char( i4 | 1 ) // Or with 1 to prevent the the terminating '\0' char
		, 0
	};
	return ret;
}



string _guiString::getLineContent( _u32 lineNo ) const
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



void _guiString::setCursorFromTouch( _coord cursorX , _coord cursorY )
{
	_u32 lineCount = this->getLineCount(); // Number of lines in the gui string
	
	// Neccesary?
	if( !lineCount ){
		this->setCursor( 0 );
		return;
	}
	
	
	// Make relative
	cursorX -= this->dimensions.x;
	cursorY -= this->dimensions.y;
	
	// Clip
	if( cursorX < 0 ) cursorX = 0;
	if( cursorY < 0 ) cursorY = 0;
	
	// Determine Y start position of text
	_coord offsetY = this->getOffsetY();
	
	
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
	_coord currentX = this->getXMetricsOfLine( lineNumber );
	
	
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



_length _guiString::getOffsetY() const 
{
	if( this->vAlign == _valign::top )
		return 0;
	else{
		_length overAllHeight = this->getTextHeight();
		if( this->vAlign == _valign::middle )
			return ( this->dimensions.height - overAllHeight - 1 ) >> 1;
		else // _valign::bottom
			return this->dimensions.height - overAllHeight;
	}
}



_length _guiString::getTextHeight() const
{
	_u32 lineCount = this->getLineCount(); // Number of lines in the gui string
	// Compute the sum of all lines and their heights
	_length overAllHeight = 0;
	for( _u32 lineNumber = 0 ; lineNumber < lineCount ; lineNumber++ )
		overAllHeight += this->lineStarts[lineNumber+1].lineHeight;
	return overAllHeight;
}



void _guiString::insert( _letterNum letterIndex , string text )
{
	this->needsUpdateFlag |= !text.empty();
	
	string::insert( this->getNumBytesFromNumLetters( letterIndex ) , move(text) );
}



_u32 _guiString::getLineContainingIndex( _u32 index ) const
{
	return std::count_if(
		this->lineStarts.begin()
		, this->lineStarts.end()
		, [index]( _lineStart start )->bool{
			return start.charIndex <= index;
		}
	) - 1;
}



_2s32 _guiString::getYMetricsOfLine( _u32 lineNumber ) const
{
	_coord currentY = this->getOffsetY() + this->dimensions.y;
	_u32 maxLine = min( lineNumber , this->getLineCount() );
	_u32 curLine = 0;
	for( ; curLine < maxLine ; curLine++ )
		currentY += this->lineStarts[curLine+1].lineHeight;
	return _2s32( currentY , this->lineStarts[curLine+1].lineHeight );
}



_2s32 _guiString::getXMetricsOfLetter( _letterNum letterNumber ) const
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
	_coord currentX = this->getXMetricsOfLine( lineNumber ).first;
	
	
	// Iterate to to the letter we are loocking for
	while( curPtr <= endPtr && letterNumber > 0 )
	{
		if( _guiString::processChar( curPtr , curFont , curSize , curColor ) )
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


_lineNumber _guiString::moveCursorByLine( bool increase )
{
	_s32 curCursorX = this->tmpCursorX;
	
	if( curCursorX < 0 ){
		_2s32	xMetrics	= this->getXMetricsOfLetter( this->cursor );
		this->tmpCursorX	= curCursorX = xMetrics.first + xMetrics.second;
	}
	
	_u32		cursor		= this->getNumBytesFromNumLetters( this->cursor );
	_lineNumber	curLine		= this->getLineContainingIndex( cursor );
	_lineNumber	newLine		= mid( 0 , curLine + ( increase ? 1 : -1 ) , this->getLineCount() - 1 );
	_2s32		yPos		= this->getYMetricsOfLine( newLine );
	this->setCursorFromTouch( tmpCursorX , yPos.first + 1 );
	this->tmpCursorX		= curCursorX;
	
	return this->cursor;
}


_letterNum _guiString::getStacksAt( _u32 index , _fontStack& fontStack , _fontSizeStack& fontSizeStack , _fontColorStack& fontColorStack ) const 
{
	// Setup Stacks
	fontStack.push( this->font );
	fontSizeStack.push( this->fontSize );
	fontColorStack.push( this->fontColor );
	
	// Setup end position
	_literal curPtr = this->c_str();
	_literal end = curPtr + min( index , this->length() );
	
	// Counter
	_letterNum numLetters = 0;
	
	// Iterate up to the line we are interested in
	while( curPtr < end && index ){
		numLetters += _guiString::processChar( curPtr , fontStack , fontSizeStack , fontColorStack );
		curPtr++;
	}
	
	return numLetters;
}

_2s32 _guiString::getXMetricsOfLine( _lineNumber lineNumber ) const
{
	if( lineNumber >= this->getLineCount() )
		return _2s32(0);
	
	// Useful variables
	_length guiStringWidth = this->dimensions.width;
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



string _guiString::colorChange( _color color ){
	_u16 col = color.getColor();
	_char tmp[4] = { (_char)escapeChar::colorChange , _char( col >> 8 ) , _char( col & 255 ) , 0 };
	return tmp;
}



string _guiString::sizeChange( _u8 newSize ){
	_char tmp[3] = { (_char)escapeChar::sizeChange , (_char)newSize , 0 };
	return tmp;
}