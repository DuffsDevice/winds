// Check if already included
#ifndef _WIN_WINDOWS_
#define _WIN_WINDOWS_

//! Types
#include "_type/type.gadget.h"
#include "_type/type.gadget.screen.h"

#include "_gadget/gadget.keyboard.h"
#include "_gadget/gadget.desktop.h"
#include "_gadget/gadget.windows.startbutton.h"
#include "_gadget/gadget.windows.taskbutton.h"
#include "_gadget/gadget.windows.taskinfo.h"
#include "_gadget/gadget.windows.programswitcher.h"

class _windows : public _gadgetScreen
{
	private:
		
		//! Start-Button
		_windowsStartButton*		startButton;
		_windowsTaskInfo*			taskInfo;
		
		_list<_windowsTaskButton*>	tasks;
		
		//! Desktop-Gadget
		_desktop*					desktop;
		
		//! Task-Switcher
		_windowsProgramSwitcher*	switcher;
		
		//! Standard Function to be executed on refresh
		static _callbackReturn	refreshHandler( _event );
		static _callbackReturn	keyHandler( _event );
		
		//! Update all Task-Buttons
		void refreshTaskWidths();
		
		//! Handler
		void taskHandler( _window* ref );
		
		friend class _window;
		
	public:
		
		//! Check if the startMenu is opened
		bool isStartMenuOpened(){ return this->startButton->isMenuOpened(); }
		
		//! Open the startMenu
		void openStartMenu(){ this->startButton->openStartMenu(); }
		
		//! Open the startMenu
		void closeStartMenu(){ this->startButton->closeStartMenu(); }
		
		//! Constructor with style
		_windows( _u8 bgId , _style&& style = _style() );
		
		//! Dtor
		~_windows();
};

#endif