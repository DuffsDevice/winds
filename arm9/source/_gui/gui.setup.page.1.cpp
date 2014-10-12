#include <_gui/gui.setup.page.1.h>
#include <_controller/controller.localization.h>
#include <_type/type.view.switcher.h>
#include <_type/type.windows.h>

_callbackReturn _guiSetupPage1::languageChangeHandler( _event event )
{
	// Write Language
	_localizationController::setLanguage( id2language[ this->slcLanguage->getIntValue() ] );
	
	// Update view
	getSwitcher().update();
	
	return handled;
}

void _guiSetupPage1::update( _gadget* viewParent ){
	this->lblChooseLanguage->setStrValue( _localizationController::getBuiltInString("lbl_choose_language") );
	this->lblClickToContinue->setStrValue( _localizationController::getBuiltInString("lbl_to_continue_click_next") );
}

void _guiSetupPage1::create( _gadget* viewParent )
{
	// Label
	this->lblChooseLanguage = new _label( 86 , 72 , ignore , ignore , _localizationController::getBuiltInString("lbl_choose_language") );
	this->lblChooseLanguage->setColor( _color::fromRGB( 30 , 30 , 30 ) );
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
		, language2id[ _windows::getDSLanguage() ]
	);
	
	this->slcLanguage->setUserEventHandler( onEdit , make_callback( this , &_guiSetupPage1::languageChangeHandler ) );
	viewParent->addChild( this->slcLanguage );
	
	// Hint to click continue
	this->lblClickToContinue = new _label( 71 , 6 , 180 , 9 , _localizationController::getBuiltInString("lbl_to_continue_click_next") );
	this->lblClickToContinue->setColor( _color::fromRGB( 17 , 17 , 31 ) );
	this->lblClickToContinue->setAlign( _align::right );
	viewParent->addChild( this->lblClickToContinue );
}

bool _guiSetupPage1::destroy( _gadget* viewParent )
{
	delete this->lblChooseLanguage;
	delete this->lblClickToContinue;
	delete this->slcLanguage;
	
	// Switching the view is allowed
	return true;
}