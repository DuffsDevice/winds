#include "_program/PROG_Explorer.h"

PROG_Explorer::PROG_Explorer( string path ) :
	_progC( static_cast<void (_progC::*)(_cmdArgs)>( &PROG_Explorer::init) , static_cast<int (_progC::*)(_cmdArgs)>( &PROG_Explorer::main) )
	, path( path )
{ }

void PROG_Explorer::init( _cmdArgs args )
{
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


int PROG_Explorer::main( _cmdArgs args )
{
	return 0;
}