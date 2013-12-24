#include "_dialog/dialog.errorwarninginfo.h"
#include "_resource/resource.image.alerts.h"
#include "_type/type.system.h"

_constBitmap _errorDialog::image = BMP_AlertImageError();
_constBitmap _warningDialog::image = BMP_AlertImageWarning();
_constBitmap _infoDialog::image = BMP_AlertImageInfo();

_errorDialog::_errorDialog( string message , _optValue<string> okLabel , _optValue<string> otherLabel ) : 
	_imageDialog( move(message) , _system::getLocalizedString("lbl_error") , image , move(okLabel) , move(otherLabel) )
{}

_infoDialog::_infoDialog( string message , _optValue<string> okLabel , _optValue<string> otherLabel ) : 
	_imageDialog( move(message) , _system::getLocalizedString("lbl_info") , image , move(okLabel) , move(otherLabel) )
{}

_warningDialog::_warningDialog( string message , _optValue<string> okLabel , _optValue<string> otherLabel ) : 
	_imageDialog( move(message) , _system::getLocalizedString("lbl_warning") , image , okLabel.isValid() ? move(okLabel) : _optValue<string>( _system::getLocalizedString("lbl_dismiss") ) , move(otherLabel) )
{}