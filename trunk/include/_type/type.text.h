#ifndef _WIN_T_TEXT_
#define _WIN_T_TEXT_

#include "_type/type.h"
#include "_type/type.font.h"

#include <vector>

class _text
{
	private:
		
		//! Text
		string			text;
		
		//! Font to be used for output
		_font*			font;
		_u8				fontSize;
		
		//! Array containing start indexes of each wrapped line
		vector<_u32>	linePositions;
		
		//! Width in pixels available to the text
		_length			width;
		
		//! Flag indicating whether the rendering of the _text-object would have changed
		bool			needsRefreshFlag;
		
		/**
		 * Wrap all of the text.
		 */
		void wrap();
		
		
	public:
		
		/**
		 * Constructor.
		 * @param font The font to use for this text object.
		 * @param fontSize The size of the font to use for this text object.
		 * @param text A string that this text object should wrap around.
		 * @param width The pixel width at which the text should wrap.
		 */
		_text( _font* font , _u8 fontSize , _length width , string text ) :
			text( text )
			, font( font )
			, fontSize( fontSize )
			, width( width )
		{
			this->wrap();
		}
		
		/**
		 * Set Font to use
		 * @param font a pointer to the font that should be used
		 */
		void setFont( _font* font , _u8 fontSize = 0 )
		{
			if( this->font == font && ( this->fontSize == fontSize || !fontSize ) )
				return;
			this->font = font;
			fontSize && ( this->fontSize = fontSize ); // only set fontSize if not zero
			this->wrap();
		}
		
		/**
		 * Set Size of the used font
		 * @param font a pointer to the font that should be used
		 */
		void setFontSize( _u8 fontSize )
		{
			if( this->fontSize == fontSize )
				return;
			this->fontSize = fontSize;
			this->wrap();
		}
		
		/**
		 * Get the used font
		 */
		_font* getFont(){ return this->font; }
		
		
		/**
		 * Get the used font size
		 */
		_u8 getFontSize(){ return this->fontSize; }
		
		
		/**
		 * Set the text in the string
		 * @param text Char array to use as the new data for this string.
		 */
		void setText( string text );
		
		
		//! Check whether the illustration of the text would have changed
		bool needsRefresh(){ return this->needsRefreshFlag; }
		
		//! Reset the flag telling whether the illustration of the text needs a refresh
		void resetRefreshFlag(){ this->needsRefreshFlag = false; }
		
		
		/**
		 * Get the text in the string
		 */
		string getText(){ return this->text; }
		
		
		/**
		 * Insert text at the specified character index.
		 * @param text The text to insert.
		 * @param index The char index to insert at.
		 */
		void insert( _length index , string text )
		{
			if( text.empty() )
				return;
			this->text.insert( index , text );
			this->wrap();
		}
		void insert( _length index , char ch , _length n = 1 )
		{
			if( !ch )
				return;
			this->text.insert( index , n , ch );
			this->wrap();
		}
		
		
		/**
		 * Remove all characters from the string from the start index onwards.
		 * @param startIndex The char index to start removing from.
		 * @param count The number of chars to remove.
		 */
		void remove( const _u32 startIndex , const _u32 count = 1 )
		{
			if( !count )
				return;
			this->text.erase( startIndex , count );
			this->wrap();
		}
		
		
		/**
		 * Sets the pixel width of the text; text wider than 
		 * this will automatically wrap.
		 * @param width Maximum pixel width of the text.
		 */
		void setWidth( _length width )
		{
			if( this->width == width )
				return;
			this->width = width;
			this->wrap();
		}
		
		/**
		 * Get the total number of lines in the text.
		 * @return The line count.
		 */
		_u32 getLineCount() const { return this->linePositions.size() - 1; };
		
		
		/**
		 * Get the line the supplied character at 'index' is in
		 */
		_u32 getLineContainingCharIndex( _u32 index )
		{
			if( index == this->text.length() )
				return this->linePositions.size()-2;
			return count_if( this->linePositions.begin() , this->linePositions.end() , [=]( _u32 val ){ return val <= index; } ) - 1;
		}
		
		/**
		 * Get the startIndex of a specific line
		 */
		_u32 getLineStart( _u32 lineNo )
		{
			if( lineNo == this->getLineCount() )
				return this->text.length() + 1;
			return this->linePositions[lineNo];
		}
		
		/**
		 * Get The text that is in the supplied line number
		 * @param lineNo The number of the line the content should be retrieved
		 * @return The text in the upplied line or
		 * "" if that line is either empty or if 'lineNo' is invalid
		**/
		string getLineContent( _u32 lineNo )
		{
			int start = this->linePositions[lineNo];
			int end = this->linePositions[lineNo+1];
			return this->text.substr(
				start // startIndex
				, end - start // length
			);
		}
};

#endif