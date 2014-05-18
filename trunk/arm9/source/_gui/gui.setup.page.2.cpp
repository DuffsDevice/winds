#include "_gui/gui.setup.page.2.h"
#include "_controller/controller.localization.h"
#include "_controller/controller.font.h"

void _guiSetupPage2::create( _gadget* viewParent )
{
	// Welcome label
	this->title = new _label( 13 , 28 , ignore , ignore , _localizationController::getBuiltInString("lbl_welcome_to_winds") );
	this->title->setColor( _color::fromRGB( 30 , 30 , 30 ) );
	this->title->setFont( _fontController::getFont( "ArialBlack13" ) );
	
	// Few steps to set up label
	this->line1 = new _label( 20 , 60 , ignore , ignore , _localizationController::getBuiltInString("txt_few_step_setup") );
	this->line1->setColor( _color::fromRGB( 30 , 30 , 30 ) );
	
	// Few steps to set up label (line 2)
	this->line2 = new _label( 20 , 70 , ignore , ignore , _localizationController::getBuiltInString("txt_few_step_setup_2") );
	this->line2->setColor( _color::fromRGB( 30 , 30 , 30 ) );			
	
	viewParent->addChild( this->title );
	viewParent->addChild( this->line1 );
	viewParent->addChild( this->line2 );
}

bool _guiSetupPage2::destroy( _gadget* viewParent )
{
	delete this->title;
	delete this->line1;
	delete this->line2;
	
	// Switching the view is allowed
	return true;
}