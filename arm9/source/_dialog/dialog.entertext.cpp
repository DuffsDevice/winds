#include <_dialog/dialog.entertext.h>
#include <_controller/controller.gui.h>
#include <_controller/controller.localization.h>

void _enterTextDialog::executeInternal(){
	this->textBox->setStrValue( this->initialValue );
	this->window->setParent( _guiController::getHost() );
	this->window->focus();
}
void _enterTextDialog::cleanupInternal(){
	this->window->setParent( nullptr );
}

_enterTextDialog::_enterTextDialog( string message , string windowLbl , string initialValue , _optValue<string> okLbl , _optValue<string> cancelLbl ) : 
	okButton( new _button( 0 , 0 , ignore , ignore , okLbl.isValid() ? (string&&)okLbl : _localizationController::getBuiltInString("lbl_ok") ) )
	, cancelButton( new _button( 0 , 0 , ignore , ignore , cancelLbl.isValid() ? (string&&)cancelLbl : _localizationController::getBuiltInString("lbl_cancel") ) )
	, msg( new _label( 2 , 2 , ignore , ignore , (string&&)message ) )
	, initialValue( move(initialValue) )
{	
	// Buttons
	this->okButton->setAutoSelect( true );
	
	// Window-Width
	_length winWidth = max( this->msg->getWidth() + 4 , this->okButton->getWidth() + this->cancelButton->getWidth() + 5 );
	
	// Textbox
	this->textBox = new _textBox( winWidth - 6 , ignore , 2 , this->msg->getHeight() + 4 , this->initialValue );
	
	_length winHeight = max( this->msg->getHeight() + this->okButton->getHeight() + 10 + this->textBox->getHeight() , 30 ) + 11; // + 11 for the window
	
	// Window
	this->window = new _dialogWindow( ( SCREEN_WIDTH - winWidth ) >> 1 , ( SCREEN_HEIGHT - winHeight ) >> 1 , winWidth , winHeight , (string&&)windowLbl , _style::notResizeable );
	
	// Move Buttons
	this->cancelButton->moveTo( winWidth - this->okButton->getWidth() - this->cancelButton->getWidth() - 4 , winHeight - this->cancelButton->getHeight() - 12 );
	this->okButton->moveTo( winWidth - this->okButton->getWidth() - 3 , winHeight - this->cancelButton->getHeight() - 12 );
	
	// Stuff...
	this->okButton->setInternalEventHandler( onMouseClick , make_callback( this , &_enterTextDialog::eventHandler ) );
	this->textBox->setInternalEventHandler( onEdit , make_callback( this , &_enterTextDialog::eventHandler ) );
	this->cancelButton->setInternalEventHandler( onMouseClick , make_callback( this , &_enterTextDialog::eventHandler ) );
	this->window->setInternalEventHandler( onClose , make_callback( this , &_enterTextDialog::eventHandler ) );
	this->textBox->triggerEvent( onEdit );
	
	// Add Gadgets
	this->window->addChild( this->okButton );
	this->window->addChild( this->cancelButton );
	this->window->addChild( this->msg );
	this->window->addChild( this->textBox );
}

_callbackReturn _enterTextDialog::eventHandler( _event event )
{
	_gadget* that = event.getGadget();
	
	// Text Box
	if( that == this->textBox ){
		this->okButton->setEnabled( !this->textBox->getStrValue().empty() );
		return handled;
	}
	// OK-Button
	else if( that == this->okButton )
		this->callCallback( _dialogResult::yes );
	// Cancel-Button or Window-Close-Button
	else if( that == this->cancelButton || that == this->window )
		this->callCallback( _dialogResult::no );
	this->cleanupInternal();
	
	return handled;
}