#ifndef _WIN_G_LOGOUT_
#define _WIN_G_LOGOUT_

#include <_type/type.gui.h>
#include <_type/type.timer.h>
#include <_gadget/gadget.label.h>
#include <_gadget/gadget.image.h>

class _guiLogout : public _gui
{
	private:
		
		_label*			logoutLabel;
		_imageGadget*	windowsLogo;
		_timer			timer;
		
	public:
		
		// Ctor
		_guiLogout();
		
		// Dtor
		~_guiLogout();
};

#endif