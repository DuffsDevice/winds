#include "_type/type.progLua.h"
#include "_type/type.system.h"
#include "_type/type.callback.h"
#include "_type/type.textphrases.h"
#include "_gadget/gadget.keyboard.h"

#include "_lua/lunar.h"
/**
 * Proxy Classes
 */
#include "_lua/lua.class.rect.h"
#include "_lua/lua.class.area.h"
#include "_lua/lua.class.border.h"
#include "_lua/lua.class.animation.h"
#include "_lua/lua.class.font.h"
#include "_lua/lua.class.direntry.h"
#include "_lua/lua.class.bitmap.h"
#include "_lua/lua.class.bitmapPort.h"
#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.event.h"
#include "_lua/lua.class.radiogroup.h"
#include "_lua/lua.class.imagefile.h"
#include "_lua/lua.class.dialog.h"
#include "_lua/lua.class.color.h"

#include "_lua/lua.gadget.button.h"
#include "_lua/lua.gadget.checkbox.h"
#include "_lua/lua.gadget.calendar.h"
#include "_lua/lua.gadget.counter.h"
#include "_lua/lua.gadget.colorpicker.h"
#include "_lua/lua.gadget.imagegadget.h"
#include "_lua/lua.gadget.label.h"
#include "_lua/lua.gadget.scrollArea.h"
#include "_lua/lua.gadget.scrollBar.h"
#include "_lua/lua.gadget.progressbar.h"
#include "_lua/lua.gadget.radio.h"
#include "_lua/lua.gadget.select.h"
#include "_lua/lua.gadget.stickybutton.h"
#include "_lua/lua.gadget.textbox.h"
#include "_lua/lua.gadget.textarea.h"
#include "_lua/lua.gadget.window.h"

bool luaL_checkboolean( lua_State* L , int narg )
{
	if( lua_isboolean(L,narg) )
		return lua_toboolean(L,narg);
	if( lua_isnumber(L,narg) )
		return lua_tonumber(L,narg);
	luaL_argerror(L,narg,"boolean expected");
	return false;
}

bool luaL_optboolean( lua_State* L , int narg , bool val )
{
	if( lua_isboolean(L,narg) )
		return lua_toboolean(L,narg);
	if( lua_isnumber(L,narg) )
		return lua_tonumber(L,narg);
	return val;
}

bool luaL_is( lua_State* L , int narg , string type )
{
	void *p = lua_touserdata(L, narg);
	if (p != NULL) {  /* value is a userdata? */
		if (lua_getmetatable(L, narg)) {  /* does it have a metatable? */
			luaL_getmetatable(L, type.c_str());  /* get correct metatable */
			if (lua_rawequal(L, -1, -2)) {  /* does it have the correct mt? */
				lua_pop(L, 2);  /* remove both metatables */
				return true;
			}
			lua_pop(L, 2);  /* remove both metatables */
		}
	}
	return false;
}

_style luaL_optstyle( lua_State* L , int narg , _style style )
{
	if( lua_isstring( L , narg ) )
		applyString2style( style , luaL_checkstring( L , narg ) );
	return style;
}

_pixel luaL_optcolor( lua_State* L , int narg , _pixel color )
{
	if( lua_isnumber( L , narg ) )
		return lua_tointeger( L , narg );
	if( lua_isstring( L , narg ) )
		return string2color[ lua_tolstring( L , narg , nullptr ) ];
	return color;
}

_pixel luaL_checkcolor( lua_State* L , int narg )
{
	if( lua_isnumber( L , narg ) )
		return lua_tointeger( L , narg );
	if( lua_isstring( L , narg ) )
		return string2color[ lua_tolstring( L , narg , nullptr ) ];
	luaL_checkint( L , narg );
	return 0;
}

int _progLua::lua_keyboardIsRegistered( lua_State* L ){ lua_pushboolean( L , _system::_keyboard_ != nullptr ); return 1; }
int _progLua::lua_keyboardIsOpened( lua_State* L ){ if( !_system::_keyboard_ ) return 0; lua_pushboolean( L , _system::_keyboard_->isOpened() ); return 1; }
int _progLua::lua_keyboardOpen( lua_State* L ){ if( !_system::_keyboard_ ) return 0; _system::_keyboard_->open(); return 0; }
int _progLua::lua_keyboardClose( lua_State* L ){ if( !_system::_keyboard_ ) return 0; _system::_keyboard_->close(); return 1; }

int _progLua::lua_writeDebug( lua_State* L ){ _system::debug( luaL_checkstring( L , 1 ) ); return 1; }
int _progLua::lua_pushEvent( lua_State* L ){ _lua_event* event = Lunar<_lua_event>::check( L , 1 ); if( event ) _system::generateEvent( _event( *event ) ); return 0; }
int _progLua::lua_getCurrentFocus( lua_State* L ){ if( !_system::_currentFocus_ ) return 0; Lunar<_lua_gadget>::push( L , new _lua_gadget( _system::_currentFocus_ ) ); return 1; }
int _progLua::lua_getLocalizedString( lua_State* L ){ lua_pushstring( L , _system::getLocalizedString( luaL_checkstring( L , 1 ) ).c_str() ); return 1; }
int _progLua::lua_addChild( lua_State* L ){ _lua_gadget* g = _lua_gadget::getLuaGadget(L,1); if( !g ) return 0; _system::_gadgetHost_->addChild( g->getGadget() ); return 0; }
int _progLua::lua_executeTimer( lua_State* L ){ _system::executeTimer( _luaCallback( L , 1 ) , luaL_checkint( L , 2 ) , luaL_optboolean( L , 3 , false ) ); return 0; }
int _progLua::lua_terminateTimer( lua_State* L ){ _system::terminateTimer( _luaCallback( L , 1 ) ); return 0; }
int _progLua::lua_readRegistryIndex( lua_State* L ){ lua_pushstring( L , _system::_registry_->readIndex( luaL_checkstring( L , 1 ) , luaL_checkstring( L , 2 ) ).c_str() ); return 1; }
int _progLua::lua_writeRegistryIndex( lua_State* L ){ _system::_registry_->writeIndex( luaL_checkstring( L , 1 ) , luaL_checkstring( L , 2 ) , luaL_checkstring( L , 3 ) ); return 0; }
int _progLua::lua_deleteRegistryIndex( lua_State* L ){ _system::_registry_->deleteIndex( luaL_checkstring( L , 1 ) , luaL_checkstring( L , 2 ) ); return 0; }
int _progLua::lua_deleteRegistrySection( lua_State* L ){ _system::_registry_->deleteSection( luaL_checkstring( L , 1 ) ); return 0; }
int _progLua::lua_getFont( lua_State* L ){ Lunar<_lua_font>::push( L , new _lua_font( _system::getFont( luaL_checkstring( L , 1 ) ) ) ); return 1; }
int _progLua::lua_sizeChangePhrase( lua_State* L ){ lua_pushstring( L , stringIntegrator::sizeChangePhrase( luaL_checkint( L , 1 ) ).c_str() ); return 1; }
int _progLua::lua_colorChangePhrase( lua_State* L ){ lua_pushstring( L , stringIntegrator::colorChangePhrase( luaL_checkcolor( L , 1 ) ).c_str() ); return 1; }
int _progLua::lua_fontChangePhrase( lua_State* L ){
	_lua_font* ft = Lunar<_lua_font>::check( L , 1 );
	if( !ft || !ft->font )
		lua_pushstring( L , "" );
	else
		lua_pushstring( L , stringIntegrator::fontChangePhrase( ft->font ).c_str() );
	return 1;
}
int _progLua::lua_RGB( lua_State* L ){ lua_pushnumber( L , RGB( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) ) ); return 1; }
int _progLua::lua_RGBA( lua_State* L ){ lua_pushnumber( L , RGBA( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkboolean( L , 4 ) ) ); return 1; }
int _progLua::lua_RGB_GETR( lua_State* L ){ lua_pushnumber( L , RGB_GETR( luaL_checkcolor( L , 1 ) ) ); return 1; }
int _progLua::lua_RGB_GETG( lua_State* L ){ lua_pushnumber( L , RGB_GETG( luaL_checkcolor( L , 1 ) ) ); return 1; }
int _progLua::lua_RGB_GETB( lua_State* L ){ lua_pushnumber( L , RGB_GETB( luaL_checkcolor( L , 1 ) ) ); return 1; }
int _progLua::lua_RGB_GETA( lua_State* L ){ lua_pushboolean( L , RGB_GETA( luaL_checkcolor( L , 1 ) ) ); return 1; }
int _progLua::lua_exit( lua_State* L ){ _progLua* prog = static_cast<_progLua*>(lua_touserdata(L,lua_upvalueindex(1))); if( prog ) prog->terminate(); return 0; }
int _progLua::lua_requirePackage( lua_State* L )
{
	string name = luaL_checkstring( L , 1 );
	
	//! Register Base Classes
	if( name == "_rect" ) 				Lunar<_lua_rect>::Register( L );
	else if( name == "_border" )		Lunar<_lua_border>::Register( L );
	else if( name == "_color" )			Lunar<_lua_color>::Register( L );
	else if( name == "_dialog" )		Lunar<_lua_dialog>::Register( L );
	else if( name == "_animation" )		Lunar<_lua_animation>::Register( L );
	else if( name == "_font" )			Lunar<_lua_font>::Register( L );
	else if( name == "_direntry" )		Lunar<_lua_direntry>::Register( L );
	else if( name == "_area" ) 			Lunar<_lua_area>::Register( L );
	else if( name == "_bitmap" ) 		Lunar<_lua_bitmap>::Register( L );
	else if( name == "_radiogroup" ) 	Lunar<_lua_radiogroup>::Register( L );
	else if( name == "_bitmapPort" )	Lunar<_lua_bitmapPort>::Register( L );
	else if( name == "_event" ) 		Lunar<_lua_event>::Register( L );
	else if( name == "_imagefile" ) 	Lunar<_lua_imagefile>::Register( L );
	else if( name == "_gadget" ) 		Lunar<_lua_gadget>::Register( L );
	
	//! Register Gadgets
	else if( name == "_window" ) 		Lunar<_lua_window>::Register( L );
	else if( name == "_calendar" ) 		Lunar<_lua_calendar>::Register( L );
	else if( name == "_counter" ) 		Lunar<_lua_counter>::Register( L );
	else if( name == "_colorpicker" ) 	Lunar<_lua_colorpicker>::Register( L );
	else if( name == "_imagegadget" )	Lunar<_lua_imagegadget>::Register( L );
	else if( name == "_button" ) 		Lunar<_lua_button>::Register( L );
	else if( name == "_select" ) 		Lunar<_lua_select>::Register( L );
	else if( name == "_checkbox" ) 		Lunar<_lua_checkbox>::Register( L );
	else if( name == "_label" ) 		Lunar<_lua_label>::Register( L );
	else if( name == "_radio" ) 		Lunar<_lua_radio>::Register( L );
	else if( name == "_scrollarea" )	Lunar<_lua_scrollarea>::Register( L );
	else if( name == "_stickybutton" )	Lunar<_lua_stickybutton>::Register( L );
	else if( name == "_scrollbar" )		Lunar<_lua_scrollbar>::Register( L );
	else if( name == "_progressbar" )	Lunar<_lua_progressbar>::Register( L );
	else if( name == "_textbox" ) 		Lunar<_lua_textbox>::Register( L );
	else if( name == "_textarea" ) 		Lunar<_lua_textarea>::Register( L );
	else{
		// Return false
		lua_pushboolean( L , false );
		return 1;
	}
	
	// Return true
	lua_pushboolean( L , true );
	return 0;
}

luaL_Reg _progLua::windowsLibrary[] = {
	{"rgb",						lua_RGB},
	{"rgba",					lua_RGBA},
	{"getRed",					lua_RGB_GETR},
	{"getGreen",				lua_RGB_GETG},
	{"getBlue",					lua_RGB_GETB},
	{"getAlpha",				lua_RGB_GETA},
	{"addChild",				lua_addChild},
	{"getCurrentFocus",			lua_getCurrentFocus},
	{"readRegistryIndex",		lua_readRegistryIndex},
	{"getLocalizedString",		lua_getLocalizedString},
	{"writeRegistryIndex",		lua_writeRegistryIndex},
	{"deleteRegistryIndex",		lua_deleteRegistryIndex},
	{"deleteRegistrySection",	lua_deleteRegistrySection},
	{"getFont",					lua_getFont},
	{"sizeChangePhrase",		lua_sizeChangePhrase},
	{"colorChangePhrase",		lua_colorChangePhrase},
	{"fontChangePhrase",		lua_fontChangePhrase},
	{"require",					lua_requirePackage},
	{"executeTimer",			lua_executeTimer},
	{"terminateTimer",			lua_terminateTimer},
	{"keyboardIsRegistered",	lua_keyboardIsRegistered},
	{"keyboardIsOpened",		lua_keyboardIsOpened},
	{"keyboardOpen",			lua_keyboardOpen},
	{"keyboardClose",			lua_keyboardClose},
	{"debug",					lua_writeDebug},
	{"pushEvent",				lua_pushEvent},
	{ NULL , NULL }
};

/**
 * Programm Stuff
 */
_progLua::_progLua( string&& prog ) : 
	_program( _programType::progLua ) ,
	content( new string( move(prog) ) )
{}

//! Loads system.*
void _progLua::registerSystem()
{
	lua_newtable( this->state );
	for( luaL_Reg* lib = windowsLibrary ; lib->func ; lib++ )
	{
		lua_pushstring( this->state , lib->name );
		lua_pushcfunction( this->state , lib->func );
		lua_settable( this->state , -3 );
	}
	
	//! Generate system.exit function
	lua_pushstring( this->state , "exit" );
	lua_pushlightuserdata( this->state , this );
	lua_pushcclosure( this->state , lua_exit , 1 );
	lua_settable( this->state , -3 );
	
	//! Set as global "system"
	lua_setglobal( this->state , "system" );
}

void _progLua::internalMain( _cmdArgs&& args )
{
	if( !this->content )
		return;
	
	// Create State
	this->state = luaL_newstate();
	
	
	// Load our lua-piece
	luaL_loadstring( this->state , this->content->c_str() );
	
	
	// _progLua.content is just there for passing the 'content' of the program to the mainFunction
	delete this->content;
	this->content = nullptr;
	
	
	if( lua_isstring( this->state , -1 ) ){
		_system::debug( "Lua-Parser-Error: %s" , lua_tostring( this->state , -1 ) );
		return;
	}	
	
	
	// Open standard functions like math, table-functions etc...
	luaL_openlibs( this->state );
	
	
	// Load system.*
	this->registerSystem();
	
	
	// Parse Whole Program
	if( lua_pcall( this->state , 0 , 0 , 0 ) ){
		_system::debug( "Lua-Parser-Error: %s" , lua_tostring( this->state , -1 ) );
	}
	
	lua_getglobal( this->state , "main" );
	
	if( lua_isfunction( this->state , -1 ) )
	{
		// Create Table for arguments
		lua_createtable( this->state , args.size() , 0 );
		int top = lua_gettop( this->state );
		
		// Push Arguments
		int i = 0;
		for( _cmdArgs::iterator it = args.begin() ; it != args.end() ; ++it )
		{
			lua_pushstring( this->state , it->c_str() ); // Key
			lua_rawseti( this->state , top , ++i );
		}
		
		if( lua_pcall( this->state , 1 /* One Argument */ , 0 , 0 ) )
			_system::debug( "Lua-Error in main(): %s", lua_tostring( this->state , -1 ) );
	}
}

void _progLua::internalVbl()
{
	// Collect garbage!
	// the Lua Garbage collector uses constant 12% cpu, no matter how many programs we have
	lua_gc( this->state , LUA_GCSTEP , max( 1 , 100 / int(_system::_programs_.size()) ) );
}

_progLua::~_progLua()
{
	if( this->content )
		delete this->content;
	
	// Remove any timers that refer to this state
	_system::terminateTimer( _luaCallback( this->state ) , true );
	
	// Collect Garbage
	lua_close( this->state );
}