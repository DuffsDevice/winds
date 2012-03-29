/*!
 *	Global Documentation:
 *	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *	
 *	+ Every Gadget has its own children e.g. some Buttons.
 *	+ The Children are sorted like that: The Last Gadget in the List is the toppest displayed.
 *	+ Every Gadget has its own Event Handlers that can react to some Events. You have to differ into two types.
 *		- Default Event Handlers: Not editable, pre installed and different from Gadget to Gadget.
 *		- Additional Event Handlers: User-defined, high-prioried, can be unregistered.
 *	+ Every Gadget has its own bitmap. It can copy anything onto it.
 *	+ WinDS will copy that one onto its own. Thats why any Gadget can be moved, without redrawing itself.
 *	+ Inputs are converted to Events in the VBL. These Events will then be parsed by WinDS itself and passed to the related Target
 *	
 *	DO'S:
 *	- ALWAYS return 'handled' in dragEvents!
 *	- Return 'not_handled' on dragStart to prevent the dragging
 *	
 **/
 
//#define _WIN_USE_WIFI_
 
 
#ifndef _WIN_PREDEFINES_
#define _WIN_PREDEFINES_

#include "nds.h"

#include "_type/type.h"
#include "_type/type.color.h"
#include "_type/type.eventArgs.h"
#include "_type/type.bitmap.h"
#include "_type/type.bitmapPort.h"
#include "_type/type.rect.h"
#include "_type/type.event.h"
#include "_type/type.font.h"
#include "_type/type.file.h"
#include "_type/type.directory.h"
#include "_type/type.luainstance.h"
#include "_type/type.animation.h"
#ifdef _WIN_USE_WIFI_
#include "_type/type.wificonnection.h"
#endif

#include "_file/file.png.h"

#include "_gadget/gadget.windows.h"
#include "_gadget/gadget.desktop.h"
#include "_gadget/gadget.window.h"
#include "_gadget/gadget.button.h"
#include "_gadget/gadget.textbox.h"
#include "_gadget/gadget.keyboard.h"
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.fileview.h"
#include "_gadget/gadget.checkbox.h"
#include "_gadget/gadget.windows.taskboard.h"

#include <nds/arm9/input.h>
#include <nds/system.h>
#include <nds/interrupts.h>

using namespace std;

namespace DSWindows
{	
	void memcpyHalfWords( _u16* src , _u16* dest , _u32 size );
	
	void submit();
	
	void initDisplay();
	
	void benchMarkStart();
	
	void benchMarkStopPrint();
	
}

#endif