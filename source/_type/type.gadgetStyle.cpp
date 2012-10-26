#include "_type/type.gadgetStyle.h"

_gadgetStyle _gadgetStyle::owner( void* owner ){
	_gadgetStyle g = _gadgetStyle();
	g.own = owner;
	return g;
}

_gadgetStyle _gadgetStyle::storeData( _int d ){
	_gadgetStyle g = _gadgetStyle();
	g.data = d;
	return g;
}

_gadgetStyle _defaultStyle_ = { 
	nullptr , // Owner
	0 , // Value
	BIT(1) | BIT(0) , // Resizable (X|Y)
	true , // destroyable
	false , // focused
	false , // pressed
	false , // enhanced
	false , // dragged
	false , // draggable
	true , // editable
	false , // showsUpInTaskBar
	true ,  // canTakeFocus
	true , // canLooseFocus
	true // canReceiveFocus
};
