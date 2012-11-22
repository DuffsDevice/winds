//! Standard Headers
#include "time.h"

//! Types
#include "_type/type.imagefile.h"
#include "_type/type.bitmapResizer.h"
#include "func.md5.h"
#include "func.memory.h"
#include "_type/type.user.h"

_bitmap _user::getUserLogoFromImage( const _bitmap& bmp )
{
	_bitmap logo = _bitmap( 14 , 14 );
	
	// Check if valid
	if( !bmp.isValid() )
	{
		logo.reset( RGB( 22 , 22 , 22 ) );
		logo.drawCircle( 5 , 5 , 3 , COLOR_RED );
		logo.drawPixel( 5 , 5 , COLOR_RED );
		logo.drawPixel( 6 , 4 , COLOR_RED );
		logo.drawPixel( 4 , 6 , COLOR_RED );
	}
	else
		logo.copy( 1 , 1 , &bmp );
	
	logo.drawRect( 0 , 0 , 14 , 14 , RGB( 31 , 29 , 18 ) );
	logo.drawPixel( 0 , 0 , RGB( 15 , 15 , 24 ) );
	logo.drawPixel( 13 , 0 , RGB( 15 , 15 , 24 ) );
	logo.drawPixel( 13 , 13 , RGB( 15 , 15 , 24 ) );
	logo.drawPixel( 0 , 13 , RGB( 15 , 15 , 24 ) );
	
	return logo;
}

_bitmap _user::getUserImage( string path )
{
	_imagefile imagefile = _imagefile( path );
		
	if( imagefile.isValid() )
		return _bitmapResizer( 12 , 12 , &imagefile );
	
	return _bitmap();
}

_user::_user( string folderName ) :
	_registry( "%USERS%/" + folderName + "/user.ini"  )
	, folderName( folderName )
{ 
	if( _registry::creation )
	{
		this->ini->getMap() = 
			{ { "_global_" , 
				{
					{ "userName" , this->folderName } ,
					{ "desktopImage" , "" } ,
					{ "desktopColor" , "RGB( 7 , 13 , 20 )" } ,
					{ "userLogo" , "flower.png" } ,
					{ "showFileExtension" , "1" } ,
					{ "startButtonText" , "start" } ,
					{ "startButtonTextColor" , "RGB( 30 , 30 , 30 )" } ,
					{ "keyRepetitionDelay" , "35" } ,
					{ "keyRepetitionSpeed" , "3" } ,
					{ "minDragDistance" , "9" } ,
					{ "maxClickCycles" , "30" } ,
					{ "maxDoubleClickCycles" , "60" } , 
					{ "maxDoubleClickArea" , "6" } ,
					{ "fileObjectHeight" , "10" } ,
					{ "selectObjectHeight" , "9" }
				}
			} };
		this->flush();
	}
	
	// Initialize
	this->mDD = this->getIntAttr( "minDragDistance" );
	this->mCC = this->getIntAttr( "maxClickCycles" );
	this->mDC = this->getIntAttr( "maxDoubleClickCycles" );
	this->mDA = this->getIntAttr( "maxDoubleClickArea" );
	this->kRD = this->getIntAttr( "keyRepetitionDelay" );
	this->kRS = this->getIntAttr( "keyRepetitionSpeed" );
	this->fOH = this->getIntAttr( "fileObjectHeight" );
	this->sOH = this->getIntAttr( "selectObjectHeight" );	
	this->sFE = this->getIntAttr( "showFileExtension" );	
	this->sBTC= this->getIntAttr( "startButtonTextColor" );	
	
	// Set Currently Working directory
	//string cwd = _direntry::getWorkingDirectory();
	
	//! todo: fix cwd => not Working!
	//_direntry::setWorkingDirectory( "%USERS%/" + this->userName );
	
	_bitmap bmp = _user::getUserImage( _registry::readIndex( "_global_" , "userLogo" ) );
	
	if( bmp.isValid() )
		// ... and a Logo
		this->userLogo = _user::getUserLogoFromImage( bmp );
	
	//_direntry::setWorkingDirectory( cwd );
}

_user::~_user()
{
	char buffer[18];
	sprintf( buffer , "%ld" , time(NULL) );
	string str = buffer;
	_registry::writeIndex( "_global_" , "lastTimeLogIn" , str );
}

string _user::getUsername()
{
	return _registry::readIndex( "_global_" , "userName" );
}

bool _user::checkPassword( string pw )
{
	return md5( pw ) == _registry::readIndex( "_global_" , "userCode" );
}

bool _user::hasPassword()
{
	return _registry::readIndex( "_global_" , "userCode" ).length() != 0;
}

void _user::setPassword( string pw )
{
	_registry::writeIndex( "_global_" , "userCode" , md5( pw ) );
}

void _user::remove()
{
	_direntry* d = new _direntry( "%USERS%/" + this->folderName );
	d->unlink();
	delete d;
}

void _user::setUsername( string uN )
{
	_registry::writeIndex( "_global_" , "userName" , uN );
}

_s32 _user::getIntAttr( string idx )
{
	string attr = _registry::readIndex( "_global_" , idx );
	
	// Allow formats of RGB( 14 , 6 , 9 ) and rgba( 1 , 20 , 25 , 0 )
	if( attr.substr( 0 , 3 ) == "RGB" || attr.substr( 0 , 3 ) == "rgb" )
	{
		_u8 i = 2 , f = 0;
		bool hasAlpha = false;
		_u8 r , b , g , a = 1;
		
		if( isalpha( attr[3] ) && ( attr[3] == 'A' || attr[3] == 'a' ) )
		{
			i = 3;
			hasAlpha = true;
		}
		else if( isalpha( attr[3] ) ) // No RGB
			return string2int( attr.c_str() );
		
		for( ; !isdigit( attr[i] ) && attr.length() > i ; i++ ); // Go to first number
		f = i; // Set Mark
		if( !isdigit( attr[i] ) ) // Check if we reached the end of the string
			return 0;
		for( ; isdigit( attr[i] ) ; i++ ); // Go to end of number
		r = string2int( attr.substr( f , i-f ).c_str() ); // Save red part
		
		for( ; !isdigit( attr[i] ) && attr.length() > i ; i++ ); // Go to second number
		f = i; // Set Mark
		if( !isdigit( attr[i] ) ) // Check if we reached the end of the string
			return 0;
		for( ; isdigit( attr[i] ) ; i++ ); // Go to end of number
		g = string2int( attr.substr( f , i-f ).c_str() ); // Save green part
		
		for( ; !isdigit( attr[i] ) && attr.length() > i ; i++ ); // Go to third number
		f = i; // Set Mark
		if( !isdigit( attr[i] ) ) // Check if we reached the end of the string
			return 0;
		for( ; isdigit( attr[i] ) ; i++ ); // Go to end of number
		b = string2int( attr.substr( f , i-f ).c_str() ); // Save blue part
		
		if( hasAlpha )
		{
			for( ; !isdigit( attr[i] ) && attr.length() > i ; i++ ); // Go to fourth number
			if( attr[i] == '0' )
				a = 0;
		}
		
		return RGBA( r , g , b , a );
	}
	return string2int( attr.c_str() );
}

string _user::getStrAttr( string idx )
{
	return _registry::readIndex( "_global_" , idx );
}