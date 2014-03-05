#ifndef _WIN_T_WINDOWSBUTTON_
#define _WIN_T_WINDOWSBUTTON_

#include "_gadget/gadget.button.h"
#include "_gadget/gadget.windows.startmenu.h"

class _windowsStartButton : public _button {
	
	friend class _windows;
	
	private:
		
		_startMenu*		startMenu;
		
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
		_windowsStartButton( _optValue<_coord> x , _optValue<_coord> y , _style&& style = _style() );
};

#endif