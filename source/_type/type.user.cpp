#include "_type/type.user.h"
#include "time.h"
#include <sstream>
#include "func.md5.h"
#include "func.memory.h"
#include "_file/direntry.png.h"
#include "_type/type.bitmapAnimation.h"

_user::_user( string username ) :
	_registry( "%USERS%/" + username + "/user.ini"  )
	, userName( username )
	, userLogo( nullptr )
	, userImage( nullptr )
{ 
	if( _registry::creation )
	{
		this->ini->getMap() = 
			{ { "_global_" , 
				{
					{ "userName" , this->userName } ,
					{ "desktopImage" , "" } ,
					{ "desktopColor" , "26839" } ,
					{ "userLogo" , "" } ,
					{ "showFileExtension" , "1" } ,
					{ "startButtonText" , "start" } ,
					{ "keyRepetitionDelay" , "35" } ,
					{ "keyRepetitionSpeed" , "3" } ,
					{ "maxClickCycles" , "30" } ,
					{ "maxDoubleClickCycles" , "60" } , 
					{ "maxDoubleClickArea" , "6" } ,
					{ "fileObjectHeight" , "10" } ,
					{ "selectObjectHeight" , "8" }
				}
			} };
	}
	
	
	this->userLogo = new _bitmap( 14 , 14 );
	this->userLogo->reset( RGB( 22 , 22 , 22 ) );
	this->userLogo->drawCircle( 5 , 5 , 3 , COLOR_RED );
	this->userLogo->drawPixel( 5 , 5 , COLOR_RED );
	this->userLogo->drawPixel( 6 , 4 , COLOR_RED );
	this->userLogo->drawPixel( 4 , 6 , COLOR_RED );
	
	_png* uImage = new _png( "%USERS%/" + this->userName + "/userimage.png" );
	
	if( uImage && uImage->getBitmap() )
	{
		_bitmapTransform* bmp = new _bitmapTransform( 13 , 13 , uImage );
		bmp->compute();
		
		// Create a Logo
		this->userImage = bmp;
		
		// ... and the Raw Image
		this->userLogo->copy( 0 , 0 , this->userImage );
	}
	
	delete uImage;
	
	this->userLogo->drawRect( 0 , 0 , 14 , 14 , RGB( 31 , 29 , 18 ) );
	this->userLogo->drawPixel( 0 , 0 , RGB( 15 , 15 , 24 ) );
	this->userLogo->drawPixel( 13 , 0 , RGB( 15 , 15 , 24 ) );
	this->userLogo->drawPixel( 13 , 13 , RGB( 15 , 15 , 24 ) );
	this->userLogo->drawPixel( 0 , 13 , RGB( 15 , 15 , 24 ) );
}

_user::~_user()
{
	stringstream s;
	s << time(NULL);
	_registry::writeIndex( "_global_" , "lastTimeLogIn" , s.str() );
	
	delete this->userLogo;
	delete this->userImage;
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
	_direntry* d = new _direntry( "%USERS%/" + this->userName );
	d->unlink();
	delete d;
}

void _user::setUsername( string uN )
{
	_direntry* d = new _direntry( "%USERS%/" + this->userName );
	d->rename( uN );
	delete d;
}

_s32 _user::getIntAttr( string idx )
{
	return string2int( _registry::readIndex( "_global_" , idx ).c_str() );
}

string _user::getStrAttr( string idx )
{
	return _registry::readIndex( "_global_" , idx );
}