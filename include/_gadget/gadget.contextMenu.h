#ifndef _WIN_G_CONTEXTMENU_
#define _WIN_G_CONTEXTMENU_

#include "_type/type.h"
#include "_type/type.gadget.h"
#include <map>

typedef map<_s32,string> _contextMenuEntryList;

class _contextMenu : public _gadget{
	
	private:
		
		static _gadgetEventReturnType blurHandler( _gadgetEvent e );
		
		_align			xDir; // can either be left or right
		_valign			yDir; // Can either bo top or bottom
		
		_gadget*		owner;
		
		bool			opened;
		
	public:
		
		void show( _coord x , _coord y );
		
		void hide();
		
		void toggle( _coord x , _coord y );
		
		bool isOpened();
		
		void setOwner( _gadget* owner );
		
		// For ContextMenu entries to close the cM and submit their value ( this event type must be _gadgetEvent::dialogClose )
		static _gadgetEventReturnType closeAndSend( _gadgetEvent e );
		
		_gadget* getOwner();
		
		_contextMenu( _length width , _length height , _gadget* owner , _gadgetStyle style = _defaultStyle_ );
		
};

#endif