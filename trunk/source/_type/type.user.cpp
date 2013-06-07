//! Standard Headers
#include <time.h>

//! Types
#include "_type/type.imagefile.h"
#include "_type/type.imagefile.builtin.h"
#include "_type/type.bitmapResizer.h"
#include "_type/type.time.h"
#include "func.md5.h"
#include "func.memory.h"
#include "_type/type.user.h"

//! BMP_DefaultUserIcon
#include "_resource/BMP_DefaultUserIcon.h"
#include "_resource/BMP_WindowsWallpaper.h"

_bitmap _user::getUserLogoFromImage( const _bitmap& bmp )
{
	_bitmap logo = _bitmap( 14 , 14 );
	
	logo.copy( 1 , 1 , bmp );
	
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
		return _bitmapResizer( 12 , 12 , imagefile );
	
	// Create default image
	_bitmap bmp = _bitmap( 12 , 12 );
	
	bmp.copy( 0 , 0 , BMP_DefaultUserIcon() );
	
	return bmp;
}

_user::_user( string folderName ) :
	_registry( _direntry( "%USERS%/" + folderName + "/user.ini" ) )
	, folderName( folderName )
{
	if( _registry::creation )
	{
		this->ini->getMutableMap() = 
			{ { "_global_" , 
				{
					{ "userName" , this->folderName } ,
					{ "userCode" , "" } ,
					{ "wallpaper" , "default" } ,
					{ "wallpaperView" , "0" } ,
					{ "desktopColor" , "RGB( 7 , 13 , 20 )" } ,
					{ "userLogo" , "guest.png" } ,
					{ "showFileExtension" , "1" } ,
					{ "startButtonText" , "start" } ,
					{ "startButtonTextColor" , "RGB( 30 , 30 , 30 )" } ,
					{ "keyRepetitionDelay" , "30" } ,
					{ "keyRepetitionSpeed" , "4" } ,
					{ "minDragDistance" , "9" } ,
					{ "maxClickCycles" , "30" } ,
					{ "maxDoubleClickCycles" , "60" } , 
					{ "maxDoubleClickArea" , "6" } ,
					{ "fileObjectHeight" , "10" } ,
					{ "selectObjectHeight" , "9" } ,
					{ "magnifyKeyboardFocus" , "1" }
				}
			} };
		
		// Write to disk!
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
	this->dTC = this->getIntAttr( "desktopColor" );	
	this->mKF = this->getIntAttr( "magnifyKeyboardFocus" );	
	
	// Set Currently Working directory
	string cwd = _direntry::getWorkingDirectory();
	
	//! todo: fix cwd => not Working!
	_direntry::setWorkingDirectory( "%USERS%/" + this->folderName );
	
	_bitmap bmp = _user::getUserImage( _registry::readIndex( "_global_" , "userLogo" ) );
	
	// ... and a Logo
	this->userLogo = _user::getUserLogoFromImage( bmp );
	
	// ... and a very nice Wallpaper!
	this->wallpaper = _imagefile( _registry::readIndex( "_global_" , "wallpaper" ) );
	if( !this->wallpaper.isValid() )
		this->wallpaper = BMP_WindowsWallpaper();
	this->wallpaperView = (_wallpaperViewType) this->getIntAttr( "wallpaperView" );
	
	switch( this->wallpaperView ){
		//case WALLPAPER_ORIG:
		case WALLPAPER_ADJUST:
			this->wallpaper = _bitmapResizer( SCREEN_WIDTH , SCREEN_HEIGHT - 10 , this->wallpaper );
			break;
		case WALLPAPER_PATTERN:
			//bP.copyPattern( 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT - 8 , wp , 128 , 96 );
			break;
		default:
			break;
	}
	
	_direntry::setWorkingDirectory( cwd );
}


void _user::createAs( string folderName )
{
	_iniStructure str = move( this->ini->getMap() );
	*this = _user( folderName );
	this->ini->getMutableMap() = move( str );
}

_user::~_user()
{
	_registry::writeIndex( "_global_" , "lastTimeLogIn" , _time::now() );
}

bool _user::checkPassword( string pw ) const 
{
	return md5( pw ) == _registry::readIndex( "_global_" , "userCode" );
}

bool _user::hasPassword() const 
{
	string value = _registry::readIndex( "_global_" , "userCode" );
	return !value.empty() && value != "d41d8cd98f00b204e9800998ecf8427e"; // this is the result for md5("")
}

void _user::setPassword( string pw ){
	_registry::writeIndex( "_global_" , "userCode" , md5( pw ) );
}