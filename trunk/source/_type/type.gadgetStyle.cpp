#include "_type/type.gadgetStyle.h"

_gadgetStyle::_gadgetStyle() : 
	resizable( BIT(1) | BIT(0) ) // Resizable in both directions
	, destroyable(true)
	, editable(true)
	, showsUpInTaskBar(false)
	, own( nullptr )
	, data( 0 )
{ }

_gadgetStyle _gadgetStyle::owner( void* owner ){
	_gadgetStyle g = _gadgetStyle();
	g.own = owner;
	return g;
}

_gadgetStyle _gadgetStyle::storeData( _u32 d ){
	_gadgetStyle g = _gadgetStyle();
	g.data = d;
	return g;
}

_gadgetStyle _defaultStyle_;