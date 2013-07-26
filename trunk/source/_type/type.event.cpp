#include "_type/type.event.h"

void _event::resetParams( _gadget* dest ){ //!<= Reset All Arguments
	this->gadget = dest;
	this->posX = 0;
	this->posY = 0;
	this->effectiveX = 0;
	this->effectiveY = 0;
	this->currentKeyCodes = 0;
	this->deltaX = 0;
	this->deltaY = 0;
	this->keyCode = 0;
	this->heldTime = 0;
	this->damagedRects = nullptr;
}

_map<string,_callbackReturn> string2callbackReturn = {
	{ "handled" , handled },
	{ "prevent_default" , prevent_default },
	{ "use_default" , use_default },
	{ "use_internal" , use_internal },
	{ "not_handled" , not_handled },
};

_map<_callbackReturn,string> callbackReturn2string = {
	{ handled , "handled" },
	{ prevent_default , "prevent_default" },
	{ use_default , "use_default" },
	{ use_internal , "use_internal" },
	{ not_handled , "not_handled" },
};

_map<string,_eventType> string2eventType = {
	{ "_none_" , _none_ },
	{ "_internal_" , _internal_ },
	{ "refresh" , refresh },
	{ "mouseRepeat" , mouseRepeat },
	{ "mouseClick" , mouseClick },
	{ "mouseDoubleClick" , mouseDoubleClick },
	{ "mouseDown" , mouseDown },
	{ "mouseUp" , mouseUp },
	{ "keyDown" , keyDown },
	{ "keyUp" , keyUp },
	{ "keyClick" , keyClick },
	{ "keyRepeat" , keyRepeat },
	{ "dragStart" , dragStart },
	{ "dragStop" , dragStop },
	{ "dragging" , dragging },
	{ "onResize" , onResize },
	{ "onAction" , onAction },
	{ "onChange" , onChange },
	{ "onBlur" , onBlur },
	{ "onFocus" , onFocus },
	{ "onOpen" , onOpen },
	{ "onClose" , onClose },
	{ "onMouseEnter" , onMouseEnter },
	{ "onMouseLeave" , onMouseLeave },
	{ "onMaximize" , onMaximize },
	{ "onUnMaximize" , onUnMaximize },
	{ "onMinimize" , onMinimize },
	{ "onRestore" , onRestore },
	{ "onScroll" , onScroll },
	{ "onStyleSet" , onStyleSet },
	{ "onDelete" , onDelete },
};

_map<_eventType,string> eventType2string = {
	{ _none_ , "_none_" },
	{ _internal_ , "_internal_" },
	{ refresh , "refresh" },
	{ mouseRepeat , "mouseRepeat" },
	{ mouseClick , "mouseClick" },
	{ mouseDoubleClick , "mouseDoubleClick" },
	{ mouseDown , "mouseDown" },
	{ mouseUp , "mouseUp" },
	{ keyDown , "keyDown" },
	{ keyUp , "keyUp" },
	{ keyClick , "keyClick" },
	{ keyRepeat , "keyRepeat" },
	{ dragStart , "dragStart" },
	{ dragStop , "dragStop" },
	{ dragging , "dragging" },
	{ onResize , "onResize" },
	{ onAction , "onAction" },
	{ onChange , "onChange" },
	{ onBlur , "onBlur" },
	{ onFocus , "onFocus" },
	{ onOpen , "onOpen" },
	{ onClose , "onClose" },
	{ onMouseEnter , "onMouseEnter" },
	{ onMouseLeave , "onMouseLeave" },
	{ onMaximize , "onMaximize" },
	{ onUnMaximize , "onUnMaximize" },
	{ onMinimize , "onMinimize" },
	{ onRestore , "onRestore" },
	{ onScroll , "onScroll" },
	{ onStyleSet , "onStyleSet" },
	{ onDelete , "onDelete" },
};

bool _event::mergeWith( _event& event )
{
	if( this->type != event.type )
		return false;
	if( this->gadget != event.gadget || !this->gadget )
		return false;
	switch( this->type )
	{
		case _eventType::dragging:
		case _eventType::onScroll:
			this->deltaX += event.deltaX;
			this->deltaY += event.deltaY;
			this->posX = event.posX;
			this->posY = event.posY;
			this->effectiveX = event.effectiveX;
			this->effectiveY = event.effectiveY;
			this->keyCode = event.keyCode;
			this->currentKeyCodes |= event.currentKeyCodes;
			break;
		case _eventType::refresh:
			return ( !this->damagedRects || this->damagedRects->empty() ) && ( !event.damagedRects || event.damagedRects->empty() );
		default:
			return false;
	}
	return true;
}