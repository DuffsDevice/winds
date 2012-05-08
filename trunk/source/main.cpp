// Includes
#include <stdio.h>
#include <sstream>
using namespace std;


// Standard LibNDS
#include "nds.h"

#include "programm_bin.h"

// Windows!!!
#include "DSWindows.h"

#define METHOD(r,o,m) (r (o::*)() )(&o::m)

int main(){	
	
	_system_->getBuiltInProgram( "explorer.exe" )->execute();
	_system_->getBuiltInProgram( "explorer.exe" )->execute();
	
	/*_window* w = new _window( 100 , 60 , 5 , 5 , "Fenster" );
	WIN->addChild( w );
	_textbox* tb = new _textbox( 20 , 20 , 40 , "Test" );
	_button* b = new _button( 40 , 40 , "Send Request!");
	w->addChild( tb );
	w->addChild( b );*/
	
	//sc = new _wifiSocketClient( "192.168.178.41" , 8080 , tcpNormal );
	/*string a = (const char*)programm_bin;
	a = a.substr(0 , programm_bin_size);
	
	_program prog = _program(a.c_str());
	prog.run( WIN );*/
	
	//_system_->_windows_->addChild( new _startMenu( 10 , 10 ) );
	
	_system_->main();
	return 0;
}
