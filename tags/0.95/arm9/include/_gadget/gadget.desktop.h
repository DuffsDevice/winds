#ifndef _WIN_G_DESKTOP_
#define _WIN_G_DESKTOP_

#include "_gadget/gadget.fileview.h"

class _desktop : public _fileView{
	
	private:
		
		static _callbackReturn  refreshHandler( _event );
		
	public:
		
		//! Constructor
		_desktop( _style&& style = _style() );
};

#endif