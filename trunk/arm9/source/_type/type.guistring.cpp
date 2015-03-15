#include <_type/type.guistring.h>


void _guiString::remove( _letterNum letterIndex , _letterNum letterCount )
{
	if( !letterCount )
		return;
	
	this->erase( letterIndex , letterCount );
	this->needsUpdateFlag = true;
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



void _guiString::insert( _letterNum letterIndex , wstring text )
{
	this->needsUpdateFlag |= !text.empty();
	wstring::insert( letterIndex , move(text) );
}