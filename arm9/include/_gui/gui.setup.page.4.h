#ifndef _WIN_G_SETUP_PAGE4_
#define _WIN_G_SETUP_PAGE4_

#include <_type/type.h>
#include <_type/type.view.h>
#include <_type/type.gadget.h>
#include <_type/type.gadget.helpers.h>

#include <_gadget/gadget.label.h>
#include <_gadget/gadget.textbox.h>
#include <_gadget/gadget.image.h>

class _guiSetupPage4 : public _view
{
	private:
	
		_label*						lblTitle;
		_label*						lblName;
		_label*						lblIcon;
		_textBox*					txtProfileName;
		_map<_imageGadget*,_bitmap>	imgProfileIcons;
		
		// profile name will be stored here
		wstring						profileName;
		_s8							profileIcon;
		
		_callbackReturn profileIconClickHandler( _event event );
		_callbackReturn profileIconDrawHandler( _event );
		_callbackReturn profileNameTextboxHandler( _event )
		{
			this->profileName = this->txtProfileName->getStrValue();
			return handled;
		}
		
		// Path to usericons
		static const string basepath;
		
		// Data for profile icons
		static const string imageId2filename[];
		static const _s8 iconNumbers[];
		
		friend class _guiSetupPage5;
	
	public:
		
		//! Ctor
		_guiSetupPage4();
		
		void create( _gadget* viewParent );
		bool destroy( _gadget* viewParent );
};

#endif