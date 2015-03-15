#include <_type/type.guistring.singleline.h>
#include <cstdio>

_length _singleLineGuiString::getPreferredTextWidth() const
{
	// Counter variable for number of preferred pixels
	_length lineWidth = 0;
	
	for( _wchar ch : *this )
	{
		if( ch != '\n' && this->font->isCharSupported( ch ) )
		{
			// advance the current lineWidth by the width of the letter to display
			_length letterWidth	= this->font->getCharacterWidth( ch , this->fontSize );
			
			if( letterWidth )
				letterWidth		+= this->font->getLetterSpace( this->fontSize );
			
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
	_letterNum	overAllLetters = length();
	_letterNum	ellipsisLetter = overAllLetters - this->ellipsis;
	_letterNum	curLetter = 0;
	
	for( _wchar ch : *this )
	{
		curLetter++;
		
		// Check if we pass the ellipsis-letter before which we could have the ellipsis
		if( this->ellipsis >= 0 && curLetter == ellipsisLetter )
			lineWidthUntil = this->lineWidth;
		
		// Process current letter
		if( ch != '\n' && this->font->isCharSupported( ch ) )
		{
			// advance the current lineWidth by the width of the letter to display
			_length letterWidth	= this->font->getCharacterWidth( ch , this->fontSize );
			
			if( letterWidth ){
				lineHeight		= max( lineHeight , this->font->getHeight( this->fontSize ) ); // Increase line height if current font is bigger that before
				letterWidth		+= this->font->getLetterSpace( this->fontSize );
			}
			this->lineWidth += letterWidth;
		}
	}
	
	this->lineHeight = lineHeight;
	
	// Subtract letter spacing since we don't have it after the last character
	_length letterSpace = this->lineWidth > 0 ? this->font->getLetterSpace( this->fontSize ) : 0;
	this->lineWidth -= letterSpace;
	
	
	// Check if we even exceeded the available width
	if( this->lineWidth > dimensions.width && this->ellipsis >= 0 )
	{
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
		wstring::iterator iter = begin();
		curLetter = 0;
		
		if( currentWidth > ellipsisPartMaxWidth )
			curLetter = overAllLetters;
		
		if( currentWidth >= ellipsisPartMaxWidth )
			iter = end(); // Aborts the loop immediately
		
		// Iterate
		for( ; iter < end() ; iter++ )
		{
			_length letterWidth	= this->font->getCharacterWidth( *iter , this->fontSize );
			if( letterWidth )
				letterWidth += this->font->getLetterSpace( this->fontSize );
			
			// advance the current lineWidth by the width of the letter
			currentWidth += letterWidth;
			
			// Increase number of processed letters (letters before the ellipsis)
			curLetter++;
			
			// Abort since we exceeded the width we have available
			if( currentWidth > ellipsisPartMaxWidth )
				break;
		}
		
		// Subtract all not-displayed letters from the lineWidth
		for( iter++ ; iter < end() ; iter++ )
		{
			_length letterWidth	= this->font->getCharacterWidth( *iter , this->fontSize );
			if( letterWidth )
				letterWidth += this->font->getLetterSpace( this->fontSize );
			
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
	wstring::iterator iter		= begin();
	wstring::iterator end		= this->end();
	wstring::iterator cursor	= this->displayCursor ? get( this->cursor ) : wstring::iterator( wstring::npos , *this );
	wstring::iterator omitStart	= get( this->omitStart );
	wstring::iterator omitEnd	= get( this->omitEnd );
	
	//
	// Compute drawing coordinates
	//
	_coord currentX = this->getLineStart( dimensions );
	_coord currentY	= this->getOffsetY( dimensions ) + dimensions.y;
	
	
	//
	// Iterate over whole text
	//
	
	// Iterate to to the ellipsis position
	for( ; iter < end && iter < omitStart ; iter++ )
	{
		// Display Cursor?
		if( iter == cursor )
			port.drawVerticalLine( currentX - 1 , currentY , this->font->getHeight( this->fontSize ) , _color::red );
		
		// Draw letter
		_length charWidth = port.drawChar( currentX , currentY , this->font , *iter , this->fontColor , this->fontSize );
		
		if( charWidth ){
			charWidth += this->font->getLetterSpace( this->fontSize );
			currentX += charWidth; // Advance X-Position
		}
	}
	
	if( iter < end )
	{
		// Draw ellipsis
		for( int i = 0 ; i < 3 ; i++ )
		{
			// Draw letter
			_length charWidth = port.drawChar( currentX , currentY , this->font , '.' , this->fontColor , this->fontSize );
			
			if( charWidth ){
				charWidth += this->font->getLetterSpace( this->fontSize );
				currentX += charWidth; // Advance X-Position
			}
		}
		
		// Iterate to end of omitted text
		for( ; iter < end && iter < omitEnd ; iter++ );
		
		// Draw the rest, if there is one
		for( ; iter < end ; iter++ )
		{
			// Display Cursor?
			if( iter == cursor )
				port.drawVerticalLine( currentX - 1 , currentY , this->font->getHeight( this->fontSize ) , _color::red );
			
			// Draw letter
			_length charWidth = port.drawChar( currentX , currentY , this->font , *iter , this->fontColor , this->fontSize );
			
			if( charWidth ){
				charWidth += this->font->getLetterSpace( this->fontSize );
				currentX += charWidth; // Advance X-Position
			}
		}
	}
	
	// Draw Cursor, if it is after the last character
	if( iter == cursor )
		port.drawVerticalLine( currentX - 1 , currentY , this->font->getHeight( this->fontSize ) , _color::red );
	
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

_2s16 _singleLineGuiString::getXMetricsOfLetter( _rect dimensions , _letterNum letterNumber ) const
{
	// Determine x-coordinate of the string
	_coord currentX = getLineStart( dimensions );
	
	if( letterNumber == 0 )
		return _2s16( currentX , 0 );
	
	// Iterate to to the letter we are looking for
	for( _wchar ch : *this )
	{
		// Compute the width of the current character
		_u32 charWidth = this->font->getCharacterWidth( ch );
		
		if( letterNumber-- <= 1 )
			return _2s16( currentX + charWidth , charWidth );
		
		// Advance current width
		if( charWidth ){
			charWidth += this->font->getLetterSpace( this->fontSize );
			currentX += charWidth;
		}
	}
	
	return _2s16( currentX , 0 );
}

void _singleLineGuiString::setCursorFromTouch( _rect dimensions , _coord cursorX , _coord cursorY )
{
	// Setup character positions
	wstring::iterator	iter	= this->begin();
	wstring::iterator	end		= this->end();
	
	
	// Determine x-offset of the text
	_coord currentX = this->getLineStart( dimensions );
	
	
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
		
		// Go to next character
		iter++;
	}
	
	this->setCursor( iter - begin() );
}