#ifndef _WIN_T_TASKINFO_
#define _WIN_T_TASKINFO_

#include "_type/type.gadget.h"
#include "_gadget/gadget.label.h"

class _windowsTaskInfo : public _gadget {
	
	private:
		
		_label*		time;
		
		static _callbackReturn  refreshHandler( _event );
	
	public:
		
		//! Ctor
		_windowsTaskInfo( _coord x , _coord y , _style&& style = _style() );
		
		//! Dtor
		~_windowsTaskInfo();
};

#endif