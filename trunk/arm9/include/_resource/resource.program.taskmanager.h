#ifndef _WIN_PROG_TASK_MANAGER_
#define _WIN_PROG_TASK_MANAGER_

#include "_type/type.system.h"
#include "_type/type.progC.h"
#include "_type/type.direntry.h"
#include "_gadget/gadget.fileview.h"
#include "_gadget/gadget.window.h"
#include "_gadget/gadget.textbox.h"

class PROG_TaskManager : public _progC
{
	private:
		
		_window*		window;
		_imagegadget*	animation;
		_timer			timer;
		_button*		submitbutton;
		
		void			main( _cmdArgs args );
		void			destruct();
		
		_callbackReturn handler( _event event );
		
	public:
		
		PROG_TaskManager();
};

#endif