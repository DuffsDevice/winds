#include "_resource/PROG_Explorer.h"
#include "_gadget/gadget.select.h"
#include "_gadget/gadget.actionButton.h"

//bool created = false;
//_label* b2 = nullptr;

PROG_Explorer::PROG_Explorer() :
	_progC( static_cast<void (_progC::*)(_cmdArgs&)>( &PROG_Explorer::main) , static_cast<void (_progC::*)()>( &PROG_Explorer::destruct) )
	, path( "/" )
{ }

void PROG_Explorer::main( _cmdArgs& args )
{
	if( !args["path"].empty() )
		this->path = args["path"];
	
	this->window = new _window( 120 , 70 , 40 , 40 , "Explorer" , _style::storeHost( this , _styleAttr() | _styleAttr::minimizeable ) );
	this->fileview = new _fileview( 118 , 47 , 0 , 12 , this->path , _style::storeHost( this ) );
	this->addressbar = new _textbox( 1 , 1 , 106 , this->path , _style::storeHost( this ) );
	this->submitbutton = new _actionButton( _actionButtonType::next, 108 , 2 , _style::storeHost( this ) );
	
	this->fileview->registerEventHandler( onChange , new _staticCallback( PROG_Explorer::handler ) );
	this->submitbutton->registerEventHandler( onAction , new _staticCallback( PROG_Explorer::handler ) );
	
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
	PROG_Explorer* prog = (PROG_Explorer*)that->getStyle().host;
	
	if( that->getType() == _gadgetType::button )
	{
		string val = prog->addressbar->getStrValue();
		prog->path = val;
		prog->fileview->setPath( val );
	}
	else if( that->getType() == _gadgetType::fileview )
	{
		prog->addressbar->setStrValue( ((_fileview*)that)->getPath() );
	}
	
	return handled;
}