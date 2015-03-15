#ifndef _WIN_T_GUISTRING_MULTILINE_
#define _WIN_T_GUISTRING_MULTILINE_

#include <_type/type.guistring.h>

class _multiLineGuiString : public _guiString
{
	public:
		
		/////////////////////////
		// STANDARD C++ THINGS //
		/////////////////////////
		
		/**
		 * Constructor
		 * @param text A string that this text object should wrap around.
		 * @param dimensions The Size and Position of the gui string relative to the gadget it is displayed on
		 * @param font The font to use for this text object
		 * @param fontColor The color of the font to use for this text object
		 * @param fontSize The size of the font to use for this text object
		 */
		_multiLineGuiString( wstring text , _fontHandle font , _color fontColor , _fontSize fontSize = 0 ) :
			_guiString( move(text) , font , fontColor , fontSize )
			, tmpCursorX( -1 )
		{}
		
		//! Override the current value of this formatString
		_multiLineGuiString& operator=( const wstring& str ){
			_guiString::operator=( str );
			return *this;
		}
		_multiLineGuiString& operator=( wstring&& str ){
			_guiString::operator=( move(str) );
			return *this;
		}
		
		///////////////////////////////////
		// METHODS CONCERNING APPEARENCE //
		///////////////////////////////////
		
		/**
		 * Print the formatted Text in the nth line to a bitmap at the specified position
		 * @param bmpPort BitmapPort reference to draw to
		 */
		void		drawTo( _rect guiStringDimensions , _bitmapPort& port );
		
		//! Get the [Y-Coordinate,height] of the supplied line
		_2s16		getYMetricsOfLine( _rect guiStringDimensions , _lineNumber lineNumber ) const ;
		
		//! Get the [X-Coordinate,width] of the supplied line
		_2s16		getXMetricsOfLine( _rect guiStringDimensions , _lineNumber lineNumber ) const ;
		
		//! Get the [X-Coordinate,width] of the specific letter
		_2s16		getXMetricsOfLetter( _rect guiStringDimensions , _letterNum letterNumber ) const ;
		
		
		//////////////////////////////////////////
		// METHODS CONCERNING CHARACTER INDICES //
		//////////////////////////////////////////
		
		/**
		 * Parses the whole text and wraps lines if needed, starting at the specified position 
		 * @param start The position from that on to be updated
		 */
		void		update( _rect guiStringDimensions , _u32 start = 0 );
		
		/**
		 * Get the character index after which to display a cursor,
		 * when the mouse is pressed at the specified position
		 */
		void		setCursorFromTouch( _rect guiStringDimensions , _coord cursorX , _coord cursorY );
		
		//! Set the gui string's current cursor
		void			setCursor( _letterNum letterIndex ){
			_guiString::setCursor( letterIndex );
			this->tmpCursorX = -1;
		}
		
		/**
		 * Increase/Decrease the cursor by one letter
		 * @return The updated cursor
		 */
		_letterNum		moveCursor( bool increase ){
			this->tmpCursorX = -1;
			return _guiString::moveCursor( increase );
		}
		
		/**
		 * Increase/Decrease the cursor by one line
		 * @return The updated cursor
		 */
		_lineNumber	moveCursorByLine( _rect guiStringDimensions , bool increase );
		
		//! Get the total number of lines in the text
		_u32		getLineCount() const { return max<_u32>( 1 , this->lineStarts.size() ) - 1; } // -1 for the start index that gets pushed onto the list
		
		//! Get the line number the supplied byte 'index' is in
		_u32		getLineContainingIndex( _u32 index ) const ;
		
		//! Get the byte index of the first character in a specific line
		_u32		getLineStart( _lineNumber lineNo ) const { return this->lineStarts[ min( lineNo , getLineCount() ) ].charIndex; }
		
		//! Get the byte index of the last character in a specific line
		_u32		getLineEnd( _lineNumber lineNo ) const { return this->lineStarts[ min( lineNo , getLineCount() ) + 1 ].charIndex - 1; }
		
		/**
		 * Get The text that is in the supplied line number
		 * @param lineNo The number of the line the content should be retrieved
		 * @return The text in the upplied line or
		 * returns "" if that line is either empty or if 'lineNo' is invalid
		**/
		wstring		getLineContent( _lineNumber lineNo ) const ;
		
		//! Get over all Text Height
		_length		getTextHeight() const ;
		
		class _lineStart{
			public:
			_u32	charIndex;
			_length	lineWidth;
			_length	lineHeight;
			bool	isSyllableBreak;
		};
	
	private:
		
		//! Array containing start indexes of each wrapped line
		_vector<_lineStart>		lineStarts;
		
		//! Flag indicating whether the rendering of the _text-object would have changed
		_s32					tmpCursorX;
		
		/**
		 * Appends a line break to the list of lineStarts at the specified position
		 * @param byteIndex			Byte index after which to start a new line
		 * @param lineWidth			The total width of the line before the line break (including the optional hyphen)
		 * @param lineHeight		The total height in pixels of the previous line
		 * @param dueToHyphenation	Whether the line break was added due to splitting a word in two parts
		 */
		void		pushBackLineBreak( _u32 byteIndex , _length lineWidth , _length maxLineHeight , bool dueToHyphenation = false );
		
		//! Used between the parts of a compound word or name or between the syllables of a word
		//! especially when divided at the end of a line of text
		static constexpr _char breakLineLetter = '-';
};

#endif