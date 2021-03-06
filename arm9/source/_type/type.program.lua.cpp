#include <_type/type.program.lua.h>
#include <_type/type.callback.h>
#include <_type/type.callback.derives.h>
#include <_type/type.tokenizer.h>
#include <_type/type.cwdchanger.h>
#include <_screen/screen.keyboard.h>
#include <_controller/controller.gui.h>
#include <_controller/controller.registry.h>
#include <_controller/controller.localization.h>
#include <_controller/controller.font.h>
#include <_controller/controller.event.h>
#include <_controller/controller.execution.h>

#include <_lua/lua.lunar.h>
#include <_lua/lua.func.h>
using namespace _luafunc;

/**
 * Proxy Classes
 */
#include <_lua/lua.class.rect.h>
#include <_lua/lua.class.area.h>
#include <_lua/lua.class.border.h>
#include <_lua/lua.class.time.h>
#include <_lua/lua.class.dialogs.h>
#include <_lua/lua.class.animation.h>
#include <_lua/lua.class.font.h>
#include <_lua/lua.class.direntry.h>
#include <_lua/lua.class.bitmap.h>
#include <_lua/lua.class.bitmap.port.h>
#include <_lua/lua.class.gadget.h>
#include <_lua/lua.class.event.h>
#include <_lua/lua.class.radiogroup.h>
#include <_lua/lua.class.mime.h>
#include <_lua/lua.class.menu.h>
#include <_lua/lua.class.menu.rule.h>
#include <_lua/lua.class.imagefile.h>
#include <_lua/lua.class.dialogs.h>
#include <_lua/lua.class.color.h>
#include <_lua/lua.class.timer.h>
#include <_lua/lua.class.ini.h>
#include <_lua/lua.class.ini.file.h>

#include <_lua/lua.controller.font.h>
#include <_lua/lua.controller.gui.h>

#include <_lua/lua.gadget.button.h>
#include <_lua/lua.gadget.checkbox.h>
#include <_lua/lua.gadget.calendar.h>
#include <_lua/lua.gadget.clock.h>
#include <_lua/lua.gadget.contextmenu.h>
#include <_lua/lua.gadget.counter.h>
#include <_lua/lua.gadget.colorpicker.gradient.h>
#include <_lua/lua.gadget.colorpicker.hexagon.h>
#include <_lua/lua.gadget.button.image.h>
#include <_lua/lua.gadget.button.sticky.h>
#include <_lua/lua.gadget.image.h>
#include <_lua/lua.gadget.label.h>
#include <_lua/lua.gadget.scrollArea.h>
#include <_lua/lua.gadget.scrollBar.h>
#include <_lua/lua.gadget.popup.h>
#include <_lua/lua.gadget.progressbar.h>
#include <_lua/lua.gadget.radio.h>
#include <_lua/lua.gadget.select.h>
#include <_lua/lua.gadget.slider.h>
#include <_lua/lua.gadget.taskinfo.h>
#include <_lua/lua.gadget.textbox.h>
#include <_lua/lua.gadget.textarea.h>
#include <_lua/lua.gadget.window.h>
#include <_lua/lua.gadget.window.bar.h>
#include <_lua/lua.gadget.window.mainframe.h>
#include <_lua/lua.gadget.window.menu.h>
#include <_lua/lua.gadget.resizehandle.h>

_vector<_tuple<_literal,_literal,void(*)(lua_State*)>>	luaClasses = {
	make_tuple( "Controller" , "Font" , &Lunar<_lua_fontcontroller>::install ),
	make_tuple( "Controller" , "Gui" , &Lunar<_lua_guicontroller>::install ),
	make_tuple( "System" , "Animation" , &Lunar<_lua_animation>::install ),
	make_tuple( "System" , "Direntry" , &Lunar<_lua_direntry>::install ),
	make_tuple( "System" , "Event" , &Lunar<_lua_event>::install ),
	make_tuple( "System" , "ImageFile" , &Lunar<_lua_imagefile>::install ),
	make_tuple( "System" , "Timer" , &Lunar<_lua_timer>::install ),
	make_tuple( "System" , "Time" , &Lunar<_lua_time>::install ),
	make_tuple( "System" , "Menu" , &Lunar<_lua_menu>::install ),
	make_tuple( "System" , "MenuRule" , &Lunar<_lua_menurule>::install ),
	make_tuple( "System" , "Ini" , &Lunar<_lua_ini>::install ),
	make_tuple( "System" , "IniFile" , &Lunar<_lua_inifile>::install ),
	make_tuple( "System" , "MimeType" , &Lunar<_lua_mimetype>::install ),
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
	make_tuple( "Dialog" , "FileSaveDialog" , &Lunar<_lua_filesavedialog>::install ),
	make_tuple( "Dialog" , "FileOpenDialog" , &Lunar<_lua_fileopendialog>::install ),
	make_tuple( "Dialog" , "FolderChooseDialog" , &Lunar<_lua_fileopendialog>::install ),
	make_tuple( "UI" , "RadioGroup" , &Lunar<_lua_radiogroup>::install ),
	make_tuple( "UI" , "Gadget" , &Lunar<_lua_gadget>::install ),
	make_tuple( "UI" , "Window" , &Lunar<_lua_window>::install ),
	make_tuple( "UI" , "WindowBar" , &Lunar<_lua_windowbar>::install ),
	make_tuple( "UI" , "WindowMenu" , &Lunar<_lua_windowmenu>::install ),
	make_tuple( "UI" , "Calendar" , &Lunar<_lua_calendar>::install ),
	make_tuple( "UI" , "Counter" , &Lunar<_lua_counter>::install ),
	make_tuple( "UI" , "ContextMenu" , &Lunar<_lua_contextmenu>::install ),
	make_tuple( "UI" , "HexagonColorPicker" , &Lunar<_lua_hexagoncolorpicker>::install ),
	make_tuple( "UI" , "GradientColorPicker" , &Lunar<_lua_gradientcolorpicker>::install ),
	make_tuple( "UI" , "ImageGadget" , &Lunar<_lua_imagegadget>::install ),
	make_tuple( "UI" , "ImageButton" , &Lunar<_lua_imagebutton>::install ),
	make_tuple( "UI" , "MainFrame" , &Lunar<_lua_mainframe>::install ),
	make_tuple( "UI" , "Button" , &Lunar<_lua_button>::install ),
	make_tuple( "UI" , "Popup" , &Lunar<_lua_popup>::install ),
	make_tuple( "UI" , "Select" , &Lunar<_lua_select>::install ),
	make_tuple( "UI" , "CheckBox" , &Lunar<_lua_checkbox>::install ),
	make_tuple( "UI" , "Label" , &Lunar<_lua_label>::install ),
	make_tuple( "UI" , "Slider" , &Lunar<_lua_slider>::install ),
	make_tuple( "UI" , "Radio" , &Lunar<_lua_radio>::install ),
	make_tuple( "UI" , "ResizeHandle" , &Lunar<_lua_resizehandle>::install ),
	make_tuple( "UI" , "ScrollArea" , &Lunar<_lua_scrollarea>::install ),
	make_tuple( "UI" , "StickyButton" , &Lunar<_lua_stickybutton>::install ),
	make_tuple( "UI" , "ScrollBar" , &Lunar<_lua_scrollbar>::install ),
	make_tuple( "UI" , "ProgressBar" , &Lunar<_lua_progressbar>::install ),
	make_tuple( "UI" , "TaskInfo" , &Lunar<_lua_taskinfo>::install ),
	make_tuple( "UI" , "TextBox" , &Lunar<_lua_textbox>::install ),
	make_tuple( "UI" , "TextArea" , &Lunar<_lua_textarea>::install ),
	make_tuple( "UI" , "Clock" , &Lunar<_lua_clockgadget>::install )
};

int _luaProgram::lua_keyboardIsRegistered( lua_State* L ){ push( L , _guiController::getKeyboard() != nullptr ); return 1; }
int _luaProgram::lua_keyboardIsOpened( lua_State* L ){ if( !_guiController::getKeyboard() ) return 0; push( L , _guiController::getKeyboard()->isOpened() ); return 1; }
int _luaProgram::lua_keyboardOpen( lua_State* L ){ if( !_guiController::getKeyboard() ) return 0; _guiController::getKeyboard()->open(); return 0; }
int _luaProgram::lua_keyboardClose( lua_State* L ){ if( !_guiController::getKeyboard() ) return 0; _guiController::getKeyboard()->close(); return 1; }
int _luaProgram::lua_keyboardIsShift( lua_State* L ){ if( !_guiController::getKeyboard() ) return 0; push( L , _guiController::getKeyboard()->isShift() ); return 1; }
int _luaProgram::lua_keyboardIsCaps( lua_State* L ){ if( !_guiController::getKeyboard() ) return 0; push( L , _guiController::getKeyboard()->isCaps() ); return 1; }
int _luaProgram::lua_keyboardSetShift( lua_State* L ){ if( !_guiController::getKeyboard() ) return 0; _guiController::getKeyboard()->setShift( check<bool>( L , 1 ) ); return 0; }
int _luaProgram::lua_keyboardSetCaps( lua_State* L ){ if( !_guiController::getKeyboard() ) return 0; _guiController::getKeyboard()->setCaps( check<bool>( L , 1 ) ); return 0; }

void _luaProgram::lua_print( lua_State* L , _literal str , _u32 len ){ printf("%s",str); }
int _luaProgram::lua_writeDebug( lua_State* L ){ _debugController::debug( check<_literal>( L , 1 ) ); return 0; }
int _luaProgram::lua_pushEvent( lua_State* L ){ _eventController::pushEvent( check<_event>( L , 1 ) , _eventCallType::normal ); return 0; }
int _luaProgram::lua_getCurrentFocus( lua_State* L ){ return push( L , _guiController::getCurrentFocus() ); }
int _luaProgram::lua_getLocalizedString( lua_State* L ){ return push( L , _localizationController::getBuiltInString( check<_literal>( L , 1 ) ) ); }
int _luaProgram::lua_getSystemRegistry( lua_State* L ){ Lunar<_lua_ini>::push( L , new _lua_ini( &_registryController::getSystemRegistry() ) ); return 1; }
int _luaProgram::lua_getUserRegistry( lua_State* L ){ Lunar<_lua_ini>::push( L , new _lua_ini( &_registryController::getUserRegistry() ) ); return 1; }
int _luaProgram::lua_getFont( lua_State* L ){
	return push( L , is_a<_literal>( L , 1 ) ? _fontController::getFont( check<_literal>( L , 1 ) ) : _fontController::getStandardFont() );
}
int _luaProgram::lua_getWorkingDir( lua_State* L ){
	_luaProgram* prog = static_cast<_luaProgram*>(lua_touserdata(L,lua_upvalueindex(1))); if( prog ) return push( L , prog->getCWD() ); return 0;
}
int _luaProgram::lua_getBinaryPath( lua_State* L ){
	_luaProgram* prog = static_cast<_luaProgram*>(lua_touserdata(L,lua_upvalueindex(1))); if( prog ) return push( L , prog->getPath() ); return 0;
}
int _luaProgram::lua_exit( lua_State* L ){
	_luaProgram* prog = static_cast<_luaProgram*>(lua_touserdata(L,lua_upvalueindex(1))); if( prog ) prog->terminate(); return 0;
}
int _luaProgram::lua_getMainFrame( lua_State* L ){
	_luaProgram* prog = static_cast<_luaProgram*>(lua_touserdata(L,lua_upvalueindex(1)));
	if( !prog )
		return 0;
	if( lua_gettop( L ) != 0 )
	{
		if( is_a<bool>( L , 3 ) )
			return push( L , prog->getMainFrame( check<_length>( L , 1 ) , check<_length>( L , 2 ) , check<bool>( L , 3 ) , lightcheck<_style>( L , 4 ) ) );
		return push( L , prog->getMainFrame( check<_length>( L , 1 ) , check<_length>( L , 2 ) , lightcheck<_style>( L , 3 ) ) );
	}
	return push( L , prog->getMainFrame() );
}
int _luaProgram::lua_usingClass( lua_State* L )
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
	
	if( found )
		return 1;
	return luaL_error( L , "The package '%s' is not available." , classPath.c_str() );
}
int _luaProgram::lua_pause( lua_State* L ){
	_debugController::submit();
	return 0;
}
int	_luaProgram::lua_closeService( lua_State* L ){
	return push( L , serviceState2string[ _executionController::closeService( check<_serviceId>( L , 1 ) ) ] );
}
int	_luaProgram::lua_startService( lua_State* L )
{
	_serviceTransfer transferData;
	
	// READ ADDITIONAL DATA
	if( lua_istable( L , 3 ) )
	{
		lua_pushnil( L );  // first key
		while( lua_next( L , 3 ) != 0 )
		{
			// Copy key, in case some function calls lua_tostring, which would break the lua_next-iteration
			lua_pushvalue( L , -2 );
			
			// uses copy of 'key' (at index -1) and 'value' (at index -2)
			_literal key = check<_literal>( L , -1 );
			
			if( strcmp(key,"strData") == 0 || ( is_a<string>( L , -3 ) && !transferData.strData) )
				transferData.strData = check<string>( L , -2 );
			else if( strcmp(key,"bitmapData") == 0 || ( is_a<_bitmap>( L , -3 ) && !transferData.bitmapData ) )
				transferData.bitmapData = check<_bitmap>( L , -2 );
			else if( strcmp(key,"int1Data") == 0 )
				transferData.int1Data = check<_int>( L , -2 );
			else if( strcmp(key,"int2Data") == 0 )
				transferData.int2Data = check<_int>( L , -2 );
			else if( strcmp(key,"float1Data") == 0 )
				transferData.float1Data = check<float>( L , -2 );
			else if( strcmp(key,"float2Data") == 0 )
				transferData.float2Data = check<float>( L , -2 );
			
			// removes 'value' and the copy of 'key' ; keeps original 'key' for next iteration
			lua_pop( L , 2 );
		}
	}
	transferData.arguments = check<_args>( L , 2 );
	return push( L , _executionController::startService( check<_literal>( L , 1 ) , move(transferData) ) );
}
int	_luaProgram::lua_getServiceState( lua_State* L ){
	return push( L , serviceState2string[ _executionController::getServiceState( check<_serviceId>( L , 1 ) ) ] );
}
int	_luaProgram::lua_setServiceHandler( lua_State* L ){
	_registryController::setPackagePath( check<_literal>( L , 1 ) , check<string>( L , 2 ) );
	return 0;
}
int	_luaProgram::lua_getServiceTransferData( lua_State* L ){
	pushTransferData( L , _executionController::getServiceTransferData( check<_serviceId>( L , 1 ) ) );
	return 1;
}
int	_luaProgram::lua_getServiceHandler( lua_State* L ){
	return push( L , _registryController::getPackagePath( check<_literal>( L , 1 ) ) );
}

bool _luaProgram::parseProgramHeader()
{
	_programHeader header;
	
	if( this->content && !this->headParsed )
	{
		wstring			attribute;
		const wstring&	content = *this->content;
		_wtokenizer		tok = _wtokenizer( content , attribute , L"\n\r" , true );
		
		/* Scan through file line by line */
		while( tok.next() )
		{
			if( attribute[0] != '-' || attribute[1] != '-' )
				goto end;
			
			// Find Position of the '='
			size_t equalSignPosition = attribute.find(L'=');
			
			// Check if the '=' was found
			if( equalSignPosition != wstring::npos )
			{
				wstring attrName = attribute.substr( 2 , equalSignPosition - 2 );
				
				// Trim whitespaces and tabs etc...
				trim( attrName );
				
				// No Attribute Name: Continue with next line
				if( attrName.empty() )
					continue;
				
				// Check if the string is long enough to also hold the value
				if( equalSignPosition + 1 >= attribute.length() )
					continue;
				
				wstring attrValue = attribute.substr( equalSignPosition + 1 , wstring::npos );
				
				// Trim whitespaces and tabs etc...
				trim( attrValue );
				
				if( attrName == "FILE_ICON" ){
					_cwdChanger changer = _cwdChanger( this->getPath() ); // Set Relative Path
					_bitmap icon = _imageFile( attrValue.cpp_str() , false ).readBitmap();
					if( icon.isValid() )
						header.fileIcon = move(icon);
				}
				else if( attrName == "WND_ICON" ){
					_cwdChanger changer = _cwdChanger( this->getPath() ); // Set Relative Path
					_bitmap icon = _imageFile( attrValue.cpp_str() , false ).readBitmap();
					if( icon.isValid() )
						header.windowIcon = move(icon);
				}
				else if( attrName == "NAME" )
					header.name = move(attrValue);
				else if( attrName == "FILE_NAME" )
					header.fileName = attrValue.cpp_str();
				else if( attrName == "WND_TITLE" )
					header.windowName = move(attrValue);
				else if( attrName == "AUTHOR" )
					header.author = move(attrValue);
				else if( attrName == "COPYRIGHT" )
					header.copyright = move(attrValue);
				else if( attrName == "VERSION" )
					header.version = move(attrValue);
				else if( attrName == "DESC" )
					header.description = move(attrValue);
				else if( attrName == "LANG" )
					header.language = attrValue.cpp_str();
			}
		}
		
		end:
		
		// Write to class attribute
		this->setHeader( move(header) );
		headParsed = true;
		
		return true;
	}
	return false;
}

luaL_Reg _luaProgram::windowsLibrary[] = {
	{"getCurrentFocus"		, lua_getCurrentFocus },
	{"getLocalizedString"	, lua_getLocalizedString },
	{"getSystemRegistry"	, lua_getSystemRegistry },
	{"getUserRegistry"		, lua_getUserRegistry },
	{"getFont"				, lua_getFont },
	{"keyboardIsRegistered"	, lua_keyboardIsRegistered },
	{"keyboardIsOpened"		, lua_keyboardIsOpened },
	{"keyboardIsShift"		, lua_keyboardIsShift },
	{"keyboardIsCaps"		, lua_keyboardIsCaps },
	{"keyboardOpen"			, lua_keyboardOpen },
	{"keyboardClose"		, lua_keyboardClose },
	{"keyboardSetShift"		, lua_keyboardSetShift },
	{"keyboardSetCaps"		, lua_keyboardSetCaps },
	{"debug"				, lua_writeDebug },
	{"pause"				, lua_pause },
	{"pushEvent"			, lua_pushEvent },
	{"startService"			, lua_startService },
	{"closeService"			, lua_closeService },
	{"getServiceState"		, lua_getServiceState },
	{"setServiceHandler"	, lua_setServiceHandler },
	{"getServiceHandler"	, lua_getServiceHandler },
	{"getServiceData"		, lua_getServiceTransferData },
	{ NULL , NULL }
};

#include <cmath>

/**
 * Programm Stuff
 */
_luaProgram::_luaProgram( string prog ) :
	_program( _programType::lua )
	, content( new string( move(prog) ) )
	, headParsed( false )
{
	lua_setprintfunc( &_luaProgram::lua_print );
	parseProgramHeader();
}

//! Loads System.*
void _luaProgram::registerSystem()
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
	
	//! Generate System.getMainFrame function
		push( this->state , "getMainFrame" );
		lua_pushlightuserdata( this->state , this );
		lua_pushcclosure( this->state , lua_getMainFrame , 1 );
		lua_settable( this->state , -3 );
	
	//! Generate System.getWorkingDir function
		push( this->state , "getWorkingDir" );
		lua_pushlightuserdata( this->state , this );
		lua_pushcclosure( this->state , lua_getWorkingDir , 1 );
		lua_settable( this->state , -3 );
	
	//! Generate System.getBinaryPath function
		push( this->state , "getBinaryPath" );
		lua_pushlightuserdata( this->state , this );
		lua_pushcclosure( this->state , lua_getBinaryPath , 1 );
		lua_settable( this->state , -3 );
	
	
	//! Set as global "system"
	lua_setglobal( this->state , "System" );
	
	//! Create 'using' function
	lua_pushcfunction( this->state , lua_usingClass );
	lua_setglobal( this->state , "using" );
}

void _luaProgram::main( _args args )
{
	if( !this->content )
		return;
	
	// Create State
	this->state = luaL_newstate();
	
	string luaSrcToken = "=" + _direntry( this->getPath() ).getFullName(true);
	
	// Load our lua-piece
	luaL_loadbuffer( this->state , this->content->c_str() , this->content->length() , luaSrcToken.c_str() );
	
	// _luaProgram.content is just there for passing the 'content' of the program to the mainFunction
	delete this->content;
	this->content = nullptr;
	
	if( lua_isstring( this->state , -1 ) ){
		_debugController::luaErrorHandler( this->state , lua_tostring( state , -1 ) );
		return;
	}
	
	// Open standard functions like math, table-functions etc...
	luaL_openlibs( this->state );
	
	// Load system.*
	this->registerSystem();
	
	// Parse Whole Program
	if( lua_pcall( this->state , 0 , 0 , 0 ) )
		_debugController::luaErrorHandler( this->state , lua_tostring( state , -1 ) );
	
	lua_getglobal( this->state , "main" );
	
	if( lua_isfunction( this->state , -1 ) )
	{
		// Push Arguments
		for( auto val : args )
			push( this->state , val );
		
		if( lua_pcall( this->state , args.size() /* One Argument */ , 0 , 0 ) )
			_debugController::luaErrorHandler( this->state , lua_tostring( state , -1 ) );
	}
	else
		lua_pop( this->state , 2 );
}

void _luaProgram::dispatch( _serviceId id , _serviceTransfer args )
{
	lua_getglobal( this->state , "dispatch" );
	
	if( lua_isfunction( this->state , -1 ) )
	{
		if( this->serviceDispatchTableIndex == LUA_NOREF ){
			lua_createtable( this->state , 1 , 0 );		// Create table for all open services
			if( (this->serviceDispatchTableIndex = luaL_ref( state , LUA_REGISTRYINDEX )) == LUA_NOREF )
				return;
		}
		
		lua_createtable( this->state , 1 , 1 );	// Create service data table
		int serviceData = lua_gettop( this->state );
		lua_rawgeti( this->state , LUA_REGISTRYINDEX , this->serviceDispatchTableIndex ); // Get main table of all open services in this state
		lua_pushvalue( this->state , -2 );		// Copy service data
		lua_rawseti( this->state , -2 , id );	// Insert data for service in service table
		lua_pop( this->state , 1 );				// Pop the main Table, which leaves us with the service data
		
		// Push State
		push( this->state , "state" );
		push( this->state , serviceState2string[_serviceState::init] );
		lua_rawset( this->state ,  serviceData );
		
		// Push Arguments
		push( this->state , move(args.arguments) );
		
		// Push Transfer data
		pushTransferData( this->state , move(args) );
		
		if( lua_pcall( this->state , 3 /* 3 Arguments */ , 0 , 0 ) )
			_debugController::luaErrorHandler( this->state , lua_tostring( this->state , -1 ) );
	}
}

void _luaProgram::pushTransferData( lua_State* L , _serviceTransfer data )
{
	// Push Additional data table
	lua_createtable( L , 0 , 6 );	// Create service data table
	
	if( data.strData ){
		push( L , "strData" );
		push( L , data.strData.release() );
		lua_rawset( L , -3 );
	}
	if( data.bitmapData ){
		push( L , "bitmapData" );
		push( L , data.bitmapData.release() );
		lua_rawset( L , -3 );
	}
	push( L , "int1Data" );
	push( L , data.int1Data );
	lua_rawset( L , -3 );
	push( L , "int2Data" );
	push( L , data.int2Data);
	lua_rawset( L , -3 );
	push( L , "float1Data" );
	push( L , data.float1Data);
	lua_rawset( L , -3 );
	push( L , "float2Data" );
	push( L , data.float2Data );
	lua_rawset( L , -3 );
}

void _luaProgram::process( _serviceId id , _serviceState& serviceState )
{
	lua_getglobal( this->state , "dispatch" );
	
	if( lua_isfunction( this->state , -1 ) )
	{
		lua_rawgeti( this->state , LUA_REGISTRYINDEX , this->serviceDispatchTableIndex ); // Get main table of all open services in this state
		lua_rawgeti( this->state , -1 , id ); // Get service data
		
		push( this->state , "state" );
		push( this->state , serviceState2string[serviceState] );
		lua_rawset( this->state , -3 );
		
		if( lua_pcall( this->state , 1 /* One Argument */ , 0 , 0 ) ){
			serviceState = _serviceState::error;
			_debugController::luaErrorHandler( this->state , lua_tostring( this->state , -1 ) );
			return;
		}
		
		lua_rawgeti( this->state , LUA_REGISTRYINDEX , this->serviceDispatchTableIndex ); // Get main table of all open services in this state
		lua_rawgeti( this->state , -1 , id ); // Get service data
		lua_rawgetfield( this->state , -1 , "state" );
		
		serviceState = string2serviceState[ optcheck<string>( this->state , 1 ) ];
		
		lua_pop( this->state , 3 );
	}
	else
		serviceState = _serviceState::error;
}

_serviceTransfer _luaProgram::getServiceData( _serviceId id )
{
	_serviceTransfer transfer;
	
	lua_rawgeti( this->state , LUA_REGISTRYINDEX , this->serviceDispatchTableIndex ); // Get main table of all open services in this state
	lua_rawgeti( this->state , -1 , id ); // Get service data
	
	if( lua_isnil( this->state , -1 ) )
		return _serviceTransfer();
	
	lua_pushnil( this->state );  // first key
	
	while( lua_next( this->state , -1 ) != 0 )
	{
		// Copy key, in case some function calls lua_tostring, which would break the lua_next-iteration
		lua_pushvalue( this->state , -2 );
		
		// uses copy of 'key' (at index -1) and 'value' (at index -2)
		_literal key = check<_literal>( this->state , -1 );
		
		if( strcmp(key,"strData") == 0 )
			transfer.strData = check<string>( this->state , -2 );
		else if( strcmp(key,"bitmapData") == 0 )
			transfer.bitmapData = check<_bitmap>( this->state , -2 );
		else if( strcmp(key,"returnValue") == 0 )
			transfer.returnData = check<_int>( this->state , -2 );
		else if( strcmp(key,"int1Data") == 0 )
			transfer.int1Data = check<_int>( this->state , -2 );
		else if( strcmp(key,"int2Data") == 0 )
			transfer.int2Data = check<_int>( this->state , -2 );
		else if( strcmp(key,"float1Data") == 0 )
			transfer.float1Data = check<float>( this->state , -2 );
		else if( strcmp(key,"float2Data") == 0 )
			transfer.float2Data = check<float>( this->state , -2 );
		
		// removes 'value' and the copy of 'key' ; keeps original 'key' for next iteration
		lua_pop( this->state , 2 );
	}
	
	return move(transfer);
}

void _luaProgram::cleanUp()
{
	if( this->content )
		delete this->content;
	
	// Collect Garbage
	lua_close( this->state );
}

void _luaProgram::frame( _int numRunningPrograms )
{
	// Collect garbage!
	// the Lua Garbage collector uses roughly ~ 12% cpu, no matter how many programs we have
	lua_gc( this->state , LUA_GCSTEP , max( 1 , 100 / numRunningPrograms ) );
}