#ifndef _WIN_T_DISPLAY_STRING_
#define _WIN_T_DISPLAY_STRING_

#include <_type/type.h>
#include <_type/type.font.h>
#include <_type/type.bitmap.port.h>
#include <_type/type.uniqueptr.h>
#include <stack>

using _lineNumber		= _u32;
using _letterNum		= size_t;

class _guiString : public wstring
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
		_guiString( wstring text , _fontHandle font , _color fontColor , _fontSize fontSize = 0 ) :
			wstring( move(text) )		, font( font )				, fontSize( fontSize )		, fontColor( fontColor )
			, selection( nullptr )		, align( _align::left )		, vAlign( _valign::top )	, cursor( 0 )
			, needsRefreshFlag( false )	, needsUpdateFlag( true )	, displaySelection( false )	, displayCursor( false )
		{}
		
		//! Override the current value of this formatString
		_guiString& operator=( const wstring& str ){
			wstring::operator=( str );
			this->needsUpdateFlag = true;
			return *this;
		}
		_guiString& operator=( wstring&& str ){
			wstring::operator=( move(str) );
			this->needsUpdateFlag = true;
			return *this;
		}
		
		///////////////////////////
		// CONCERNING APPEARENCE //
		///////////////////////////
		
		/**
		 * Print the formatted Text in the nth line to a bitmap at the specified position
		 * @param bmpPort BitmapPort reference to draw to
		 */
		virtual void	drawTo( _rect guiStringDimensions , _bitmapPort& port ) = 0;
		
		//! Get the [X-Coordinate,width] of the specific letter
		virtual _2s16	getXMetricsOfLetter( _rect guiStringDimensions , _letterNum letterNumber ) const = 0;
		
		//! Get over all Text Height
		virtual _length	getTextHeight() const = 0;
		
		//! Get the number of pixels the text would need to be displayed best
		virtual _length	getPreferredTextWidth() const {
			return 0;
		}
		
		/**
		 * Parses the whole text and wraps lines if needed, starting at the specified position 
		 * @param start The position from that on to be updated
		 */
		virtual void	update( _rect guiStringDimensions , _u32 start = 0 ) = 0;
		
		/**
		 * Get the character index after which to display a cursor,
		 * when the mouse is pressed at the specified position
		 */
		void			setCursorFromTouch( _rect guiStringDimensions , _coord cursorX , _coord cursorY );
		
		/**
		 * This method should be called, when the dimensions of the guistring change
		 * It will automatically update the wrapping of the text
		 */
		void			indicateNewDimensions(){
			this->needsUpdateFlag = true;
		}
		
		//! Get the number of pixels from the top of the guistring to the start of the text
		_length			getOffsetY( _rect guiStringDimensions ) const ;
		
		
		////////////////////////
		// CONCERNING INDICES //
		////////////////////////
		
		/**
		 * Get Cursor (number of letters after which to display the cursor)
		 * @return number of letters
		 */
		_letterNum		getCursor() const { return this->cursor; }
		
		//! Set the gui string's current cursor
		void			setCursor( _letterNum letterIndex ){
			letterIndex = min( letterIndex , length() );
			this->needsRefreshFlag |= this->displayCursor && letterIndex != this->cursor;
			this->cursor = letterIndex;
		}
		
		/**
		 * Increase/Decrease the cursor by one letter
		 * @return The updated cursor
		 */
		_letterNum		moveCursor( bool increase ){
			if( increase || this->cursor > 0 )
				this->setCursor( this->cursor + ( increase ? 1 : -1 ) );
			return this->cursor;
		}
		
		/**
		 * Insert text at the specified character index.
		 * @param text The text to insert.
		 * @param index The index to insert the character after
		 */
		void			insert( _letterNum position , wstring text );
		void			insert( _letterNum position , _wchar ch , _length n = 1 ){
			this->insert( position , wstring( n , ch ) );
		}
		
		/**
		 * Remove all characters from the string from the start index onwards.
		 * @param startIndex The char index to start removing from.
		 * @param count The number of chars to remove.
		 */
		void			remove( _letterNum letterIndex , _letterNum letterCount = 1 );
		
		
		///////////////////////
		// SETTERS & GETTERS //
		///////////////////////
		
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
		void		setFont( _fontHandle font ){
			this->needsUpdateFlag |= this->font != font;
			this->font = font;
		}
		
		//! Set initial size of the used font
		void		setFontSize( _fontSize fontSize ){
			this->needsUpdateFlag |= this->fontSize != fontSize;
			this->fontSize = fontSize;
		}
		
		//! Set initial size of the used font
		void		setFontColor( _color fontColor ){
			this->needsUpdateFlag |= this->fontColor != fontColor && !this->empty();
			this->fontColor = fontColor;
		}
		
		//! Get the used font
		_fontHandle	getFont() const { return this->font; }
		
		//! Get the used font size
		_fontSize	getFontSize() const { return this->fontSize; }
		
		//! Get the used font color
		_color		getFontColor() const { return this->fontColor; }
		
		//! Check whether the appearence of the text would have changed
		bool		needsRefresh() const { return this->needsRefreshFlag || this->needsUpdateFlag; }
		
		//! Check whether the _guiString wants to rearrange the lines inside
		bool		needsUpdate() const { return this->needsUpdateFlag; }
		
		//! Tell the _guiString that its appearence is on the latest state
		void		resetRefreshFlag(){ this->needsRefreshFlag = false; }
		
		//! Enable, whether the cursor should be displayed or not
		void		setCursorEnabled( bool enabled ){
			this->needsRefreshFlag |= this->displayCursor != enabled;
			this->displayCursor = enabled;
		}
		
		//! The Type of a selection
		class _strRange{
			public:
			_u32 start;
			_u32 length;
		};
	
	protected:
		
		//! Font to be used for output
		_fontHandle				font;
		_fontSize				fontSize;
		_color					fontColor;
		
		//! Structure holding the selection attributes if some text is selected
		_uniquePtr<_strRange>	selection;
		
		//! Text-Alignment
		_align					align;
		_valign					vAlign;
		
		//! Flag indicating whether the rendering of the _text-object would have changed
		struct{
			_u32				cursor;
			bool				needsRefreshFlag : 1;
			bool				needsUpdateFlag : 1;
			bool				displaySelection : 1;
			bool				displayCursor : 1;
		}PACKED;
};

#endif