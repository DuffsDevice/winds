#include "_resource/PROG_Explorer.h"
#include "_gadget/gadget.select.h"
#include "_gadget/gadget.actionButton.h"
#include "_gadget/gadget.window.bar.h"
#include "_gadget/gadget.window.menu.h"

PROG_Explorer::PROG_Explorer() :
	path( "/" )
{ }

void PROG_Explorer::main( _cmdArgs&& args )
{
	if( !args.empty() && !args[0].empty() )
		this->path = args[0];
	
	this->window = new _window( 40 , 40 , 120 , 90 , "Explorer" , true , true , _styleAttr() | _styleAttr::draggable );
	this->fileview = new _fileview( 0 , 21 , ignore , ignore , this->path );
	this->addressbar = new _textbox( 1 , 10 , ignore , 10 , this->path );
	this->submitbutton = new _actionButton( ignore , 10 , _actionButtonType::next , _style() | _styleAttr::canNotTakeFocus );
	
	this->fileview->setUserEventHandler( onEdit , make_callback( this , &PROG_Explorer::handler ) );
	this->fileview->setUserEventHandler( onParentResize , _gadgetHelpers::sizeParent( 1 , 30 ) );
	this->fileview->setUserEventHandler( onParentSet , _gadgetHelpers::sizeParent( 1 , 30 ) );
	
	this->addressbar->setInternalEventHandler( onParentResize , _gadgetHelpers::sizeParent( 12 , ignore ) );
	this->addressbar->setInternalEventHandler( onParentSet , _gadgetHelpers::sizeParent( 12 , ignore ) );
	
	this->submitbutton->setUserEventHandler( onMouseClick , make_callback( this , &PROG_Explorer::handler ) );
	this->submitbutton->setUserEventHandler( onParentResize , _gadgetHelpers::rightBottomAlign( 1 , ignore ) );
	this->submitbutton->setUserEventHandler( onParentSet , _gadgetHelpers::rightBottomAlign( 1 , ignore ) );
	
	this->window->setUserEventHandler( onClose , make_callback( this , &PROG_Explorer::handler ) );
	
	// Adjust the window's title
	this->setWindowTitle();
	
	this->window->addChild( this->fileview );
	this->window->addChild( new _windowBar() );
	this->window->addChild( new _windowMenu( _windowMenu::getStandardMenu() ) );
	this->window->addChild( this->addressbar );
	this->window->addChild( this->submitbutton );
	this->gadgetHost->addChild( this->window );
}

void PROG_Explorer::destruct()
{
	if( this->window )
		delete this->window;
	if( this->fileview )
		delete this->fileview;
	if( this->addressbar )
		delete this->addressbar;
	if( this->submitbutton )
		delete this->submitbutton;
}

void PROG_Explorer::setWindowTitle()
{
	string path = _direntry( this->fileview->getPath() ).getName();
	this->window->setStrValue( ( path.empty() ? "/" : path ) + " - Explorer");
}

_callbackReturn PROG_Explorer::handler( _event event )
{
	_gadget* that = event.getGadget();
	
	if( event == onClose )
		this->terminate();
	
	if( that->getType() == _gadgetType::button )
	{
		string val = this->addressbar->getStrValue();
		this->path = val;
		this->fileview->setPath( val );
		this->setWindowTitle();
	}
	else if( that->getType() == _gadgetType::fileview )
	{
		this->addressbar->setStrValue( this->fileview->getPath() );
		this->setWindowTitle();
	}
	
	return handled;
}