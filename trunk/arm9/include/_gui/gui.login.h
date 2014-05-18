#ifndef _WIN_G_LOGIN_
#define _WIN_G_LOGIN_

#include "_type/type.gui.h"
#include "_gui/gui.login.userwrapper.h"
#include "_type/type.animation.h"
#include "_type/type.timer.h"
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.image.h"


class _guiLogin : public _gui
{
	private:
		
		_callbackReturn userLoginHandler( _event );
		
		// Switch to desktop
		void loginSwitchDesktop();
		
		// Setter for Login - Animation
		void loginAnimSetter( int );
		
		_list<string>					users; // Collect all users in here
		_list<_uniquePtr<_userWrapper>>	userWrapper; // Collect all userWrappers in here
		_animation						loginAnim;
		_timer							loginTmr;
		
		_label*							welcome;
		_label*							welcomeBg;
		_imageGadget*					separator;
		
		friend class _userWrapper;
	
	public:
		
		// Ctor
		_guiLogin();
		
		// Dtor
		~_guiLogin();
};

#endif