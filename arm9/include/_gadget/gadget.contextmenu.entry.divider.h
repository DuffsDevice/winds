#ifndef _WIN_G_CONTEXTMENUENTRYDIVIDER_
#define _WIN_G_CONTEXTMENUENTRYDIVIDER_

#include "_gadget/gadget.contextmenu.h"

class _contextMenuEntryDivider : public _contextMenuEntry
{
	private:
		
		// Event Handlers
		static _callbackReturn refreshHandler( _event );
	
	public:
		
		//! Ctor
		_contextMenuEntryDivider( _style&& style = _style() );
};

#endif