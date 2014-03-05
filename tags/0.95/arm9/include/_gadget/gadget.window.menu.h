#ifndef _WIN_G_WINDOWMENU_
#define _WIN_G_WINDOWMENU_

#include "_type/type.menu.h"
#include "_type/type.gadget.h"
#include "_type/type.gadget.helpers.h"

class _windowMenu : public _menu , public _gadget
{
	public:
		
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn updateHandler( _event );
		void menuHandler( _s32 list , _s32 index );
		
		void generateChildren();
		
	public:
		
		static const _menu& getStandardMenu();
		
		//! Ctor
		_windowMenu( const _menu& menu = _menu() , _style&& style = _style() );
};

#endif