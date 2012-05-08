#include "_lua/lua.class.gadgetEventArgs.h"

#include "_lua/lua.class.bitmap.h"
#include "_lua/lua.class.window.h"
#include "_lua/lua.class.button.h"
#include "_lua/lua.class.checkbox.h"

#include <vector>
#include <sstream>

/*##################################
##           Lua-Gagget           ##
##################################*/

_lua_gadget* _lua_gadget::getLuaGadget( lua_State* L , int narg ){
	
	if( luaL_is( L , 1 , "_window" ) )
		return Lunar<_lua_window>::check( L , narg ); 
	else if( luaL_is( L , 1 , "_button" ) )
		return Lunar<_lua_button>::check( L , narg ); 
	else if( luaL_is( L , 1 , "_checkbox" ) )
		return Lunar<_lua_checkbox>::check( L , narg ); 
	return Lunar<_lua_gadget>::check( L , narg ); 
}

_lua_gadget::_lua_gadget( _gadget* w ){ this->gadget = w; }

// Lua-Ctor
_lua_gadget::_lua_gadget( lua_State* L ){ this->gadget = new _gadget( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) ); }

//! Lua-Dtor
_lua_gadget::~_lua_gadget(){ if( this->gadget != NULL ){ delete this->gadget; this->gadget = NULL; } }

//! bubbleEvent
int _lua_gadget::bubbleEvent(lua_State* L){  _lua_gadgetEvent* e = Lunar<_lua_gadgetEvent>::check( L , 1 ); if( e ) this->gadget->bubbleEvent( *e , luaL_optint( L , 2 , 0 ) ); return 0; }

//! bubbleRefresh
int _lua_gadget::bubbleRefresh(lua_State* L){ this->gadget->bubbleRefresh( luaL_optint( L , 1 , 0 ) ); return 0; }

//! refreshBitmap
int _lua_gadget::refreshBitmap( lua_State* L ){ this->gadget->refreshBitmap(); return 0; }

//! getBitmap
int _lua_gadget::getBitmap( lua_State* L ){ Lunar<_lua_bitmap>::push( L , new _lua_bitmap( this->gadget->getBitmap() ) , true ); return 1; }

//! getWindows
int _lua_gadget::getWindows( lua_State* L ){ Lunar<_lua_gadget>::push( L , new _lua_gadget( this->gadget->getWindows() ) , true ); return 1; }

//! registerEventHandler
int _lua_gadget::registerEventHandler( lua_State* L ){ 
	_gadgetEventType t = string2eventType[ luaL_checkstring( L , 1 ) ];
	if( lua_isfunction( L , -1 ) ){
		int i = luaL_ref(L, LUA_REGISTRYINDEX);
		this->gadget->registerLuaEventHandler( t , i , L ); 
	}
	return 0;
}

//! unregisterEventHandler
int _lua_gadget::unregisterEventHandler( lua_State* L ){ _gadgetEventType t = string2eventType[ luaL_checkstring( L , 1 ) ]; luaL_unref(L, LUA_REGISTRYINDEX , this->gadget->unregisterLuaEventHandler( t ) ); return 0; }

//! generateEvent
int _lua_gadget::generateEvent(lua_State* L){  _lua_gadgetEvent* e = Lunar<_lua_gadgetEvent>::check( L , 1 ); if( e ) this->gadget->generateEvent( *e ); return 0; }

//! triggerEvent
int _lua_gadget::triggerEvent(lua_State* L){  _lua_gadgetEvent* e = Lunar<_lua_gadgetEvent>::check( L , 1 ); if( e ) this->gadget->triggerEvent( *e ); return 0; }

//! canReactTo
int _lua_gadget::canReactTo( lua_State* L ){ lua_pushboolean( L , this->gadget->canReactTo( string2eventType[ luaL_checkstring( L , 1 ) ] ) ); return 1; }

//! handleEvent
int _lua_gadget::handleEvent( lua_State* L ){ _lua_gadgetEvent* e = Lunar<_lua_gadgetEvent>::check( L , 1 ); if( !e ) return 0; lua_pushstring( L , eventReturnType2string[ this->gadget->handleEvent( *e ) ].c_str() ); return 1; }

//! handleEventDefault
int _lua_gadget::handleEventDefault( lua_State* L ){ _lua_gadgetEvent* e = Lunar<_lua_gadgetEvent>::check( L , 1 ); if( !e ) return 0; lua_pushstring( L , eventReturnType2string[ this->gadget->handleEventDefault( *e ) ].c_str() ); return 1; }

//! Get absolute X
int _lua_gadget::getAbsoluteX(lua_State* L){ lua_pushnumber( L , this->gadget->getAbsoluteX() ); return 1; }

//! Get absolute Y
int _lua_gadget::getAbsoluteY(lua_State* L){ lua_pushnumber( L , this->gadget->getAbsoluteY() ); return 1; }

//! Get X
int _lua_gadget::getX(lua_State* L){ lua_pushnumber( L , this->gadget->getX() ); return 1; }

//! Get Y
int _lua_gadget::getY(lua_State* L){ lua_pushnumber( L , this->gadget->getY() ); return 1; }

//! Set X
int _lua_gadget::setX(lua_State* L){ this->gadget->setX( luaL_checkint( L , 1 ) ); return 0; }

//! Set Y
int _lua_gadget::setY(lua_State* L){ this->gadget->setY( luaL_checkint( L , 1 ) ); return 0; }

//! moveTo
int _lua_gadget::moveTo(lua_State* L){ this->gadget->moveTo( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) ); return 0; }

//! moveRelative
int _lua_gadget::moveRelative(lua_State* L){ this->gadget->moveRelative( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) ); return 0; }

//! getParent
int _lua_gadget::getParent( lua_State* L ){ Lunar<_lua_gadget>::push( L , new _lua_gadget( this->gadget->getParent() ) , true ); return 1; }

//! setParent
int _lua_gadget::setParent( lua_State* L ){ _lua_gadget* g = NULL; if( ( g = this->getLuaGadget( L , 1 ) ) != NULL ) this->gadget->setParent( g->gadget ); return 0; }

//! removeChild
int _lua_gadget::removeChild( lua_State* L ){ _lua_gadget* g = NULL; if( ( g = this->getLuaGadget( L , 1 ) ) != NULL ) this->gadget->removeChild( g->gadget ); return 0; }

//! addChild
int _lua_gadget::addChild( lua_State* L ){ _lua_gadget* g = NULL; if( ( g = this->getLuaGadget( L , 1 ) ) != NULL ) this->gadget->addChild( g->gadget ); return 0; }

//! getDimensions
int _lua_gadget::getDimensions( lua_State* L ){ Lunar<_lua_rect>::push( L , new _lua_rect( this->gadget->getDimensions() ) , true ); return 1; }

//! getAbsoluteDimensions
int _lua_gadget::getAbsoluteDimensions( lua_State* L ){ Lunar<_lua_rect>::push( L , new _lua_rect( this->gadget->getAbsoluteDimensions() ) , true ); return 1; }

//! setDimensions
int _lua_gadget::setDimensions( lua_State* L ){ _lua_rect* rc = Lunar<_lua_rect>::check( L , 1 ); if( rc ) this->gadget->setDimensions( *rc ); return 0; }

//! getHeight
int _lua_gadget::getHeight( lua_State* L ){ lua_pushnumber( L , this->gadget->getHeight() ); return 0; }

//! getWidth
int _lua_gadget::getWidth( lua_State* L ){ lua_pushnumber( L , this->gadget->getWidth() ); return 0; }

//! setWidth
int _lua_gadget::setWidth(lua_State* L){ this->gadget->setWidth( luaL_checkint( L , 1 ) ); return 0; }

//! setHeight
int _lua_gadget::setHeight(lua_State* L){ this->gadget->setHeight( luaL_checkint( L , 1 ) ); return 0; }

//! toDerived
int _lua_gadget::toDerived( lua_State* L ){ 
	switch( this->gadget->getType() ){
		case window:
			Lunar<_lua_window>::push( L , new _lua_window( (_window*)this->gadget ) , true );
			return 1;
		case checkbox:
			Lunar<_lua_checkbox>::push( L , new _lua_checkbox( (_checkbox*)this->gadget ) , true );
			return 1;
		case button:
			Lunar<_lua_button>::push( L , new _lua_button( (_button*)this->gadget ) , true );
			return 1;
		default:
			break;
	}
	return 0;
}

//! Lua-_gadget
const char _lua_gadget::className[] = "_gadget";
Lunar<_lua_gadget>::RegType _lua_gadget::methods[] = {
	LUNAR_DECLARE_ALL_METHODS_OF( _lua_gadget ),
	{0,0}
};





/*##################################
##         Lua-GadgetEvent        ##
##################################*/
// Ctor...
_lua_gadgetEvent::_lua_gadgetEvent( _gadgetEvent e ) :
	_gadgetEvent( e )
{ }

// Lua-Ctor...
_lua_gadgetEvent::_lua_gadgetEvent( lua_State* L ) : _gadgetEvent( ){
	// _gadgetEvent( type , args )
	if( string("string") == luaL_typename( L , 1 ) )
	{
		_gadgetEvent::setType( string2eventType[ luaL_checkstring( L , 1 ) ] );
		
		if( luaL_is( L , 2 , "_gadgetEventArgs" ) ){
			_lua_gadgetEventArgs* ea = Lunar<_lua_gadgetEventArgs>::check( L , 2 );
			if( ea )
				_gadgetEvent::setArgs( *ea );
		}
	}
	// _gadgetEvent( src* , type , args )
	else if( lua_isuserdata( L , 1 ) )
	{
		_lua_gadget* g = Lunar<_lua_gadget>::check( L , 1 );
		
		if( g )
			_gadgetEvent::getArgs().setSource( g->gadget );
		
		// Is a gadgetEventType?
		if( string("string") == luaL_typename( L , 2 ) )
		{
			_gadgetEvent::setType( string2eventType[ luaL_checkstring( L , 2 ) ] );
			
			// Additionally args?
			if( luaL_is( L , 3 , "_gadgetEventArgs" ) )
			{
				_lua_gadgetEventArgs* ea = Lunar<_lua_gadgetEventArgs>::check( L , 3 );
				
				if( ea )
					_gadgetEvent::setArgs( *ea );
			}
		}
	}
}

//! setType
int _lua_gadgetEvent::setType( lua_State* L ){ _gadgetEvent::setType( string2eventType[ luaL_checkstring( L , 1 ) ] ); return 0; }

//! getType
int _lua_gadgetEvent::getType( lua_State* L ){ lua_pushstring( L , eventType2string[ _gadgetEvent::getType() ].c_str() ); return 1; }

//! getArgs
int _lua_gadgetEvent::getArgs( lua_State* L ){ Lunar<_lua_gadgetEventArgs>::push( L , new _lua_gadgetEventArgs( _gadgetEvent::getArgs() ) , true ); return 1; }

//! setArgs
int _lua_gadgetEvent::setArgs( lua_State* L ){ _lua_gadgetEventArgs* ea = Lunar<_lua_gadgetEventArgs>::check( L , 1 ); if( ea ) _gadgetEvent::setArgs( *ea ); return 0; }

//! getGadget
int _lua_gadgetEvent::getGadget( lua_State* L ){ Lunar<_lua_gadget>::push( L , new _lua_gadget( _gadgetEvent::getGadget() ) , true ); return 1; }

//! Lua-_gadgetEvent
const char _lua_gadgetEvent::className[] = "_gadgetEvent";
Lunar<_lua_gadgetEvent>::RegType _lua_gadgetEvent::methods[] = {
  LUNAR_DECLARE_METHOD(_lua_gadgetEvent, setType),
  LUNAR_DECLARE_METHOD(_lua_gadgetEvent, getType),
  LUNAR_DECLARE_METHOD(_lua_gadgetEvent, setArgs),
  LUNAR_DECLARE_METHOD(_lua_gadgetEvent, getArgs),
  LUNAR_DECLARE_METHOD(_lua_gadgetEvent, getGadget),
  {0,0}
};





/*##################################
##       Lua-GaggetEventargs      ##
##################################*/
_lua_gadgetEventArgs::_lua_gadgetEventArgs( _gadgetEventArgs gea ) :
	_gadgetEventArgs( gea )
{ }

//! Lua-Ctor...
_lua_gadgetEventArgs::_lua_gadgetEventArgs( lua_State* L ) : 
	_gadgetEventArgs( )
{ }

//! reset
int _lua_gadgetEventArgs::reset(lua_State* L){ _gadgetEventArgs::reset(); return 0; }

//! getDestination
int _lua_gadgetEventArgs::getDestination(lua_State* L){  Lunar<_lua_gadget>::push( L , new _lua_gadget( (_gadget*)_gadgetEventArgs::getDestination() ) , true ); return 1; }

//! getSource
int _lua_gadgetEventArgs::getSource(lua_State* L){  Lunar<_lua_gadget>::push( L , new _lua_gadget( (_gadget*)_gadgetEventArgs::getSource() ) , true ); return 1; }

//! setDestination
int _lua_gadgetEventArgs::setDestination(lua_State* L){ _lua_gadget* g = Lunar<_lua_gadget>::check( L , 1 ); if( g ) _gadgetEventArgs::setDestination( g->gadget ); return 0; }

//! setSource
int _lua_gadgetEventArgs::setSource(lua_State* L){  _lua_gadget* g = Lunar<_lua_gadget>::check( L , 1 ); if( g ) _gadgetEventArgs::setSource( g->gadget ); return 0; }

//! Set X
int _lua_gadgetEventArgs::setPosX(lua_State* L){ _gadgetEventArgs::setPosX( luaL_checkint( L , 1 ) ); return 0; }

//! Set Y
int _lua_gadgetEventArgs::setPosY(lua_State* L){ _gadgetEventArgs::setPosY( luaL_checkint( L , 1 ) ); return 0; }

//! Get X
int _lua_gadgetEventArgs::getPosX(lua_State* L){ lua_pushnumber( L , _gadgetEventArgs::getPosX() ); return 1; }

//! Get Y
int _lua_gadgetEventArgs::getPosY(lua_State* L){ lua_pushnumber( L , _gadgetEventArgs::getPosY() ); return 1; }

//! Set X
int _lua_gadgetEventArgs::setDeltaX(lua_State* L){ _gadgetEventArgs::setDeltaX( luaL_checkint( L , 1 ) ); return 0; }

//! Set Y
int _lua_gadgetEventArgs::setDeltaY(lua_State* L){ _gadgetEventArgs::setDeltaY( luaL_checkint( L , 1 ) ); return 0; }

//! Get X
int _lua_gadgetEventArgs::getDeltaX(lua_State* L){ lua_pushnumber( L , _gadgetEventArgs::getDeltaX() ); return 1; }

//! Get Y
int _lua_gadgetEventArgs::getDeltaY(lua_State* L){ lua_pushnumber( L , _gadgetEventArgs::getDeltaY() ); return 1; }

//! Get Key-Code
int _lua_gadgetEventArgs::getKeyCode(lua_State* L){ lua_pushnumber( L , _gadgetEventArgs::getKeyCode() ); return 1; }

//! Set Key-Code
int _lua_gadgetEventArgs::setKeyCode(lua_State* L){ _gadgetEventArgs::setKeyCode( luaL_checkint( L , 1 ) ); return 0; }

//! Get Held Time of the key that triggered the Event
int _lua_gadgetEventArgs::getHeldTime(lua_State* L){ lua_pushnumber( L , _gadgetEventArgs::getHeldTime() ); return 1; }

//! Set Held Time of the key that triggered the Event
int _lua_gadgetEventArgs::setHeldTime(lua_State* L){ _gadgetEventArgs::setHeldTime( luaL_checkint( L , 1 ) ); return 0; }

//! Get KeyCode State of that Moment the Event was triggered
int _lua_gadgetEventArgs::getCurrentKeyCodes(lua_State* L){ lua_pushnumber( L , _gadgetEventArgs::getCurrentKeyCodes() ); return 1; }

//! Set KeyCode State of that Moment the Event was triggered
int _lua_gadgetEventArgs::setCurrentKeyCodes(lua_State* L){ _gadgetEventArgs::setCurrentKeyCodes( luaL_checkint( L , 1 ) ); return 0; }

//! Set Damaged Rects(The Rects that have to be repainted by the parent)
int _lua_gadgetEventArgs::setDamagedRects(lua_State* L){ _lua_area* a = Lunar<_lua_area>::check( L , 1 ); if( a ) _gadgetEventArgs::setDamagedRects( *a ); return 0; }

//! Get Damaged Rects(The Rects that have to be repainted by the parent)
int _lua_gadgetEventArgs::getDamagedRects(lua_State* L){  Lunar<_lua_area>::push( L , new _lua_area( _gadgetEventArgs::getDamagedRects() ) , true ); return 1; }

//! Check if event is a bubble-Refresh-One
int _lua_gadgetEventArgs::isBubblePrevented(lua_State* L){ lua_pushboolean( L , _gadgetEventArgs::isBubblePrevented() ); return 1; }

//! Check if event is a bubble-Refresh-One
int _lua_gadgetEventArgs::preventBubble(lua_State* L){ _gadgetEventArgs::preventBubble( luaL_checkint( L , 1 ) ); return 0; }

//! Lua-_gadgetEventArgs
const char _lua_gadgetEventArgs::className[] = "_gadgetEventArgs";
Lunar<_lua_gadgetEventArgs>::RegType _lua_gadgetEventArgs::methods[] = {
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, reset),
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, getSource),
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, getDestination),
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, setSource),
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, setDestination),
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, reset),
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, setPosX),
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, setPosY),
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, getPosX),
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, getPosY),
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, setDeltaX),
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, setDeltaY),
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, getDeltaX),
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, getDeltaY),
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, setKeyCode),
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, getKeyCode),
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, setHeldTime),
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, getHeldTime),
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, setCurrentKeyCodes),
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, getCurrentKeyCodes),
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, isBubblePrevented),
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, preventBubble),
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, setDamagedRects),
  LUNAR_DECLARE_METHOD(_lua_gadgetEventArgs, getDamagedRects),
  {0,0}
};