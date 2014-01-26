#include "_dialog/dialog.file.save.h"
#include "_type/type.system.h"

void _fileSaveDialog::executeInternal(){
	this->fileNameBox->setStrValue( this->initialName );
	this->window->setParent( _system::_gadgetHost_ );
	this->window->focus();
}
void _fileSaveDialog::cleanupInternal(){
	this->window->setParent( nullptr );
}

_fileSaveDialog::_fileSaveDialog( _fileTypeList possibleFileExtensions , _optValue<string> initialName , _int initialFileExtension , _optValue<string> saveLabel ) :
	initialName( initialName.isValid() ? (string&&)initialName : _system::getLocalizedString("lbl_unnamed") )
	, fileTypes( move(possibleFileExtensions) )
{
	// Constants
	_coord firstLineY = 70;
	_coord secondLineY = 82;
	_coord fileViewY = 12;
	
	// Labels
	this->fileNameLabel = new _label( 2 , firstLineY , ignore , ignore , _system::getLocalizedString("def_filename") );
	this->fileTypeLabel = new _label( 2 , secondLineY , ignore , ignore , _system::getLocalizedString("def_filetype") );	
	
	
	// Determine some widths
	const _length textBoxAndSelectWidth = 60;
	const _length labelWidth = max( this->fileNameLabel->getWidth() , this->fileTypeLabel->getWidth() );
	
	
	// Create Select & Filename Box
	this->fileNameBox = new _textBox( labelWidth + 3 , firstLineY , textBoxAndSelectWidth , ignore , this->initialName );
	this->fileTypeChooser = new _select( labelWidth + 3 , secondLineY , textBoxAndSelectWidth , generateMenuList() , initialFileExtension );
	
	
	// Adjust Labels
	this->fileNameLabel->setSize( labelWidth , this->fileTypeChooser->getHeight() );
	this->fileTypeLabel->setSize( labelWidth , this->fileNameBox->getHeight() );
	this->fileNameLabel->setAlign( _align::right );
	this->fileTypeLabel->setAlign( _align::right );
	
	
	// Buttons
	this->cancelButton = new _button( labelWidth + textBoxAndSelectWidth + 4 , firstLineY , ignore , ignore , _system::getLocalizedString("lbl_cancel") );
	this->saveButton = new _button( labelWidth + textBoxAndSelectWidth + 4 , secondLineY , ignore , ignore , saveLabel.isValid() ? (string&&)saveLabel : _system::getLocalizedString("lbl_save") );
	
	
	// Button Width
	_length buttonWidth = max( this->cancelButton->getWidth() , this->saveButton->getWidth() );
	this->cancelButton->setWidth( buttonWidth );
	this->saveButton->setWidth( buttonWidth );
	
	
	// Window
	_length winWidth = labelWidth + buttonWidth + textBoxAndSelectWidth + 7;
	_length winHeight = secondLineY + this->saveButton->getHeight() + 12;
	this->window = new _window( ( SCREEN_WIDTH - winWidth ) >> 1 , ( SCREEN_HEIGHT - winHeight ) >> 1 , winWidth , winHeight , _system::getLocalizedString("lbl_save") , false , true , _styleAttr() | _styleAttr::notResizeable | _styleAttr::draggable );
	
	
	// FileView & Addressbar & Button
	this->fileViewAddress = new _textBox( 1 , 1 , winWidth - 14 , ignore , "/moonmemo/" );
	this->gotoButton = new _actionButton( winWidth - 12 , 1 , _actionButtonType::next );
	this->gotoButton->setUserEventHandler( onMouseClick , make_callback( this , &_fileSaveDialog::eventHandler ) );
	this->fileView = new _fileView( 1 , fileViewY , winWidth - 4 , firstLineY - fileViewY - 1 , "/moonmemo/" , _fileViewType::list );
	this->fileView->setFileMask( _fileExtensionList( { std::get<1>(this->fileTypes[this->getFileType()]) } ) );
	this->fileView->setEventHandler( make_callback( this , &_fileSaveDialog::eventHandler ) );
	this->fileView->setUserEventHandler( onEdit , make_callback( this , &_fileSaveDialog::eventHandler ) );
	
	
	// Stuff...
	this->saveButton->setAutoSelect( true );
	this->saveButton->setInternalEventHandler( onMouseClick , make_callback( this , &_fileSaveDialog::eventHandler ) );
	this->cancelButton->setInternalEventHandler( onMouseClick , make_callback( this , &_fileSaveDialog::eventHandler ) );
	this->fileTypeChooser->setInternalEventHandler( onEdit , make_callback( this , &_fileSaveDialog::eventHandler ) );
	this->window->setInternalEventHandler( onClose , make_callback( this , &_fileSaveDialog::eventHandler ) );
	
	
	// Add Gadgets
	this->window->addChild( this->saveButton );
	this->window->addChild( this->cancelButton );
	this->window->addChild( this->fileNameBox );
	this->window->addChild( this->fileTypeChooser );
	this->window->addChild( this->fileTypeLabel );
	this->window->addChild( this->fileNameLabel );
	this->window->addChild( this->fileView );
	this->window->addChild( this->fileViewAddress );
	this->window->addChild( this->gotoButton );
}

_callbackReturn _fileSaveDialog::eventHandler( _event event )
{
	_gadget* that = event.getGadget();
	
	// Single or Double Click on one of the fileobjects
	if( that->getType() == _gadgetType::fileobject )
	{
		_fileObject* fO = (_fileObject*)that;
		if( event == onMouseClick && !fO->getDirentry().isDirectory() )
			this->fileNameBox->setStrValue( fO->getDirentry().getDisplayName() );
		else if( event == onMouseDblClick ){
			if( fO->getDirentry().isDirectory() )
				fO->execute();
			else{
				this->cleanupInternal();
				this->callCallback( _dialogResult::yes );
			}
		}
		return handled;
	}
	
	// The value in the filetype _select has changed
	else if( that == this->fileTypeChooser )
		this->fileView->setFileMask( _fileExtensionList( { std::get<1>(this->fileTypes[this->getFileType()]) } ) );
	
	// The fileview has changed its path!
	else if( that == this->fileView )
		this->fileViewAddress->setStrValue( this->fileView->getPath() );
	
	// The Goto Button was pressed
	else if( that == this->gotoButton )
		this->fileView->setPath( this->fileViewAddress->getStrValue() );
	
	// Cancel-Button or Window-Close-Button
	else
	{
		this->cleanupInternal();
		
		// Cancel Button or Window-close
		if( that == this->cancelButton || that == this->window )
			this->callCallback( _dialogResult::no );
		// OK-Button
		if( that == this->saveButton )
			this->callCallback( _dialogResult::yes );
	}
	
	
	return handled;
}