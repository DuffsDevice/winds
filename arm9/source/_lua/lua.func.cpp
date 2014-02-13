#include "_lua/lua.func.h"
#include "_lua/lua.lunar.h"
#include "_type/type.system.h"
#include "_dialog/dialog.runtimeerror.h"

namespace _luafunc
{
	_map<lua_State*,_runtimeErrorDialog*> errorDialogs;
	
	void errorHandler( lua_State* L , const char* message , ... )
	{
		_runtimeErrorDialog*& dialogRef = errorDialogs[L];
		
		// If there is already a dialog: return, we only allow one dialog per state
		if( dialogRef )
			return;
		
		static char output[521];
		
		// Format right
		va_list args;
		va_start(args, message);
		vsnprintf(output, 512 , message , args);
		va_end(args);
		
		// Create error-dialog that will destroy itself once it finished
		dialogRef = new _runtimeErrorDialog( _system::getLocalizedString( "lbl_lua_parser_error" ) , output );
		dialogRef->setCallback( make_inline_callback<void(_dialogResult)>( [&dialogRef]( _dialogResult )->void{ delete dialogRef; dialogRef = nullptr; } ) );
		dialogRef->execute();
	}
}