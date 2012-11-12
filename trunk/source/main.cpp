#include "_type/type.system.h"
#include "_type/type.imagefile.h"
#include "_type/type.callback.h"
#include "_gadget/gadget.textbox.h"
#include "_gadget/gadget.select.h"
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.radio.h"
#include "_gadget/gadget.window.h"
#include "_gadget/gadget.progressbar.h"

_callbackReturn dragHandler( _event event ){
	_gadget* that = event.getGadget();
	
	if( event.getType() == dragStart )
		return handled;
	// Has the Gadget to move?
	if( event.getDeltaX() == 0 && event.getDeltaY() == 0 )
		return handled;
	
	// Move it relatively
	that->moveRelative( event.getDeltaX() , event.getDeltaY() );
	
	// Return
	return handled;
}

int main( int argc , char* argv[] )
{
	_system_ = new _system();
	//_system_->getBuiltInProgram( "explorer.exe" )->execute({{"path","/"}});
	
	/*_scrollBar* bar = new _scrollBar( 70 , 70 , 50 , 50 , 50 , _dimension::vertical , 100 );
	_system_->_gadgetHost_->addChild( bar );
	bar->setLength2( 2 );*/
	
	/*_textbox* bar = new _textbox( 70 , 10 , 50 , "Hello" );
	_system_->_gadgetHost_->addChild( bar );*/
	
	//_select* bar2 = new _select( 70 , 40 , 20 , 20 , { { 0 , "Ooption 1" } , { 1 , "Superb 2" } , { 2 , "Totall!" } , { 3 , "Enormous 5" } } );
	//_system_->_gadgetHost_->addChild( bar2 );
	//
	//_radiogroup* grp = new _radiogroup();
	//_window* wnd = new _window( 100 , 100 , 10 , 10 , "Test-Window" );
	//
	//wnd->addChild( new _radio( 20 , 20 , grp ) );
	//wnd->addChild( new _radio( 20 , 30 , grp ) );
	//wnd->addChild( new _radio( 20 , 40 , grp ) );
	//wnd->addChild( new _radio( 20 , 55 , grp ) );
	//wnd->addChild( new _label( 32 , 22 , "I Like" ) );
	//wnd->addChild( new _label( 32 , 32 , "Quite Good" ) );
	//wnd->addChild( new _label( 32 , 42 , "It's OK" ) );
	//wnd->addChild( new _label( 32 , 57 , "Not Good at all" ) );
	//
	//_system_->_gadgetHost_->addChild( wnd );
	//
	//_system_->_gadgetHost_->addChild( new _textbox( 50 , 103 , 50 , "fdsgf" ) );
	
	//_direntry d = _direntry("Hello.txt");
	//d.setAttrs(0);
	//sc = new _wifiSocketClient( "192.168.178.41" , 8080 , tcpNormal );
	
	//_direntry d = _direntry("windows/users/Jakob/user.ini");
	//printf("Text: %s",d.readString().c_str() );
	//while(true); 
	
	_system_->main();
	return 0;
}
