#ifndef _WIN_S_DESKTOP_STARTBUTTON_
#define _WIN_S_DESKTOP_STARTBUTTON_

#include "_gadget/gadget.button.h"
#include "_screen/screen.desktop.startmenu.h"

class _desktopScreenStartButton : public _button {
	
	friend class _desktopScreen;
	
	private:
		
		_desktopScreenStartMenu*		startMenu;
		
		static _callbackReturn  refreshHandler( _event );
		_callbackReturn  		mouseHandler( _event );
	
	public:
		
		//! Check if the startMenu is opened
		bool isMenuOpened(){ return this->startMenu->isOpened(); }
		
		//! Open the startMenu
		void openStartMenu(){ this->startMenu->show( this->getAbsoluteX() , this->getAbsoluteY() , false ); }
		
		//! Open the startMenu
		void closeStartMenu(){ this->startMenu->shelve(); }
		
		//! Ctor
		_desktopScreenStartButton( _optValue<_coord> x , _optValue<_coord> y , _style&& style = _style() );
		
		//! Dtor
		~_desktopScreenStartButton();
};

#endif