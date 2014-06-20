#include "_dialog/dialog.folder.choose.h"
#include "_type/type.gadget.helpers.h"
#include "_controller/controller.gui.h"
#include "_controller/controller.localization.h"

void _folderChooseDialog::executeInternal(){
	this->folderPath.clear();
	this->window->setParent( _guiController::getHost() );
	this->window->focus();
}
void _folderChooseDialog::cleanupInternal(){
	this->window->setParent( nullptr );
}

_folderChooseDialog::_folderChooseDialog( _optValue<string> descriptionLabel , _optValue<string> okLabel , _optValue<string> windowLabel ) :
	descriptionLabel( nullptr )
{
	// Determine some dimensions
	const _length	fileTreeWidth = 88;
	const _length	fileTreeHeight = 75;
	const _length	firstLineY = 5;
	_length			secondLineY = 14;
	const _length	xStart = 4;
	const _length	winWidth = fileTreeWidth + 10;
	_length			winHeight = fileTreeHeight + 43;
	
	// Build initial folder path and description label
	windowLabel = windowLabel.isValid() ? (string&&)windowLabel : _localizationController::getBuiltInString("lbl_choose_folder");
	
	// Description Label
	if( descriptionLabel.isValid() )
		this->descriptionLabel = new _label( xStart , firstLineY , fileTreeWidth , ignore , descriptionLabel );
	else{
		winHeight -= (secondLineY - firstLineY);
		secondLineY = firstLineY;
	}
	
	// Create File Tree
	this->fileTree = new _fileTree( xStart , secondLineY , fileTreeWidth , fileTreeHeight , "/" );
	
	
	// Buttons
	this->cancelButton = new _button( ignore , ignore , ignore , ignore , _localizationController::getBuiltInString("lbl_cancel") );
	this->okButton = new _button( ignore , ignore , ignore , ignore , okLabel.isValid() ? (string&&)okLabel : _localizationController::getBuiltInString("lbl_ok") );
	this->cancelButton->setUserEventHandler( onParentAdd , _gadgetHelpers::rightBottomAlign( 5 , 3 ) );
	this->okButton->setUserEventHandler( onParentAdd , _gadgetHelpers::moveBesidePrecedent( _direction::left , 1 ) );
	
	
	// Window
	this->window = new _dialogWindow( ( SCREEN_WIDTH - winWidth ) >> 1 , ( SCREEN_HEIGHT - winHeight ) >> 1 , winWidth , winHeight , (string&&)windowLabel , _style::notResizeable );
	
	
	// FileTree
	this->fileTree->setEventHandler( make_callback( this , &_folderChooseDialog::eventHandler ) );
	this->fileTree->setUserEventHandler( onEdit , make_callback( this , &_folderChooseDialog::eventHandler ) );
	
	
	// Stuff...
	this->okButton->setAutoSelect( true );
	this->okButton->setInternalEventHandler( onMouseClick , make_callback( this , &_folderChooseDialog::eventHandler ) );
	this->okButton->setInternalEventHandler( onEdit , make_callback( this , &_folderChooseDialog::eventHandler ) );
	this->cancelButton->setInternalEventHandler( onMouseClick , make_callback( this , &_folderChooseDialog::eventHandler ) );
	this->window->setInternalEventHandler( onClose , make_callback( this , &_folderChooseDialog::eventHandler ) );
	
	
	// Add Gadgets
	this->window->addChild( this->cancelButton );
	this->window->addChild( this->okButton , true );
	this->window->addChild( this->fileTree );
	if( this->descriptionLabel )
		this->window->addChild( this->descriptionLabel );
}

_callbackReturn _folderChooseDialog::eventHandler( _event event )
{
	_gadget* that = event.getGadget();
	
	// Single or Double Click on one of the fileobjects
	if( that->getType() == _gadgetType::fileobject )
	{
		_fileObject* fO = (_fileObject*)that;
		if( event == onMouseClick ){
			this->folderPath = fO->getDirentry().getFileName();
			this->okButton->triggerEvent( onEdit );
			return use_internal;
		}
		else if( event == onMouseDblClick ){
			this->cleanup();
			this->callCallback( _dialogResult::yes );
		}
		return handled;
	}
	
	// okButton should be updated
	else if( that == this->okButton && event == onEdit )
		this->okButton->setEnabled( !this->folderPath.empty() );
	
	// Cancel-Button or Window-Close-Button
	else
	{
		this->cleanup();
		
		// Cancel Button or Window-close
		if( that == this->cancelButton || that == this->window )
			this->callCallback( _dialogResult::cancel );
		// OK-Button
		if( that == this->okButton )
			this->callCallback( _dialogResult::yes );
	}
	
	
	return handled;
}