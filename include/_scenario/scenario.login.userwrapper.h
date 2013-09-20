#ifndef _WIN_SC_LOGIN_USERWRAPPER_
#define _WIN_SC_LOGIN_USERWRAPPER_

#include "_type/type.h"
#include "_type/type.user.h"
#include "_gadget/gadget.imagegadget.h"
#include "_gadget/gadget.passcodebox.h"
#include "_gadget/gadget.actionButton.h"


//! Class for each user generated
class _userWrapper : public _gadget
{
	private:
		
		_passcodebox*	passwordbox;
		_actionButton*	passwordsubmit;
		_user*			user;
		
		static _callbackReturn focusHandler( _event );
		static _callbackReturn submitHandler( _event );
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn textboxRefreshHandler( _event event );
		
		friend class _scLogin;
	
	public:
		
		//! Ctor
		_userWrapper( _coord x , _coord y , _user* user , _style&& style = _style() );
		
		//! Dtor
		~_userWrapper();
};

#endif