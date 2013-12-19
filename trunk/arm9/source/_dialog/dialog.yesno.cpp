#include "_dialog/dialog.yesno.h"
#include "_type/type.system.h"

void _yesNoDialog::executeInternal(){
	this->window->setParent( _system::_gadgetHost_ );
	this->window->focus();
}
void _yesNoDialog::cleanupInternal(){
	this->window->setParent( nullptr );
}

_yesNoDialog::_yesNoDialog( string message , string windowLbl , _optValue<string> yesLbl , _optValue<string> noLbl ) :
	yesButton( new _button( 0 , 0 , ignore , ignore , yesLbl.isValid() ? (string&&)yesLbl : _system::getLocalizedString("lbl_yes") ) )
	, noButton( new _button( 0 , 0 , ignore , ignore , noLbl.isValid() ? (string&&)noLbl : _system::getLocalizedString("lbl_no") ) )
	, msg( new _label( 2 , 2 , ignore , ignore , (string&&)message ) )
{
	// Buttons
	this->yesButton->setAutoSelect( true );
	
	// Window
	_length winWidth = max( this->msg->getWidth() + 4 , this->noButton->getWidth() + this->yesButton->getWidth() + 5 );
	_length winHeight = max( this->msg->getHeight() + this->noButton->getHeight() + 8 , 30 ) + 11; // + 11 for the window
	
	// Create Window
	this->window = new _window( ( SCREEN_WIDTH - winWidth ) >> 1 , ( SCREEN_HEIGHT - winHeight ) >> 1 , winWidth , winHeight , windowLbl , false , true , _styleAttr() | _styleAttr::canNotLooseFocus | _styleAttr::notResizeable | _styleAttr::draggable );
	
	// Move Buttons to right position
	this->yesButton->moveTo( winWidth - this->yesButton->getWidth() - 3 , winHeight - this->yesButton->getHeight() - 12 );
	this->noButton->moveTo( winWidth - this->noButton->getWidth() - this->yesButton->getWidth() - 4 , winHeight - this->yesButton->getHeight() - 12 );
	
	// Add the to the window
	this->window->addChild( this->noButton );
	this->window->addChild( this->yesButton );
	this->window->addChild( this->msg );
	
	// Register some handlers
	this->noButton->setInternalEventHandler( onMouseClick , make_callback( this , &_yesNoDialog::eventHandler ) );
	this->yesButton->setInternalEventHandler( onMouseClick , make_callback( this , &_yesNoDialog::eventHandler ) );
	this->window->setInternalEventHandler( onClose , make_callback( this , &_yesNoDialog::eventHandler ) );
}

_callbackReturn _yesNoDialog::eventHandler( _event event )
{
	_gadget* that = event.getGadget();
	
	// No-Button
	if( that == this->noButton ){
		this->callCallback( _dialogResult::no );
		this->cleanupInternal();
	}
	// Yes-Button
	else if( that == this->yesButton ){
		this->callCallback( _dialogResult::yes );
		this->cleanupInternal();
	}
	// Window-Close-Button
	else if( that == this->window ){
		this->callCallback( _dialogResult::cancel );
		this->cleanupInternal();
	}
	
	return handled;
}