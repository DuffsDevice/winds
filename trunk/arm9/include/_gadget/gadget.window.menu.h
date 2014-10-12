#ifndef _WIN_G_WINDOWMENU_
#define _WIN_G_WINDOWMENU_

#include <_type/type.menu.h>
#include <_type/type.gadget.h>
#include <_type/type.gadget.helpers.h>

class _windowMenu : public _menu , public _gadget
{
	public:
		
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn updateHandler( _event );
		
		void menuHandler( _u16 list , _u16 index );
		void generateChildren();
		
	public:
		
		static const _menu& getStandardMenu();
		
		//! Ctor
		_windowMenu( _menu menu = _menu() , _style&& style = _style() );
		
		//! Dtor
		~_windowMenu(){
			this->removeChildren( true );
		}
};

#endif