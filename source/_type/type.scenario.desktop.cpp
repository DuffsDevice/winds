#include "_type/type.scenario.desktop.h"
#include "_type/type.system.h"
#include "_type/type.system.controller.h"

#include "_gadget/gadget.windows.h"
#include "_gadget/gadget.keyboard.h"

_scDesktop::_scDesktop()
{
	// Clean up	
	_system::deleteGadgetHost();
	_system::deleteKeyboard();
	
	// Create BootupScreen
	_system::_gadgetHost_ = new _windows( _system::_bgIdBack_ );
	_system::_keyboard_ = new _keyboard( _system::_bgIdFront_ , _system::_gadgetHost_ , _system::_topScreen_ );
	
	_system::getBuiltInProgram( "explorer.exe" )->execute({{"path","/LUA"}});
}

_scDesktop::~_scDesktop()
{
	
}