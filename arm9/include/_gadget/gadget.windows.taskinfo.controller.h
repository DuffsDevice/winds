#ifndef _WIN_T_TASKINFOCONTROLLER_
#define _WIN_T_TASKINFOCONTROLLER_

#include "_type/type.gadget.h"
#include "_type/type.timer.h"
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.windows.taskinfo.h"

class _windowsTaskInfoController : public _gadget {
	
	private:
		
		_label*	time;
		_timer	timer;
		
		// Event Handler
		static _callbackReturn  refreshHandler( _event );
		
		// Will be registered in _windowsTaskInfo
		void taskInfoHandler( _windowsTaskInfo* info );
	
	public:
		
		//! Ctor
		_windowsTaskInfoController( _optValue<_coord> x , _optValue<_coord> y , _style&& style = _style() );
		
		//! Dtor
		~_windowsTaskInfoController();
};

#endif