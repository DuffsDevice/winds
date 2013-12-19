#ifndef _WIN_G_WINDOWMENUENTRY_
#define _WIN_G_WINDOWMENUENTRY_

#include "_type/type.menu.h"
#include "_type/type.gadget.h"
#include "_type/type.gadget.helpers.h"
#include "_gadget/gadget.contextmenu.h"

class _windowMenuEntry : public _gadget
{
	private:
		
		// Text to display
		string	text;
		_s32	listReference;
		
		// contextmenu that should be opened up on clicking
		_contextMenu* menuList;
		
		// Event Handlers
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn updateHandler( _event );
		static _callbackReturn mouseHandler( _event );
		static _callbackReturn selectHandler( _event );
	
	public:
		
		//! Ctor
		_windowMenuEntry( _s32 listIndex , _menu& menu , _style&& style = _style() );
		
		//! Dtor
		~_windowMenuEntry(){
			delete this->menuList;
		}
};

#endif