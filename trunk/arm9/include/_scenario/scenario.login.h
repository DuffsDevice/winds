#ifndef _WIN_SC_LOGIN_
#define _WIN_SC_LOGIN_

#include "_type/type.scenario.h"
#include "_scenario/scenario.login.userwrapper.h"
#include "_type/type.user.h"
#include "_type/type.animation.h"
#include "_type/type.timer.h"
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.image.h"


class _scLogin : public _scenario
{
	private:
		
		_callbackReturn userLoginHandler( _event );
		
		// Switch to desktop
		void loginSwitchDesktop();
		
		// Setter for Login - Animation
		void loginAnimSetter( int );
		
		_list<_user*>			users; // Collect all users in here
		_list<_userWrapper*>	userWrapper; // Collect all userWrappers in here
		_animation				loginAnim;
		_timer					loginTmr;
		
		_label*					welcome;
		_label*					welcomeBg;
		_imageGadget*			separator;
		
		friend class _userWrapper;
	
	public:
		
		// Ctor
		_scLogin();
		
		// Dtor
		~_scLogin();
};

#endif