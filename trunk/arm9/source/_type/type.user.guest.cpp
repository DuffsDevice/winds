#include <_type/type.user.guest.h>
#include <_controller/controller.localization.h>

_guestUser::_guestUser() :
	_user( "" )
{
	_iniFile::writeIndex( "_global_" , "userCode" , "" );
	_iniFile::writeIndex( "_global_" , "homeFolder" , "%USERS%/%guest" );
}

const wstring&	_guestUser::getUserName() const {
	return _localizationController::getBuiltInString("lbl_guest");
}