#include <_dialog/dialog.file.open.h>
#include <_type/type.tokenizer.h>
#include <_resource/resource.icon.folder.up.h>
#include <_controller/controller.gui.h>
#include <_controller/controller.localization.h>

_fileExtensionList _fileOpenDialog::getFileMask( _int value ) const {
	const string& extensions = std::get<1>( this->fileTypes[value] );
	return tokenize( extensions , "," , true );
}

const _menuEntryList _fileOpenDialog::generateMenuList()
{
	if( !this->fileTypes.count(0) )
		this->fileTypes[0] = make_pair( _localizationController::getBuiltInString("lbl_all_types") , "*" );
	
	_menuEntryList menuList;
	for( auto& value : fileTypes ){
		string& val = menuList[value.first].text;
		val.swap( value.second.first );
		val += " (.";
		_vector<string> extensions = tokenize( value.second.second , "," , true );
		val += unTokenize( extensions , ", ." ).c_str();
		val += ")";
	}
	return move(menuList);
}

void _fileOpenDialog::executeInternal(){
	this->fileNameBox->setStrValue("");
	this->window->setParent( _guiController::getHost() );
	this->window->focus();
}
void _fileOpenDialog::cleanupInternal(){
	this->window->setParent( nullptr );
}

_fileOpenDialog::_fileOpenDialog( _fileTypeList possibleFileExtensions , string initialFilePath , _int initialFileExtension , _optValue<string> openLabel , _optValue<string> windowLabel ) :
	fileTypes( move(possibleFileExtensions) )
{
	// Constants
	_coord firstLineY = 70;
	_coord secondLineY = 82;
	_coord fileViewY = 12;
	
	// Labels
	this->fileNameLabel = new _label( 2 , firstLineY , ignore , ignore , _localizationController::getBuiltInString("def_filename") );	
	
	// Determine some widths
	const _length textBoxWidth = 60;
	const _length labelWidth = this->fileNameLabel->getWidth();
	
	
	// Create Filename Box
	this->fileNameBox = new _textBox( labelWidth + 3 , firstLineY , textBoxWidth , ignore );
	
	
	// Determine Start of 2nd X Coordinate
	_coord secondRowX = this->fileNameBox->getWidth() + this->fileNameLabel->getWidth() + 4;
	
	
	// Create the two buttons
	this->cancelButton = new _button( secondRowX , secondLineY , ignore , ignore , _localizationController::getBuiltInString("lbl_cancel") );
	this->openButton = new _button( secondRowX + cancelButton->getWidth() + 1 , secondLineY , ignore , ignore , openLabel.isValid() ? (string&&)openLabel : _localizationController::getBuiltInString("lbl_open") );
	
	
	// Create file type Selectbox
	_length selectWidth = this->cancelButton->getWidth() + this->openButton->getWidth() + 1;
	this->fileTypeChooser = new _select( secondRowX , firstLineY , selectWidth , generateMenuList() , initialFileExtension );
	
	
	// Adjust Label
	this->fileNameLabel->setHeight( this->fileNameBox->getHeight() );
	
	
	// Window
	_length winWidth = labelWidth + this->fileTypeChooser->getWidth() + textBoxWidth + 7;
	_length winHeight = secondLineY + this->openButton->getHeight() + 12;
	this->window = new _dialogWindow( ( SCREEN_WIDTH - winWidth ) >> 1 , ( SCREEN_HEIGHT - winHeight ) >> 1 , winWidth , winHeight , windowLabel.isValid() ? (string&&)windowLabel : _localizationController::getBuiltInString("lbl_open") , _style::notResizeable );
	
	// Fix Filepath
	initialFilePath = _direntry( initialFilePath ).getFileName();
	
	// FileView & Addressbar & Button
	this->fileViewAddress = new _textBox( 1 , 1 , winWidth - 14 - 11 , ignore , initialFilePath );
	this->gotoButton = new _actionButton( winWidth - 12 , 1 , _actionButtonType::next );
	this->gotoButton->setUserEventHandler( onMouseClick , make_callback( this , &_fileOpenDialog::eventHandler ) );
	this->folderUpButton = new _imageButton( winWidth - 14 - this->gotoButton->getWidth() , 1 , 10 , 9 , BMP_FolderUpIcon() );
	this->folderUpButton->setUserEventHandler( onMouseClick , make_callback( this , &_fileOpenDialog::eventHandler ) );
	this->fileView = new _fileView( 1 , fileViewY , winWidth - 4 , firstLineY - fileViewY - 1 , initialFilePath , _fileViewType::list );
	this->fileView->setFileMask( this->getFileMask( this->getFileType() ) );
	this->fileView->setEventHandler( make_callback( this , &_fileOpenDialog::eventHandler ) );
	this->fileView->setUserEventHandler( onEdit , make_callback( this , &_fileOpenDialog::eventHandler ) );
	
	
	// Stuff...
	this->openButton->setAutoSelect( true );
	this->openButton->setInternalEventHandler( onMouseClick , make_callback( this , &_fileOpenDialog::eventHandler ) );
	this->cancelButton->setInternalEventHandler( onMouseClick , make_callback( this , &_fileOpenDialog::eventHandler ) );
	this->fileTypeChooser->setInternalEventHandler( onEdit , make_callback( this , &_fileOpenDialog::eventHandler ) );
	this->window->setInternalEventHandler( onClose , make_callback( this , &_fileOpenDialog::eventHandler ) );
	this->fileNameBox->setUserEventHandler( onEdit , make_callback( this , &_fileOpenDialog::eventHandler ) );
	this->fileNameBox->handleEvent( onEdit ); // Update
	
	
	// Add Gadgets
	this->window->addChild( this->openButton );
	this->window->addChild( this->cancelButton );
	this->window->addChild( this->fileNameBox );
	this->window->addChild( this->fileTypeChooser );
	this->window->addChild( this->fileNameLabel );
	this->window->addChild( this->fileView );
	this->window->addChild( this->fileViewAddress );
	this->window->addChild( this->gotoButton );
	this->window->addChild( this->folderUpButton );
}

_callbackReturn _fileOpenDialog::eventHandler( _event event )
{
	_gadget* that = event.getGadget();
	
	// Single or Double Click on one of the fileobjects
	if( that->getType() == _gadgetType::fileobject )
	{
		_fileObject* fO = (_fileObject*)that;
		if( event == onMouseClick && !fO->getDirentry().isDirectory() ){
			this->fileNameBox->setStrValue( fO->getDirentry().getFullName(true) );
			this->fileNameBox->triggerEvent( onEdit );
		}
		else if( event == onMouseDblClick ){
			if( fO->getDirentry().isDirectory() )
				fO->execute();
			else{
				this->cleanup();
				this->callCallback( _dialogResult::yes );
			}
		}
		return handled;
	}
	
	// The value in the filetype _select has changed
	else if( that == this->fileTypeChooser )
		this->fileView->setFileMask( this->getFileMask( this->getFileType() ) );
	
	// The fileview has changed its path!
	else if( that == this->fileView )
		this->fileViewAddress->setStrValue( this->fileView->getPath() );
	
	// The Goto Button was pressed
	else if( that == this->gotoButton )
		this->fileView->setPath( this->fileViewAddress->getStrValue() );
	
	// The Folder Up Button
	else if( that == this->folderUpButton ){
		string path = _direntry( this->fileView->getPath() ).getParentDirectory();
		this->fileViewAddress->setStrValue( path );
		this->fileView->setPath( path );
	}
	// Open-Button
	else if( that == this->openButton ){
		if( _direntry( this->getFileName() ).isExisting() ){
			this->cleanup();
			this->callCallback( _dialogResult::yes );
		}
	}
	// Text-Box
	else if( that == this->fileNameBox )
		this->openButton->setEnabled( !this->fileNameBox->getStrValue().empty() );
	// Cancel-Button or Window-Close-Button
	else
	{
		this->cleanup();
		
		// Cancel Button or Window-close
		if( that == this->cancelButton || that == this->window )
			this->callCallback( _dialogResult::cancel );
	}
	
	return handled;
}