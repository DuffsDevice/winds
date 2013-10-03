#ifndef _WIN_SC_SETUP_PAGE5_
#define _WIN_SC_SETUP_PAGE5_

#include "_type/type.h"
#include "_type/type.view.h"
#include "_type/type.system.h"

#include "_gadget/gadget.label.h"
#include "_gadget/gadget.progressbar.h"

class _scSetupPage5 : public _view
{
	private:
	
		_label*			lblInitializingProgress;
		_progressbar*	progressBar;
		_timer			timer;
		
		void timerCallback();
	
	public:
		
		void create( _gadget* viewParent )
		{
			timer = _timer( make_callback( this , &_scSetupPage5::timerCallback ) , 0 , false );
			
			// Label
			this->lblInitializingProgress = new _label( ignore , ignore , 0 , 60 , _system::getLocalizedString("lbl_wait_installing") );
			this->lblInitializingProgress->setColor( RGB( 30 , 30 , 30 ) );
			this->lblInitializingProgress->setAlign( _align::center );
			this->lblInitializingProgress->setWidth( SCREEN_WIDTH );
			viewParent->addChild( this->lblInitializingProgress );
			
			// Progressbar
			this->progressBar = new _progressbar( 50 , 101 , 72 , false );
			viewParent->addChild( this->progressBar );
			
			// Start execution
			timer.start();
		}
		
		bool destroy( _gadget* viewParent )
		{
			delete this->lblInitializingProgress;
			
			// Switching the view is allowed
			return true;
		}
};

#endif