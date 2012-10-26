#include "_type/type.event.h"

_gadgetEvent::_gadgetEvent( _gadget* src , _gadgetEventType type ){
	this->type = type;
	this->src = src;
}

_gadgetEvent::_gadgetEvent( _gadgetEventType type ){
	this->type = type;
}

_gadgetEvent _gadgetEvent::refreshEvent( _gadget* src , _area damagedRects ){
	return _gadgetEvent( src , "refresh" ).setDamagedRects( damagedRects ).preventBubble( true );
}

_gadgetEvent _gadgetEvent::dialogClose( _gadget* src , _s32 intValue , string strValue ){
	return _gadgetEvent( src , "dialogClose" );
	//e.intValue = intValue;
	//e.strValue = strValue;
}

void _gadgetEvent::resetParams( void* dest ){//!<= Reset All Arguments
	this->src = nullptr;
	this->dest = dest;
	this->posX = 0;
	this->posY = 0;
	this->effectiveX = 0;
	this->effectiveY = 0;
	this->keyCode = 0;
	this->currentKeyCodes = 0;
	this->heldTime = 0;
	this->bubble = false;
	this->damagedRects.clearRects();
}

map<string,_gadgetEventReturnType> string2eventReturnType = {
	{ "handled" , handled },
	{ "use_normal" , use_normal },
	{ "use_default" , use_default },
	{ "not_handled" , not_handled },
};

map<_gadgetEventReturnType,string> eventReturnType2string = {
	{ handled , "handled" },
	{ use_normal , "use_normal" },
	{ use_default , "use_default" },
	{ not_handled , "not_handled" },
};