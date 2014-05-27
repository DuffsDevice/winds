#include "_type/type.guistring.singleline.h"

_length _singleLineGuiString::getPreferredTextWidth() const
{
	// Stacks for font, size, color
	_fontStack		fontStack;		// Current font
	_fontSizeStack	fontSizeStack;	// Current font size
	_fontColorStack	fontColorStack;	// Current font color
	
	// Setup stacks
	getStacksAt( 0 , fontStack , fontSizeStack , fontColorStack );
	
	// Text iteration variables
	_literal	curStr = this->c_str();
	_literal	endStr = curStr + this->length();
	
	// Counter variable for number of preferred pixels
	_length lineWidth = 0;
	
	for( ; curStr < endStr ; curStr++ )
	{
		// Check if the current character is an escape sequence, if yes, skip it
		if( !_guiString::processChar( curStr , fontStack , fontSizeStack , fontColorStack ) )
			continue;
		
		if( *curStr != '\n' && fontStack.top()->isCharSupported( *curStr ) )
		{
			// advance the current lineWidth by the width of the letter to display
			_length letterWidth	= fontStack.top()->getCharacterWidth( *curStr , fontSizeStack.top() );
			
			if( letterWidth )
				letterWidth		+= fontStack.top()->getLetterSpace( fontSizeStack.top() );
			
			lineWidth += letterWidth;
		}
	}
	
	return lineWidth;
}

void _singleLineGuiString::update( _rect dimensions , _u32 )
{
	if( !this->font || !this->font->isValid() )
		return;
	
	this->lineWidth				= 0;										// Width in pixels of all letters in the text
	_length		lineWidthUntil	= 0;										// Width in pixels of all letters before the possible ellipsis
	_length		lineHeight		= this->font->getHeight( this->fontSize );	// Height in pixels of the current line
	
	// Letter counters
	_letterNum	overAllLetters = this->getNumLetters();
	_letterNum	ellipsisLetter = overAllLetters - this->ellipsis;
	_letterNum	curLetter = 0;
	
	
	// Stacks for font, size, color
	_fontStack		fontStack;		// Current font
	_fontSizeStack	fontSizeStack;	// Current font size
	_fontColorStack	fontColorStack;	// Current font color
	
	// Setup stacks
	getStacksAt( 0 , fontStack , fontSizeStack , fontColorStack );
	
	
	// Text iteration variables
	_literal	originalStr = this->c_str();
	_literal	startStr = originalStr;
	_literal	curStr = startStr;
	_literal	endStr = curStr + strlen( curStr );
	
	for( ; curStr < endStr ; curStr++ )
	{
		// Check if the current character is an escape sequence, if yes, skip it
		if( !_guiString::processChar( curStr , fontStack , fontSizeStack , fontColorStack ) )
			continue;
		
		curLetter++;
		
		// Check if we pass the ellipsis-letter before which we could have the ellipsis
		if( this->ellipsis >= 0 && curLetter == ellipsisLetter )
			lineWidthUntil = this->lineWidth;
		
		// Process current letter
		if( *curStr != '\n' && fontStack.top()->isCharSupported( *curStr ) )
		{
			// advance the current lineWidth by the width of the letter to display
			_length letterWidth	= fontStack.top()->getCharacterWidth( *curStr , fontSizeStack.top() );
			
			if( letterWidth ){
				lineHeight		= max( lineHeight , fontStack.top()->getHeight( fontSizeStack.top() ) ); // Increase line height if current font is bigger that before
				letterWidth		+= fontStack.top()->getLetterSpace( fontSizeStack.top() );
			}
			this->lineWidth += letterWidth;
		}
	}
	
	this->lineHeight = lineHeight;
	
	// Subtract letter spacing since we don't have it after the last character
	_length letterSpace = this->lineWidth > 0 ? fontStack.top()->getLetterSpace( fontSizeStack.top() ) : 0;
	this->lineWidth -= letterSpace;
	
	// Check if we even exceeded the available width
	if( this->lineWidth > dimensions.width && this->ellipsis >= 0 )
	{
		// Reset stacks again
		getStacksAt( 0 , fontStack , fontSizeStack , fontColorStack );
		
		
		// Number of pixels that are outside the width of the guistring
		_length pixelsBehindWidth = this->lineWidth - dimensions.width; 
		
		// Number of pixels the part before the letter before which we have the ellipsis must not exceed
		_length ellipsisPartMaxWidth = lineWidthUntil - pixelsBehindWidth;
		
		// Various declarations
		_length currentWidth = 0;
		_length dotWidth = this->font->getCharacterWidth( '.' , this->fontSize );
		_length letterSpaceWidth = this->font->getLetterSpace( this->fontSize );
		
		
		// Since the ellipsis (...) hast to fit into the available width, start with the initial value of the ellipsis-width
		currentWidth = 3 * ( dotWidth + letterSpaceWidth );
		this->lineWidth += currentWidth;
		
		// Again iterate over all characters that fit before the ellipsis
		curStr = startStr;
		curLetter = 0;
		
		if( currentWidth > ellipsisPartMaxWidth )
			curLetter = overAllLetters;
		
		if( currentWidth >= ellipsisPartMaxWidth )
			curStr = endStr; // Aborts the loop immediately
		
		// Iterate
		for( ; curStr < endStr ; curStr++ )
		{
			// Check if the current character is an escape sequence, if yes, skip it
			if( !_guiString::processChar( curStr , fontStack , fontSizeStack , fontColorStack ) )
				continue;
			
			_length letterWidth	= fontStack.top()->getCharacterWidth( *curStr , fontSizeStack.top() );
			if( letterWidth )
				letterWidth += fontStack.top()->getLetterSpace( fontSizeStack.top() );
			
			// advance the current lineWidth by the width of the letter
			currentWidth += letterWidth;
			
			// Increase number of processed letters (letters before the ellipsis)
			curLetter++;
			
			// Abort since we exceeded the width we have available
			if( currentWidth > ellipsisPartMaxWidth )
				break;
		}
		
		// Subtract all not-displayed letters from the lineWidth
		for( curStr++ ; curStr < endStr ; curStr++ )
		{
			// Check if the current character is an escape sequence, if yes, skip it
			if( !_guiString::processChar( curStr , fontStack , fontSizeStack , fontColorStack ) )
				continue;
			
			_length letterWidth	= fontStack.top()->getCharacterWidth( *curStr , fontSizeStack.top() );
			if( letterWidth )
				letterWidth += fontStack.top()->getLetterSpace( fontSizeStack.top() );
			
			this->lineWidth -= letterWidth;
		}
		
		// Write Attributes
		omitStart	= curLetter;
		omitEnd		= ellipsisLetter;
	}
	else{
		omitStart	= overAllLetters;
		omitEnd		= overAllLetters;
	}
	
	this->needsUpdateFlag = false;
	this->needsRefreshFlag = true;
}



void _singleLineGuiString::drawTo( _rect dimensions , _bitmapPort& port )
{
	if( this->needsUpdateFlag )
		this->update( dimensions );
	
	// Iteration variables
	_literal startPtr		= this->c_str();
	_literal curPtr			= startPtr;
	_literal endPtr			= startPtr + this->length();
	_letterNum cursor		= this->displayCursor ? this->cursor : -1;
	_letterNum curLetter	= 0;
	
	
	// Stacks
	_fontStack		curFont; // Current font
	_fontSizeStack	curSize; // Current font size
	_fontColorStack	curColor; // Current font color
	
	// Setup stacks
	getStacksAt( 0 , curFont , curSize , curColor );
	
	//
	// Compute drawing coordinates
	//
	_coord currentX = this->getLineStart( dimensions );
	_coord currentY	= this->getOffsetY( dimensions ) + dimensions.y;
	
	
	//
	// Iterate over whole text
	//
	
	// Iterate to to the ellipsis position
	for( ; curPtr < endPtr && curLetter < this->omitStart ; curPtr++ )
	{
		if( !_guiString::processChar( curPtr , curFont , curSize , curColor ) )
			continue;
		
		// Display Cursor?
		if( curLetter == cursor )
			port.drawVerticalLine( currentX - 1 , currentY , curFont.top()->getHeight( curSize.top() ) , _color::red );
		
		curLetter++;
		
		// Draw letter
		_length charWidth = port.drawChar( currentX , currentY , curFont.top() , *curPtr , curColor.top() , curSize.top() );
		
		if( charWidth ){
			charWidth += curFont.top()->getLetterSpace( curSize.top() );
			currentX += charWidth; // Advance X-Position
		}
	}
	
	if( curPtr < endPtr )
	{
		// Draw ellipsis
		for( int i = 0 ; i < 3 ; i++ )
		{
			// Draw letter
			_length charWidth = port.drawChar( currentX , currentY , this->font , '.' , this->fontColor , this->fontSize );
			
			if( charWidth ){
				charWidth += curFont.top()->getLetterSpace( curSize.top() );
				currentX += charWidth; // Advance X-Position
			}
		}
		
		// Iterate to end of omitted text
		for( ; curPtr < endPtr && curLetter < this->omitEnd ; curPtr++ )
		{
			if( _guiString::processChar( curPtr , curFont , curSize , curColor ) )
				curLetter++;
		}
		
		// Draw the rest, if there is one
		for( ; curPtr < endPtr ; curPtr++ )
		{
			if( !_guiString::processChar( curPtr , curFont , curSize , curColor ) )
				continue;
			
			// Display Cursor?
			if( curLetter == cursor )
				port.drawVerticalLine( currentX - 1 , currentY , curFont.top()->getHeight( curSize.top() ) , _color::red );
			
			curLetter++;
			
			// Draw letter
			_length charWidth = port.drawChar( currentX , currentY , curFont.top() , *curPtr , curColor.top() , curSize.top() );
			
			if( charWidth ){
				charWidth += curFont.top()->getLetterSpace( curSize.top() );
				currentX += charWidth; // Advance X-Position
			}
		}
	}
	
	// Draw Cursor, if it is after the last character
	if( curLetter == cursor )
		port.drawVerticalLine( currentX - 1 , currentY , curFont.top()->getHeight( curSize.top() ) , _color::red );
	
	this->needsRefreshFlag = false;
}

_length _singleLineGuiString::getLineStart( _rect dimensions ) const
{
	_coord	offsetX = dimensions.x;
	_length	maxX = offsetX + dimensions.width;
	_coord 	twiceXcenter = maxX + offsetX;
	
	// Determine x-coordinate
	switch( this->align ){
		case _align::left:
			return offsetX;
		case _align::center:
			return ( twiceXcenter - this->lineWidth ) >> 1;
		case _align::right:
		default:
			return maxX - this->lineWidth;
	}
}

_2s32 _singleLineGuiString::getXMetricsOfLetter( _rect dimensions , _letterNum letterNumber ) const
{
	// Iteration variables
	_literal startPtr		= this->c_str();
	_literal curPtr			= startPtr;
	_literal endPtr			= startPtr + this->length();	
	
	// Stacks
	_fontStack		curFont; // Current font
	_fontSizeStack	curSize; // Current font size
	_fontColorStack	curColor; // Current font color
	
	// Setup stacks
	getStacksAt( 0 , curFont , curSize , curColor );
	
	
	// Determine x-coordinate of the string
	_coord currentX = getLineStart( dimensions );
	
	
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

void _singleLineGuiString::setCursorFromTouch( _rect dimensions , _coord cursorX , _coord cursorY )
{
	// Setup character positions
	_literal	textOrigin	= this->c_str();	
	_literal	curPtr		= textOrigin;
	_literal	endPtr		= textOrigin + this->length();
	_letterNum	numLetters	= 0;
	
	
	// Setup Stacks
	_fontStack		curFont; // Current font
	_fontSizeStack	curSize; // Current font size
	_fontColorStack	curColor; // Current font color
	getStacksAt( 0 , curFont , curSize , curColor );
	
	
	// Determine x-offset of the text
	_coord currentX = this->getLineStart( dimensions );
	
	
	// Iterate to to the cursor position
	while( curPtr < endPtr && cursorX > currentX )
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