// Check if already included
#ifndef _WIN_S_DESKTOP_
#define _WIN_S_DESKTOP_

//! Types
#include <_type/type.gadget.h>
#include <_type/type.screen.gadget.h>

#include <_screen/screen.desktop.view.h>
#include <_screen/screen.desktop.startbutton.h>
#include <_screen/screen.desktop.taskbutton.h>
#include <_screen/screen.desktop.infoarea.h>
#include <_gadget/gadget.programswitcher.h>

class _desktopScreen : public _gadgetScreen
{
	private:
		
		//! Start-Button
		_desktopScreenStartButton*			startButton;
		_desktopScreenInfoArea*				taskInfo;
		
		_list<_desktopScreenTaskButton*>	tasks;
		
		//! FileView and Wallpaper
		_desktopScreenView*					desktop;
		
		//! Program-Switcher
		_programSwitcher*			switcher;
		
		
		//! Standard Function to be executed on refresh
		static _callbackReturn		refreshHandler( _event );
		static _callbackReturn		keyHandler( _event );
		_callbackReturn				taskInfoMoveHandler( _event );
		
		//! Update all Task-Buttons
		void						refreshTaskWidths();
		
		//! Handler
		void						taskHandler( _window* ref );
		
	public:
		
		//! Check if the startMenu is opened
		bool isStartMenuOpened(){ return this->startButton->isMenuOpened(); }
		
		//! Open the startMenu
		void openStartMenu(){ this->startButton->openStartMenu(); }
		
		//! Open the startMenu
		void closeStartMenu(){ this->startButton->closeStartMenu(); }
		
		//! Toggle the startMenu
		void toggleStartMenu(){ this->startButton->triggerEvent( onMouseClick ); }
		
		//! Constructor with style
		_desktopScreen( _u8 bgId , _style&& style = _style() );
		
		//! Dtor
		~_desktopScreen();
};

#endif