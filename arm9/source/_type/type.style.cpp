#include "_type/type.style.h"
#include "_type/type.tokenizer.h"
	
_style _style::storeHandle( void* data , _style attr ){
	_style g = _style( attr );
	g.data = data;
	return g;
}

_style _style::storeInt( _int data , _style attr ){
	_style g = _style( attr );
	g.val = data;
	return g;
}

_style _style::storeHost( _program* data , _style attr ){
	_style g = _style( attr );
	g.host = data;
	return g;
}

void applyString2style( _style& style , string input )
{
	string line;
	_tokenizer tok = _tokenizer( input , line , " |" , true );
	
	while( tok.next() )
	{
		// Trim the line
		trim( line );
		
		// Resizeable
		if( line == "resizeable" )
			style |= _style::resizeable;
		else if( line == "notResizeable" )
			style |= _style::notResizeable;
		
		// ResizeableX
		else if( line == "resizeableX" )
			style |= _style::resizeableX;
		else if( line == "notResizeableX" )
			style |= _style::notResizeableX;
		
		// ResizeableY
		else if( line == "resizeableY" )
			style |= _style::resizeableY;
		else if( line == "notResizeableY" )
			style |= _style::notResizeableY;
		
		// Destroyable
		else if( line == "enabled" )
			style |= _style::enabled;
		else if( line == "disabled" )
			style |= _style::disabled;
		
		// Clickable
		else if( line == "rightClickable" )
			style |= _style::rightClickable;
		else if( line == "notRightClickable" )
			style |= _style::notRightClickable;
		
		// canTakeFocus
		else if( line == "canTakeFocus" )
			style |= _style::canTakeFocus;
		else if( line == "canNotTakeFocus" )
			style |= _style::canNotTakeFocus;
		
		// blurrable
		else if( line == "blurrable" )
			style |= _style::blurrable;
		else if( line == "notBlurrable" )
			style |= _style::notBlurrable;
		
		// focusable
		else if( line == "focusable" )
			style |= _style::focusable;
		else if( line == "notFocusable" )
			style |= _style::notFocusable;
		
		// DragTrigger
		else if( line == "defaultDragThld" )
			style |= _style::defaultDragThld;
		else if( line == "smallDragThld" )
			style |= _style::smallDragThld;
		
		// DragTrigger
		else if( line == "noClickRepeat" )
			style |= _style::noClickRepeat;
		else if( line == "clickRepeat" )
			style |= _style::clickRepeat;
		
		// doubleClickable
		else if( line == "doubleClickable" )
			style |= _style::doubleClickable;
		else if( line == "notDoubleClickable" )
			style |= _style::notDoubleClickable;
		
		// keyboardRequest
		else if( line == "keyboardRequest" )
			style |= _style::keyboardRequest;
		else if( line == "noKeyboardRequest" )
			style |= _style::noKeyboardRequest;
		
		// keyboardRequest
		else if( line == "draggable" )
			style |= _style::draggable;
		else if( line == "notDraggable" )
			style |= _style::notDraggable;
		
		// clickable
		else if( line == "clickable" )
			style |= _style::clickable;
		else if( line == "notClickable" )
			style |= _style::notClickable;
		
		// drawGreyscaleIfStatic
		else if( line == "drawGreyIfDisabled" )
			style |= _style::drawGreyIfDisabled;
		else if( line == "customDrawIfDisabled" )
			style |= _style::customDrawIfDisabled;
	}
}