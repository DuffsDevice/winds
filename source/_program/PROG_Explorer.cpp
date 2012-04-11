#include "_program/PROG_Explorer.h"

PROG_Explorer::PROG_Explorer( string path ) :
	_progC( static_cast<_progfunc>( &PROG_Explorer::init) , static_cast<_progfunc>( &PROG_Explorer::main) )
	, path( path )
{ 
	this->dir = new _directory( path );
}

int PROG_Explorer::init( _cmdArgs args )
{
	this->window = new _window( 120 , 70 , 40 , 40 , "Explorer" );
	this->fileview = new _fileview( 118 , 47 , 1 , 22 , this->dir );
	this->addressbar = new _textbox( 2 , 11 , 98 , this->path );
	this->submitbutton = new _button( 17 , 10 , 101 , 11 , "->" );
	
	this->window->addChild( this->fileview );
	this->window->addChild( this->addressbar );
	this->window->addChild( this->submitbutton );
	
	return 0;
}

int PROG_Explorer::main( _cmdArgs args )
{
	this->windowsInstance->addChild( this->window );
	return 0;
}