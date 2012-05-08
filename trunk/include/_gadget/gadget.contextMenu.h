#ifndef _WIN_G_CONTEXTMENU_
#define _WIN_G_CONTEXTMENU_

#include "_type/type.h"
#include "_type/type.gadget.h"

class _contextMenu : public _gadget{
	
	private:
		
		static _gadgetEventReturnType blurHandler( _gadgetEvent e );
		
		_align			xDir;
		_valign			yDir;
		
	public:
		
		void show( _coord x , _coord y );
		
		void hide();
		
		void toggle( _coord x , _coord y );
		
		_contextMenu( _length width , _length height , _gadgetStyle style = _defaultStyle_ );
		
};

#endif