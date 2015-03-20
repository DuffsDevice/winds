#include <_dialog/dialog.errorwarninginfo.h>
#include <_resource/resource.image.alerts.h>
#include <_controller/controller.localization.h>
#include <_type/type.winds.soundbank.h>

void _errorDialog::executeInternal(){
	_windsSoundBank::play( _windsSound::fatalError );
	_imageDialog::executeInternal();
}

void _warningDialog::executeInternal(){
	_windsSoundBank::play( _windsSound::alert );
	_imageDialog::executeInternal();
}

void _infoDialog::executeInternal(){
	_windsSoundBank::play( _windsSound::bubble );
	_imageDialog::executeInternal();
}

_errorDialog::_errorDialog( wstring message , _optValue<wstring> okLabel , _optValue<wstring> otherLabel ) : 
	_imageDialog( move(message) , _localizationController::getBuiltInString("lbl_error") , BMP_AlertImageError() , move(okLabel) , move(otherLabel) )
{}

_infoDialog::_infoDialog( wstring message , _optValue<wstring> okLabel , _optValue<wstring> otherLabel ) : 
	_imageDialog( move(message) , _localizationController::getBuiltInString("lbl_info") , BMP_AlertImageInfo() , move(okLabel) , move(otherLabel) )
{}

_warningDialog::_warningDialog( wstring message , _optValue<wstring> okLabel , _optValue<wstring> otherLabel ) : 
	_imageDialog( move(message) , _localizationController::getBuiltInString("lbl_warning") , BMP_AlertImageWarning() , okLabel.isValid() ? move(okLabel) : _optValue<wstring>( _localizationController::getBuiltInString("lbl_dismiss") ) , move(otherLabel) )
{}