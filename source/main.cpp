#include "_type/type.system.h"

int main( int argc , char* argv[] )
{
	_system::start();
	
	//_direntry d = _direntry("Hello World.txt");
	//d.setAttrs(0);
	
	_system::main();
	_system::end();
	
	return 0;
}