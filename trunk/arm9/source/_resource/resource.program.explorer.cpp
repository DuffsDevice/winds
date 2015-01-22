#include <_resource/resource.program.explorer.h>
#include <_resource/resource.icon.folder.up.h>
#include <_controller/controller.localization.h>

// Special Pages
//#include <_resource/resource.explorer.computer.h>

PROG_Explorer::PROG_Explorer() :
	path( "/" )
{
	_programHeader header;
	header.name = string("Explorer");
	header.fileName = string("Explorer");
	header.author = string("WinDS");
	header.description = string("Program to browse the File System");
	this->setHeader( move(header) );
}

void PROG_Explorer::main( _args args )
{
	if( !args.empty() && !args[0].empty() )
		this->path = _direntry(args[0]).getFileName();
	
	//this->viewSwitcher.addView( _localizationController::getBuiltInString("lbl_computer") , new _explorerPageComputer() );
	//this->viewSwitcher.addView( _localizationController::getBuiltInString("lbl_systemsettings") , _explorerPageSystemSettings() );
	
	_mainFrame* mainFrame = _program::getMainFrame( 120 , 90 );
	
	this->scrollArea = new _scrollArea( 0 , 21 , ignore , ignore , _style::storeHandle( this ) );
	this->fileView = new _fileView( 0 , 21 , ignore , ignore , this->path , _fileViewType::list );
	this->addressBar = new _textBox( 1 , 10 , ignore , 10 , this->path );
	this->submitButton = new _actionButton( ignore , 10 , _actionButtonType::next , _style::canNotTakeFocus );
	this->folderUpButton = new _imageButton( ignore , 10 , 10 , 9 , BMP_FolderUpIcon() , ignore , "" , _style::canNotTakeFocus );
	
	this->fileView->setUserEventHandler( onEdit , make_callback( this , &PROG_Explorer::handler ) );
	this->fileView->setUserEventHandler( onParentResize , _gadgetHelpers::sizeParent( 1 , 30 ) );
	this->fileView->setUserEventHandler( onParentAdd , _gadgetHelpers::eventForward(onParentResize) );
	
	this->scrollArea->setUserEventHandler( onParentResize , _gadgetHelpers::sizeParent( 1 , 30 ) );
	this->scrollArea->setUserEventHandler( onParentAdd , _gadgetHelpers::eventForward(onParentResize) );
	
	// Set viewParent of viewSwitcher
	this->viewSwitcher.setViewParent( this->scrollArea );
	
	this->addressBar->setInternalEventHandler( onParentResize , _gadgetHelpers::sizeParent( 23 , ignore ) );
	this->addressBar->setInternalEventHandler( onParentAdd , _gadgetHelpers::eventForward(onParentResize) );
	
	this->submitButton->setUserEventHandler( onMouseClick , make_callback( this , &PROG_Explorer::handler ) );
	this->submitButton->setUserEventHandler( onParentResize , _gadgetHelpers::rightBottomAlign( 1 , ignore ) );
	this->submitButton->setUserEventHandler( onParentAdd , _gadgetHelpers::eventForward(onParentResize) );
	
	this->folderUpButton->setUserEventHandler( onMouseClick , make_callback( this , &PROG_Explorer::handler ) );
	this->folderUpButton->setUserEventHandler( onParentResize , _gadgetHelpers::rightBottomAlign( 11 , ignore ) );
	this->folderUpButton->setUserEventHandler( onParentAdd , _gadgetHelpers::eventForward(onParentResize) );
	
	// Adjust the window's title
	this->setWindowTitle();
	
	// Create Menu
	_menu menu = _windowMenu::getStandardMenu();
	menu.setList( 1 , { { 101 , _localizationController::getBuiltInString("lbl_exit") } } );
	
	mainFrame->addChild( this->fileView );
	mainFrame->addChild( this->scrollArea );
	
	// Updates the fileview and eventually display a special page
	this->updateFileView();
	
	mainFrame->addChild( this->windowBar = new _windowBar() );
	mainFrame->addChild( this->windowMenu = new _windowMenu(move(menu)) );
	mainFrame->addChild( this->addressBar );
	mainFrame->addChild( this->submitButton );
	mainFrame->addChild( this->folderUpButton );
}

void PROG_Explorer::cleanUp()
{
	delete this->fileView;
	delete this->addressBar;
	delete this->submitButton;
	delete this->folderUpButton;
	delete this->windowMenu;
	delete this->windowBar;
}

void PROG_Explorer::setWindowTitle()
{
	string path = _direntry( this->fileView->getPath() ).getName();
	_program::getMainFrame()->setTitle( path.empty() ? "/" : path );
}

void PROG_Explorer::updateFileView()
{
	string path = this->fileView->getPath();
	
	if( path.front() == '/' )
		path = path.substr( 1 );
	
	// Display Special Page?
	if( this->viewSwitcher.getViewByName( path ) ){
		this->fileView->hide();
		//this->scrollArea->show();
		this->viewSwitcher.set( path );
	}
	else{
		this->viewSwitcher.unset();
		this->scrollArea->hide();
		this->fileView->show();
	}
}

_callbackReturn PROG_Explorer::handler( _event event )
{
	_gadget* that = event.getGadget();
	
	if( that == this->submitButton ){
		string val = this->addressBar->getStrValue();
		this->path = val;
		this->fileView->setPath( val );
		this->setWindowTitle();
	}
	if( that == this->folderUpButton ){
		this->path = _direntry( this->fileView->getPath() ).getParentDirectory();
		this->addressBar->setStrValue( this->path );
		this->fileView->setPath( this->path );
		this->setWindowTitle();
	}
	else if( that->getType() == _gadgetType::fileview ){
		this->addressBar->setStrValue( this->fileView->getPath() );
		this->setWindowTitle();
		this->updateFileView();
	}
	
	return handled;
}