#ifndef _WIN_T_SCENARIO_LOGIN_
#define _WIN_T_SCENARIO_LOGIN_

#include "_type/type.scenario.h"
#include "_type/type.scenario.login.userwrapper.h"
#include "_type/type.user.h"
#include "_type/type.animation.h"


class _scLogin : public _scenario
{
	private:
		
		_callbackReturn userLoginHandler( _event );
		
		// Switch to desktop
		void loginSwitchDesktop();
		
		// Setter for Login - Animation
		int loginAnimSetter( int );
		
		_list<_user*>			users; // Collect all users in here
		_list<_userWrapper*>	userWrapper; // Collect all userWrappers in here
		_animation				loginAnim;
		
		friend class _userWrapper;
	
	public:
		
		// Ctor
		_scLogin();
		
		// Dtor
		~_scLogin();
};

#endif