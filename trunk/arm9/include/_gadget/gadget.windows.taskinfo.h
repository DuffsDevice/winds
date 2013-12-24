#ifndef _WIN_T_TASKINFO_
#define _WIN_T_TASKINFO_

#include "_type/type.gadget.h"
#include "_type/type.timer.h"
#include "_gadget/gadget.label.h"

class _windowsTaskInfo : public _gadget {
	
	private:
		
		_label*	time;
		_timer	timer;
		
		static _callbackReturn  refreshHandler( _event );
	
	public:
		
		//! Ctor
		_windowsTaskInfo( _optValue<_coord> x , _optValue<_coord> y , _style&& style = _style() );
		
		//! Dtor
		~_windowsTaskInfo();
};

#endif