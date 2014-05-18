#include "_type/type.cwdchanger.h"
#include "_controller/controller.filesystem.h"

namespace unistd{
#include <unistd.h>
}

void _cwdChanger::setCWD( const string& dir ){
	unistd::chdir( dir.c_str() );
}

string _cwdChanger::getCWD(){
	_char val[PATH_MAX];
	unistd::getcwd( val , PATH_MAX );
	return val;
}

_cwdChanger::_cwdChanger( const string& newDir ) :
	oldCwd( _cwdChanger::getCWD() )
{
	_cwdChanger::setCWD( _filesystemController::replaceAssocDirs( newDir ) );
}