#include "_type/type.gadgetStyle.h"

_style _style::owner( void* owner ){
	_style g = _style();
	g.own = owner;
	return g;
}

_style _style::storeData( _int d ){
	_style g = _style();
	g.data = d;
	return g;
}

_style::_style() :
	own( nullptr )
	, data( 0 )
	, resizable( BIT(1) | BIT(0) ) // Resizable (X|Y)
	, destroyable( true )
	, focused( false )
	, pressed( false )
	, enhanced( false )
	, dragged( false )
	, draggable( false )
	, editable( true )
	, showsUpInTaskBar( false )
	, canTakeFocus( true )
	, canLooseFocus( true )
	, canReceiveFocus( true )
	, smallDragTrig( false )
{}
