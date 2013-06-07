#include "_type/type.event.h"

void _event::resetParams( _gadget* dest ){ //!<= Reset All Arguments
	this->gadget = dest;
	this->posX = 0;
	this->posY = 0;
	this->effectiveX = 0;
	this->effectiveY = 0;
	this->deltaX = 0;
	this->deltaY = 0;
	this->keyCode = 0;
	this->currentKeyCodes = 0;
	this->heldTime = 0;
	this->damagedRects.clearRects();
}

_map<string,_callbackReturn> string2callbackReturn = {
	{ "handled" , handled },
	{ "prevent_default" , prevent_default },
	{ "use_default" , use_default },
	{ "not_handled" , not_handled },
};

_map<_callbackReturn,string> callbackReturn2string = {
	{ handled , "handled" },
	{ prevent_default , "prevent_default" },
	{ use_default , "use_default" },
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
	{ "refreshUser" , refreshUser }
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
	{ refreshUser , "refreshUser" }
};