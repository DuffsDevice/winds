// Includes
#include <stdio.h>
#include <sstream>
using namespace std;


// Standard LibNDS
//#include "nds.h"

//#define METHOD(r,o,m) (r (o::*)() )(&o::m)
#include "_type/type.system.h"
#include "_type/type.ini.h"

_gadgetEventReturnType dragHandler( _gadgetEvent event ){
	_gadget* that = event.getGadget();
	
	if( event.getType() == "dragStart" )
		return handled;
	// Has the Gadget to move?
	if( event.getArgs().getDeltaX() == 0 && event.getArgs().getDeltaY() == 0 )
		return handled;
	
	// Move it relatively
	that->moveRelative( event.getArgs().getDeltaX() , event.getArgs().getDeltaY() );
	
	// Return
	return handled;
}

int main(){	
	
	_system_->getBuiltInProgram( "explorer.exe" )->execute({{"path","/windows/"}});
	
	//sc = new _wifiSocketClient( "192.168.178.41" , 8080 , tcpNormal );
	/*string a = (const char*)programm_bin;
	a = a.substr(0 , programm_bin_size);
	
	_program prog = _program(a.c_str());
	prog.run( WIN );*/
	
	_system_->main();
	return 0;
}