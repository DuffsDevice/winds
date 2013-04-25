#include "_type/type.scenario.desktop.h"
#include "_type/type.system.h"
#include "_type/type.system.controller.h"

#include "_gadget/gadget.windows.h"
#include "_gadget/gadget.progressbar.h"
#include "_gadget/gadget.keyboard.h"
#include "_gadget/gadget.calendar.h"

_scDesktop::_scDesktop()
{
	// Clean up	
	_system::deleteGadgetHost();
	_system::deleteKeyboard();
	
	// Create BootupScreen
	_system::_gadgetHost_ = new _windows( _system::_bgIdBack_ );
	_system::_keyboard_ = new _keyboard( _system::_bgIdFront_ , _system::_gadgetHost_ , _system::_topScreen_ , SCREEN_WIDTH - 80 );
	
	_direntry( "%SYSTEM%/explorer.exe" ).execute( { { "path" , "/LUA" } } );
	
	_system::_gadgetHost_->addChild( new _calendar( 99 , 80 , 10 , 10 , _time::date( 2015 , 3 , 1 ) ) );
}

_scDesktop::~_scDesktop()
{
	
}