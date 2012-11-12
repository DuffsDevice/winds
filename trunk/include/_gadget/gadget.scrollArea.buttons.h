#ifndef _WIN_G_SCROLLBUTTON_
#define _WIN_G_SCROLLBUTTON_

#include "_type/type.h"
#include "_type/type.gadget.h"

typedef enum{
	buttonTop,
	buttonBottom,
	buttonRight,
	buttonLeft,
	buttonHandleX,
	buttonHandleY
}_scrollButtonType;
	

class _scrollButton : public _gadget{
	
	private:
		
		_scrollButtonType type;
		
		static _callbackReturn refreshHandler( _event event );
		
		static _callbackReturn resizeHandler( _event e );
		
	public:
		
		_scrollButton( _scrollButtonType type , _coord x , _coord y , _length length = 0 , _style style = _style() );
};	
#endif