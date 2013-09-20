#ifndef _WIN_G_SCROLLBUTTON_
#define _WIN_G_SCROLLBUTTON_

#include "_type/type.h"
#include "_type/type.gadget.h"
#include "_gadget/gadget.button.h"

enum class _scrollButtonType : _u8{
	buttonHandleX = 0,
	buttonLeft = 1,
	buttonRight = 2,
	buttonHandleY = 3,
	buttonBottom = 4,
	buttonTop = 5
};
	

class _scrollButton : public _button{
	
	private:
		
		_scrollButtonType type;
		
		static _callbackReturn refreshHandler( _event event );
		
	public:
	
		//! Get button type
		_scrollButtonType getButtonType(){ return this->type; }
		
		//! Set type of button
		void setButtonType( _scrollButtonType type ){ if( this->type == type ) return; this->type = type; this->redraw(); }
		
		//! Ctor
		_scrollButton( _length width , _length height , _coord x , _coord y , _scrollButtonType type , _style&& style = _style() );
};	
#endif