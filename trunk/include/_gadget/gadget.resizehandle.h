#ifndef _WIN_G_RESIZEHANDLE_
#define _WIN_G_RESIZEHANDLE_

#include "_type/type.h"
#include "_type/type.gadget.h"

class _resizeHandle : public _gadget
{
	private:
		
		_pixel bgColor;
		
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn dragHandler( _event );
		static _callbackReturn positionAdjuster( _event );
		static _callbackReturn updateHandler( _event );
	
	public:
		
		//! Set background color of the handle
		void setBgColor( _pixel col )
		{
			if( this->bgColor == col )
				return;
			this->bgColor = col;
			this->redraw();
		}
		
		//! Get the background color
		_pixel getBgColor(){ return this->bgColor; }
		
		//! Ctor
		_resizeHandle( _optValue<_pixel> bgColor = ignore , _style&& style = _style() );
};

#endif