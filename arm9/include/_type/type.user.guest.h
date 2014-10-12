#ifndef _WIN_T_USER_GUEST_
#define _WIN_T_USER_GUEST_

#include <_type/type.user.h>

class _guestUser : public _user
{
	public:
		
		//! Ctor!
		_guestUser();
		
		//! These Functions overwrite the ones found in _user
		_constBitmap&	getLogo() const ;
		const string&	getName() const ;
};

#endif