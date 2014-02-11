#include "_type/type.ini.file.h"

_iniFile::_iniFile( const string& filename ) :
	_direntry( filename )
	, firstTimeUse( !_direntry::isExisting() )
{
	if( _direntry::isExisting() )
		_ini::read( _direntry::readString() );
}

void _iniFile::flush()
{
	_direntry::openwrite( true );
	_direntry::writeString( _ini::write() );
	_direntry::close();
}