#ifndef _WIN_RESOURCE_EXPLORER_COMPUTER_
#define _WIN_RESOURCE_EXPLORER_COMPUTER_

#include <_type/type.h>
#include <_type/type.view.h>
#include <_gadget/gadget.fileview.h>

class _explorerPageComputer : public _view
{
	private:
		
		_uniquePtr<_scrollArea> scrollArea;
		
		// Event-Handler for mouseClick Events
		_callbackReturn mouseClickHandler( _event event );
		
	public:
		
		//! Ctor
		_explorerPageComputer();
		
		void create( _gadget* viewParent );
		bool destroy( _gadget* viewParent );
};

#endif