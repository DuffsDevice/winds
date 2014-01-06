#include "_type/type.ini.file.h"

_iniFile::_iniFile( const string& filename ) :
	_direntry( filename )
{
	if( _direntry::isExisting() ){
		_ini::read( _direntry::readString() );
		_ini::deleteIndex( "_global_" , ".firstTimeUse" );
		this->creation = false;
	}
	else{
		_direntry::create();
		_ini::writeIndex( "_global_" , ".firstTimeUse" , "1" );
		this->creation = true;
	}
}

void _iniFile::flush()
{
	_direntry::writeString( _ini::write() );
	_direntry::close();
}