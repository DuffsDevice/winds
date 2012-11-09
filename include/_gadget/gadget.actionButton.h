#ifndef _WIN_G_ACTIONBUTTON_
#define _WIN_G_ACTIONBUTTON_

#include "_gadget/gadget.button.h"

enum class _actionButtonType : _u8
{
	next = 0,
	prev,
	logout,
	shutdown,
};

class _actionButton : public _button {
	
	private:
		
		static _bitmap* bitmaps[];
		
		_actionButtonType type;
		
		static _gadgetEventReturnType  refreshHandler( _gadgetEvent );
	
	public:
		
		_actionButton( _actionButtonType type , _coord x , _coord y , _gadgetStyle style = _defaultStyle_ );
};

#endif