// Check if already included
#ifndef _WIN_WINDOWS_
#define _WIN_WINDOWS_

//! Types
#include "_type/type.gadget.h"
#include "_type/type.gadgetScreen.h"

#include "_gadget/gadget.keyboard.h"
#include "_gadget/gadget.desktop.h"
#include "_gadget/gadget.windows.startbutton.h"
#include "_gadget/gadget.windows.taskbutton.h"
#include "_gadget/gadget.windows.taskinfo.h"

class _windows : public _gadgetScreen
{
	private:
		
		//! Start-Button
		_windowsStartButton*		startButton;
		_windowsTaskInfo*			taskInfo;
		
		_list<_windowsTaskButton*>	tasks;
		
		//! Desktop-Gadget
		_desktop*					desktop;
		
		//! Standard Function to be executed on refresh
		static _callbackReturn	refreshHandler( _event );
		
		//! register a gadget to shown in the taskbar
		void registerTask( _window* w );
		
		//! unregister a gadget from the taskbar
		void removeTask( _window* w );
		
		//! refresh all Task-Buttons
		void refreshTasks();
		
		//! Refresh the taskbutton of the window passed
		void refreshTask( _window* w );
		
		friend class _window;
		
	public:
		
		//! Maximized Dimensions for (mainly) _windows, that are maximized
		_rect getMaximizedDimensions(){ return _rect( 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT - 10 ); }
		
		//! Check if the startMenu is opened
		bool isStartMenuOpened(){ return this->startButton->isMenuOpened(); }
		
		//! Open the startMenu
		void openStartMenu(){ this->startButton->openStartMenu(); }
		
		//! Open the startMenu
		void closeStartMenu(){ this->startButton->closeStartMenu(); }
		
		//! Constructor with style
		_windows( _u8 bgId , _style&& style = _style() );
		
};

#endif