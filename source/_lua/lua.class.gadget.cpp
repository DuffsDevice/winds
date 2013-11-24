#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.bitmap.h"
#include "_lua/lua.class.event.h"
#include "_lua/lua.class.rect.h"
#include "_lua/lua.class.area.h"
#include "_lua/lua.class.bitmapPort.h"
#include "_lua/lua.class.border.h"

#include "_type/type.system.h"
#include "_lua/lua.func.h"
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

//! redrawParents
int _lua_gadget::redrawParents(lua_State* L)
{
	this->gadget->redrawParents();
	return 0;
}

//! redraw
int _lua_gadget::redraw(lua_State* L)
{
	this->gadget->redraw();
	return 0;
}

//! update
int _lua_gadget::update( lua_State* L ){ this->gadget->update(); return 0; }

//! updateNow
int _lua_gadget::updateNow( lua_State* L ){ push( L , this->gadget->updateNow() ); return 1; }

//! getBitmap
int _lua_gadget::getBitmap( lua_State* L ){ Lunar<_lua_bitmap>::push( L , new _lua_bitmap( &this->gadget->getBitmap() ) ); return 1; }

//! setBitmap
int _lua_gadget::setBitmap( lua_State* L ){
	_lua_bitmap* bmp = Lunar<_lua_bitmap>::check( L , 1 );
	if( bmp )
		this->gadget->setBitmap( *bmp );
	return 0;
}

//! getBitmapPort
int _lua_gadget::getBitmapPort( lua_State* L )
{
	// Check number of arguments
	if( lua_gettop( L ) ){
		_lua_area* area = Lunar<_lua_area>::lightcheck( L , 1 );
		if( area ){
			Lunar<_lua_bitmapPort>::push( L , new _lua_bitmapPort( this->gadget->getBitmapPort( *area ) ) );
			return 1;
		}
		
		_lua_event* evt = Lunar<_lua_event>::lightcheck( L , 1 );
		if( evt ){
			Lunar<_lua_bitmapPort>::push( L , new _lua_bitmapPort( this->gadget->getBitmapPort( *evt ) ) );
			return 1;
		}
	}
	
	// Fallback method
	Lunar<_lua_bitmapPort>::push( L , new _lua_bitmapPort( this->gadget->getBitmapPort() ) );
	return 1;
}

//! getScreen
int _lua_gadget::getScreen( lua_State* L ){ _gadget* g = this->gadget->getScreen(); if( !g ) return 0; return push( L , g ); }

//! getToppestChild
int _lua_gadget::getToppestChild( lua_State* L ){ _gadget* g = this->gadget->getToppestChild(); if( !g ) return 0; return push( L , g ); }

//! getLowestChild
int _lua_gadget::getLowestChild( lua_State* L ){ _gadget* g = this->gadget->getLowestChild(); if( !g ) return 0; push( L , g ); return 1; }

//! getToppestEnhancedChild
int _lua_gadget::getToppestEnhancedChild( lua_State* L ){ _gadget* g = this->gadget->getToppestEnhancedChild(); if( !g ) return 0; return push( L , g ); }

//! getPrecedentChild
int _lua_gadget::getPrecedentChild( lua_State* L ){ _gadget* g = this->gadget->getPrecedentChild( lightcheck<bool>( L , 1 , false ) ); if( !g ) return 0; return push( L , g ); }

//! getSubcedentChild
int _lua_gadget::getSubcedentChild( lua_State* L ){ _gadget* g = this->gadget->getSubcedentChild( lightcheck<bool>( L , 1 , false ) ); if( !g ) return 0; return push( L , g ); }

//! setUserEventHandler
int _lua_gadget::setUserEventHandler( lua_State* L ){
	this->gadget->setUserEventHandler( string2eventType[ check<string>( L , 1 ) ] , _luaCallback<_eventHandler>( L , 2 ) );
	return 0;
}

//! setInternalEventHandler
int _lua_gadget::setInternalEventHandler( lua_State* L ){ 
	this->gadget->setInternalEventHandler( string2eventType[ check<string>( L , 1 ) ] , _luaCallback<_eventHandler>( L , 2 ) );
	return 0;
}

//! removeUserHandler
int _lua_gadget::removeUserEventHandler( lua_State* L ){
	_eventType t = string2eventType[ check<string>( L , 1 ) ];
	this->gadget->removeUserEventHandler( t );
	return 0;
}

//! removeInternalHandler
int _lua_gadget::removeInternalEventHandler( lua_State* L ){
	_eventType t = string2eventType[ check<string>( L , 1 ) ];
	this->gadget->removeInternalEventHandler( t );
	return 0;
}

//! triggerEvent
int _lua_gadget::triggerEvent(lua_State* L)
{
	_lua_event* e = Lunar<_lua_event>::check( L , 1 );
	if( !e )
		return 0;
	
	if( lua_isnil( L , 2 ) || lua_isstring( L , 2 ) )
		this->gadget->triggerEvent( *e , lightcheck<_eventCallType>( L , 2 , _eventCallType::normal ) );
	else
		this->gadget->triggerEvent( *e , check<bool>( L , 2 ) );
	
	return 0;
}

//! triggerEventUser
int _lua_gadget::triggerEventUser(lua_State* L)
{
	_lua_event* e = Lunar<_lua_event>::check( L , 1 );
	if( e )
		this->gadget->triggerEventUser( *e );
	return 0;
}

//! triggerEventInternal
int _lua_gadget::triggerEventInternal(lua_State* L)
{
	_lua_event* e = Lunar<_lua_event>::check( L , 1 );
	if( e )
		this->gadget->triggerEventInternal( *e );
	return 0;
}

//! triggerEventDefault
int _lua_gadget::triggerEventDefault(lua_State* L)
{
	_lua_event* e = Lunar<_lua_event>::check( L , 1 );
	if( e )
		this->gadget->triggerEventDefault( *e );
	return 0;
}

//! canReactTo
int _lua_gadget::canReactTo( lua_State* L ){ lua_pushboolean( L , this->gadget->canReactTo( string2eventType[ check<string>( L , 1 ) ] ) ); return 1; }

//! handleEvent
int _lua_gadget::handleEvent( lua_State* L ){ _lua_event* e = Lunar<_lua_event>::check( L , 1 ); if( !e ) return 0; lua_pushstring( L , callbackReturn2string[ this->gadget->handleEvent( *e , lightcheck<bool>( L , 2 , false ) ) ].c_str() ); return 1; }

//! handleEventUser
int _lua_gadget::handleEventUser( lua_State* L ){ _lua_event* e = Lunar<_lua_event>::check( L , 1 ); if( !e ) return 0; lua_pushstring( L , callbackReturn2string[ this->gadget->handleEventUser( *e ) ].c_str() ); return 1; }

//! handleEventInternal
int _lua_gadget::handleEventInternal( lua_State* L ){ _lua_event* e = Lunar<_lua_event>::check( L , 1 ); if( !e ) return 0; lua_pushstring( L , callbackReturn2string[ this->gadget->handleEventInternal( *e ) ].c_str() ); return 1; }

//! handleEventDefault
int _lua_gadget::handleEventDefault( lua_State* L ){ _lua_event* e = Lunar<_lua_event>::check( L , 1 ); if( !e ) return 0; lua_pushstring( L , callbackReturn2string[ this->gadget->handleEventDefault( *e ) ].c_str() ); return 1; }

//! Get absolute X
int _lua_gadget::getAbsoluteX(lua_State* L){ lua_pushnumber( L , this->gadget->getAbsoluteX() ); return 1; }

//! Get absolute Y
int _lua_gadget::getAbsoluteY(lua_State* L){ lua_pushnumber( L , this->gadget->getAbsoluteY() ); return 1; }

//! Get absolute Position
int _lua_gadget::getAbsolutePosition(lua_State* L){ _2s32 pos = this->gadget->getAbsolutePosition(); lua_pushnumber( L , pos.first ); lua_pushnumber( L , pos.second ); return 2; }

//! Get X
int _lua_gadget::getX(lua_State* L){ lua_pushnumber( L , this->gadget->getX() ); return 1; }

//! Get Y
int _lua_gadget::getY(lua_State* L){ lua_pushnumber( L , this->gadget->getY() ); return 1; }

//! Set X
int _lua_gadget::setX(lua_State* L){
	if( lua_isnil( L , 1 ) )
		this->gadget->requestAutoX();
	else
		this->gadget->setX( check<int>( L , 1 ) );
	return 0;
}

//! setXIfAuto
int _lua_gadget::setXIfAuto(lua_State* L){ this->gadget->setXIfAuto( check<int>( L , 1 ) ); return 0; }

//! Request auto X
int _lua_gadget::requestAutoX(lua_State* L){ this->gadget->requestAutoX(); return 0; }

//! Set Y
int _lua_gadget::setY(lua_State* L){
	if( lua_isnil( L , 1 ) )
		this->gadget->requestAutoY();
	else
		this->gadget->setY( check<int>( L , 1 ) );
	return 0;
}

//! setYIfAuto
int _lua_gadget::setYIfAuto(lua_State* L){ this->gadget->setYIfAuto( check<int>( L , 1 ) ); return 0; }

//! Request auto Y
int _lua_gadget::requestAutoY(lua_State* L){ this->gadget->requestAutoY(); return 0; }

//! moveTo
int _lua_gadget::moveTo(lua_State* L){ this->gadget->moveTo( check<int>( L , 1 ) , check<int>( L , 2 ) ); return 0; }

//! moveToIfAuto
int _lua_gadget::moveToIfAuto(lua_State* L){ this->gadget->moveToIfAuto( check<int>( L , 1 ) , check<int>( L , 2 ) ); return 0; }

//! Request auto Position
int _lua_gadget::requestAutoPosition(lua_State* L){ this->gadget->requestAutoPosition(); return 0; }

//! moveRelative
int _lua_gadget::moveRelative(lua_State* L){ this->gadget->moveRelative( check<int>( L , 1 ) , check<int>( L , 2 ) ); return 0; }

//! moveRelativeIfAuto
int _lua_gadget::moveRelativeIfAuto(lua_State* L){ this->gadget->moveRelativeIfAuto( check<int>( L , 1 ) , check<int>( L , 2 ) ); return 0; }

//! setSize
int _lua_gadget::setSize(lua_State* L){ this->gadget->setSize( check<int>( L , 1 ) , check<int>( L , 2 ) ); return 0; }

//! setSizeIfAuto
int _lua_gadget::setSizeIfAuto( lua_State* L ){ this->gadget->setSizeIfAuto( check<int>( L , 1 ) , check<int>( L , 2 ) ); return 0; }

//! requestAutoSize
int _lua_gadget::requestAutoSize( lua_State* L ){
	this->gadget->requestAutoSize();
	return 0;
}

//! getParent
int _lua_gadget::getParent( lua_State* L ){ if( !this->gadget ) return 0; Lunar<_lua_gadget>::push( L , new _lua_gadget( this->gadget->getParent() , false ) ); return 1; }

//! setParent
int _lua_gadget::setParent( lua_State* L ){ _gadget* g = nullptr; if( ( g = lightcheck<_gadget*>( L , 1 , nullptr ) ) != nullptr ) this->gadget->setParent( g ); return 0; }

//! enhanceToParent
int _lua_gadget::enhanceToParent( lua_State* L ){ _gadget* g = nullptr; if( ( g = lightcheck<_gadget*>( L , 1 , nullptr ) ) != nullptr ) this->gadget->enhanceToParent( g ); return 0; }

//! removeChild
int _lua_gadget::removeChild( lua_State* L ){ _gadget* g = nullptr; if( ( g = lightcheck<_gadget*>( L , 1 , nullptr ) ) != nullptr ) this->gadget->removeChild( g ); return 0; }

//! removeChildren
int _lua_gadget::removeChildren( lua_State* L ){ this->gadget->removeChildren(); return 0; }

//! removeEnhancedChildren
int _lua_gadget::removeEnhancedChildren( lua_State* L ){ this->gadget->removeEnhancedChildren(); return 0; }

//! addChild
int _lua_gadget::addChild( lua_State* L ){ _gadget* g = lightcheck<_gadget*>( L , 1 , nullptr ); if( g ) this->gadget->addChild( g ); return 0; }

//! addEnhancedChild
int _lua_gadget::addEnhancedChild( lua_State* L ){ _gadget* g = nullptr; if( ( g = lightcheck<_gadget*>( L , 1 , nullptr ) ) != nullptr ) this->gadget->addEnhancedChild( g ); return 0; }

//! focusChild
int _lua_gadget::focusChild( lua_State* L ){ _gadget* g = lightcheck<_gadget*>( L , 1 , nullptr ); if( g ) this->gadget->focusChild( g ); return 0; }

//! getDimensions
int _lua_gadget::getDimensions( lua_State* L ){ Lunar<_lua_rect>::push( L , new _lua_rect( this->gadget->getDimensions() ) ); return 1; }

//! getAbsoluteDimensions
int _lua_gadget::getAbsoluteDimensions( lua_State* L ){ Lunar<_lua_rect>::push( L , new _lua_rect( this->gadget->getAbsoluteDimensions() ) ); return 1; }

//! setDimensionsIfAuto
int _lua_gadget::setDimensionsIfAuto( lua_State* L ){
	_lua_rect* rc = Lunar<_lua_rect>::check( L , 1 );
	if( rc )
		this->gadget->setDimensionsIfAuto( *rc );
	return 0;
}

//! setDimensions
int _lua_gadget::setDimensions( lua_State* L ){
	_lua_rect* rc = Lunar<_lua_rect>::check( L , 1 );
	if( rc )
		this->gadget->setDimensions( *rc );
	return 0;
}

//! requestAutoDimensions
int _lua_gadget::requestAutoDimensions( lua_State* L ){
	this->gadget->requestAutoDimensions();
	return 0;
}

//! getHeight
int _lua_gadget::getHeight( lua_State* L ){ lua_pushnumber( L , this->gadget->getHeight() ); return 1; }

//! getWidth
int _lua_gadget::getWidth( lua_State* L ){ lua_pushnumber( L , this->gadget->getWidth() ); return 1; }

//! getMinHeight
int _lua_gadget::getMinHeight( lua_State* L ){ lua_pushnumber( L , this->gadget->getMinHeight() ); return 1; }

//! getMinWidth
int _lua_gadget::getMinWidth( lua_State* L ){ lua_pushnumber( L , this->gadget->getMinWidth() ); return 1; }

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

//! requestAutoWidth
int _lua_gadget::requestAutoWidth(lua_State* L){
	this->gadget->requestAutoWidth();
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

//! requestAutoHeight
int _lua_gadget::requestAutoHeight(lua_State* L){
	this->gadget->requestAutoHeight();
	return 0;
}

//! setPadding
int _lua_gadget::setPadding(lua_State* L){ _lua_border* rc = Lunar<_lua_border>::check( L , 1 ); if( rc ) this->gadget->setPadding( *(_border*)rc ); return 0; }

//! getPadding
int _lua_gadget::getPadding(lua_State* L){ Lunar<_lua_border>::push( L , new _lua_border( this->gadget->getPadding() ) ); return 1; }

//! applyStyle
int _lua_gadget::applyStyle(lua_State* L){ _style s = this->gadget->getStyle(); applyString2style( s , check<string>( L , 1 ) ); this->gadget->setStyle( move(s) ); return 0; }

//! blurChild
int _lua_gadget::blurChild(lua_State* L){ this->gadget->blurChild(); return 0; }

//! blink
int _lua_gadget::blink(lua_State* L){ this->gadget->blink(); return 0; }

//! hide
int _lua_gadget::hide(lua_State* L){ this->gadget->hide(); return 0; }

//! show
int _lua_gadget::show(lua_State* L){ this->gadget->show(); return 0; }

//! focus
int _lua_gadget::focus(lua_State* L){ this->gadget->focus(); return 0; }

//! blur
int _lua_gadget::blur(lua_State* L){ this->gadget->blur(); return 0; }

//! isEnhanced
int _lua_gadget::isEnhanced(lua_State* L){ lua_pushboolean( L , this->gadget->isEnhanced() ); return 1; }

//! isHidden
int _lua_gadget::isHidden(lua_State* L){ lua_pushboolean( L , this->gadget->isHidden() ); return 1; }

//! isResizeable
int _lua_gadget::isResizeable(lua_State* L){ lua_pushboolean( L , this->gadget->isResizeable() ); return 1; }

//! isResizeableX
int _lua_gadget::isResizeableX(lua_State* L){ lua_pushboolean( L , this->gadget->isResizeableX() ); return 1; }

//! isResizeableY
int _lua_gadget::isResizeableY(lua_State* L){ lua_pushboolean( L , this->gadget->isResizeableY() ); return 1; }

//! hasSmallDragTrig
int _lua_gadget::hasSmallDragTrig(lua_State* L){ lua_pushboolean( L , this->gadget->hasSmallDragTrig() ); return 1; }

//! isMouseClickRepeat
int _lua_gadget::isMouseClickRepeat(lua_State* L){ lua_pushboolean( L , this->gadget->isMouseClickRepeat() ); return 1; }

//! isDoubleClickable
int _lua_gadget::isDoubleClickable(lua_State* L){ lua_pushboolean( L , this->gadget->isDoubleClickable() ); return 1; }

//! requestsKeyboard
int _lua_gadget::requestsKeyboard(lua_State* L){ lua_pushboolean( L , this->gadget->requestsKeyboard() ); return 1; }

//! hasFocus
int _lua_gadget::hasFocus(lua_State* L){ lua_pushboolean( L , this->gadget->hasFocus() ); return 1; }

//! isPressed
int _lua_gadget::isPressed(lua_State* L){ lua_pushboolean( L , this->gadget->isPressed() ); return 1; }

//! isChildDragged
int _lua_gadget::isChildDragged(lua_State* L){ lua_pushboolean( L , this->gadget->isChildDragged() ); return 1; }

//! isEnabled
int _lua_gadget::isEnabled(lua_State* L){ lua_pushboolean( L , this->gadget->isEnabled() ); return 1; }

//! isClickable
int _lua_gadget::isClickable(lua_State* L){ lua_pushboolean( L , this->gadget->isClickable() ); return 1; }

//! isDraggable
int _lua_gadget::isDraggable(lua_State* L){ lua_pushboolean( L , this->gadget->isDraggable() ); return 1; }


//! hasAutoWidth
int _lua_gadget::hasAutoWidth(lua_State* L){ lua_pushboolean( L , this->gadget->hasAutoWidth() ); return 1; }

//! hasAutoHeight
int _lua_gadget::hasAutoHeight(lua_State* L){ lua_pushboolean( L , this->gadget->hasAutoHeight() ); return 1; }

//! hasAutoX
int _lua_gadget::hasAutoX(lua_State* L){ lua_pushboolean( L , this->gadget->hasAutoX() ); return 1; }

//! hasAutoY
int _lua_gadget::hasAutoY(lua_State* L){ lua_pushboolean( L , this->gadget->hasAutoY() ); return 1; }

//! getSizeRect
int _lua_gadget::getSizeRect(lua_State* L){ Lunar<_lua_rect>::push( L , new _lua_rect( this->gadget->getSizeRect() ) ); return 1; }

//! getClientRect
int _lua_gadget::getClientRect(lua_State* L){ Lunar<_lua_rect>::push( L , new _lua_rect( this->gadget->getClientRect() ) ); return 1; }

//! getSize
int _lua_gadget::getSize(lua_State* L){ _2s32 size = this->gadget->getSize(); lua_pushnumber( L , size.first ); lua_pushnumber( L , size.second ); return 2; }

//! getType
int _lua_gadget::getType(lua_State* L){ lua_pushstring( L , gadgetType2string[this->gadget->getType()].c_str() ); return 1; }

//! base
int _lua_gadget::baseFunc( lua_State* L ){
	Lunar<_lua_gadget>::push( L , new _lua_gadget( this->gadget , false ) );
	return 1;
}

//! Lua-_gadget
const char _lua_gadget::className[] = "Gadget";
Lunar<_lua_gadget>::FunctionType _lua_gadget::methods[] = {
	LUA_CLASS_FUNC(_lua_gadget,applyStyle),
	LUA_CLASS_FUNC(_lua_gadget,hasFocus),
	LUA_CLASS_FUNC(_lua_gadget,isPressed),
	LUA_CLASS_FUNC(_lua_gadget,isChildDragged),
	LUA_CLASS_FUNC(_lua_gadget,isEnhanced),
	LUA_CLASS_FUNC(_lua_gadget,isHidden),
	LUA_CLASS_FUNC(_lua_gadget,isEnabled),
	LUA_CLASS_FUNC(_lua_gadget,isClickable),
	LUA_CLASS_FUNC(_lua_gadget,isResizeableX),
	LUA_CLASS_FUNC(_lua_gadget,isResizeableY),
	LUA_CLASS_FUNC(_lua_gadget,hasSmallDragTrig),
	LUA_CLASS_FUNC(_lua_gadget,isMouseClickRepeat),
	LUA_CLASS_FUNC(_lua_gadget,requestsKeyboard),
	LUA_CLASS_FUNC(_lua_gadget,isDraggable),
	LUA_CLASS_FUNC(_lua_gadget,isDoubleClickable),
	/*  */
	LUA_CLASS_FUNC(_lua_gadget,redraw),
	LUA_CLASS_FUNC(_lua_gadget,update),
	LUA_CLASS_FUNC(_lua_gadget,getBitmapPort),
	LUA_CLASS_FUNC(_lua_gadget,getBitmap),
	/*  */
	LUA_CLASS_FUNC(_lua_gadget,setInternalEventHandler),
	LUA_CLASS_FUNC(_lua_gadget,setUserEventHandler),
	LUA_CLASS_FUNC(_lua_gadget,removeInternalEventHandler),
	LUA_CLASS_FUNC(_lua_gadget,removeUserEventHandler),
	LUA_CLASS_FUNC(_lua_gadget,canReactTo),
	/*  */
	LUA_CLASS_FUNC(_lua_gadget,handleEvent),
	LUA_CLASS_FUNC(_lua_gadget,handleEventUser),
	LUA_CLASS_FUNC(_lua_gadget,handleEventInternal),
	LUA_CLASS_FUNC(_lua_gadget,handleEventDefault),
	LUA_CLASS_FUNC(_lua_gadget,triggerEvent),
	LUA_CLASS_FUNC(_lua_gadget,triggerEventUser),
	LUA_CLASS_FUNC(_lua_gadget,triggerEventInternal),
	LUA_CLASS_FUNC(_lua_gadget,triggerEventDefault),
	/*  */
	LUA_CLASS_FUNC(_lua_gadget,getAbsoluteDimensions),
	/*  */
	LUA_CLASS_FUNC(_lua_gadget,getAbsolutePosition),
	LUA_CLASS_FUNC(_lua_gadget,getAbsoluteX),
	LUA_CLASS_FUNC(_lua_gadget,getAbsoluteY),
	/*  */
	LUA_CLASS_FUNC(_lua_gadget,moveToIfAuto),
	LUA_CLASS_FUNC(_lua_gadget,moveTo),
	LUA_CLASS_FUNC(_lua_gadget,requestAutoPosition),
	LUA_CLASS_FUNC(_lua_gadget,moveRelativeIfAuto),
	LUA_CLASS_FUNC(_lua_gadget,moveRelative),
	LUA_CLASS_FUNC(_lua_gadget,hasAutoX),
	LUA_CLASS_FUNC(_lua_gadget,hasAutoY),
	/*  */
	LUA_CLASS_FUNC(_lua_gadget,setSizeIfAuto),
	LUA_CLASS_FUNC(_lua_gadget,setSize),
	LUA_CLASS_FUNC(_lua_gadget,requestAutoSize),
	LUA_CLASS_FUNC(_lua_gadget,getSizeRect),
	LUA_CLASS_FUNC(_lua_gadget,getSize),
	LUA_CLASS_FUNC(_lua_gadget,getClientRect),
	LUA_CLASS_FUNC(_lua_gadget,hasAutoWidth),
	LUA_CLASS_FUNC(_lua_gadget,hasAutoHeight),
	LUA_CLASS_FUNC(_lua_gadget,getDimensions),
	LUA_CLASS_FUNC(_lua_gadget,setDimensions),
	LUA_CLASS_FUNC(_lua_gadget,setDimensionsIfAuto),
	/*  */
	LUA_CLASS_FUNC(_lua_gadget,enhanceToParent),
	LUA_CLASS_FUNC(_lua_gadget,getScreen),
	LUA_CLASS_FUNC(_lua_gadget,removeChild),
	LUA_CLASS_FUNC(_lua_gadget,removeChildren),
	LUA_CLASS_FUNC(_lua_gadget,removeEnhancedChildren),
	LUA_CLASS_FUNC(_lua_gadget,addChild),
	LUA_CLASS_FUNC(_lua_gadget,addEnhancedChild),
	LUA_CLASS_FUNC(_lua_gadget,getToppestChild),
	LUA_CLASS_FUNC(_lua_gadget,getToppestEnhancedChild),
	LUA_CLASS_FUNC(_lua_gadget,getPrecedentChild),
	LUA_CLASS_FUNC(_lua_gadget,getSubcedentChild),
	LUA_CLASS_FUNC(_lua_gadget,focusChild),
	LUA_CLASS_FUNC(_lua_gadget,blurChild),
	/*  */
	LUA_CLASS_FUNC(_lua_gadget,hide),
	LUA_CLASS_FUNC(_lua_gadget,show),
	LUA_CLASS_FUNC(_lua_gadget,blur),
	LUA_CLASS_FUNC(_lua_gadget,focus),
	LUA_CLASS_FUNC(_lua_gadget,blink),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_gadget>::PropertyType _lua_gadget::properties[] = {
	{ "padding" , &_lua_gadget::getPadding , &_lua_gadget::setPadding },
	{ "x" , &_lua_gadget::getX , &_lua_gadget::setX },
	{ "y" , &_lua_gadget::getY , &_lua_gadget::setY },
	{ "parent" , &_lua_gadget::getParent , &_lua_gadget::setParent },
	{ "height" , &_lua_gadget::getHeight , &_lua_gadget::setHeight },
	{ "width" , &_lua_gadget::getWidth , &_lua_gadget::setWidth },
	{ "minHeight" , &_lua_gadget::getMinHeight , &_lua_gadget::setMinHeight },
	{ "minWidth" , &_lua_gadget::getMinWidth , &_lua_gadget::setMinWidth },
	{ "type" , &_lua_gadget::getType , nullptr },
	{ "bitmap" , &_lua_gadget::getBitmap , &_lua_gadget::setBitmap },
	LUA_CLASS_ATTR_END
};