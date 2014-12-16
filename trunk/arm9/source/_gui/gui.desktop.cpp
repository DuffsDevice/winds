#include <_gui/gui.desktop.h>
#include <_screen/screen.desktop.h>
#include <_controller/controller.gui.h>
#include <_type/type.windows.h>

_guiDesktop::_guiDesktop()
{
	// Create Windows
	_guiController::setHost<_desktopScreen>();
	_guiController::enableKeyboard();
	
	//_networkClient client = _networkClient();
	//printf("Connected: %d\n",client.connect( "www.palmato.com" , 80 ));
	//printf("Sending: %d\n",client.request("/WinDSAPI/wifi.php"));
	//printf("Response: %s\n",client.receive().c_str());
	
	//_windows::executeCommand("%SYSTEM%/taskmgr.exe");
	//_windows::executeCommand("%SYSTEM%/explorer.exe -Computer");
	//_windows::executeCommand("%SYSTEM%/filedetail.exe -/filename.jpg");
	//_windows::executeCommand("%SYSTEM%/credits.exe");
	//_windows::executeCommand("%SYSTEM%/progmapper.exe -Hello.jpg -save_choice");
	//_windows::executeCommand("%WINDIR%/accessories/exampleprogram.exe");
	//_windows::executeCommand("%WINDIR%/accessories/paint.exe");
	//_windows::executeCommand("%WINDIR%/accessories/calculator.exe");
	//_windows::executeCommand("%WINDIR%/games/pong.exe");
}

_guiDesktop::~_guiDesktop(){ }