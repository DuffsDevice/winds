#include <_resource/resource.program.explorer.h>
#include <_resource/resource.icon.folder.up.h>
#include <_controller/controller.localization.h>
#include <_type/type.winds.soundbank.h>

// Special Pages
#include <_resource/resource.explorer.computer.h>
#include <_resource/resource.explorer.folder.h>

_view* PROG_Explorer::getViewByName( string assocName ) const
{
	// Try to find a special page
	_view* page = _viewSwitcher::getViewByName( assocName );
	
	if( page )
		return page;
	
	// Set new Path
	this->folderPage->setPath( assocName );
	
	return this->folderPage;
}

PROG_Explorer::PROG_Explorer()
{
	_programHeader header;
	header.name = string("Explorer");
	header.fileName = string("Explorer");
	header.author = string("WinDS");
	header.description = string("Program to browse the File System");
	this->setHeader( move(header) );
}

string PROG_Explorer::beforeChange( string newViewName ) const
{
	// We dont accept a filesystem root, but only the 'Computer-Page as the Root'
	if( newViewName.empty() || newViewName == "/" )
		return _localizationController::getBuiltInString("lbl_computer").cpp_str();
	
	// Play Sound
	if( !_viewSwitcher::getActiveView().empty() )
		_windsSoundBank::play( _windsSound::navigation );
	
	// Adjust the window's title
	_direntry	newPath = _direntry( newViewName );
	string		title = newPath.getName();
	
	// Set Window Title and AddressBar
	_program::getMainFrame()->setTitle( title.empty() ? "/" : title );
	
	this->addressBar->setStrValue( newPath.getFileName() );
	
	return string();
}

void PROG_Explorer::main( _args args )
{
	string path;
	
	if( !args.empty() && !args[0].empty() )
		path = _direntry(args[0]).getFileName();
	
	// Create Frame for program
	_mainFrame* mainFrame = _program::getMainFrame( 120 , 90 );
	
	// Add Special Pages
	this->setViewPadding( _padding( 0 , 11 , 0 , 0 ) );
	this->folderPage = new _explorerPageFolder();
	this->addView( _localizationController::getBuiltInString("lbl_computer").cpp_str() , _explorerPageComputer() );
	
	// Set viewParent of viewSwitcher
	_viewSwitcher::setViewParent( mainFrame );
	
	
	
	// GUI Stuff
	
	this->addressBar = new _textBox( 1 , 10 , ignore , 10 , path );
	this->submitButton = new _actionButton( ignore , 10 , _actionButtonType::next , _style::canNotTakeFocus );
	this->folderUpButton = new _imageButton( ignore , 10 , 10 , 9 , BMP_FolderUpIcon() , ignore , "" , _style::canNotTakeFocus );
	
	this->addressBar->setInternalEventHandler( onParentResize , _gadgetHelpers::sizeParent( 23 , ignore ) );
	this->addressBar->setInternalEventHandler( onParentAdd , _gadgetHelpers::eventForward(onParentResize) );
	
	this->submitButton->setUserEventHandler( onMouseClick , make_callback( this , &PROG_Explorer::handler ) );
	this->submitButton->setUserEventHandler( onParentResize , _gadgetHelpers::rightBottomAlign( 1 , ignore ) );
	this->submitButton->setUserEventHandler( onParentAdd , _gadgetHelpers::eventForward(onParentResize) );
	
	this->folderUpButton->setUserEventHandler( onMouseClick , make_callback( this , &PROG_Explorer::handler ) );
	this->folderUpButton->setUserEventHandler( onParentResize , _gadgetHelpers::rightBottomAlign( 11 , ignore ) );
	this->folderUpButton->setUserEventHandler( onParentAdd , _gadgetHelpers::eventForward(onParentResize) );
	
	// Create Menu
	_menu menu = _windowMenu::getStandardMenu();
	menu.setList( 1 , { { 101 , _localizationController::getBuiltInString("lbl_exit") } } );
	
	// Set the new active page
	_viewSwitcher::set( path );
	
	mainFrame->addChild( this->windowBar = new _windowBar() );
	mainFrame->addChild( this->windowMenu = new _windowMenu(move(menu)) );
	mainFrame->addChild( this->addressBar );
	mainFrame->addChild( this->submitButton );
	mainFrame->addChild( this->folderUpButton );
}

_callbackReturn PROG_Explorer::handler( _event event )
{
	_gadget* that = event.getGadget();
	
	if( that == this->submitButton )
		_viewSwitcher::set( this->addressBar->getStrValue().cpp_str() );
	else if( that == this->folderUpButton )
		_viewSwitcher::set( _direntry( _viewSwitcher::getActiveView() ).getParentDirectory() );
	
	return handled;
}