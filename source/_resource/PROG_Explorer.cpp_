#include "_resource/PROG_Explorer.h"
#include "_gadget/gadget.select.h"

//bool created = false;
//_label* b2 = nullptr;

PROG_Explorer::PROG_Explorer() :
	_progC( static_cast<void (_progC::*)(_cmdArgs&)>( &PROG_Explorer::init) , static_cast<void (_progC::*)()>( &PROG_Explorer::destruct) , static_cast<int (_progC::*)(_cmdArgs&)>( &PROG_Explorer::main) )
	, path( "/" )
{ }

void PROG_Explorer::init( _cmdArgs& args )
{
	if( args["path"].length() )
		this->path = args["path"];
	
	this->window = new _window( 120 , 70 , 40 , 40 , "Explorer" , _gadgetStyle::owner( this ) );
	this->fileview = new _fileview( 118 , 47 , 0 , 12 , this->path , _gadgetStyle::owner( this ) );
	this->addressbar = new _textbox( 1 , 1 , 98 , this->path , _gadgetStyle::owner( this ) );
	this->submitbutton = new _button( 17 , 10 , 100 , 1 , "->" , _gadgetStyle::owner( this ) );
	
	this->submitbutton->registerEventHandler( "listener" , PROG_Explorer::handler );
	
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

_gadgetEventReturnType PROG_Explorer::handler( _gadgetEvent event )
{
	
	_gadget* that = event.getGadget();
	PROG_Explorer* prog = (PROG_Explorer*)that->getStyle().own;
	
	if( that->getType() == _gadgetType::button )
	{
		string val = prog->addressbar->getStrValue();
		prog->path = val;
		prog->fileview->setPath( val );
		/*if( created )
			delete b2;
		b2 = new _label( 23 , 20 , "Hasllo" );
		b2->registerEventHandler( "mouseClick" , PROG_Explorer::handler );
		//printf("->>%p,%p",prog,prog->window);
		prog->window->addChild( b2 );
		created = true;*/
	}
	//if( that->getType() == _gadgetType::label )
		//((_label*)that)->setStrValue( "hello" );
	
	return handled;
}


int PROG_Explorer::main( _cmdArgs& args )
{
	return 0;
}