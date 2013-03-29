#ifndef _WIN_G_DESKTOP_
#define _WIN_G_DESKTOP_

#include "_gadget/gadget.fileview.h"

class _desktop : public _fileview{
	
	private:
		
		static _callbackReturn  refreshHandler( _event e );
		
	public:
		
		//! Constructor
		_desktop( _style style = _style() );
};

#endif