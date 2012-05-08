#ifndef _WIN_G_STARTMENU_
#define _WIN_G_STARTMENU_

#include "_type/type.gadget.h"
#include "_gadget/gadget.contextMenu.h"

class _startMenu : public _contextMenu{
	
	private:
	
		static _gadgetEventReturnType refreshHandler( _gadgetEvent e );
	
	public:
	
		_startMenu( _gadgetStyle style = _defaultStyle_ );

};	

#endif