#include "_type/type.event.h"

_event::_event( _eventType type ){
	this->type = type;
	resetParams();
}

_event _event::refreshEvent( _area damagedRects ){
	return _event( refresh ).setDamagedRects( damagedRects ).preventBubble( true );
}

_event _event::dialogClose( _s32 intValue , string strValue ){
	return _event( onClose );
}

void _event::resetParams( void* dest ){//!<= Reset All Arguments
	this->dest = dest;
	this->posX = 0;
	this->posY = 0;
	this->effectiveX = 0;
	this->effectiveY = 0;
	this->deltaX = 0;
	this->deltaY = 0;
	this->keyCode = 0;
	this->currentKeyCodes = 0;
	this->heldTime = 0;
	this->bubble = false;
	this->damagedRects.clearRects();
}

_map<string,_callbackReturn> string2eventReturnType = {
	{ "handled" , handled },
	{ "use_normal" , use_normal },
	{ "use_default" , use_default },
	{ "not_handled" , not_handled },
};

_map<_callbackReturn,string> eventReturnType2string = {
	{ handled , "handled" },
	{ use_normal , "use_normal" },
	{ use_default , "use_default" },
	{ not_handled , "not_handled" },
};

_map<string,_eventType> string2eventType = {
	{ "_none_" , _none_ },
	{ "_internal_" , _internal_ },
	{ "refresh" , refresh },
	{ "mouseClick" , mouseClick },
	{ "mouseDoubleClick" , mouseDoubleClick },
	{ "mouseDown" , mouseDown },
	{ "mouseUp" , mouseUp },
	{ "keyDown" , keyDown },
	{ "keyUp" , keyUp },
	{ "keyClick" , keyClick },
	{ "dragStart" , dragStart },
	{ "dragStop" , dragStop },
	{ "dragging" , dragging },
	{ "blur" , blur },
	{ "focus" , focus },
	{ "close" , close },
	{ "onResize" , onResize },
	{ "onAction" , onAction },
	{ "onChange" , onChange },
	{ "onBlur" , onBlur },
	{ "onFocus" , onFocus },
	{ "onClose" , onClose },
	{ "onStyleSet" , onStyleSet }
};

_map<_eventType,string> eventType2string = {
	{ _none_ , "_none_" },
	{ _internal_ , "_internal_" },
	{ refresh , "refresh" },
	{ mouseClick , "mouseClick" },
	{ mouseDoubleClick , "mouseDoubleClick" },
	{ mouseDown , "mouseDown" },
	{ mouseUp , "mouseUp" },
	{ keyDown , "keyDown" },
	{ keyUp , "keyUp" },
	{ keyClick , "keyClick" },
	{ dragStart , "dragStart" },
	{ dragStop , "dragStop" },
	{ dragging , "dragging" },
	{ blur , "blur" },
	{ focus , "focus" },
	{ close , "close" },
	{ onResize , "onResize" },
	{ onAction , "onAction" },
	{ onChange , "onChange" },
	{ onBlur , "onBlur" },
	{ onFocus , "onFocus" },
	{ onClose , "onClose" },
	{ onStyleSet , "onStyleSet" }
};