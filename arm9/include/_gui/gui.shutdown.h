#ifndef _WIN_G_SHUTDOWN_
#define _WIN_G_SHUTDOWN_

#include <_type/type.gui.h>
#include <_type/type.timer.h>
#include <_gadget/gadget.label.h>
#include <_gadget/gadget.image.h>

class _guiShutdown : public _gui
{
	private:
		
		_label*			shutdownLabel;
		_imageGadget*	windowsLogo;
		_timer			timer;
		
	public:
		
		// Ctor
		_guiShutdown();
		
		// Dtor
		~_guiShutdown();
};

#endif