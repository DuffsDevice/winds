#ifndef _WIN_T_DISPLAY_STRING_
#define _WIN_T_DISPLAY_STRING_

#include "_type/type.h"
#include "_type/type.font.h"
#include "_type/type.bitmap.port.h"
#include "_type/type.flexptr.h"
#include <stack>

using _fontStack		= std::stack<const _font*>;
using _fontSizeStack	= std::stack<_u8>;
using _fontColorStack	= std::stack<_color>;
using _lineNumber		= _u32;
using _letterNum		= size_t;

class _guiString : public string
{
	public:
		
		/////////////////////////
		// STANDARD C++ THINGS //
		/////////////////////////
		
		/**
		 * Constructor
		 * @param font The font to use for this text object.
		 * @param fontSize The size of the font to use for this text object.
		 * @param text A string that this text object should wrap around.
		 * @param dimensions The Size and Position of the gui string relative to the gadget it is displayed on
		 */
		_guiString( string text , _rect dimensions , const _font* font , _color fontColor , _u8 fontSize = 0 ) :
			string( move(text) )		, font( font )				, fontSize( fontSize )		, fontColor( fontColor )
			, selection( nullptr )		, dimensions( dimensions )	, align( _align::left )		, vAlign( _valign::top )
			, cursor( 0 )				, tmpCursorX( -1 )			, needsRefreshFlag( false )	, needsUpdateFlag( true )
			, displaySelection( false )	, displayCursor( false )	, allowLineBreak( true )	, allowFontChange( true )
			, allowSizeChange( true )	, allowColorChange( true )
		{}
		
		//! Override the current value of this formatString
		_guiString& operator=( const string& str ){
			string::operator=( str );
			this->needsUpdateFlag = true;
			return *this;
		}
		_guiString& operator=( string&& str ){
			string::operator=( move(str) );
			this->needsUpdateFlag = true;
			return *this;
		}
		
		///////////////////////////////////
		// METHODS CONCERNING APPEARENCE //
		///////////////////////////////////
		
		/**
		 * Sets the position and size of the gui string. This will automatically
		 * update the wrapping of the text, if the width of the gui string changes.
		 * @param newDimensions Dew dimensions of this gui string
		 */
		void		setDimensions( _rect newDimensions ){
			this->needsUpdateFlag |= this->dimensions != newDimensions;
			this->dimensions = newDimensions;
		}
		
		//! Get Current Dimensions of the _guiString
		_rect		getDimensions() const { return this->dimensions; }
		
		/**
		 * Print the formatted Text in the nth line to a bitmap at the specified position
		 * @param bmpPort BitmapPort reference to draw to
		 */
		void		drawTo( _bitmapPort& port );
		
		//! Get the [Y-Coordinate,height] of the supplied line
		_2s32		getYMetricsOfLine( _lineNumber lineNumber ) const ;
		
		//! Get the [X-Coordinate,width] of the supplied line
		_2s32		getXMetricsOfLine( _lineNumber lineNumber ) const ;
		
		//! Get the [X-Coordinate,width] of the specific letter
		_2s32		getXMetricsOfLetter( _letterNum letterNumber ) const ;
		
		
		//////////////////////////////////////////
		// METHODS CONCERNING CHARACTER INDICES //
		//////////////////////////////////////////
		
		/**
		 * Parses the whole text and wraps lines if needed, starting at the specified position 
		 * @param start The position from that on to be updated
		 */
		void		update( _u32 start = 0 );
		
		/**
		 * Get the character index after which to display a cursor,
		 * when the mouse is pressed at the specified position
		 */
		void		setCursorFromTouch( _coord cursorX , _coord cursorY );
		
		/**
		 * Get Cursor (number of letters after which to display the cursor)
		 * @return number of letters
		 */
		_letterNum	getCursor() const { return this->cursor; }
		
		//! Set the gui string's current cursor
		void		setCursor( _letterNum letterIndex ){
			this->needsRefreshFlag |= this->displayCursor && letterIndex != this->cursor;
			if( letterIndex > this->cursor )
				letterIndex = min( letterIndex , this->getNumLetters() );
			this->cursor = letterIndex;
			this->tmpCursorX = -1;
		}
		
		/**
		 * Increase/Decrease the cursor by one letter
		 * @return The updated cursor
		 */
		_letterNum	moveCursor( bool increase ){
			if( increase || this->cursor > 0 )
				this->setCursor( this->cursor + ( increase ? 1 : -1 ) );
			this->tmpCursorX = -1;
			return this->cursor;
		}
		
		/**
		 * Increase/Decrease the cursor by one line
		 * @return The updated cursor
		 */
		_lineNumber	moveCursorByLine( bool increase );
		
		/**
		 * Insert text at the specified character index.
		 * @param text The text to insert.
		 * @param index The index to insert the character after
		 */
		void		insert( _letterNum position , string text );
		void		insert( _letterNum position , _char ch , _length n = 1 ){
			this->insert( position , string(n,ch) );
		}
		
		/**
		 * Remove all characters from the string from the start index onwards.
		 * @param startIndex The char index to start removing from.
		 * @param count The number of chars to remove.
		 */
		void		remove( _letterNum letterIndex , _letterNum letterCount = 1 );
		
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
		string		getLineContent( _lineNumber lineNo ) const ;
		
		//! Get over all Text Height
		_length		getTextHeight() const ;
		
		//! Get the number of characters (bytes) needed to display 'letterNum' letters
		_u32		getNumBytesFromNumLetters( _letterNum letterNum , _letterNum offsetLetter = 0 ) const ;
		
		/**
		 * Get the number of letters displayed after processing
		 * 'charNum' characters (bytes) starting at 'offset' bytes
		 * @param numBytes the number of bytes of which we want to know the number of letters they build
		 * @param offset The position where we start counting
		 */
		_letterNum	getNumLettersFromNumBytes( _u32 numBytes , _u32 offset = 0 ) const ;
		
		//! Get Numebr of letters in the string
		_letterNum	getNumLetters() const ;
		
		
		///////////////////////
		// GETTERS & SETTERS //
		///////////////////////
		
		//! Get the number of pixels from the top of the guistring to the start of the text
		_length		getOffsetY() const ;
		
		//! Set Horizontal Alignment of gui string
		void		setAlign( _align value ){
			this->needsRefreshFlag |= this->align != value;
			this->align = value;
		}
		
		//! Get Horizontal Alignment of gui string
		_align		getAlign() const { return this->align; }
		
		//! Set Vertical Alignment of gui string
		void		setVAlign( _valign value ){
			this->needsRefreshFlag |= this->vAlign != value;
			this->vAlign = value;
		}
		
		//! Get Vertical Alignment of gui string
		_valign		getVAlign() const { return this->vAlign; }
		
		//! Is Cursor enabled
		bool		isCursorEnabled() const { return this->displayCursor; }
		
		//! Set initial font to use
		void		setFont( const _font* font ){
			this->needsUpdateFlag |= this->font != font;
			this->font = font;
		}
		
		//! Set initial size of the used font
		void		setFontSize( _u8 fontSize ){
			this->needsUpdateFlag |= this->fontSize != fontSize;
			this->fontSize = fontSize;
		}
		
		//! Set initial size of the used font
		void		setFontColor( _color fontColor ){
			this->needsUpdateFlag |= this->fontColor != fontColor && !this->empty();
			this->fontColor = fontColor;
		}
		
		//! Get the used font
		_fontPtr	getFont() const { return this->font; }
		
		//! Get the used font size
		_u8			getFontSize() const { return this->fontSize; }
		
		//! Get the used font color
		_color		getFontColor() const { return this->fontColor; }
		
		//! Check whether the appearence of the text would have changed
		bool		needsRefresh(){ return this->needsRefreshFlag || this->needsUpdateFlag; }
		
		//! Check whether the _guiString wants to rearrange the lines inside
		bool		needsUpdate(){ return this->needsUpdateFlag; }
		
		//! Tell the _guiString that its appearence is on the latest state
		void		resetRefreshFlag(){ this->needsRefreshFlag = false; }
		
		//! Enable, whether the cursor should be displayed or not
		void		setCursorEnabled( bool enabled ){
			this->needsRefreshFlag |= this->displayCursor != enabled;
			this->displayCursor = enabled;
		}
		
		//! Enable or disable line breaks
		void		setLineBreaksEnabled( bool enabled ){
			this->needsUpdateFlag |= this->allowLineBreak != enabled;
			this->allowLineBreak = enabled;
		}
		
		//! Enable or disable font changes
		void		setFontChangeEnabled( bool enabled ){
			this->needsUpdateFlag |= this->allowFontChange != enabled;
			this->allowFontChange = enabled;
		}
		
		//! Enable or disable font size changes
		void		setFontSizeChangeEnabled( bool enabled ){
			this->needsUpdateFlag |= this->allowSizeChange != enabled;
			this->allowSizeChange = enabled;
		}
		
		//! Enable or disable font color changes
		void		setFontColorChangeEnabled( bool enabled ){
			this->needsRefreshFlag |= this->allowColorChange != enabled;
			this->allowColorChange = enabled;
		}
		
		/////////////////////////////////////////
		// METHODS FOR CHANGE ESCAPE SEQUENCES //
		/////////////////////////////////////////
		
		//! Generates an escape sequence that will change the displayed fontSize in a string
		static string sizeChange( _u8 newSize );
		
		//! Generates an escape sequence that will change the displayed font color
		static string colorChange( _color color );
		
		//! Generates an escape sequence that will change the used font
		static string fontChange( const _font* ft );
		
		
		//////////////////////////////////////////
		// METHODS FOR RESTORE ESCAPE SEQUENCES //
		//////////////////////////////////////////
		
		//! Generates an escape sequence that will restore the previously used font
		static string fontRestore(){ _char tmp[2] = { (_char)escapeChar::fontRestore , 0 }; return tmp; }
		
		//! Generates an escape sequence that will restore the previously used font size
		static string fontSizeRestore(){ _char tmp[2] = { (_char)escapeChar::sizeRestore , 0 }; return tmp; }
		
		//! Generates an escape sequence that will restore the previously used font color
		static string fontColorRestore(){ _char tmp[2] = { (_char)escapeChar::colorRestore , 0 }; return tmp; }
		
	
	private:
		
		struct _strRange{
			_u32 start;
			_u32 length;
		};
		
		struct _lineStart{
			_u32	charIndex;
			_length	lineWidth;
			_length	lineHeight;
			bool	isSyllableBreak;
		};
		
		//! Font to be used for output
		const _font*			font;
		_u8						fontSize;
		_color					fontColor;
		
		//! Array containing start indexes of each wrapped line
		_vector<_lineStart>		lineStarts;
		flex_ptr<_strRange>		selection;
		
		//! Area available to the text
		_rect					dimensions;
		_align					align;
		_valign					vAlign;
		
		//! Flag indicating whether the rendering of the _text-object would have changed
		struct{
			_u32				cursor;
			_s32				tmpCursorX;
			bool				needsRefreshFlag : 1;
			bool				needsUpdateFlag : 1;
			bool				displaySelection : 1;
			bool				displayCursor : 1;
			bool				allowLineBreak : 1;
			bool				allowFontChange : 1;
			bool				allowSizeChange : 1;
			bool				allowColorChange : 1;
		} PACKED ;
		
		/**
		 * Appends a line break to the list of lineStarts at the specified position
		 * @param byteIndex			Byte index after which to start a new line
		 * @param lineWidth			The total width of the line before the line break (including the optional hyphen)
		 * @param lineHeight		The total height in pixels of the previous line
		 * @param dueToHyphenation	Whether the line break was added due to splitting a word in two parts
		 */
		void		pushBackLineBreak( _u32 byteIndex , _length lineWidth , _length maxLineHeight , bool dueToHyphenation = false );
		
		/**
		 * Checks, if the current character in the supplied string is an escape-char
		 * and, if so, modifies the supplied font/size/color-Stack. The caller has nothing
		 * to do but loop through the whole text and increment the string ptr every round
		 * @return Whether or not the current character is not an escape code and can be processed
		 */
		bool		processChar( _literal& str , _fontStack& fontStack , _fontSizeStack& fontSizeStack , _fontColorStack& fontColorStack ) const ;
		
		//! Get the font-stacks-state at the supplied byte position and returns how many letters that were
		_letterNum	getStacksAt( _u32 index , _fontStack& fontStack , _fontSizeStack& fontSizeStack , _fontColorStack& fontColorStack ) const ;
		
		//! Predefined Escape characters that introduce an escape sequence
		enum class escapeChar : _char{
			fontChange		= 22,
			colorChange		= 23,
			sizeChange		= 25,
			fontRestore		= 26,
			sizeRestore		= 29,
			colorRestore	= 30
		};
		
		//! Used between the parts of a compound word or name or between the syllables of a word
		//! especially when divided at the end of a line of text
		static constexpr _char breakLineLetter = '-';
};

#endif