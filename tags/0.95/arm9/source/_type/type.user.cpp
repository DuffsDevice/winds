//! Standard Headers
#include <time.h>

//! Types
#include "_type/type.imagefile.h"
#include "_type/type.bitmap.resizer.h"
#include "_type/type.cwdchanger.h"
#include "_type/type.time.h"
#include "func.md5.h"
#include "func.memory.h"
#include "_type/type.user.h"

//! BMP_DefaultUserIcon
#include "_resource/resource.image.defaultuser.h"
#include "_resource/resource.image.windows.wallpaper.h"

_bitmap _user::getUserLogoFromImage( _constBitmap& bmp )
{
	_bitmap logo = _bitmap( 14 , 14 );
	
	logo.copy( 1 , 1 , bmp );
	
	logo.drawRect( 0 , 0 , 14 , 14 , _color::fromRGB( 31 , 29 , 18 ) );
	logo.drawPixel( 0 , 0 , _color::fromRGB( 15 , 15 , 24 ) );
	logo.drawPixel( 13 , 0 , _color::fromRGB( 15 , 15 , 24 ) );
	logo.drawPixel( 13 , 13 , _color::fromRGB( 15 , 15 , 24 ) );
	logo.drawPixel( 0 , 13 , _color::fromRGB( 15 , 15 , 24 ) );
	
	return logo;
}

_bitmap _user::getUserImage( string path )
{
	_imageFile imagefile = _imageFile( path );
	_bitmap image = imagefile.readBitmap();
	
	if( image.isValid() )
		return _bitmapResizer( 12 , 12 , image );
	
	// Create default image
	_bitmap bmp = _bitmap( 12 , 12 );
	
	bmp.copy( 0 , 0 , BMP_DefaultUserIcon() );
	
	return bmp;
}

void _user::readConstants(){
	this->mDD		= this->getIntAttr( "minDragDistance" );
	this->mCC		= this->getIntAttr( "maxClickCycles" );
	this->mDC		= this->getIntAttr( "maxDoubleClickCycles" );
	this->mDA		= this->getIntAttr( "maxDoubleClickArea" );
	this->kRD		= this->getIntAttr( "keyRepetitionDelay" );
	this->kRS		= this->getIntAttr( "keyRepetitionSpeed" );
	this->fOH		= this->getIntAttr( "fileObjectHeight" );
	this->lIH		= this->getIntAttr( "listItemHeight" );
	this->sFE		= this->getIntAttr( "showFileExtension" );
	this->sBTC		= this->getIntAttr( "startButtonTextColor" );
	this->dTC		= this->getIntAttr( "desktopColor" );
	this->mKF		= this->getIntAttr( "magnifyKeyboardFocus" );
	this->isAdmin	= this->getIntAttr( "adminRights" );
	this->cOH		= this->getIntAttr( "counterObjectHeight" );
	this->sOH		= this->getIntAttr( "selectObjectHeight" );
	this->rCD		= this->getIntAttr( "rightClickDelay" );
}

_user::_user( string folderName ) :
	_iniFile( "%USERS%/" + folderName + "/user.ini" )
	, folderName( folderName )
{
	if( _iniFile::isUsedFirstTime() )
	{
		this->getMap() = 
			{
				{ "_global_" , {
					{ "userName" , this->folderName } ,
					{ "userCode" , "" } ,
					{ "adminRights" , "1" } ,
					{ "userLogo" , "%APPDATA%/usericons/guest.png" } ,
					{ "wallpaper" , "default" } ,
					{ "wallpaperView" , "0" } ,
					{ "desktopColor" , "RGB( 7 , 13 , 20 )" } ,
					{ "showFileExtension" , "1" } ,
					{ "startButtonTextColor" , "RGB( 30 , 30 , 30 )" } ,
					{ "keyRepetitionDelay" , "30" } ,
					{ "keyRepetitionSpeed" , "6" } ,
					{ "minDragDistance" , "9" } ,
					{ "maxClickCycles" , "30" } ,
					{ "maxDoubleClickCycles" , "60" } , 
					{ "maxDoubleClickArea" , "6" } ,
					{ "fileObjectHeight" , "10" } ,
					{ "listItemHeight" , "9" } ,
					{ "magnifyKeyboardFocus" , "1" } ,
					{ "counterObjectHeight" , "16" } ,
					{ "selectObjectHeight" , "10" } ,
					{ "rightClickDelay" , "45" }
				} }
			};
		
		// Write to disk!
		this->flush();
	}
	
	//
	// Read Some constants into fast-accessible variables
	//
	this->readConstants();
	
	
	//
	// Read The User's image
	//
	
	// Set Currently Working directory
	_cwdChanger cw ( "%USERS%/" + this->folderName );
	
	// A userImage
	_bitmap bmp = _user::getUserImage( _iniFile::readIndex( "_global_" , "userLogo" ) );
	
	// ... and a Logo
	this->userLogo = _user::getUserLogoFromImage( bmp );
	
	// ... and a very nice Wallpaper!
	this->wallpaper = _imageFile( _iniFile::readIndex( "_global_" , "wallpaper" ) ).readBitmap();
	if( !this->wallpaper.isValid() )
		this->wallpaper = BMP_WindowsWallpaper();
	this->wallpaperView = (_wallpaperViewType) this->getIntAttr( "wallpaperView" );
	
	switch( this->wallpaperView ){
		case WALLPAPER_ORIG:
			break;
		case WALLPAPER_ADJUST:
			this->wallpaper = _bitmapResizer( SCREEN_WIDTH , SCREEN_HEIGHT - 10 , this->wallpaper );
			break;
		case WALLPAPER_PATTERN:
			//bP.copyPattern( 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT - 8 , wp , 128 , 96 );
			break;
		default:
			break;
	}
	
	
	//
	// Read the start menu
	//
	
	for( auto entry : _iniFile::readSection("startmenu") )
	{
		const string& str = entry.second;
		size_t colonPos = str.find(',');
		
		// Split into path and name
		string path = str.substr( 0 , colonPos );
		string name = colonPos != string::npos ? str.substr( colonPos + 1 ) : "";
		
		// Trim Path and name
		trim( path );
		trim( name );
		
		// Add
		startMenuEntries.push_back( make_pair( move(path) , move(name) ) );
	}
}

_user::~_user()
{
	// Index last time the user logged in
	_iniFile::writeIndex( "_global_" , "lastTimeLogIn" , _time::now() );
	
	// Write start menu back into .ini
	int curIdx = 1;
	for( _pair<string,string>& entry : startMenuEntries )
	{
		string value = entry.first;
		if( !entry.second.empty() )
			value += "," + entry.second;
		_iniFile::writeIndex( "startMenu" , int2string(curIdx) , move(value) );
	}
}

bool _user::checkPassword( const string& pw ) const {
	return md5( pw ) == _iniFile::readIndex( "_global_" , "userCode" );
}

bool _user::hasPassword() const {
	string value = _iniFile::readIndex( "_global_" , "userCode" );
	return !value.empty() && value != "d41d8cd98f00b204e9800998ecf8427e"; // this is the result for md5("")
}

void _user::setPassword( const string& pw ){
	_iniFile::writeIndex( "_global_" , "userCode" , md5( pw ) );
}