#ifndef _WIN_G_DESKTOP_
#define _WIN_G_DESKTOP_

#include "_type/type.gadget.h"

class _desktop : public _gadget{
	
	private:
	
		static _callbackReturn  refreshHandler( _event e );
	
	public:
	
		// Constructor
		_desktop( _style style = _style() );
	
};

#endif