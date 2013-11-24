#ifndef _WIN_G_WINDOWBAR_
#define _WIN_G_WINDOWBAR_

#include "_type/type.gadget.h"
#include "_type/type.gadget.helpers.h"

class _windowBar : public _gadget
{
	public:
		
		_pixel bgColor;
		
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn dragHandler( _event );
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
		_windowBar( _optValue<_length> height = ignore , _optValue<_pixel> bgColor = ignore , _style&& style = _style() );
};

#endif