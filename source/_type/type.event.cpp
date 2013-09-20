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

bool _event::mergeWith( _event& event )
{
	if( this->type != event.type )
		return false;
	if( this->gadget != event.gadget || !this->gadget )
		return false;
	switch( this->type )
	{
		case onDragging:
		case onScroll:
			this->deltaX += event.deltaX;
			this->deltaY += event.deltaY;
			this->posX = event.posX;
			this->posY = event.posY;
			this->effectiveX = event.effectiveX;
			this->effectiveY = event.effectiveY;
			this->keyCode = event.keyCode;
			this->currentKeyCodes |= event.currentKeyCodes;
			break;
		case onDraw:
			return ( !this->damagedRects || this->damagedRects->empty() ) && ( !event.damagedRects || event.damagedRects->empty() );
		case onEdit:
		case onUpdate:
			break;
		case onResize:
		case onMove:
		case onRestyle:
		case onSet:
		case onParentResize:
		case onParentMove:
		case onParentRestyle:
		case onParentSet:
		case onChildResize:
		case onChildMove:
		case onChildRestyle:
		case onChildSet:
		case onPreResize:
		case onPreMove:
		case onPreRestyle:
		case onPreSet:
		case onPostResize:
		case onPostMove:
		case onPostRestyle:
		case onPostSet:
			return this->depParam.gadget == event.depParam.gadget;
		case onVisibility:
		case onParentVisibility:
		case onChildVisibility:
		case onPreVisibility:
		case onPostVisibility:
			return this->depParam.gadget == event.depParam.gadget && this->depParam.flag != event.depParam.flag;
		default:
			return false;
	}
	return true;
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
	{ "onDraw" , onDraw },
	{ "onMouseClick" , onMouseClick },
	{ "onMouseDblClick" , onMouseDblClick },
	{ "onMouseDown" , onMouseDown },
	{ "onMouseUp" , onMouseUp },
	{ "onMouseRepeat" , onMouseRepeat },
	{ "onKeyDown" , onKeyDown },
	{ "onKeyUp" , onKeyUp },
	{ "onKeyClick" , onKeyClick },
	{ "onKeyRepeat" , onKeyRepeat },
	{ "onDragStart" , onDragStart },
	{ "onDragStop" , onDragStop },
	{ "onDragging" , onDragging },
	
	{ "onUpdate" , onUpdate },
	{ "onEdit" , onEdit },
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
	
	{ "onResize" , onResize },
	{ "onMove" , onMove },
	{ "onRestyle" , onRestyle },
	{ "onVisibility" , onVisibility },
	{ "onSet" , onSet },
	{ "onParentResize" , onParentResize },
	{ "onParentMove" , onParentMove },
	{ "onParentRestyle" , onParentRestyle },
	{ "onParentVisibility" , onParentVisibility },
	{ "onParentSet" , onParentSet },
	{ "onChildResize" , onChildResize },
	{ "onChildMove" , onChildMove },
	{ "onChildRestyle" , onChildRestyle },
	{ "onChildVisibility" , onChildVisibility },
	{ "onChildSet" , onChildSet },
	{ "onPreResize" , onPreResize },
	{ "onPreMove" , onPreMove },
	{ "onPreRestyle" , onPreRestyle },
	{ "onPreVisibility" , onPreVisibility },
	{ "onPreSet" , onPreSet },
	{ "onPostResize" , onPostResize },
	{ "onPostMove" , onPostMove },
	{ "onPostRestyle" , onPostRestyle },
	{ "onPostVisibility" , onPostVisibility },
	{ "onPostSet" , onPostSet }
};

_map<_eventType,string> eventType2string = {
	{ _none_ , "_none_" },
	{ onDraw , "onDraw" },
	{ onMouseRepeat , "onMouseRepeat" },
	{ onMouseClick , "onMouseClick" },
	{ onMouseDblClick , "onMouseDblClick" },
	{ onMouseDown , "onMouseDown" },
	{ onMouseUp , "onMouseUp" },
	{ onKeyDown , "onKeyDown" },
	{ onKeyUp , "onKeyUp" },
	{ onKeyClick , "onKeyClick" },
	{ onKeyRepeat , "onKeyRepeat" },
	{ onDragStart , "onDragStart" },
	{ onDragStop , "onDragStop" },
	{ onDragging , "onDragging" },

	{ onUpdate , "onUpdate" },
	{ onEdit , "onEdit" },
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
	
	{ onResize , "onResize" },
	{ onMove , "onMove" },
	{ onRestyle , "onRestyle" },
	{ onVisibility , "onVisibility" },
	{ onSet , "onSet" },
	{ onParentResize , "onParentResize" },
	{ onParentMove , "onParentMove" },
	{ onParentRestyle , "onParentRestyle" },
	{ onParentVisibility , "onParentVisibility" },
	{ onParentSet , "onParentSet" },
	{ onChildResize , "onChildResize" },
	{ onChildMove , "onChildMove" },
	{ onChildRestyle , "onChildRestyle" },
	{ onChildVisibility , "onChildVisibility" },
	{ onChildSet , "onChildSet" },
	{ onPreResize , "onPreResize" },
	{ onPreMove , "onPreMove" },
	{ onPreRestyle , "onPreRestyle" },
	{ onPreVisibility , "onPreVisibility" },
	{ onPreSet , "onPreSet" },
	{ onPostResize , "onPostResize" },
	{ onPostMove , "onPostMove" },
	{ onPostRestyle , "onPostRestyle" },
	{ onPostVisibility , "onPostVisibility" },
	{ onPostSet , "onPostSet" }
};