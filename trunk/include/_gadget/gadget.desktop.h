#ifndef _WIN_G_DESKTOP_
#define _WIN_G_DESKTOP_

#include "_type/type.gadget.h"

class _desktop : public _gadget{
	
	private:
	
		static _gadgetEventReturnType  refreshHandler( _gadgetEvent e );
	
	public:
	
		// Constructor
		_desktop( _gadgetStyle style = _defaultStyle_ );
	
};

#endif