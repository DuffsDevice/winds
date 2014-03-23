#include "_dialog/dialog.runtimeerror.h"
#include "_type/type.system.h"
#include "_resource/resource.image.alerts.h"

_constBitmap _runtimeErrorDialog::errorimage = BMP_AlertImageError();

void _runtimeErrorDialog::executeInternal(){
	this->window->setParent( _system::_gadgetHost_ );
	this->window->focus();
}
void _runtimeErrorDialog::cleanupInternal(){
	this->window->setParent( nullptr );
}

_runtimeErrorDialog::_runtimeErrorDialog( string message , string additionalInfo ) : 
	closeButton( new _button( 0 , 0 , ignore , ignore , _system::getLocalizedString("lbl_close") ) )
	, msg( new _label( errorimage.getWidth() + 6 , 2 , ignore , ignore , (string&&)message ) )
	, image( new _imageGadget( 3 , 3 , errorimage ) )
	, additionalInfo( new _textArea( 1 , 1 , 50 , 25 , (string&&)additionalInfo ) )
{
	// Buttons
	this->closeButton->setAutoSelect( true );
	
	// Labels
	this->msg->setVAlign( _valign::middle );
	
	// Set the height of the label to be minimum the size of the image
	this->msg->setHeight( max( errorimage.getHeight() + 1 , (int) this->msg->getHeight() ) );
	
	// Window-Width
	_length winWidth = max(
		90
		, max(
			this->msg->getWidth() + errorimage.getWidth() + 10
			, this->closeButton->getWidth() + 4
		)
	);
	
	// Set textarea height
	this->additionalInfo->setWidth( winWidth - 4 );
	
	// Window-Height
	_length winHeight = this->msg->getHeight() + this->closeButton->getHeight() + 7 + 11 + 35; // + 11 for the window, + 35 for the textarea
	
	// Creating the window
	this->window = new _window( ( SCREEN_WIDTH - winWidth ) >> 1 , ( SCREEN_HEIGHT - winHeight ) >> 1 , winWidth , winHeight , _system::getLocalizedString("lbl_runtime_error") , false , true , _style::notResizeable | _style::draggable );
	
	// Move buttons to right position
	this->closeButton->moveTo( winWidth - this->closeButton->getWidth() - 3 , this->msg->getHeight() + 4 );
	this->additionalInfo->moveTo( 1 , winHeight - this->additionalInfo->getHeight() - 12 );
	
	this->window->addChild( this->closeButton );
	this->window->addChild( this->image );
	this->window->addChild( this->msg );
	this->window->addChild( this->additionalInfo );

	this->closeButton->setInternalEventHandler( onMouseClick , make_callback( this , &_runtimeErrorDialog::eventHandler ) );
	this->window->setInternalEventHandler( onClose , make_callback( this , &_runtimeErrorDialog::eventHandler ) );
}

_callbackReturn _runtimeErrorDialog::eventHandler( _event event )
{
	// Dismiss-Button or Window-Close-Button
	this->cleanup();
	this->callCallback( _dialogResult::cancel );
	
	return handled;
}