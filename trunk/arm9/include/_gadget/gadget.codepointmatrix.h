// Check if already included
#ifndef _WIN_G_CODEPOINTMATRIX_
#define _WIN_G_CODEPOINTMATRIX_

#include <_type/type.gadget.h>
#include <_type/type.font.h>

struct _codepointMatrixEntryEx{
	_wchar	codepoint;
	_color	foreground;
	_color	background;
};
typedef _wchar _codepointMatrixEntry;

class _codepointMatrix : public _gadget
{
	private:
		
		//! String to be displayed
		union{
			_codepointMatrixEntry*		basic;
			_codepointMatrixEntryEx*	extended;
		}data;
		const bool	useExtended : 1;
		_u16		numRows;
		_u16		numCols;
		
		//! Tile Dimensions
		_u8			tileWidth;
		_u8			tileHeight;
		
		//! Temporary Settings
		_color		curForeground;
		_color		curBackground;
		_pos		cursor;
		
		//! Visual attributes
		_fontHandle	font;
		_fontSize	fontSize;
		
		//! Various Event Handlers
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn updateHandler( _event );
		static _callbackReturn keyHandler( _event );
		
		//! Returns a position that is guaranteed to be inside the matrix
		bool validatePosition( _pos& pos ){
			bool ret = false;
			if( pos.first >= this->numCols )
				pos.first = this->numCols - 1;
			else if( pos.first < 0 )
				pos.first = this->numCols - 1;
			else
				ret = true;
			if( pos.second >= this->numRows )
				pos.second = this->numRows - 1;
			else if( pos.second < 0 )
				pos.second = 0;
			else
				return ret;
			return false;
		}
		
		//! Resize the internal matrix
		void resizeMatrixTo( _u16 numCols , _u16 numRows );
		
		//! Moves the matrix data up one line
		void moveUpCanvas();
		
		//! Redraws a specific tile
		void redrawTile( _pos tile );
		
		//! Get the maximum used character position the specified line
		_length getMaximumUsedTileInLine( _length lineNumber ) const ;
		
		//! Get the following tile
		_pos getAdjacentTile( _pos pos , bool next );
		
		//! Set a Tiles value
		void setTile( _pos pos , _wchar value , _color fg , _color bg ){
			if( this->useExtended )
				this->data.extended[ this->numCols * pos.second + pos.first ] = { 0 , fg , bg };
			else
				this->data.basic[ this->numCols * pos.second + pos.first ] = 0;
		}
		
		//! Copy Tile from source to dest
		void copyTile( _pos dest , _pos source )
		{
			if( !validatePosition( dest ) )
				return;
			if( !validatePosition( source ) )
				setTile( dest , '\0' , this->curForeground , this->curBackground );
			else if( this->useExtended )
				this->data.extended[ this->numCols * dest.second + dest.first ] = this->data.extended[ this->numCols * source.second + source.first ];
			else
				this->data.basic[ this->numCols * dest.second + dest.first ] = this->data.basic[ this->numCols * source.second + source.first ];
		}
		
		
	public:
		
		//! Set background color to use when writing a character
		void setBgColor( _color col ){ this->curBackground = col; }
		
		//! Set background color to use when writing a character
		void setFgColor( _color col ){ this->curForeground = col; }
		
		//! Set the cursor where to write
		void setCursor( _pos pos );
		
		//! Increase or decrease the current cursor position
		void moveCursor( bool increase );
		
		//! Get the Current cursor position
		_pos getCursor() const { return this->cursor; }
		
		//! Set Text Font
		void setFont( _fontHandle ft );
		
		//! Get Text Font
		_fontHandle getFont() const { return this->font; }
		
		//! Get Text FontSize
		_fontSize getFontSize() const { return this->fontSize; }
		
		//! Get Text FontSize
		void setFontSize( _fontSize size );
		
		//! Set Tile Width
		void setTileWidth( _u8 value );
		
		//! Set Tile Height
		void setTileHeight( _u8 value );
		
		//! Get Tile Width
		_u8 getTileWidth() const { return this->tileWidth; }
		
		//! Get Tile Height
		_u8 getTileHeight() const { return this->tileHeight; }
		
		//! Get Tile Metrics
		_rect getTileMetrics( _pos pos ){
			return _rect(
				pos.first * this->tileWidth
				, pos.second * this->tileHeight
				, this->tileWidth
				, this->tileHeight
			);
		}
		
		//! Get the codepoint at the specified tile position
		_wchar getCodepointAt( _pos position )
		{
			if( !validatePosition( position ) )
				return '\0';
			
			if( this->useExtended )
				return this->data.extended[ position.second * this->numCols + position.first ].codepoint;
			else
				return this->data.basic[ position.second * this->numCols + position.first ];
		}
		
		//! Print a whole string onto the canvas starting at the current cursor position
		void print( wstring str ){
			for( _wchar ch : str )
				print( ch );
		}
		
		//! Print a character at the current cursor Position
		void print( _wchar ch );
		
		//! Removes the character at the current position
		//! and moves the cursor one place left. This methods also calls remove()
		void backspace();
		
		//! Deletes the contents of the cursor-tile and moves everything
		//! behind that one one position backwards
		void remove();
		
		//! Constructor
		_codepointMatrix( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height
					, bool extendedMode = false , _style&& style = _style() );
		
		//! Destructor
		~_codepointMatrix(){
			if( this->useExtended )
				delete[] this->data.extended;
			else
				delete[] this->data.basic;
			this->data.basic = nullptr;
		}
};

#endif