#include "_type/type.windows.h"

int main( int argc , char* argv[] )
{
	_windows::init();
	_windows::main();
	_windows::end();
	
	return 0;
}