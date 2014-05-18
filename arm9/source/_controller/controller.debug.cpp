#include "_controller/controller.debug.h"
#include "_dialog/dialog.runtimeerror.h"
#include "_controller/controller.localization.h"

extern "C" void scanKeys();
extern "C" void consoleDemoInit();

bool _debugController::init(){
	debugFile = new _direntry("%WINDIR%/debug.txt");
	debugFile->create();
	debugFile->openwrite();
	return true;
}

void _debugController::end(){
	luaErrorDialogs.clear();
	debugFile = nullptr;
}


void _debugController::luaErrorHandler( lua_State* L , const char* message , ... )
{
	_uniquePtr<_runtimeErrorDialog>& dialogRef = _debugController::luaErrorDialogs[L];
	
	// If there is already a dialog: return, we only allow one dialog per state
	if( dialogRef )
		return;
	
	char output[521];
	
	// Format right
	va_list args;
	va_start( args , message);
	vsnprintf( output , 512 , message , args);
	va_end( args );
	
	// Create error-dialog that will destroy itself once it finished
	dialogRef = new _runtimeErrorDialog( _localizationController::getBuiltInString( "lbl_lua_parser_error" ) , output );
	dialogRef->setCallback( make_inline_callback<void(_dialogResult)>( [&dialogRef]( _dialogResult )->void{ dialogRef = nullptr; } ) );
	dialogRef->execute();
}

void _debugController::debug( const char* fmt , ... )
{
	if( !fmt )
		return;
	
	static char output[256];

	// Declare a va_list type variable
	va_list args;						// Initialise the va_list variable with the ... after fmt
	va_start( args , fmt );
	vsprintf( output , fmt , args );	// Forward the '...' to sprintf
	va_end( args );						// Clean up the va_list
	
	// Enhance the message!
	string result = string( _time::now() ) + ": " + string( output ) + "\r\n";
	
	// Debug to screen
	printf( "%s" , result.c_str() );
	
	// Debug to file
	if( debugFile )
		debugFile->writeString( result );
}

void _debugController::vdebug( const char* fmt , va_list args )
{
	if( !fmt )
		return;
	
	static char output[256];

    // Forward the '...' to sprintf
    vsprintf(output, fmt , args);
	
	// Enhance the message!
	string result = string( _time::now() ) + ": " + string( output ) + "\r\n";
	
	// Debug to screen
	printf( "%s" , result.c_str() );
	
	// Debug to file
	if( debugFile )
		debugFile->writeString( move(result) );
}

void _debugController::submit(){
	while( true )
	{
		scanKeys();
		if( _hardwareKeyPattern(_hardwareKeyPattern::getNewInputs()).isKeySet( _key::a ) )
			break;
	}
}

_map<lua_State*,_uniquePtr<_runtimeErrorDialog>>	_debugController::luaErrorDialogs;
_uniquePtr<_direntry>								_debugController::debugFile;