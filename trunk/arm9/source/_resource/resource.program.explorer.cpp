#include "_resource/resource.program.explorer.h"
#include "_resource/resource.icon.folder.up.h"
#include "_gadget/gadget.select.h"
#include "_gadget/gadget.button.action.h"
#include "_gadget/gadget.window.bar.h"
#include "_gadget/gadget.window.menu.h"
#include "_gadget/gadget.windows.taskinfo.h"

PROG_Explorer::PROG_Explorer() :
	path( "/" )
{ }

void PROG_Explorer::main( _programArgs args )
{
	if( !args.empty() && !args[0].empty() )
		this->path = _direntry(args[0]).getFileName();
	
	this->window = new _window( 40 , 40 , 120 , 90 , "Explorer" , true , true , _style::draggable );
	this->fileView = new _fileView( 0 , 21 , ignore , ignore , this->path , _fileViewType::list );
	this->addressBar = new _textBox( 1 , 10 , ignore , 10 , this->path );
	this->submitButton = new _actionButton( ignore , 10 , _actionButtonType::next , _style::canNotTakeFocus );
	this->folderUpButton = new _imageButton( ignore , 10 , 10 , 9 , BMP_FolderUpIcon() , ignore , "" , _style::canNotTakeFocus );
	
	this->fileView->setUserEventHandler( onEdit , make_callback( this , &PROG_Explorer::handler ) );
	this->fileView->setUserEventHandler( onParentResize , _gadgetHelpers::sizeParent( 1 , 30 ) );
	this->fileView->setUserEventHandler( onParentAdd , _gadgetHelpers::eventForward(onParentResize) );
	
	this->addressBar->setInternalEventHandler( onParentResize , _gadgetHelpers::sizeParent( 23 , ignore ) );
	this->addressBar->setInternalEventHandler( onParentAdd , _gadgetHelpers::eventForward(onParentResize) );
	
	this->submitButton->setUserEventHandler( onMouseClick , make_callback( this , &PROG_Explorer::handler ) );
	this->submitButton->setUserEventHandler( onParentResize , _gadgetHelpers::rightBottomAlign( 1 , ignore ) );
	this->submitButton->setUserEventHandler( onParentAdd , _gadgetHelpers::eventForward(onParentResize) );
	
	this->folderUpButton->setUserEventHandler( onMouseClick , make_callback( this , &PROG_Explorer::handler ) );
	this->folderUpButton->setUserEventHandler( onParentResize , _gadgetHelpers::rightBottomAlign( 11 , ignore ) );
	this->folderUpButton->setUserEventHandler( onParentAdd , _gadgetHelpers::eventForward(onParentResize) );
	
	this->window->setUserEventHandler( onClose , make_callback( this , &PROG_Explorer::handler ) );
	
	// Adjust the window's title
	this->setWindowTitle();
	
	// Create Menu
	_menu menu = _windowMenu::getStandardMenu();
	menu.setList( 1 , { { 101 , _system::getLocalizedString("lbl_exit") } } );
	
	this->window->addChild( this->fileView );
	this->window->addChild( this->windowBar = new _windowBar() );
	this->window->addChild( this->windowMenu = new _windowMenu(menu) );
	this->window->addChild( this->addressBar );
	this->window->addChild( this->submitButton );
	this->window->addChild( this->folderUpButton );
	this->gadgetHost->addChild( this->window );
}

void PROG_Explorer::destruct()
{
	if( this->window )
		delete this->window;
	if( this->fileView )
		delete this->fileView;
	if( this->addressBar )
		delete this->addressBar;
	if( this->submitButton )
		delete this->submitButton;
	if( this->folderUpButton )
		delete this->folderUpButton;
	if( this->windowMenu )
		delete this->windowMenu;
	if( this->windowBar )
		delete this->windowBar;
}

void PROG_Explorer::setWindowTitle()
{
	string path = _direntry( this->fileView->getPath() ).getName();
	this->window->setStrValue( ( path.empty() ? "/" : path ) + " - Explorer");
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
	}
	else if( that->getType() == _gadgetType::window )
		this->terminate();
	
	return handled;
}