#include "_dialog/dialog.image.h"
#include "_type/type.system.h"

void _imageDialog::executeInternal(){
	this->window->setParent( _system::_gadgetHost_ );
	this->window->focus();
}
void _imageDialog::cleanupInternal(){
	this->window->setParent( nullptr );
}

_imageDialog::_imageDialog( string message , string windowLbl , _bitmap&& bitmap ,  _optValue<string> okLabel , _optValue<string> otherLabel ) : 
	okButton( new _button( 0 , 0 , okLabel.isValid() ? (string&&)okLabel : _system::getLocalizedString("lbl_ok") ) )
	, otherButton( otherLabel.isValid() ? new _button( 0 , 0 , otherLabel ) : nullptr )
	, msg( new _label( bitmap.getWidth() + 6 , 2 , (string&&)message ) )
	, image( new _imagegadget( 3 , 3 , bitmap ) )
{	
	// Buttons
	this->okButton->setAutoSelect( true );	
	
	// Labels
	this->msg->setVAlign( _valign::middle );
	
	// Set the height of the label to be minimum the size of the image
	this->msg->setHeight( max( bitmap.getHeight() + 1 , (int) this->msg->getHeight() ) );
	
	
	// Window-Width
	_length winWidth = max(
		55
		, max(
			this->msg->getWidth() + bitmap.getWidth() + 10
			, (
				this->otherButton ? this->otherButton->getWidth() + 1 : 0
			) + this->okButton->getWidth() + 4
		)
	);
	
	// Window-Height
	_length winHeight = max( this->msg->getHeight() + this->okButton->getHeight() + 7 , 30 ) + 11; // + 11 for the window
	
	// Creating the window
	this->window = new _window( winWidth , winHeight , ( SCREEN_WIDTH - winWidth ) >> 1 , ( SCREEN_HEIGHT - winHeight ) >> 1  , windowLbl , false , true , _styleAttr() | _styleAttr::canNotLooseFocus | _styleAttr::notResizeable | _styleAttr::draggable );
	
	// Move buttons to right position
	this->okButton->moveTo( winWidth - this->okButton->getWidth() - 3 , winHeight - this->okButton->getHeight() - 12 );
	if( this->otherButton )
		this->otherButton->moveTo( winWidth - this->okButton->getWidth() - 4 - this->otherButton->getWidth() , winHeight - this->okButton->getHeight() - 12 );
	
	this->window->addChild( this->okButton );
	this->window->addChild( this->image );
	this->window->addChild( this->msg );
	
	if( this->otherButton )
		this->window->addChild( this->otherButton );
	
	this->okButton->setInternalEventHandler( onMouseClick , make_callback( this , &_imageDialog::eventHandler ) );
	this->window->setInternalEventHandler( onClose , make_callback( this , &_imageDialog::eventHandler ) );
	if( this->otherButton )
		this->otherButton->setInternalEventHandler( onMouseClick , make_callback( this , &_imageDialog::eventHandler ) );
}

_callbackReturn _imageDialog::eventHandler( _event event )
{
	_gadget* that = event.getGadget();
	
	// Other-Button
	if( that == this->otherButton ){
		this->callCallback( _dialogResult::yes );
		this->cleanupInternal();
	}
	// Dismiss-Button or Window-Close-Button
	else{
		this->callCallback( _dialogResult::cancel );
		this->cleanupInternal();
	}
	
	return handled;
}