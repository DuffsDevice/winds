#ifndef _WIN_T_USER_GUEST_
#define _WIN_T_USER_GUEST_

#include "_type/type.user.h"
#include "_type/type.system.h"

class _guestUser : public _user
{
	public:
		
		//! Ctor!
		_guestUser() :
			_user( "$guest" )
		{
			_registry::writeIndex( "_global_" , "userName" , _system::getLocalizedString("lbl_guest") );
			_registry::writeIndex( "_global_" , "userCode" , "" );
		}
};

#endif