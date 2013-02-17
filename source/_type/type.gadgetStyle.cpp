#include "_type/type.gadgetStyle.h"
	
_style _style::storeHandle( void* data , _styleAttr attr ){
	_style g = _style( attr );
	g.data = data;
	return g;
}

_style _style::storeInt( _int data , _styleAttr attr ){
	_style g = _style( attr );
	g.val = data;
	return g;
}

_style _style::storeHost( _program* data , _styleAttr attr ){
	_style g = _style( attr );
	g.host = data;
	return g;
}

// Default
_styleAttr::_styleAttr()
{
	sum = _styleAttr(0)
		| resizeable
		| destroyable
		| editable
		| notMinimizeable
		| canTakeFocus
		| canLooseFocus
		| canReceiveFocus
		| defaultDragTrig
		| mouseClickDefault
		| doubleClickable
		| noKeyboardRequest
		| notDraggable
	;
}

void applyString2style( _style& style , string input )
{
	_u32 pos = 0;
	string line;
	
	_styleAttr attr = _styleAttr( style.attrs );
	
	while( true )
	{
		_u32 tmp = pos;
		
		if( tmp == string::npos )
			break;
		
		pos = input.find_first_of( "|" , tmp );
		
		line = input.substr( tmp , pos - tmp );
		
		// Move beyond the line delimiter
		pos++;
		
		// Trim the line
		trim( line );
		
		// Resizeable
		if( line == "resizeable" )
			attr |= _styleAttr::resizeable;
		else if( line == "notResizeable" )
			attr |= _styleAttr::notResizeable;
		
		// ResizeableX
		else if( line == "resizeableX" )
			attr |= _styleAttr::resizeableX;
		else if( line == "notResizeableX" )
			attr |= _styleAttr::notResizeableX;
		
		// ResizeableY
		else if( line == "resizeableY" )
			attr |= _styleAttr::resizeableY;
		else if( line == "notResizeableY" )
			attr |= _styleAttr::notResizeableY;
		
		// Destroyable
		else if( line == "destroyable" )
			attr |= _styleAttr::destroyable;
		else if( line == "notDestroyable" )
			attr |= _styleAttr::notDestroyable;
		
		// Editable
		else if( line == "editable" )
			attr |= _styleAttr::editable;
		else if( line == "notEditable" )
			attr |= _styleAttr::notEditable;
		
		// Minimizeable
		else if( line == "minimizeable" )
			attr |= _styleAttr::minimizeable;
		else if( line == "notMinimizeable" )
			attr |= _styleAttr::notMinimizeable;
		
		// CanTakeFocus
		else if( line == "canTakeFocus" )
			attr |= _styleAttr::canTakeFocus;
		else if( line == "canNotTakeFocus" )
			attr |= _styleAttr::canNotTakeFocus;
		
		// CanLooseFocus
		else if( line == "canLooseFocus" )
			attr |= _styleAttr::canLooseFocus;
		else if( line == "canNotLooseFocus" )
			attr |= _styleAttr::canNotLooseFocus;
		
		// canReceiveFocus
		else if( line == "canReceiveFocus" )
			attr |= _styleAttr::canReceiveFocus;
		else if( line == "canNotReceiveFocus" )
			attr |= _styleAttr::canNotReceiveFocus;
		
		// DragTrig
		else if( line == "defaultDragTrig" )
			attr |= _styleAttr::defaultDragTrig;
		else if( line == "smallDragTrig" )
			attr |= _styleAttr::smallDragTrig;
		
		// DragTrig
		else if( line == "mouseClickDefault" )
			attr |= _styleAttr::mouseClickDefault;
		else if( line == "mouseClickRepeat" )
			attr |= _styleAttr::mouseClickRepeat;
		
		// doubleClickable
		else if( line == "doubleClickable" )
			attr |= _styleAttr::doubleClickable;
		else if( line == "notDoubleClickable" )
			attr |= _styleAttr::notDoubleClickable;
		
		// keyboardRequest
		else if( line == "keyboardRequest" )
			attr |= _styleAttr::keyboardRequest;
		else if( line == "noKeyboardRequest" )
			attr |= _styleAttr::noKeyboardRequest;
		
		// keyboardRequest
		else if( line == "draggable" )
			attr |= _styleAttr::draggable;
		else if( line == "notDraggable" )
			attr |= _styleAttr::notDraggable;
		
	}
	
	// Copy attributes back into _style class
	style.attrs = attr;
}