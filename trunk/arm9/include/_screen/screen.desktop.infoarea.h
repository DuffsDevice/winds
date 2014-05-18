#ifndef _WIN_S_DESKTOP_TASKINFOCONTROLLER_
#define _WIN_S_DESKTOP_TASKINFOCONTROLLER_

#include "_type/type.gadget.h"
#include "_type/type.timer.h"
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.taskinfo.h"

class _desktopScreenInfoArea : public _gadget {
	
	private:
		
		_uniquePtr<_label>	time;
		_timer				timer;
		
		// Event Handler
		static _callbackReturn  refreshHandler( _event );
		void			  		timerHandler();
		
		// Will be registered in _taskInfo
		void taskInfoHandler( _taskInfo* info );
	
	public:
		
		//! Ctor
		_desktopScreenInfoArea( _optValue<_coord> x , _optValue<_coord> y , _style&& style = _style() );
		
		//! Dtor
		~_desktopScreenInfoArea();
};

#endif