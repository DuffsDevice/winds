#include "_resource/resource.program.mapper.h"
#include "_resource/resource.program.mapper.progobject.h"
#include "_type/type.gadget.helpers.h"

PROG_Mapper::PROG_Mapper() :
	chooseButNotOpen( false )
	, openDialog( nullptr )
{
	_programHeader header;
	//header.fileIcon = BMP_WinDSIcon();
	header.name = string("Program Mapper");
	header.author = string("WinDS");
	header.description = string("Choose by which Prog. to open a file");
	this->setHeader( header );
}

void PROG_Mapper::main( _programArgs args )
{
	for( string& arg : args ){
		if( arg == "save_choice" )
			this->chooseButNotOpen = true;
		else
			this->fileToOpen = move(arg);
	}
	
	// Window
	this->window		= new _window( 10 , 10 , 155 , 140 , _system::getLocalizedString("lbl_open_with") , false , true , _style::notResizeable );
	
	// Cancel-button
	this->cancelButton	= new _button( ignore , ignore , ignore , ignore , _system::getLocalizedString("lbl_cancel") );
	this->cancelButton->setUserEventHandler( onParentAdd , _gadgetHelpers::rightBottomAlign( 5 , 3 ) );
	this->cancelButton->setUserEventHandler( onMouseClick , make_callback( this , &PROG_Mapper::handler ) );
	
	// OK-button
	this->okButton		= new _button( ignore , ignore , ignore , ignore , _system::getLocalizedString("lbl_ok") );
	this->okButton->setUserEventHandler( onParentAdd , _gadgetHelpers::moveBesidePrecedent( _direction::left , 1 ) );
	this->okButton->setAutoSelect( true );
	this->okButton->setUserEventHandler( onMouseClick , make_callback( this , &PROG_Mapper::handler ) );
	
	// Browse-button
	this->browseButton	= new _button( 4 , ignore , ignore , ignore , _system::getLocalizedString("lbl_browse") + "..." );
	this->browseButton->setUserEventHandler( onParentAdd , _gadgetHelpers::rightBottomAlign( ignore , 3 ) );
	this->browseButton->setUserEventHandler( onMouseClick , make_callback( this , &PROG_Mapper::handler ) );
	
	// Checkbox
	this->saveDescision	= new _checkbox( 4 , ignore );
	if( this->chooseButNotOpen ){
		this->saveDescision->setIntValue( true );
		this->saveDescision->removeInternalEventHandler( onMouseClick );
	}
	this->saveDescision->setUserEventHandler( onParentAdd , _gadgetHelpers::moveBesidePrecedent( _direction::down , 1 , 2 ) );
	
	// .. and its description
	this->saveDescisionLabel	= new _label( ignore , ignore , ignore , ignore , _system::getLocalizedString("lbl_save_choice") );
	this->saveDescisionLabel->setUserEventHandler( onParentAdd , _gadgetHelpers::moveBesidePrecedent( _direction::right , 2 , 1 ) );
	
	// Label
	this->description	= new _label( 5 , 2 , ignore , ignore , _system::getLocalizedString("def_choose_program_to_open") );
	
	// File Label
	this->fileLabel = new _label( 5 , 11 , ignore , ignore , _system::getLocalizedString("def_filename") + "  " + _direntry(this->fileToOpen).getDisplayName(true) );
	this->fileLabel->setColor( _color::fromBW( 12 ) );
	
	// Create List of Programs
	this->scrollArea = new _scrollArea( 4 , 22 , 145 , 75 );
	this->scrollArea->setPaddingOffset( _padding(1) );
	this->scrollArea->setInternalEventHandler( onDraw , make_callback( this , &PROG_Mapper::handler ) );
	
	_assocVector<string,int> handlerList;
	
	// Create List of Programs
	for( const _pair<string,string>& value : _system::getRegistry().readSection("filemapper") )
	{
		const string& val = value.second;
		
		if( val.empty() || value.first == "*" ) // Skip if filehandler is empty
			continue;
		
		//
		// Parse Command and filter out the executeable
		//
		size_t fileNameEnd = string::npos;
		int escaped = val[0] == '"' ? 1 : 0;
		
		if( escaped ) // If it starts with an apostroph
			fileNameEnd = val.find( '"' , 1 );
		else
			fileNameEnd = val.find( ' ' );
		
		// Add to List
		handlerList[ _direntry( val.substr( escaped , fileNameEnd - escaped ) ).getFileName() ]++;
	}
	
	// Add Standard Programs
	handlerList[_direntry::replaceASSOCS("%WINDIR%/accessories/paint.exe")]++;
	
	// Create ProgObjects, once per program
	for( _pair<string,int>& value : handlerList )
	{
		auto* ptr = new PROG_Mapper_Object( 70 , value.first );
		ptr->setUserEventHandler( onParentAdd , _gadgetHelpers::moveBesidePrecedent( _direction::right , 1 , 1 , true , 1 , 1 ) );
		this->scrollArea->addChild( ptr , true );
	}
	
	this->window->addChild( this->cancelButton );
	this->window->addChild( this->browseButton );
	this->window->addChild( this->okButton , true );
	this->window->addChild( this->scrollArea , true );
	this->window->addChild( this->saveDescision , true );
	this->window->addChild( this->saveDescisionLabel , true );
	this->window->addChild( this->description );
	this->window->addChild( this->fileLabel );
	this->gadgetHost->addChild( this->window );
}

void PROG_Mapper::destruct()
{
	if( this->window )				delete this->window;
	if( this->description )			delete this->description;
	if( this->cancelButton )		delete this->cancelButton;
	if( this->browseButton )		delete this->browseButton;
	if( this->okButton )			delete this->okButton;
	if( this->saveDescision )		delete this->saveDescision;
	if( this->saveDescisionLabel )	delete this->saveDescisionLabel;
	if( this->fileLabel )			delete this->fileLabel;
	
	if( this->scrollArea ){
		this->scrollArea->removeChildren( true );
		delete this->scrollArea;
	}
}

_callbackReturn PROG_Mapper::handler( _event event )
{
	_gadget* that = event.getGadget();
	
	if( event == onDraw )
	{
		// Get BitmapPort
		_bitmapPort bP = that->getBitmapPort( event );
		
		bP.fill( _color::white );
		bP.drawRect( 0 , 0 , bP.getWidth() , bP.getHeight() , _system::getRTA().getControlForeground() );
		
		return use_default;
	}
	else if( event == onClose )
		this->terminate();
	else if( event == onMouseClick )
	{
		if( that == this->cancelButton )
			this->terminate();
		else if( that == this->browseButton )
		{
			// Create Dialog
			if( !this->openDialog ){
				this->openDialog = new _fileOpenDialog(	{ { 1 , { "Program" , "exe,lua" } } } , "/" , 1 , _system::getLocalizedString("lbl_ok") , _system::getLocalizedString("lbl_open_with") );
				this->openDialog->setCallback( make_callback( this , &PROG_Mapper::dialogHandler ) );
			}
			
			// Execute Dialog in case it was not already
			if( !this->openDialog->isRunning() )
				this->openDialog->execute();
		}
		else if( that == this->okButton )
		{	
			// Receive selected Program
			PROG_Mapper_Object* selection = (PROG_Mapper_Object*) this->scrollArea->getSelectedChild();
			
			// Check if a program was selected
			if( selection )
			{
				// Write to Registry
				if( !this->fileToOpen.empty() )
				{
					if( this->saveDescision->getIntValue() )
					{
						// Save to registry
						string command = "\"";
						command += selection->getPath();
						command += "\" -\"$F\"";
						
						_system::getRegistry().writeIndex(
							"filemapper"
							, _direntry(this->fileToOpen).getExtension()
							, move(command)
						);
					}
					
					if( !this->chooseButNotOpen )
					{
						string cmd = "\"" + selection->getPath() + "\" -\"" + this->fileToOpen + "\"";
						
						// Execute File
						_system::executeCommand( move(cmd) );
					}
				}
				
				// Terminate the Program, since we did what we had to do
				this->terminate();
			}
		}
		
		// Forward onMouseClick to buttons so that they behave as expected
		return use_internal;
	}
	
	return handled;
}

// Handles results from the 'Choose Program' dialog
void PROG_Mapper::dialogHandler( _dialogResult result )
{
	if( result == _dialogResult::yes )
	{
		string pathToAdd = _direntry( this->openDialog->getFileName() ).getFileName();
		
		for( _gadget* progObject : this->scrollArea->getChildren( false ) )
		{
			PROG_Mapper_Object* tmp = (PROG_Mapper_Object*)progObject;
			if( tmp->getPath() == pathToAdd )
				return;
		}
		
		// Add Prog_Object
		auto* ptr = new PROG_Mapper_Object( 70 , pathToAdd );
		ptr->setUserEventHandler( onParentAdd , _gadgetHelpers::moveBesidePrecedent( _direction::right , 1 , 1 , true , 1 , 1 ) );
		this->scrollArea->addChild( ptr , true );
	}
}