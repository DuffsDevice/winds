#ifndef _WIN_G_SETUP_PAGE5_
#define _WIN_G_SETUP_PAGE5_

#include "_type/type.h"
#include "_type/type.view.h"

#include "_gadget/gadget.label.h"
#include "_gadget/gadget.progressbar.h"

class _guiSetupPage5 : public _view
{
	private:
	
		_label*			lblInitializingProgress;
		_progressBar*	progressBar;
		_timer			timer;
		
		void timerCallback();
	
	public:
		
		void create( _gadget* viewParent );
		bool destroy( _gadget* viewParent );
};

#endif