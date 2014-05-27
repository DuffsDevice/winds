#ifndef _WIN_T_GUISTRING_SINGLELINE_
#define _WIN_T_GUISTRING_SINGLELINE_

#include "_type/type.guistring.h"

class _singleLineGuiString : public _guiString
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
		_singleLineGuiString( string text , _fontHandle font , _color fontColor , _u8 fontSize = 0 ) :
			_guiString( move(text) , font , fontColor , fontSize )
			, ellipsis( -1 )
			, omitStart( -1 )
			, omitEnd( -1 )
			, lineHeight( 0 )
			, lineWidth( 0 )
		{}
		
		//! Override the current value of this formatString
		_singleLineGuiString& operator=( const string& str ){
			_guiString::operator=( str );
			return *this;
		}
		_singleLineGuiString& operator=( string&& str ){
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
		
		//! Get the [X-Coordinate,width] of the specific letter
		_2s32		getXMetricsOfLetter( _rect guiStringDimensions , _letterNum letterNumber ) const ;
		
		//! Get the preferred Width of the gui-string
		_length		getTextWidth() const { return this->lineWidth; }
		
		//! Preferred Number of Pixels
		_length		getPreferredTextWidth() const ;
		
		//! Get over all Text Height
		_length		getTextHeight() const { return this->lineHeight; }
		
		//! Set Type of Ellipsis ( '-1' = no ellipsis ; '>= 0' = N letters before end
		void		setEllipsis( _s8 value ){
			this->needsUpdateFlag |= this->ellipsis = value;
			this->ellipsis = value;
		}
		
		//! Get type of ellipsis
		_s8			getEllipsis() const { return this->ellipsis; }
		
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
		
	private:
		
		_length		getLineStart( _rect guiStringDimensions ) const ;
		
		_s8			ellipsis; // -1 = No Ellipsis ; Else = Auto Ellipsis N characters before the end
		_letterNum	omitStart;
		_letterNum	omitEnd;
		_length		lineHeight;
		_length		lineWidth;
};

#endif // #ifndef _WIN_T_GUISTRING_SINGLELINE_