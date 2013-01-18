#include "_type/type.gadgetStyle.h"
#include "stdio.h"
	
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
_styleAttr::_styleAttr(){ sum = _styleAttr(0) | resizeable | destroyable | editable | notMinimizeable | canTakeFocus | canLooseFocus | canReceiveFocus | defaultDragTrig; }

void applyString2style( _style& style , string input )
{
	_u32 end = 0, start;
	string line;
	
	_styleAttr attr = _styleAttr( style.attrs );
	
	while( true )
	{
		start = end;
		
		if( start == string::npos )
			break;
		
		// Find Delimiter (end of line)
		end = input.find_first_of( "|" , start );
		
		if( end != string::npos )
		{
			// Get text from Start (start) to End (end)
			line = input.substr( start , end - start );
			
			// Move beyond delimiter
			end++;
		}
		else
			line = input.substr( start );
		
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
	}
	
	// Copy attributes back into _style class
	style.attrs = attr;
}