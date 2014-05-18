#ifndef _WIN_G_SETUP_PAGE2_
#define _WIN_G_SETUP_PAGE2_

#include "_type/type.h"
#include "_type/type.view.h"

#include "_gadget/gadget.label.h"
#include "_gadget/gadget.select.h"

class _guiSetupPage2 : public _view
{
	private:
	
		_label*		title;
		_label*		line1;
		_label*		line2;
	
	public:
		
		void create( _gadget* viewParent );
		bool destroy( _gadget* viewParent );
};

#endif