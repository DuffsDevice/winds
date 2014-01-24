#include "_scenario/scenario.desktop.h"
#include "_type/type.system.h"
#include "_type/type.system.controller.h"
#include "_type/type.color.h"
#include "_type/type.text.phrases.h"
#include "_type/type.font.glyphs.h"

#include "_type/type.connection.h"
_wifiSocketClient* client = nullptr;

#include "_gadget/gadget.windows.h"
#include "_dialog/dialog.file.save.h"
#include "_gadget/gadget.counter.h"
#include "_gadget/gadget.progressbar.h"
#include "_gadget/gadget.keyboard.h"
#include "_gadget/gadget.colorpicker.h"
#include "_gadget/gadget.button.expand.h"

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
	
	_system::executeCommand("%SYSTEM%/explorer.exe -/LUA/" );
	//_system::executeCommand("%SYSTEM%/exampleprogram.exe");
	//_system::executeCommand("%SYSTEM%/paint.exe");
	//_system::executeCommand("%SYSTEM%/pong.exe");
	_system::executeCommand("%SYSTEM%/calculator.exe");
	
	(new _fileSaveDialog( {
		{ 0 , make_tuple("Windows-Bitmap" , "bmp") } ,
		{ 1 , make_tuple("Portable-Network-Graphic" , "png") }
	} ))->execute();
	
	//for( int i = 0; i < 60 ; i++)
	//{
	//	_window* wnd = new _window( 30 , 30 , 30 , 30 , "Hallo" , _style() | _styleAttr::notResizeable | _styleAttr::minimizeable );
	//	_system::_gadgetHost_->addChild( wnd );
	//	delete wnd;
	//}
	//_button* btn = new _button( 50 , 50 , "Do Request!" );
	//_button* btn2 = new _button( 50 , 70 , "Connect" );
	//btn->registerEventHandler( mouseClick , new _staticCallback( clickHandler ) );
	//btn2->registerEventHandler( mouseClick , new _staticCallback( clickHandler2 ) );
	//_system::_gadgetHost_->addChild( btn );
	//_system::_gadgetHost_->addChild( btn2 );
}

_scDesktop::~_scDesktop()
{
	
}