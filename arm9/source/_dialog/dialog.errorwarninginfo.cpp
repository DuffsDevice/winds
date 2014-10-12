#include <_dialog/dialog.errorwarninginfo.h>
#include <_resource/resource.image.alerts.h>
#include <_controller/controller.localization.h>
#include <_type/type.windows.soundbank.h>

void _errorDialog::executeInternal(){
	_windowsSoundBank::play( _windowsSound::fatalError );
	_imageDialog::executeInternal();
}

void _warningDialog::executeInternal(){
	_windowsSoundBank::play( _windowsSound::alert );
	_imageDialog::executeInternal();
}

void _infoDialog::executeInternal(){
	_windowsSoundBank::play( _windowsSound::bubble );
	_imageDialog::executeInternal();
}

_errorDialog::_errorDialog( string message , _optValue<string> okLabel , _optValue<string> otherLabel ) : 
	_imageDialog( move(message) , _localizationController::getBuiltInString("lbl_error") , BMP_AlertImageError() , move(okLabel) , move(otherLabel) )
{}

_infoDialog::_infoDialog( string message , _optValue<string> okLabel , _optValue<string> otherLabel ) : 
	_imageDialog( move(message) , _localizationController::getBuiltInString("lbl_info") , BMP_AlertImageInfo() , move(okLabel) , move(otherLabel) )
{}

_warningDialog::_warningDialog( string message , _optValue<string> okLabel , _optValue<string> otherLabel ) : 
	_imageDialog( move(message) , _localizationController::getBuiltInString("lbl_warning") , BMP_AlertImageWarning() , okLabel.isValid() ? move(okLabel) : _optValue<string>( _localizationController::getBuiltInString("lbl_dismiss") ) , move(otherLabel) )
{}