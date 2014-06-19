#include "_gui/gui.desktop.h"
#include "_screen/screen.desktop.h"

#include "_controller/controller.gui.h"
#include "_controller/controller.memory.h"
#include "_type/type.windows.h"
#include "_gadget/gadget.taskinfo.h"

_guiDesktop::_guiDesktop()
{
	// Create Windows
	_guiController::setHost<_desktopScreen>();
	_guiController::enableKeyboard();
	
	//_windows::executeCommand("%SYSTEM%/taskmgr.exe");
	_windows::executeCommand("%SYSTEM%/explorer.exe");
	//_windows::executeCommand("%SYSTEM%/credits.exe");
	//_windows::executeCommand("\"%SYSTEM%/progmapper.exe\" -\"C:/Windows/Hello.drp\"");
	//_windows::executeCommand("%SYSTEM%/progmapper.exe -Hello.jpg -save_choice");
	//_windows::executeCommand("%WINDIR%/accessories/exampleprogram.exe");
	//_windows::executeCommand("%WINDIR%/accessories/paint.exe");
	_windows::executeCommand("%WINDIR%/accessories/calculator.exe");
	//_windows::executeCommand("%WINDIR%/games/pong.exe");
}

_guiDesktop::~_guiDesktop()
{
	
}