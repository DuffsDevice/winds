#include "_resource/PROG_Explorer.h"
#include "_gadget/gadget.select.h"
#include "_gadget/gadget.actionButton.h"

PROG_Explorer::PROG_Explorer() :
	path( "/" )
{ }

void PROG_Explorer::main( _cmdArgs&& args )
{
	if( !args.empty() && !args[0].empty() )
		this->path = args[0];
	
	this->window = new _window( 120 , 90 , 40 , 40 , "Explorer" , _styleAttr() | _styleAttr::minimizeable | _styleAttr::draggable );
	this->fileview = new _fileview( 118 , 67 , 0 , 12 , this->path );
	this->addressbar = new _textbox( 1 , 1 , 106 , this->path );
	this->submitbutton = new _actionButton( _actionButtonType::next, 108 , 2 );
	
	this->window->setUserEventHandler( onResize , _classCallback( this , &PROG_Explorer::handler ) );
	this->fileview->setUserEventHandler( onChange , _classCallback( this , &PROG_Explorer::handler ) );
	this->submitbutton->setUserEventHandler( onAction , _classCallback( this , &PROG_Explorer::handler ) );
	this->window->setUserEventHandler( onClose , _classCallback( this , &PROG_Explorer::handler ) );
	
	this->window->addChild( this->fileview );
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

_callbackReturn PROG_Explorer::handler( _event event )
{
	_gadget* that = event.getGadget();
	
	if( event.getType() == onClose )
		this->terminate();
	
	if( that->getType() == _gadgetType::button )
	{
		string val = this->addressbar->getStrValue();
		this->path = val;
		this->fileview->setPath( val );
		
		string path = _direntry( val ).getName();
		this->window->setStrValue( ( path.empty() ? "/" : path ) + " - Explorer");
	}
	else if( that->getType() == _gadgetType::fileview )
	{
		this->addressbar->setStrValue( this->fileview->getPath() );
		
		string path = _direntry( this->fileview->getPath() ).getName();
		this->window->setStrValue( ( path.empty() ? "/" : path ) + " - Explorer");
	}
	else if( that->getType() == _gadgetType::window )
	{
		this->fileview->setWidth( that->getWidth() - 2 );
		this->fileview->setHeight( that->getHeight() - 23 );
		this->addressbar->setWidth( that->getWidth() - 14 );
		this->submitbutton->setX( that->getWidth() - 12 );
		
		return ((_window*)that)->resizeHandler( event );
	}
	
	return handled;
}