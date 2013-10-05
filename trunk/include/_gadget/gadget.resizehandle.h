#ifndef _WIN_G_RESIZEHANDLE_
#define _WIN_G_RESIZEHANDLE_

#include "_type/type.h"
#include "_type/type.gadget.h"

class _resizeHandle : public _gadget
{
	
	static _callbackReturn refreshHandler( _event );
	static _callbackReturn dragHandler( _event );
	static _callbackReturn positionAdjuster( _event );
	static _callbackReturn updateHandler( _event );
	
	public:
		
		_resizeHandle( _style&& style = _style() );

};

#endif