#include <_type/type.guistring.h>

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



void _guiString::remove( _letterNum letterIndex , _letterNum letterCount )
{
	if( !letterCount )
		return;
	
	_u32 startIndex = this->getNumBytesFromNumLetters( letterIndex );
	_u32 numBytes = this->getNumBytesFromNumLetters( letterCount , startIndex );
	
	this->erase( startIndex , numBytes );
	this->needsUpdateFlag = true;
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



_length _guiString::getOffsetY( _rect dimensions ) const 
{
	if( this->vAlign == _valign::top )
		return 0;
	else{
		_length overAllHeight = this->getTextHeight();
		if( this->vAlign == _valign::middle )
			return ( dimensions.height - overAllHeight - 1 ) >> 1;
		else // _valign::bottom
			return dimensions.height - overAllHeight;
	}
}



void _guiString::insert( _letterNum letterIndex , string text )
{
	this->needsUpdateFlag |= !text.empty();
	string::insert( this->getNumBytesFromNumLetters( letterIndex ) , move(text) );
}



_letterNum _guiString::getStacksAt( _u32 index , _fontStack& fontStack , _fontSizeStack& fontSizeStack , _fontColorStack& fontColorStack ) const 
{
	// Reset Stacks
	fontStack = _fontStack();
	fontSizeStack = _fontSizeStack();
	fontColorStack = _fontColorStack();
	
	// Setup Stacks
	fontStack.push( this->font );
	fontSizeStack.push( this->fontSize );
	fontColorStack.push( this->fontColor );
	
	// Setup end position
	_literal curPtr = this->c_str();
	_literal end = curPtr + min<_u32>( index , this->length() );
	
	// Counter
	_letterNum numLetters = 0;
	
	// Iterate up to the line we are interested in
	while( curPtr < end && index ){
		numLetters += _guiString::processChar( curPtr , fontStack , fontSizeStack , fontColorStack );
		curPtr++;
	}
	
	return numLetters;
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