#include "_type/type.program.lua.h"
#include "_type/type.system.h"
#include "_type/type.callback.h"
#include "_type/type.callback.derives.h"
#include "_type/type.text.phrases.h"
#include "_type/type.tokenizer.h"
#include "_gadget/gadget.keyboard.h"

#include "_lua/lua.lunar.h"
#include "_lua/lua.func.h"
using namespace _luafunc;
/**
 * Proxy Classes
 */
#include "_lua/lua.class.rect.h"
#include "_lua/lua.class.area.h"
#include "_lua/lua.class.border.h"
#include "_lua/lua.class.time.h"
#include "_lua/lua.class.dialogs.h"
#include "_lua/lua.class.animation.h"
#include "_lua/lua.class.font.h"
#include "_lua/lua.class.direntry.h"
#include "_lua/lua.class.bitmap.h"
#include "_lua/lua.class.bitmap.port.h"
#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.event.h"
#include "_lua/lua.class.radiogroup.h"
#include "_lua/lua.class.menu.h"
#include "_lua/lua.class.menu.rule.h"
#include "_lua/lua.class.imagefile.h"
#include "_lua/lua.class.dialogs.h"
#include "_lua/lua.class.color.h"
#include "_lua/lua.class.timer.h"

#include "_lua/lua.gadget.button.h"
#include "_lua/lua.gadget.checkbox.h"
#include "_lua/lua.gadget.calendar.h"
#include "_lua/lua.gadget.clock.h"
#include "_lua/lua.gadget.counter.h"
#include "_lua/lua.gadget.colorpicker.h"
#include "_lua/lua.gadget.button.image.h"
#include "_lua/lua.gadget.button.sticky.h"
#include "_lua/lua.gadget.image.h"
#include "_lua/lua.gadget.label.h"
#include "_lua/lua.gadget.scrollArea.h"
#include "_lua/lua.gadget.scrollBar.h"
#include "_lua/lua.gadget.progressbar.h"
#include "_lua/lua.gadget.radio.h"
#include "_lua/lua.gadget.select.h"
#include "_lua/lua.gadget.slider.h"
#include "_lua/lua.gadget.textbox.h"
#include "_lua/lua.gadget.textarea.h"
#include "_lua/lua.gadget.window.h"
#include "_lua/lua.gadget.window.bar.h"
#include "_lua/lua.gadget.window.menu.h"
#include "_lua/lua.gadget.resizehandle.h"

_vector<_tuple<_literal,_literal,void(*)(lua_State*)>>	luaClasses = {
	make_tuple( "System" , "Animation" , &Lunar<_lua_animation>::install ),
	make_tuple( "System" , "Direntry" , &Lunar<_lua_direntry>::install ),
	make_tuple( "System" , "Event" , &Lunar<_lua_event>::install ),
	make_tuple( "System" , "ImageFile" , &Lunar<_lua_imagefile>::install ),
	make_tuple( "System" , "Timer" , &Lunar<_lua_timer>::install ),
	make_tuple( "System" , "Time" , &Lunar<_lua_time>::install ),
	make_tuple( "System" , "Menu" , &Lunar<_lua_menu>::install ),
	make_tuple( "System" , "MenuRule" , &Lunar<_lua_menurule>::install ),
	make_tuple( "Drawing" , "Area" , &Lunar<_lua_area>::install ),
	make_tuple( "Drawing" , "Bitmap" , &Lunar<_lua_bitmap>::install ),
	make_tuple( "Drawing" , "BitmapPort" , &Lunar<_lua_bitmapport>::install ),
	make_tuple( "Drawing" , "Rect" , &Lunar<_lua_rect>::install ),
	make_tuple( "Drawing" , "Border" , &Lunar<_lua_border>::install ),
	make_tuple( "Drawing" , "Color" , &Lunar<_lua_color>::install ),
	make_tuple( "Drawing" , "Font" , &Lunar<_lua_font>::install ),
	make_tuple( "Dialog" , "YesNoDialog" , &Lunar<_lua_yesnodialog>::install ),
	make_tuple( "Dialog" , "EnterTextDialog" , &Lunar<_lua_entertextdialog>::install ),
	make_tuple( "Dialog" , "ImageDialog" , &Lunar<_lua_imagedialog>::install ),
	make_tuple( "Dialog" , "ErrorDialog" , &Lunar<_lua_errordialog>::install ),
	make_tuple( "Dialog" , "WarningDialog" , &Lunar<_lua_warningdialog>::install ),
	make_tuple( "Dialog" , "InfoDialog" , &Lunar<_lua_infodialog>::install ),
	make_tuple( "UI" , "RadioGroup" , &Lunar<_lua_radiogroup>::install ),
	make_tuple( "UI" , "Gadget" , &Lunar<_lua_gadget>::install ),
	make_tuple( "UI" , "Window" , &Lunar<_lua_window>::install ),
	make_tuple( "UI" , "WindowBar" , &Lunar<_lua_windowbar>::install ),
	make_tuple( "UI" , "WindowMenu" , &Lunar<_lua_windowmenu>::install ),
	make_tuple( "UI" , "Calendar" , &Lunar<_lua_calendar>::install ),
	make_tuple( "UI" , "Counter" , &Lunar<_lua_counter>::install ),
	make_tuple( "UI" , "ColorPicker" , &Lunar<_lua_colorpicker>::install ),
	make_tuple( "UI" , "ImageGadget" , &Lunar<_lua_imagegadget>::install ),
	make_tuple( "UI" , "ImageButton" , &Lunar<_lua_imagebutton>::install ),
	make_tuple( "UI" , "Button" , &Lunar<_lua_button>::install ),
	make_tuple( "UI" , "Select" , &Lunar<_lua_select>::install ),
	make_tuple( "UI" , "CheckBox" , &Lunar<_lua_checkbox>::install ),
	make_tuple( "UI" , "Label" , &Lunar<_lua_label>::install ),
	make_tuple( "UI" , "Slider" , &Lunar<_lua_slider>::install ),
	make_tuple( "UI" , "Radio" , &Lunar<_lua_radio>::install ),
	make_tuple( "UI" , "ResizeHandle" , &Lunar<_lua_resizehandle>::install ),
	make_tuple( "UI" , "ScrollArea" , &Lunar<_lua_scrollarea>::install ),
	make_tuple( "UI" , "StickyButton" , &Lunar<_lua_stickybutton>::install ),
	make_tuple( "UI" , "ScrollBar" , &Lunar<_lua_scrollbar>::install ),
	make_tuple( "UI" , "PrograssBar" , &Lunar<_lua_progressbar>::install ),
	make_tuple( "UI" , "TextBox" , &Lunar<_lua_textbox>::install ),
	make_tuple( "UI" , "TextArea" , &Lunar<_lua_textarea>::install ),
	make_tuple( "UI" , "Clock" , &Lunar<_lua_clockgadget>::install )
};

int _progLua::lua_keyboardIsRegistered( lua_State* L ){ push( L , _system::_keyboard_ != nullptr ); return 1; }
int _progLua::lua_keyboardIsOpened( lua_State* L ){ if( !_system::_keyboard_ ) return 0; push( L , _system::_keyboard_->isOpened() ); return 1; }
int _progLua::lua_keyboardOpen( lua_State* L ){ if( !_system::_keyboard_ ) return 0; _system::_keyboard_->open(); return 0; }
int _progLua::lua_keyboardClose( lua_State* L ){ if( !_system::_keyboard_ ) return 0; _system::_keyboard_->close(); return 1; }

int _progLua::lua_writeDebug( lua_State* L ){ _system::debug( check<string>( L , 1 ).c_str() ); return 1; }
int _progLua::lua_pushEvent( lua_State* L ){ _lua_event* event = Lunar<_lua_event>::check( L , 1 ); if( event ) _system::generateEvent( _event( *event ) , _eventCallType::normal ); return 0; }
int _progLua::lua_getCurrentFocus( lua_State* L ){ if( !_system::_currentFocus_ ) return 0; Lunar<_lua_gadget>::push( L , new _lua_gadget( _system::_currentFocus_ ) ); return 1; }
int _progLua::lua_getLocalizedString( lua_State* L ){ push( L , _system::getLocalizedString( check<string>( L , 1 ) ).c_str() ); return 1; }
int _progLua::lua_addChild( lua_State* L ){ _system::_gadgetHost_->addChild( check<_gadget*>( L , 1 ) ); return 0; }
int _progLua::lua_readRegistryIndex( lua_State* L ){ return push( L , _system::getRegistry().readIndex( check<string>( L , 1 ) , check<string>( L , 2 ) ).c_str() ); }
int _progLua::lua_writeRegistryIndex( lua_State* L ){ _system::getRegistry().writeIndex( check<string>( L , 1 ) , check<string>( L , 2 ) , check<string>( L , 3 ) ); return 0; }
int _progLua::lua_deleteRegistryIndex( lua_State* L ){ _system::getRegistry().deleteIndex( check<string>( L , 1 ) , check<string>( L , 2 ) ); return 0; }
int _progLua::lua_deleteRegistrySection( lua_State* L ){ _system::getRegistry().deleteSection( check<string>( L , 1 ) ); return 0; }
int _progLua::lua_getFont( lua_State* L ){
	return push( L , lua_isstring( L , 1 ) ? _system::getFont( lua_tostring( L , 1 ) ) : _system::getFont() );
}
int _progLua::lua_sizeChangePhrase( lua_State* L ){ push( L , stringIntegrator::sizeChangePhrase( check<int>( L , 1 ) ).c_str() ); return 1; }
int _progLua::lua_colorChangePhrase( lua_State* L ){ push( L , stringIntegrator::colorChangePhrase( check<_pixel>( L , 1 ) ).c_str() ); return 1; }
int _progLua::lua_fontChangePhrase( lua_State* L ){
	const _font* ft = check<const _font*>( L , 1 );
	if( !ft )
		push( L , "" );
	else
		push( L , stringIntegrator::fontChangePhrase( ft ) );
	return 1;
}
int _progLua::lua_RGB( lua_State* L ){ return push( L , RGB( check<int>( L , 1 ) , check<int>( L , 2 ) , check<int>( L , 3 ) , lightcheck<bool>( L , 4 , true ) ) ); }
int _progLua::lua_RGB255( lua_State* L ){ return push( L , RGB255( check<int>( L , 1 ) , check<int>( L , 2 ) , check<int>( L , 3 ) , lightcheck<bool>( L , 4 , true ) ) ); }
int _progLua::lua_RGB_GETR( lua_State* L ){ return push( L , RGB_GETR( check<_pixel>( L , 1 ) ) ); }
int _progLua::lua_RGB_GETG( lua_State* L ){ return push( L , RGB_GETG( check<_pixel>( L , 1 ) ) ); }
int _progLua::lua_RGB_GETB( lua_State* L ){ return push( L , RGB_GETB( check<_pixel>( L , 1 ) ) ); }
int _progLua::lua_RGB_GETA( lua_State* L ){ return push( L , RGB_GETA( check<_pixel>( L , 1 ) ) ); }
int _progLua::lua_exit( lua_State* L ){ _progLua* prog = static_cast<_progLua*>(lua_touserdata(L,lua_upvalueindex(1))); if( prog ) prog->terminate(); return 0; }
int _progLua::lua_usingClass( lua_State* L )
{
	string classPath = check<string>( L , 1 );
	
	string token;
	_vector<string> tokens;
	_tokenizer tok = _tokenizer( classPath , token , " ." , true );
	
	while( tok.next() )
		tokens.push_back( move(token) );
	
	// Flag
	bool found = false;
	
	for( auto data : luaClasses )
	{
		if( tokens[0] == std::get<0>(data) && ( tokens[1] == "*" || tokens[1] == std::get<1>(data) ) )
		{
			// Call registration
			(* std::get<2>(data) )( L );
			
			// Indicate that at least one class was registered
			found = true;
		}
	}
	
	// Return Result
	push( L , found );
	
	return 1;
}

luaL_Reg _progLua::windowsLibrary[] = {
	{"rgb",						lua_RGB},
	{"rgb255",					lua_RGB255},
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
	{"require",					lua_usingClass},
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
_progLua::_progLua( string prog ) : 
	_program( _programType::progLua )
	, content( new string( move(prog) ) )
{}

//! Loads System.*
void _progLua::registerSystem()
{
	lua_newtable( this->state );
	for( luaL_Reg* lib = windowsLibrary ; lib->func ; lib++ )
	{
		push( this->state , lib->name );
		lua_pushcfunction( this->state , lib->func );
		lua_settable( this->state , -3 );
	}
	
	//! Generate System.exit function
	push( this->state , "exit" );
	lua_pushlightuserdata( this->state , this );
	lua_pushcclosure( this->state , lua_exit , 1 );
	lua_settable( this->state , -3 );
	
	//! Set as global "system"
	lua_setglobal( this->state , "System" );
	
	//! Create 'using' function
	lua_pushcfunction( this->state , lua_usingClass );
	lua_setglobal( this->state , "using" );
}

void _progLua::internalMain( _cmdArgs args )
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
		_luafunc::errorHandler( this->state , lua_tostring( state , -1 ) );
		return;
	}
	
	// Open standard functions like math, table-functions etc...
	luaL_openlibs( this->state );
	
	// Load system.*
	this->registerSystem();
	
	// Parse Whole Program
	if( lua_pcall( this->state , 0 , 0 , 0 ) )
		_luafunc::errorHandler( this->state , lua_tostring( state , -1 ) );
	
	lua_getglobal( this->state , "main" );
	
	if( lua_isfunction( this->state , -1 ) )
	{
		// Push Arguments
		for( auto val : args )
			push( this->state , val );
		
		if( lua_pcall( this->state , args.size() /* One Argument */ , 0 , 0 ) )
			_luafunc::errorHandler( this->state , lua_tostring( state , -1 ) );
	}
	else
		lua_pop( this->state , 2 );
}

void _progLua::internalVbl()
{
	// Collect garbage!
	// the Lua Garbage collector uses constant 12% cpu, no matter how many programs we have
	lua_gc( this->state , LUA_GCSTEP , max( 1 , 100 / int(_program::getRunningPrograms().size()) ) );
}

_progLua::~_progLua()
{
	if( this->content )
		delete this->content;
	
	// Collect Garbage
	lua_close( this->state );
}