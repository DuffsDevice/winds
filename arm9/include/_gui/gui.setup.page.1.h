#ifndef _WIN_G_SETUP_PAGE1_
#define _WIN_G_SETUP_PAGE1_

#include <_type/type.h>
#include <_type/type.view.h>

#include <_gadget/gadget.label.h>
#include <_gadget/gadget.select.h>

class _guiSetupPage1 : public _view
{
	private:
	
		_label*		lblChooseLanguage;
		_label*		lblClickToContinue;
		_select*	slcLanguage;
		
		_callbackReturn languageChangeHandler( _event event );
	
	public:
		
		void update( _gadget* viewParent );
		void create( _gadget* viewParent );
		bool destroy( _gadget* viewParent );
};

#endif