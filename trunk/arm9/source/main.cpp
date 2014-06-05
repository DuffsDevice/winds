#include "_type/type.windows.h"
#include "_type/type.bitmap.h"

int main( int argc , char* argv[] )
{
	_windows::init();
	_windows::main();
	_windows::end();
	
	std::exit(0);
}