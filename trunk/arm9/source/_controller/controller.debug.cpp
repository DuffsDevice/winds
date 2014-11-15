#include <_controller/controller.debug.h>
#include <_dialog/dialog.runtimeerror.h>
#include <_controller/controller.localization.h>

extern "C" void scanKeys();
extern "C" void consoleDemoInit();

bool _debugController::init(){
	debugFile = new _direntry("%WINDIR%/debug.txt");
	debugFile->create();
	debugFile->openWrite();
	return true;
}

void _debugController::end(){
	luaErrorDialogs.clear();
	debugFile = nullptr;
}


void _debugController::luaErrorHandler( lua_State* L , _literal message , ... )
{
	_uniquePtr<_runtimeErrorDialog>& dialogRef = _debugController::luaErrorDialogs[L];
	
	// If there is already a dialog: return, we only allow one dialog per state
	if( dialogRef )
		return;
	
	string messageNew;
	
	// Format right
	va_list args;
	va_start( args , message );
	messageNew = vfmt2string( message , args );
	va_end( args );
	
	// Create error-dialog that will destroy itself once it finished
	dialogRef = new _runtimeErrorDialog( _localizationController::getBuiltInString( "lbl_lua_parser_error" ) , move(messageNew) );
	dialogRef->setCallback( make_inline_callback<void(_dialogResult)>( [&dialogRef]( _dialogResult )->void{ dialogRef = nullptr; } ) );
	dialogRef->execute();
}

void _debugController::debug( _literal fmt , ... )
{
	if( !fmt )
		return;
	
	// Format right
	va_list args;
	va_start( args , fmt );
	
	// Enhance the message
	string result = string( _time::now() ) + ": " + vfmt2string( fmt , args ) + "\r\n";
	
	va_end( args );
	
	// Debug to screen
	printf( "%s" , result.c_str() );
	
	// Debug to file
	if( debugFile )
		debugFile->writeString( result );
}

void _debugController::vdebug( _literal fmt , va_list args )
{
	// Enhance the message!
	string result = string( _time::now() ) + ": " + vfmt2string( fmt , args ) + "\r\n";
	
	// Debug to screen
	printf( "%s" , result.c_str() );
	
	// Debug to file
	if( debugFile )
		debugFile->writeString( move(result) );
}

void _debugController::submit(){
	printf("Press A Key To Continue...\n");
	while( true )
	{
		scanKeys();
		if( _hardwareKeyPattern(_hardwareKeyPattern::getNewInputs()).isKeySet( _key::a ) )
			break;
	}
}

_map<lua_State*,_uniquePtr<_runtimeErrorDialog>>	_debugController::luaErrorDialogs;
_uniquePtr<_direntry>								_debugController::debugFile;