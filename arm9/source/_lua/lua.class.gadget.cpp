#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.bitmap.h"
#include "_lua/lua.class.event.h"
#include "_lua/lua.class.rect.h"
#include "_lua/lua.class.area.h"
#include "_lua/lua.class.bitmap.port.h"
#include "_lua/lua.class.border.h"

#include "_type/type.system.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##           Lua-Gadget           ##
##################################*/

_lua_gadget::_lua_gadget( _gadget* g , bool wasAllocated ) :
	gadget( g ),
	wasAllocated( wasAllocated )
{}

void _lua_gadget::setGadget( _gadget* g , bool wasAllocated ){
	if( this->wasAllocated && this->gadget )
		delete this->gadget;
	
	// Register new gadget
	this->gadget = g;
	this->wasAllocated = wasAllocated;
}

//! Lua-Ctor
_lua_gadget::_lua_gadget( lua_State* L ) :
	gadget(
		new _gadget(
			string2gadgetType[ check<string>( L , 1 ) ]
			, check<int>( L , 2 )
			, check<int>( L , 3 )
			, check<int>( L , 4 )
			, check<int>( L , 5 )
		)
	)
	, wasAllocated( true )
{ }

//! Lua-Dtor
_lua_gadget::~_lua_gadget()
{
	if( this->gadget != nullptr && this->wasAllocated )
		delete this->gadget;
}

//! getBitmapPort
int _lua_gadget::getBitmapPort( lua_State* L )
{
	// Check number of arguments
	if( lua_gettop( L ) )
	{
		_lua_event* evt = Lunar<_lua_event>::lightcheck( L , 1 );
		
		if( evt ){
			push( L , this->gadget->getBitmapPort( *evt ) );
			return 1;
		}
		
		_lua_area* area = Lunar<_lua_area>::check( L , 1 );
		if( area ){
			push( L , this->gadget->getBitmapPort( *area ) );
			return 1;
		}
	}
	
	// Fallback method
	push( L , this->gadget->getBitmapPort() );
	return 1;
}

//! triggerEvent
int _lua_gadget::triggerEvent(lua_State* L)
{
	if( is_a<bool>( L , 2 ) )
		this->gadget->triggerEvent( check<_event>( L , 1 ) , check<bool>( L , 2 ) );
	else
		this->gadget->triggerEvent( check<_event>( L , 1 ) , lightcheck<_eventCallType>( L , 2 , _eventCallType::normal ) );
	
	return 0;
}

//! Set X
int _lua_gadget::setX(lua_State* L){
	if( lua_isnil( L , 1 ) )
		this->gadget->requestAutoX();
	else
		this->gadget->setX( check<int>( L , 1 ) , lightcheck<bool>( L , 2 , false ) );
	return 0;
}

//! Set Y
int _lua_gadget::setY(lua_State* L){
	if( lua_isnil( L , 1 ) )
		this->gadget->requestAutoY();
	else
		this->gadget->setY( check<int>( L , 1 ) , lightcheck<bool>( L , 2 , false ) );
	return 0;
}

//! Set Position
int _lua_gadget::moveTo(lua_State* L){
	this->gadget->moveTo( check<int>( L , 1 ) , check<int>( L , 2 ) , lightcheck<bool>( L , 3 , false ) );
	return 0;
}

//! Move Relatively
int _lua_gadget::moveRelative(lua_State* L){
	this->gadget->moveRelative( check<int>( L , 1 ) , check<int>( L , 2 ) , lightcheck<bool>( L , 3 , false ) );
	return 0;
}

//! setMinHeight
int _lua_gadget::setMinHeight( lua_State* L ){
	if( lua_isnil( L , 1 ) )
		this->gadget->setMinHeight();
	else
		this->gadget->setMinHeight( check<int>( L , 1 ) );
	return 0;
}

//! setMinWidth
int _lua_gadget::setMinWidth( lua_State* L ){
	if( lua_isnil( L , 1 ) )
		this->gadget->setMinWidth();
	else
		this->gadget->setMinWidth( check<int>( L , 1 ) );
	return 0;
}

//! setWidth
int _lua_gadget::setWidth(lua_State* L){
	if( lua_isnil( L , 1 ) )
		this->gadget->requestAutoWidth();
	else
		this->gadget->setWidth( check<int>( L , 1 ) );
	return 0;
}

//! setHeight
int _lua_gadget::setHeight(lua_State* L){
	if( lua_isnil( L , 1 ) )
		this->gadget->requestAutoHeight();
	else
		this->gadget->setHeight( check<int>( L , 1 ) );
	return 0;
}

//! applyStyle
int _lua_gadget::applyStyle(lua_State* L){ _style s = this->gadget->getStyle(); applyString2style( s , check<string>( L , 1 ) ); this->gadget->setStyle( move(s) ); return 0; }

//! getType
int _lua_gadget::getType(lua_State* L){ return push( L , gadgetType2string[this->gadget->getType()] ); }

//! equals
int _lua_gadget::equals(lua_State* L){
	return push( L , bool(check<_gadget*>( L , 1 ) == this->gadget) );
}

//! addChild
int _lua_gadget::addChild(lua_State* L){
	this->gadget->addChild( check<_gadget*>( L , 1 ) , lightcheck<bool>( L , 2 , false ) , lightcheck<_gadget*>( L , 3 , nullptr ) );
	return 0;
}

//! addEnhancedChild
int _lua_gadget::addEnhancedChild(lua_State* L){
	this->gadget->addEnhancedChild( check<_gadget*>( L , 1 ) , lightcheck<bool>( L , 2 , false ) , lightcheck<_gadget*>( L , 3 , nullptr ) );
	return 0;
}

//! Lua-_gadget
const char _lua_gadget::className[] = "Gadget";
Lunar<_lua_gadget>::FunctionType _lua_gadget::methods[] = {
	{ "applyStyle"					, &_lua_gadget::applyStyle },
	{ "hasFocus"					, wrap( _lua_gadget , &_gadget::hasFocus ) },
	{ "isSelected"					, wrap( _lua_gadget , &_gadget::isSelected ) },
	{ "isPressed"					, wrap( _lua_gadget , &_gadget::isPressed ) },
	{ "isChildDragged"				, wrap( _lua_gadget , &_gadget::isChildDragged ) },
	{ "isEnhanced"					, wrap( _lua_gadget , &_gadget::isEnhanced ) },
	{ "isEnabled"					, wrap( _lua_gadget , &_gadget::isEnabled ) },
	{ "isRightClickable"			, wrap( _lua_gadget , &_gadget::isRightClickable ) },
	{ "isResizeableX"				, wrap( _lua_gadget , &_gadget::isResizeableX ) },
	{ "isResizeableY"				, wrap( _lua_gadget , &_gadget::isResizeableY ) },
	{ "wantsSmallDragThld"			, wrap( _lua_gadget , &_gadget::wantsSmallDragThld ) },
	{ "wantsClickRepeat"			, wrap( _lua_gadget , &_gadget::wantsClickRepeat ) },
	{ "requestsKeyboard"			, wrap( _lua_gadget , &_gadget::requestsKeyboard ) },
	{ "isDraggable"					, wrap( _lua_gadget , &_gadget::isDraggable ) },
	{ "isDoubleClickable"			, wrap( _lua_gadget , &_gadget::isDoubleClickable ) },
	/*  */
	{ "redraw"						, wrap( _lua_gadget , (void(_gadget::*)())&_gadget::redraw ) },
	{ "redrawParents"				, wrap( _lua_gadget , (void(_gadget::*)())&_gadget::redrawParents ) },
	{ "getBitmapPort"				, &_lua_gadget::getBitmapPort },
	/*  */
	{ "setInternalEventHandler"		, wrap( _lua_gadget , &_gadget::setInternalEventHandler ) },
	{ "setUserEventHandler"			, wrap( _lua_gadget , &_gadget::setUserEventHandler ) },
	{ "removeInternalEventHandler"	, wrap( _lua_gadget , &_gadget::removeInternalEventHandler ) },
	{ "removeUserEventHandler"		, wrap( _lua_gadget , &_gadget::removeUserEventHandler ) },
	{ "canReactTo"					, wrap( _lua_gadget , &_gadget::canReactTo ) },
	/*  */
	{ "handleEvent"					, wrap( _lua_gadget , &_gadget::handleEvent ) },
	{ "handleEventUser"				, wrap( _lua_gadget , &_gadget::handleEventUser ) },
	{ "handleEventInternal"			, wrap( _lua_gadget , &_gadget::handleEventInternal ) },
	{ "handleEventDefault"			, wrap( _lua_gadget , &_gadget::handleEventDefault ) },
	{ "triggerEvent"				, &_lua_gadget::triggerEvent },
	{ "triggerEventUser"			, wrap( _lua_gadget , &_gadget::triggerEventUser ) },
	{ "triggerEventInternal"		, wrap( _lua_gadget , &_gadget::triggerEventInternal ) },
	{ "triggerEventDefault"			, wrap( _lua_gadget , &_gadget::triggerEventDefault ) },
	/*  */
	{ "getAbsoluteDimensions"		, wrap( _lua_gadget , &_gadget::getAbsoluteDimensions ) },
	/*  */
	{ "getAbsolutePosition"			, wrap( _lua_gadget , &_gadget::getAbsolutePosition ) },
	{ "getAbsoluteX"				, wrap( _lua_gadget , &_gadget::getAbsoluteX ) },
	{ "getAbsoluteY"				, wrap( _lua_gadget , &_gadget::getAbsoluteY ) },
	/*  */
	{ "moveToIfAuto"				, wrap( _lua_gadget , &_gadget::moveToIfAuto ) },
	{ "moveTo"						, &_lua_gadget::moveTo },
	{ "requestAutoPosition"			, wrap( _lua_gadget , &_gadget::requestAutoPosition ) },
	{ "moveRelativeIfAuto"			, wrap( _lua_gadget , &_gadget::moveRelativeIfAuto ) },
	{ "moveRelative"				, &_lua_gadget::moveRelative },
	{ "hasAutoX"					, wrap( _lua_gadget , &_gadget::hasAutoX ) },
	{ "hasAutoY"					, wrap( _lua_gadget , &_gadget::hasAutoY ) },
	/*  */
	{ "setSizeIfAuto"				, wrap( _lua_gadget , &_gadget::setSizeIfAuto ) },
	{ "setSize"						, wrap( _lua_gadget , &_gadget::setSize ) },
	{ "requestAutoSize"				, wrap( _lua_gadget , &_gadget::requestAutoSize ) },
	{ "getSizeRect"					, wrap( _lua_gadget , &_gadget::getSizeRect ) },
	{ "getSize"						, wrap( _lua_gadget , &_gadget::getSize ) },
	{ "getClientRect"				, wrap( _lua_gadget , &_gadget::getClientRect ) },
	{ "hasAutoWidth"				, wrap( _lua_gadget , &_gadget::hasAutoWidth ) },
	{ "hasAutoHeight"				, wrap( _lua_gadget , &_gadget::hasAutoHeight ) },
	{ "getDimensions"				, wrap( _lua_gadget , &_gadget::getDimensions ) },
	{ "setDimensions"				, wrap( _lua_gadget , &_gadget::setDimensions ) },
	{ "setDimensionsIfAuto"			, wrap( _lua_gadget , &_gadget::setDimensionsIfAuto ) },
	/*  */
	{ "enhanceToParent"				, wrap( _lua_gadget , &_gadget::enhanceToParent ) },
	{ "removeChild"					, wrap( _lua_gadget , &_gadget::removeChild ) },
	{ "removeChildren"				, wrap( _lua_gadget , &_gadget::removeChildren ) },
	{ "removeEnhancedChildren"		, wrap( _lua_gadget , &_gadget::removeEnhancedChildren ) },
	{ "addChild"					, &_lua_gadget::addChild },
	{ "addEnhancedChild"			, &_lua_gadget::addEnhancedChild },
	{ "getFocusedChild"				, wrap( _lua_gadget , &_gadget::getFocusedChild ) },
	{ "getSelectedChild"			, wrap( _lua_gadget , &_gadget::getSelectedChild ) },
	{ "getLowestChild"				, wrap( _lua_gadget , &_gadget::getLowestChild ) },
	{ "getToppestChild"				, wrap( _lua_gadget , &_gadget::getToppestChild ) },
	{ "getLowestEnhancedChild"		, wrap( _lua_gadget , &_gadget::getLowestEnhancedChild ) },
	{ "getToppestEnhancedChild"		, wrap( _lua_gadget , &_gadget::getToppestEnhancedChild ) },
	{ "getPrecedentChild"			, wrap( _lua_gadget , &_gadget::getPrecedentChild ) },
	{ "getSubcedentChild"			, wrap( _lua_gadget , &_gadget::getSubcedentChild ) },
	{ "focusChild"					, wrap( _lua_gadget , &_gadget::focusChild ) },
	{ "blurChild"					, wrap( _lua_gadget , &_gadget::blurChild ) },
	{ "selectChild"					, wrap( _lua_gadget , &_gadget::selectChild ) },
	{ "deselectChild"				, wrap( _lua_gadget , &_gadget::deselectChild ) },
	/*  */
	{ "hide"						, wrap( _lua_gadget , &_gadget::hide ) },
	{ "show"						, wrap( _lua_gadget , &_gadget::show ) },
	{ "blur"						, wrap( _lua_gadget , &_gadget::blur ) },
	{ "focus"						, wrap( _lua_gadget , &_gadget::focus ) },
	{ "select"						, wrap( _lua_gadget , &_gadget::select ) },
	{ "deselect"					, wrap( _lua_gadget , &_gadget::deselect ) },
	{ "blink"						, wrap( _lua_gadget , &_gadget::blink ) },
	{ "equals"						, &_lua_gadget::equals  },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_gadget>::PropertyType _lua_gadget::properties[] = {
	{ "padding"		, wrap( _lua_gadget , &_gadget::getPadding )	, wrap( _lua_gadget , &_gadget::setPadding ) },
	{ "x"			, wrap( _lua_gadget , &_gadget::getX )			, &_lua_gadget::setX },
	{ "y"			, wrap( _lua_gadget , &_gadget::getY )			, &_lua_gadget::setY },
	{ "parent"		, wrap( _lua_gadget , &_gadget::getParent )		, wrap( _lua_gadget , &_gadget::setParent ) },
	{ "height"		, wrap( _lua_gadget , &_gadget::getHeight )		, &_lua_gadget::setHeight },
	{ "width"		, wrap( _lua_gadget , &_gadget::getWidth )		, &_lua_gadget::setWidth },
	{ "minHeight"	, wrap( _lua_gadget , &_gadget::getMinHeight )	, &_lua_gadget::setMinHeight },
	{ "minWidth"	, wrap( _lua_gadget , &_gadget::getMinWidth )	, &_lua_gadget::setMinWidth },
	{ "type"		, &_lua_gadget::getType							, nullptr },
	{ "bitmap"		, wrap( _lua_gadget , &_gadget::getBitmap )		, wrap( _lua_gadget , &_gadget::setBitmap ) },
	LUA_CLASS_ATTR_END
};