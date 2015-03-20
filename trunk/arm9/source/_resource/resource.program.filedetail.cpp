#include <_resource/resource.program.filedetail.h>
#include <_type/type.gadget.helpers.h>
#include <_controller/controller.localization.h>
#include <_controller/controller.registry.h>
#include <_controller/controller.font.h>
#include <_controller/controller.gui.h>
#include <_controller/controller.debug.h>
#include <_type/type.windows.h>

PROG_FileDetail::PROG_FileDetail()
{
	_programHeader header;
	header.name = _localizationController::getBuiltInString("lbl_file_properties");
	header.fileName = string("FileDetail");
	header.author = string("WinDS");
	header.description = string("Gives Information about a file");
	this->setHeader( move(header) );
}

void PROG_FileDetail::main( _args args )
{
	if( args[0].empty() ){
		this->terminate();
		return;
	}
	// Create a _direntry structure
	this->file = new _direntry( args[0] );
	
	// Window
	_mainFrame* mainFrame = _program::getMainFrame( 125 , 165 , _style::notResizeable );
	mainFrame->setUserEventHandler( onClose , make_callback( this , &PROG_FileDetail::handler ) );
	
	this->fileIcon			= new _imageGadget( 7 , 7 , this->file->getFileImage() );
	this->fileNameTextbox	= new _textBox( 25 , 7 , 91 , ignore , this->file->getFullName() );
	mainFrame->addChild( this->fileIcon );
	mainFrame->addChild( this->fileNameTextbox );
	
	// Buttons
		this->okButton			= new _button( ignore , ignore , ignore , ignore , _localizationController::getBuiltInString("lbl_ok") );
		this->cancelButton		= new _button( ignore , ignore , ignore , ignore , _localizationController::getBuiltInString("lbl_cancel") );
		
		this->okButton->setAutoSelect( true );
		this->okButton->setUserEventHandler( onParentAdd , _gadgetHelpers::rightBottomAlign( 2 , 2 ) );
		this->okButton->setUserEventHandler( onMouseClick , make_callback( this , &PROG_FileDetail::handler ) );
		this->cancelButton->setUserEventHandler( onParentAdd , _gadgetHelpers::moveBesidePrecedent( _direction::left , 2 ) );
		this->cancelButton->setUserEventHandler( onMouseClick , make_callback( this , &PROG_FileDetail::handler ) );
		
		mainFrame->addChild( this->okButton , true );
		mainFrame->addChild( this->cancelButton , true );
	
	// Attribute Table
		this->fileTypeLabel = new _label( 4 , 24 , 48 , ignore , "Filetype:" );
		this->fileTypeValueLabel = new _label( 56 , 24 , 61 , ignore , this->file->getMimeType().getName() );
		this->fileTypeLabel->setAlign( _align::left );
		this->fileTypeLabel->setEllipsis( true , 1 );
		this->fileTypeValueLabel->setAlign( _align::left );
		mainFrame->addChild( this->fileTypeLabel );
		mainFrame->addChild( this->fileTypeValueLabel );
	
	switch( this->file->getMimeType() )
	{
		case _mime::directory:
		case _mime::application_octet_stream:
		case _mime::application_microsoft_installer:
		case _mime::application_x_lua_bytecode:
		case _mime::application_x_bat:
			break;
		default:
			{
				string command = _registryController::getFileTypeHandler( this->file->getExtension() );
				_uniquePtr<_program> progObject = _program::fromFile( _args::splitCommand( command ).first );
				wstring progName = command;
				if( progObject )
				{
					// Fetch Header of program
					_programHeader& header = progObject->getHeader();
					
					// Set Name to display
					if( header.name && !header.name->empty() )
						progName = move( *header.name );
				}
				this->openedWithLabel = new _label( 4 , 34 , 48 , ignore , "Def. program:" );
				this->openedWithValueLabel = new _label( 56 , 34 , 46 , ignore , progName );
				this->openedWithLabel->setAlign( _align::left );
				this->openedWithLabel->setEllipsis( true , 1 );
				this->openedWithValueLabel->setAlign( _align::left );
				mainFrame->addChild( this->openedWithLabel );
				mainFrame->addChild( this->openedWithValueLabel );
				
				this->openedWithChangeButton = new _button( 104 , 32 , 13 , 11 , "✎" );
				this->openedWithChangeButton->setUserEventHandler( onMouseClick , make_callback( this , &PROG_FileDetail::handler ) );
				mainFrame->addChild( this->openedWithChangeButton );
			}
	}
	
		_literal dateTimeFormat = _localizationController::getBuiltInString("fmt_date_time").c_str();
		
		this->locationLabel = new _label( 4 , 48 , 48 , ignore , "Location:" );
		this->locationValueLabel = new _label( 56 , 48 , 59 , ignore , this->file->getParentDirectory() );
		this->locationLabel->setAlign( _align::left );
		this->locationLabel->setEllipsis( true , 1 );
		this->locationValueLabel->setAlign( _align::left );
		mainFrame->addChild( this->locationLabel );
		mainFrame->addChild( this->locationValueLabel );
		
		this->sizeLabel = new _label( 4 , 58 , 48 , ignore , "Size:" );
		this->sizeValueLabel = new _label( 56 , 58 , 59 , ignore , this->file->getSizeReadable() );
		this->sizeLabel->setAlign( _align::left );
		this->sizeLabel->setEllipsis( true , 1 );
		this->sizeValueLabel->setAlign( _align::left );
		mainFrame->addChild( this->sizeLabel );
		mainFrame->addChild( this->sizeValueLabel );
		
		this->dateCreatedLabel = new _label( 4 , 72 , 48 , ignore , "Creation:" );
		this->dateCreatedValueLabel = new _label( 56 , 72 , 64 , ignore , this->file->getCreationTime().toString(dateTimeFormat) );
		this->dateCreatedLabel->setAlign( _align::left );
		this->dateCreatedLabel->setEllipsis( true , 1 );
		this->dateCreatedValueLabel->setAlign( _align::left );
		mainFrame->addChild( this->dateCreatedLabel );
		mainFrame->addChild( this->dateCreatedValueLabel );
		
		this->dateLastChangeLabel = new _label( 4 , 82 , 48 , ignore , "Last Change:" );
		this->dateLastChangeValueLabel = new _label( 56 , 82 , 64 , ignore , this->file->getLastWriteTime().toString(dateTimeFormat)  );
		this->dateLastChangeLabel->setAlign( _align::left );
		this->dateLastChangeLabel->setEllipsis( true , 1 );
		this->dateLastChangeValueLabel->setAlign( _align::left );
		mainFrame->addChild( this->dateLastChangeLabel );
		mainFrame->addChild( this->dateLastChangeValueLabel );
		
		this->dateLastAccessLabel = new _label( 4 , 92 , 48 , ignore , "Last Access:" );
		this->dateLastAccessValueLabel = new _label( 56 , 92 , 64 , ignore , this->file->getLastAccessTime().toString(dateTimeFormat) );
		this->dateLastAccessLabel->setAlign( _align::left );
		this->dateLastAccessLabel->setEllipsis( true , 1 );
		this->dateLastAccessValueLabel->setAlign( _align::left );
		mainFrame->addChild( this->dateLastAccessLabel );
		mainFrame->addChild( this->dateLastAccessValueLabel );
	
	// Attributes
		
		_direntryAttributes attrs = this->file->getAttrs();
		
		this->attributesLabel = new _label( 4 , 106 , 48 , ignore , "Attributes:" );
		this->attributesLabel->setAlign( _align::left );
		this->attributesLabel->setEllipsis( true , 1 );
		mainFrame->addChild( this->attributesLabel );
		
		this->readonlyAttrLabel = new _label( 68 , 106 , 47 , ignore , "Readonly" );
		this->readonlyAttrLabel->setAlign( _align::left );
		this->readonlyCheckbox = new _checkBox( 56 , 106 , attrs.readonly );
		mainFrame->addChild( this->readonlyAttrLabel );
		mainFrame->addChild( this->readonlyCheckbox );
		
		this->hiddenAttrLabel = new _label( 68 , 116 , 47 , ignore , "Hidden" );
		this->hiddenAttrLabel->setAlign( _align::left );
		this->hiddenCheckbox = new _checkBox( 56 , 116 , attrs.hidden );
		mainFrame->addChild( this->hiddenAttrLabel );
		mainFrame->addChild( this->hiddenCheckbox );
		
		this->systemAttrLabel = new _label( 68 , 126 , 47 , ignore , "System" );
		this->systemAttrLabel->setAlign( _align::left );
		this->systemCheckbox = new _checkBox( 56 , 126 , attrs.system );
		mainFrame->addChild( this->systemAttrLabel );
		mainFrame->addChild( this->systemCheckbox );
	
	// Divider
		this->divider1 = new _dividerGadget( _dimension::horizontal , 44 );
		this->divider2 = new _dividerGadget( _dimension::horizontal , 68 );
		this->divider3 = new _dividerGadget( _dimension::horizontal , 102 );
		mainFrame->addChild( this->divider1 );
		mainFrame->addChild( this->divider2 );
		mainFrame->addChild( this->divider3 );
}

_callbackReturn PROG_FileDetail::handler( _event event )
{
	_gadget* that = event.getGadget();
	
	if( that == this->okButton ){
		if( this->fileNameTextbox->getStrValue().length() )
		{
			_direntryAttributes attrs = this->file->getAttrs();
			attrs.readonly = this->readonlyCheckbox->getIntValue();
			attrs.hidden = this->hiddenCheckbox->getIntValue();
			attrs.system = this->systemCheckbox->getIntValue();
			this->file->setAttrs( attrs );
			this->file->rename( this->fileNameTextbox->getStrValue().cpp_str() );
		}
		else{
			this->fileNameTextbox->blink();
			return handled;
		}
	}
	else if( that == this->openedWithChangeButton ){
		_winds::execute(
			string("%SYSTEM%/progmapper.exe -\"*.") + this->file->getExtension().c_str() + "\" -save_choice" 
		);
		return handled;
	}
	this->terminate();
	
	return handled;
}