// Includes
#include <stdio.h>
#include <sstream>
using namespace std;


// Standard LibNDS
#include "nds.h"

// Windows!!!
#include "DSWindows.h"

_windows* WIN = nullptr;

#define METHOD(r,o,m) (r (o::*)() )(&o::m)

int main(){	
	
	_system_->_runtimeAttributes_->wallpaper = new BMP_WindowsWallpaper();
	
	WIN = new _windows();
	_system_->setWindows( WIN );
	
	_fileview* view = new _fileview( 200 , 150 , 0 , 0 , _diskRoot_ );
	WIN->addChild( view );
	/*_window* w = new _window( 100 , 60 , 5 , 5 , "Fenster" );
	WIN->addChild( w );
	_textbox* tb = new _textbox( 20 , 20 , 40 , "Test" );
	_button* b = new _button( 40 , 40 , "Send Request!");
	w->addChild( tb );
	w->addChild( b );*/
	
	//sc = new _wifiSocketClient( "192.168.178.41" , 8080 , tcpNormal );
	/*_file a("/datei.lua");
	_program prog = _program(a.readString( a.getSize() ).c_str());
	prog.run( WIN );*/
	
	_system_->run();
	return 0;
}
