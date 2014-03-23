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
	this->keyCode = _key::none;
	this->heldTime = 0;
	this->damagedRects = nullptr;
}

bool _event::mergeWith( _event& event )
{
	if( this->gadget != event.gadget )
		return false;
	
	if( this->type != event.type )
	{
		if( this->depParam.gadget != event.depParam.gadget )
			return false;
		
		if( ( this->type == onParentHide && event.type == onParentShow )
			| ( this->type == onParentShow && event.type == onParentHide )
		)
			return true;
		
		else if( ( this->type == onChildHide && event.type == onChildShow )
			| ( this->type == onChildShow && event.type == onChildHide )
		)
			return true;
		
		else if( ( this->type == onPreHide && event.type == onPreShow )
			| ( this->type == onPreShow && event.type == onPreHide )
		)
			return true;
		
		else if( ( this->type == onPostHide && event.type == onPostShow )
			| ( this->type == onPostShow && event.type == onPostHide )
		)
			return true;
		
		return false;
	}
	
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
			this->pressure = event.pressure;
			this->currentKeyCodes.add( event.currentKeyCodes );
			break;
		case onDraw:
			return ( !this->damagedRects || this->damagedRects->isEmpty() ) && ( !event.damagedRects || event.damagedRects->isEmpty() );
		case onEdit:
		case onUpdate:
		case onResize:
		case onMove:
		case onRestyle:
			break;
		
		case onParentResize:
		case onParentMove:
		case onParentRestyle:
		case onParentAdd:
		case onParentRemove:
		case onParentFocus:
		case onParentBlur:
		
		case onChildResize:
		case onChildMove:
		case onChildRestyle:
		case onChildAdd:
		case onChildRemove:
		case onChildFocus:
		case onChildBlur:
		
		case onPreResize:
		case onPreMove:
		case onPreRestyle:
		case onPreAdd:
		case onPreRemove:
		case onPreFocus:
		case onPreBlur:
		
		case onPostResize:
		case onPostMove:
		case onPostRestyle:
		case onPostAdd:
		case onPostRemove:
		case onPostFocus:
		case onPostBlur:
			return this->depParam.gadget == event.depParam.gadget;
		default:
			return false;
	}
	return true;
}

_fromStr<_callbackReturn> string2callbackReturn = {
	{ "handled" , handled },
	{ "prevent_default" , prevent_default },
	{ "use_default" , use_default },
	{ "use_internal" , use_internal },
	{ "not_handled" , not_handled },
};

_toStr<_callbackReturn> callbackReturn2string = {
	{ handled , "handled" },
	{ prevent_default , "prevent_default" },
	{ use_default , "use_default" },
	{ use_internal , "use_internal" },
	{ not_handled , "not_handled" },
};

_fromStr<_eventType> string2eventType = {
	{ "_none_"				, _none_ },
	{ "onDraw"				, onDraw },
	{ "onMouseClick"		, onMouseClick },
	{ "onMouseDblClick"		, onMouseDblClick },
	{ "onMouseDown"			, onMouseDown },
	{ "onMouseUp"			, onMouseUp },
	{ "onMouseRepeat"		, onMouseRepeat },
	{ "onKeyDown"			, onKeyDown },
	{ "onKeyUp"				, onKeyUp },
	{ "onKeyClick"			, onKeyClick },
	{ "onKeyRepeat"			, onKeyRepeat },
	{ "onDragStart"			, onDragStart },
	{ "onDragStop"			, onDragStop },
	{ "onDragging"			, onDragging },
	{ "onMouseRightClick"	, onMouseRightClick },
	
	{ "onUpdate"			, onUpdate },
	{ "onEdit"				, onEdit },
	
	
	{ "onOpen"				, onOpen },
	{ "onClose"				, onClose },
	{ "onMouseEnter"		, onMouseEnter },
	{ "onMouseLeave"		, onMouseLeave },
	{ "onMaximize"			, onMaximize },
	{ "onUnMaximize"		, onUnMaximize },
	{ "onMinimize"			, onMinimize },
	{ "onRestore"			, onRestore },
	{ "onScroll"			, onScroll },
	{ "onSelect"			, onSelect },
	{ "onDeselect"			, onDeselect },
	
	{ "onResize"			, onResize },
	{ "onMove"				, onMove },
	{ "onRestyle"			, onRestyle },
	{ "onShow"				, onShow },
	{ "onHide"				, onHide },
	{ "onFocus"				, onFocus },
	{ "onBlur"				, onBlur },
	{ "onAdd"				, onAdd },
	{ "onDelete"			, onDelete },
	
	{ "onParentResize"		, onParentResize },
	{ "onParentMove"		, onParentMove },
	{ "onParentRestyle"		, onParentRestyle },
	{ "onParentShow"		, onParentShow },
	{ "onParentHide"		, onParentHide },
	{ "onParentFocus"		, onParentFocus },
	{ "onParentBlur"		, onParentBlur },
	{ "onParentAdd"			, onParentAdd },
	{ "onParentRemove"		, onParentRemove },
	
	{ "onChildResize"		, onChildResize },
	{ "onChildMove"			, onChildMove },
	{ "onChildRestyle"		, onChildRestyle },
	{ "onChildShow"			, onChildShow },
	{ "onChildHide"			, onChildHide },
	{ "onChildFocus"		, onChildFocus },
	{ "onChildBlur"			, onChildBlur },
	{ "onChildAdd"			, onChildAdd },
	{ "onChildRemove"		, onChildRemove },
	
	{ "onPreResize"			, onPreResize },
	{ "onPreMove"			, onPreMove },
	{ "onPreRestyle"		, onPreRestyle },
	{ "onPreShow"			, onPreShow },
	{ "onPreHide"			, onPreHide },
	{ "onPreFocus"			, onPreFocus },
	{ "onPreBlur"			, onPreBlur },
	{ "onPreAdd"			, onPreAdd },
	{ "onPreRemove"			, onPreRemove },
	
	{ "onPostResize"		, onPostResize },
	{ "onPostMove"			, onPostMove },
	{ "onPostRestyle"		, onPostRestyle },
	{ "onPostShow"			, onPostShow },
	{ "onPostHide"			, onPostHide },
	{ "onPostFocus"			, onPostFocus },
	{ "onPostBlur"			, onPostBlur },
	{ "onPostAdd"			, onPostAdd },
	{ "onPostRemove"		, onPostRemove }
};

_toStr<_eventType> eventType2string = {
	{ _none_			, "_none_" },
	{ onDraw			, "onDraw" },
	{ onMouseRepeat		, "onMouseRepeat" },
	{ onMouseClick		, "onMouseClick" },
	{ onMouseDblClick	, "onMouseDblClick" },
	{ onMouseDown		, "onMouseDown" },
	{ onMouseUp			, "onMouseUp" },
	{ onKeyDown			, "onKeyDown" },
	{ onKeyUp			, "onKeyUp" },
	{ onKeyClick		, "onKeyClick" },
	{ onKeyRepeat		, "onKeyRepeat" },
	{ onDragStart		, "onDragStart" },
	{ onDragStop		, "onDragStop" },
	{ onDragging		, "onDragging" },
	{ onMouseRightClick	, "onMouseRightClick" },

	{ onUpdate			, "onUpdate" },
	{ onEdit			, "onEdit" },
	
	{ onOpen			, "onOpen" },
	{ onClose			, "onClose" },
	{ onMouseEnter		, "onMouseEnter" },
	{ onMouseLeave		, "onMouseLeave" },
	{ onMaximize		, "onMaximize" },
	{ onUnMaximize		, "onUnMaximize" },
	{ onMinimize		, "onMinimize" },
	{ onRestore			, "onRestore" },
	{ onScroll			, "onScroll" },
	{ onSelect			, "onSelect" },
	{ onDeselect		, "onDeselect" },
	
	{ onResize			, "onResize" },
	{ onMove			, "onMove" },
	{ onRestyle			, "onRestyle" },
	{ onShow			, "onShow" },
	{ onHide			, "onHide" },
	{ onFocus			, "onFocus" },
	{ onBlur			, "onBlur" },
	{ onAdd				, "onAdd" },
	{ onDelete			, "onDelete" },
	
	{ onParentResize	, "onParentResize" },
	{ onParentMove		, "onParentMove" },
	{ onParentRestyle	, "onParentRestyle" },
	{ onParentShow		, "onParentShow" },
	{ onParentHide		, "onParentHide" },
	{ onParentFocus		, "onParentFocus" },
	{ onParentBlur		, "onParentBlur" },
	{ onParentAdd		, "onParentAdd" },
	{ onParentRemove	, "onParentRemove" },
	
	{ onChildResize		, "onChildResize" },
	{ onChildMove		, "onChildMove" },
	{ onChildRestyle	, "onChildRestyle" },
	{ onChildShow		, "onChildShow" },
	{ onChildHide		, "onChildHide" },
	{ onChildFocus		, "onChildFocus" },
	{ onChildBlur		, "onChildBlur" },
	{ onChildAdd		, "onChildAdd" },
	{ onChildRemove		, "onChildRemove" },
	
	{ onPreResize		, "onPreResize" },
	{ onPreMove			, "onPreMove" },
	{ onPreRestyle		, "onPreRestyle" },
	{ onPreShow			, "onPreShow" },
	{ onPreHide			, "onPreHide" },
	{ onPreFocus		, "onPreFocus" },
	{ onPreBlur			, "onPreBlur" },
	{ onPreAdd			, "onPreAdd" },
	{ onPreRemove		, "onPreRemove" },
	
	{ onPostResize		, "onPostResize" },
	{ onPostMove		, "onPostMove" },
	{ onPostRestyle		, "onPostRestyle" },
	{ onPostShow		, "onPostShow" },
	{ onPostHide		, "onPostHide" },
	{ onPostFocus		, "onPostFocus" },
	{ onPostBlur		, "onPostBlur" },
	{ onPostAdd			, "onPostAdd" },
	{ onPostRemove		, "onPostRemove" },
};