#include "_type/type.user.h"
#include "_type/type.imagefile.h"
#include "_type/type.bitmap.transform.h"
#include "_type/type.cwdchanger.h"
#include "_type/type.time.h"
#include "func.md5.h"

#include "_resource/resource.image.defaultuser.h" // Border of a user's logo
#include "_resource/resource.image.windows.wallpaper.h" // Default Wallpaper

_bitmap _user::getLogoFromImage( _constBitmap& bmp ){
	_bitmap logo = _bitmap( 14 , 14 );
	
	logo.copy( 1 , 1 , bmp );
	
	logo.drawRect( 0 , 0 , 14 , 14 , _color::fromRGB( 31 , 29 , 18 ) );
	logo.drawPixel( 0 , 0 , _color::fromRGB( 15 , 15 , 24 ) );
	logo.drawPixel( 13 , 0 , _color::fromRGB( 15 , 15 , 24 ) );
	logo.drawPixel( 13 , 13 , _color::fromRGB( 15 , 15 , 24 ) );
	logo.drawPixel( 0 , 13 , _color::fromRGB( 15 , 15 , 24 ) );
	
	return logo;
}

_bitmap _user::getImage( string path )
{
	_bitmap image = _imageFile( path ).readBitmap();
	
	if( image.isValid() )
		return _bitmapTransform( image , (_length)12 , (_length)12 );
	
	// Create default image
	_bitmap bmp = _bitmap( 12 , 12 );
	
	bmp.copy( 0 , 0 , BMP_DefaultUserIcon() );
	
	return bmp;
}


_user::~_user()
{
	// Write User data back to file
	_iniFile::writeIndex( "_global_" , "lastTimeLogIn" , _time::now() );
	_iniFile::writeIndex( "_global_" , "desktopColor" , int2string(desktopColor) );
	_iniFile::writeIndex( "_global_" , "adminRights" , int2string(hasAdminRights) );
	_iniFile::writeIndex( "_global_" , "wallpaperViewType" , int2string( (_u8) wallpaperViewType) );
	
	// Write start Menu
	int curIdx = 1;
	for( _pair<string,string>& entry : startMenuEntries )
	{
		string value = entry.first;
		if( !entry.second.empty() )
			value += "," + entry.second;
		_iniFile::writeIndex( "startMenu" , int2string(curIdx) , move(value) );
	}
}

_user::_user( string pathToIni ) :
	_iniFile( pathToIni )
	, desktopColor( _color::fromRGB( 7 , 13 , 20 ) )
	, hasAdminRights( false )
{
	if( pathToIni.empty() )
		return;
	
	// Set Currently Working directory
	_cwdChanger cw{ _iniFile::readIndex( "_global_" , "homeFolder" ) };
	
	// Read Logo
	setLogo( _iniFile::readIndex( "_global_" , "userLogo" ) );
	
	// Read Wallpaper
	setWallpaper( _iniFile::readIndex( "_global_" , "wallpaper" ) );
	setWallpaperViewType( (_wallpaperViewType) _iniFile::readIndexInt( "_global_" , "wallpaperViewType" ) );
	
	// Cache some miscellaneous attrributes of the user
	desktopColor	= _iniFile::readIndexInt( "_global_" , "desktopColor" );
	hasAdminRights	= _iniFile::readIndexInt( "_global_" , "adminRights" );
	
	// Read the start menu
	for( auto entry : _iniFile::readSection("startMenu") )
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

void _user::setLogo( string path )
{
	// Write to Registry
	_iniFile::writeIndex( "_global_" , "userLogo" , move(path) );
	
	// Update Logo
	_bitmap userImage	= _user::getImage( path );
	userLogo			= _user::getLogoFromImage( move(userImage) );
}

void _user::setLogo( _u8 standardLogoNumber ){
	_user::setLogo( string("%APPDATA%/usericons/") + _user::standardLogos[standardLogoNumber] + string(".bmp") );
}

void _user::setWallpaper( string path ){
	_iniFile::writeIndex( "_global_" , "wallpaper" , move(path) );	// Write to registry
	wallpaper = _imageFile( path ).readBitmap();					// Update Wallpaper
}

void _user::setWallpaperViewType( _wallpaperViewType viewType )
{
	_iniFile::writeIndex( "_global_" , "wallpaperViewType" , int2string( (_u8)viewType ) );	// Write to registry
	wallpaperViewType = viewType;															// Update View Type
	
	_length width = wallpaper.getWidth();
	_length height = wallpaper.getHeight();
	_length availWidth = SCREEN_WIDTH;
	_length availHeight = SCREEN_HEIGHT - 10;
	
	// Adjust Wallpaper at certain view types
	if( wallpaperViewType == _wallpaperViewType::stretch )
		wallpaper = _bitmapTransform( wallpaper , availWidth , availHeight );
	else
	{
		float wallpaperRatio = float(width) / float(height);
		float screenRatio = float(availWidth) / float(availHeight);
		
		// Height has to be adjusted
		if(
			( wallpaperRatio > screenRatio && wallpaperViewType == _wallpaperViewType::contain ) // Wallpaper is broader than screen
			|| ( wallpaperRatio < screenRatio && wallpaperViewType == _wallpaperViewType::contain ) // Wallpaper is greater than screen
		)
			wallpaper = _bitmapTransform( wallpaper , availWidth , availWidth / wallpaperRatio );
		// Width has to be adjusted
		else
			wallpaper = _bitmapTransform( wallpaper , availHeight * wallpaperRatio , availHeight );
	}
}

bool _user::checkPassword( const string& pw ) const {
	return md5( pw ) == _iniFile::readIndex( "_global_" , "userCode" );
}

bool _user::hasPassword() const
{
	const string& value = _iniFile::readIndex( "_global_" , "userCode" ); // Buffer Reference to value
	
	return !value.empty() && value != "d41d8cd98f00b204e9800998ecf8427e"; // this is the result for md5("")
}

void _user::setPassword( const string& pw ){
	_iniFile::writeIndex( "_global_" , "userCode" , md5(pw) );
}

void _user::setName( string name ){
	_iniFile::writeIndex( "_global_" , "userName" , move(name) );
}

const string& _user::getName() const {
	return _iniFile::readIndex( "_global_" , "userName" );
}

const string& _user::getHomeFolder() const {
	return _iniFile::readIndex( "_global_" , "homeFolder" );
}

void _user::setHomeFolder( string path ){
	_iniFile::writeIndex( "_global_" , "homeFolder" , move(path) );
}

_bitmap _user::getStandardLogo( _u8 logoNumber ){
	return _user::getLogoFromImage(
		_user::getImage(
			string("%APPDATA%/usericons/") + _user::standardLogos[logoNumber] + string(".bmp")
		)
	);
}

_array<_literal,24> _user::standardLogos = {
	"airplane",		"astronaut",
	"ball",			"beach",
	"butterfly",	"car",
	"cat",			"chess",
	"dirt_bike",	"dog",
	"drip",			"duck",
	"fish",			"frog",
	"guest",		"guitar",
	"horses",		"kick",
	"lift_off",		"palm_tree",
	"pink_flower",	"red_flower",
	"skater",		"snowflake"
};