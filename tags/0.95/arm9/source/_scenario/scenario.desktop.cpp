#include "_scenario/scenario.desktop.h"
#include "_type/type.system.h"
#include "_type/type.system.controller.h"
#include "_type/type.color.h"
#include "_type/type.text.phrases.h"
#include "_type/type.font.glyphs.h"

#include "_type/type.connection.h"
_wifiSocketClient* client = nullptr;

#include "_gadget/gadget.windows.h"
#include "_gadget/gadget.keyboard.h"

namespace{

	_callbackReturn dragHandler( _event event ){
		_gadget* that = event.getGadget();
		
		// Has the Gadget to move?
		if( event.getDeltaX() == 0 && event.getDeltaY() == 0 )
			return handled;
		
		// Move it relatively
		that->moveRelative( event.getDeltaX() , event.getDeltaY() );
		
		// Return
		return handled;
	}

}

//_callbackReturn clickHandler( _event event )
//{
//	if( client )
//		client->request("GET: keyboard refresh!");
//	return handled;
//}
//
//_callbackReturn clickHandler2( _event event )
//{
//	client = new _wifiSocketClient( "192.168.178.41" , 8080 , _tcpType::tcpNormal );
//	return handled;
//}

_scDesktop::_scDesktop()
{
	// Create BootupScreen
	_system::_gadgetHost_ = new _windows( _system::_bgIdBack_ );
	_system::_keyboard_ = new _keyboard( _system::_bgIdFront_ , _system::_gadgetHost_ , _system::_topScreen_ );
	
	_system::executeCommand("%SYSTEM%/explorer.exe");
	//_system::executeCommand("%WINDIR%/accessories/exampleprogram.exe");
	//_system::executeCommand("%WINDIR%/accessories/paint.exe");
	//_system::executeCommand("%WINDIR%/accessories/calculator.exe");
	//_system::executeCommand("%WINDIR%/games/pong.exe");
}

_scDesktop::~_scDesktop()
{
	
}