#ifndef _WIN_G_STARTMENU_
#define _WIN_G_STARTMENU_

#include "_type/type.gadget.h"
#include "_gadget/gadget.contextMenu.h"

class _startMenu : public _contextMenu{
	
	private:
	
		static _callbackReturn refreshHandler( _event e );
	
	public:
	
		_startMenu( _gadget* owner , _style style = _style() );

};	

#endif