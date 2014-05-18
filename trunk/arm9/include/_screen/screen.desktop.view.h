#ifndef _WIN_S_DESKTOP_VIEW_
#define _WIN_S_DESKTOP_VIEW_

#include "_gadget/gadget.fileview.h"

class _desktopScreenView : public _fileView{
	
	private:
		
		static _callbackReturn  refreshHandler( _event );
		
	public:
		
		//! Constructor
		_desktopScreenView( _style&& style = _style() );
};

#endif