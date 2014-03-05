#include "_scenario/scenario.setup.page5.h"

#include "_scenario/scenario.setup.page4.h"
#include "_scenario/scenario.setup.page3.h"

void _scSetupPage5::timerCallback()
{
	_viewSwitcher& switcher = this->getSwitcher();
	
	//_scSetupPage3* page3 = (_scSetupPage3*) switcher.getViewByName( "3" );
	_scSetupPage4* page4 = (_scSetupPage4*) switcher.getViewByName( "4" );
	
	string profileName = page4->profileName;
	string profileIcon = _scSetupPage4::imageId2filename[ page4->profileIcon ];
	
	_user user = _user( profileName );
	user.writeIndex( "_global_" , "userLogo" , profileIcon );
}