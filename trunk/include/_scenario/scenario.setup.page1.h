#ifndef _WIN_SC_SETUP_PAGE1_
#define _WIN_SC_SETUP_PAGE1_

#include "_type/type.h"
#include "_type/type.view.h"
#include "_type/type.system.h"

#include "_gadget/gadget.label.h"
#include "_gadget/gadget.select.h"

class _scSetupPage1 : public _view
{
	private:
	
		_label*		lblChooseLanguage;
		_label*		lblClickToContinue;
		_select*	slcLanguage;
		
		_callbackReturn languageChangeHandler( _event event )
		{
			// Write Language
			_system::setLanguage( (_language)this->slcLanguage->getIntValue() );
			
			// Update view
			getSwitcher().update();
			
			return handled;
		}
	
	public:
		
		void update( _gadget* viewParent ){
			this->lblChooseLanguage->setStrValue( _system::getLocalizedString("lbl_choose_language") );
			this->lblClickToContinue->setStrValue( _system::getLocalizedString("lbl_to_continue_click_next") );
		}
		
		void create( _gadget* viewParent )
		{
			// Label
			this->lblChooseLanguage = new _label( 86 , 72 , ignore , ignore , _system::getLocalizedString("lbl_choose_language") );
			this->lblChooseLanguage->setColor( RGB( 30 , 30 , 30 ) );
			viewParent->addChild( this->lblChooseLanguage );
			
			// Select
			this->slcLanguage = new _select(
				85 , 80 , 94
				, {
					{ 1 , "English" }
					, { 2 , "Français" }
					, { 3 , "Deutsch" }
					, { 4 , "Italiano" }
					, { 5 , "Español" }
				}
				, (_u8)_system::getLanguage()
			);
			
			this->slcLanguage->setUserEventHandler( onEdit , make_callback( this , &_scSetupPage1::languageChangeHandler ) );
			viewParent->addChild( this->slcLanguage );
			
			// Hint to click continue
			this->lblClickToContinue = new _label( 71 , 6 , 180 , 9 , _system::getLocalizedString("lbl_to_continue_click_next") );
			this->lblClickToContinue->setColor( RGB( 17 , 17 , 31 ) );
			this->lblClickToContinue->setAlign( _align::right );
			viewParent->addChild( this->lblClickToContinue );
		}
		
		bool destroy( _gadget* viewParent )
		{
			delete this->lblChooseLanguage;
			delete this->lblClickToContinue;
			delete this->slcLanguage;
			
			// Switching the view is allowed
			return true;
		}
};

#endif