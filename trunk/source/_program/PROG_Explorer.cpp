#include "_program/PROG_Explorer.h"

PROG_Explorer::PROG_Explorer() :
	_progC( static_cast<void (_progC::*)(_cmdArgs&)>( &PROG_Explorer::init) , static_cast<void (_progC::*)()>( &PROG_Explorer::destruct) , static_cast<int (_progC::*)(_cmdArgs&)>( &PROG_Explorer::main) )
	, path( "/" )
{ }

void PROG_Explorer::init( _cmdArgs& args )
{
	if( args["path"].length() )
		this->path = args["path"];
	
	this->window = new _window( 120 , 70 , 40 , 40 , "Explorer" );
	this->fileview = new _fileview( 118 , 47 , 1 , 22 , this->path );
	this->addressbar = new _textbox( 2 , 11 , 98 , this->path );
	this->submitbutton = new _button( 17 , 10 , 101 , 11 , "->" );
	
	this->submitbutton->registerEventHandler( individual , PROG_Explorer::handler );
	
	this->window->setProgramHandle( this );
	this->window->addChild( this->fileview );
	this->window->addChild( this->addressbar );
	this->window->addChild( this->submitbutton );
	this->windowsInstance->addChild( this->window );
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

_gadgetEventReturnType PROG_Explorer::handler( _gadgetEvent event )
{
	
	_gadget* that = event.getGadget();
	
	if( that->getType() == _gadgetType::button )
	{
		PROG_Explorer* prog = (PROG_Explorer*)((_window*)that->getParent())->getProgramHandle();
		string val = prog->addressbar->getStrValue();
		prog->path = val;
		prog->fileview->setPath( val );
	}
	
	return handled;
}


int PROG_Explorer::main( _cmdArgs& args )
{
	return 0;
}