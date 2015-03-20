#include <_gui/gui.desktop.h>
#include <_screen/screen.desktop.h>
#include <_controller/controller.gui.h>
#include <_controller/controller.debug.h>
#include <_type/type.winds.h>
#include <_gadget/gadget.codepointmatrix.h>
#include <_type/type.ref.h>

#include <assert.h>

_guiDesktop::_guiDesktop()
{
	// Create Windows
	_guiController::enableKeyboard();
	_guiController::setHost<_desktopScreen>();
	
	//_networkClient client = _networkClient();
	//printf("Connected: %d\n",client.connect( "www.palmato.com" , 80 ));
	//printf("Sending: %d\n",client.request("/WinDSAPI/wifi.php"));
	//printf("Response: %s\n",client.receive().c_str());
	
	//_winds::execute("%SYSTEM%/taskmgr.exe");
	//_winds::execute("%SYSTEM%/explorer.exe -Computer");
	//_winds::execute("%SYSTEM%/filedetail.exe -/filename.jpg");
	//_winds::execute("%SYSTEM%/credits.exe");
	//_winds::execute("%SYSTEM%/progmapper.exe -Hello.jpg -save_choice");
	//_winds::execute("%WINDIR%/accessories/exampleprogram.exe");
	//_winds::execute("%WINDIR%/accessories/paint.exe");
	//_winds::execute("%WINDIR%/accessories/calculator.exe");
	//_winds::execute("%WINDIR%/games/pong.exe");
	//_winds::execute("%SYSTEM%/console.exe");
	
	
	//_label* lbl = new _label( 34 , 34 , 12 , 3 , "0" );
	//printf("New: %p %s\n",lbl,lbl->getStrValue().c_str());
	//
	//_ref<_label> ref1 = _ref<_label>( move(lbl) );
	//_ref<_gadget> ref2 = ref1.data();
	//_ref<_gadget> ref3 = move(ref1);
	//_ref<_gadget> ref4 = ref1;
	//_ref<_gadget> ref5;
	//ref5 = ref3;
	//printf("Reference: %p %d\n",ref1.data(),ref1.isStrong());
	//printf("Reference: %p %d\n",ref2.data(),ref2.isStrong());
	//printf("Reference: %p %d\n",ref3.data(),ref3.isStrong());
	//printf("Reference: %p %d\n",ref4.data(),ref4.isStrong());
	//printf("Reference: %p %d\n",ref5.data(),ref5.isStrong());
	//_debugController::submit();
}


_guiDesktop::~_guiDesktop(){ }