#include "_type/type.registry.h"
#include "_type/type.time.h"
#include <nds/system.h>

_registry::_registry() :
	_iniFile( "%WINDIR%/registry.ini" )
{
	if( _iniFile::creation )
	{
		this->getMap() = 
			{
				{ "filemapper" , {
					{ "jpg" , "%SYSTEM%/paint.exe -$F" } ,
					{ "png" , "%SYSTEM%/paint.exe -$F" } ,
					{ "bmp" , "%SYSTEM%/paint.exe -$F" } ,
					{ "gif" , "%SYSTEM%/paint.exe -$F" } ,
					{ "ico" , "%SYSTEM%/paint.exe -$F" }
				} } ,
				{ "internal" , {
					{ "correctShutdown" , "0" } ,
					{ "timeOnline" , "0" } ,
					{ "language" , language2string[(_language)PersonalData->language] } ,
				} }
			};
		
		// Write to disk!
		this->flush();
	}
	
	// Set start time
	this->systemStartTime = _time::now();
	
	// Read Language
	this->language = string2language[ _iniFile::readIndex( "internal" , "language" ) ];
}

const string& _registry::getFileTypeHandler( const string& extension ) const {
	return _iniFile::readIndex( "filemapper" , extension );
}

bool _registry::wasSafelyShutDown(){
	return _iniFile::readIndexInt( "internal" , "correctShutdown" );
}

void _registry::indicateSafeShutdown(){
	_iniFile::writeIndex( "internal" , "correctShutdown" , "1" );
}

_registry::~_registry()
{
	// Write language
	_iniFile::writeIndex( "internal" , "language" , language2string[this->language] );
	
	// Update time
	_unixTime timeOnline = _iniFile::readIndexInt( "internal" , "timeOnline" );
	
	// Add difference between system end and system start
	timeOnline += _time::now().toUnixTime() - this->systemStartTime;
	
	// Write new time
	_iniFile::writeIndex( "internal" , "timeOnline" , int2string(timeOnline) );
}