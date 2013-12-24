#ifndef _WIN_G_ACTIONBUTTON_
#define _WIN_G_ACTIONBUTTON_

#include "_gadget/gadget.button.h"

enum class _actionButtonType : _u8{
	next,
	prev,
	shutdown,
	logoff,
	switchuser
};

extern _toStr<_actionButtonType>	actionButtonType2string;
extern _fromStr<_actionButtonType>	string2actionButtonType;

class _actionButton : public _button {
	
	private:
		
		_actionButtonType type;
		
		static _constBitmap bitmaps[5];
		static _callbackReturn  refreshHandler( _event );
	
	public:
		
		//! Set type of the _actionbutton
		void setButtonType( _actionButtonType type ){
			if( this->type == type )
				return;
			this->type = type;
			this->redraw();
		}
		
		//! Get type of the _actionbutton
		_actionButtonType getButtonType(){ return this->type; }
		
		//! Ctor
		_actionButton( _optValue<_coord> x , _optValue<_coord> y , _actionButtonType type , _style&& style = _style() );
};

#endif