#include "_type/type.registry.h"
#include "_type/type.time.h"
#include "_type/type.imagefile.h"
#include <nds/system.h>

//! File-Images
#include "_resource/resource.icon.file.h"
#include "_resource/resource.icon.exe.h"
#include "_resource/resource.icon.txt.h"
#include "_resource/resource.icon.xml.h"
#include "_resource/resource.icon.nds.h"
#include "_resource/resource.icon.gba.h"
#include "_resource/resource.icon.ini.h"
#include "_resource/resource.icon.lua.h"
#include "_resource/resource.icon.folder.h"
#include "_resource/resource.icon.font.h"
#include "_resource/resource.icon.music.h"
#include "_resource/resource.icon.jpg.h"
#include "_resource/resource.icon.image.h"

_registry::_registry() :
	_iniFile( "%WINDIR%/registry.ini" )
{
	if( _iniFile::creation )
	{
		this->getMap() = 
			{
				{ "filemapper" , {
					{ "jpg" , "%SYSTEM%/accessories/paint.exe -$F" } ,
					{ "png" , "%SYSTEM%/accessories/paint.exe -$F" } ,
					{ "bmp" , "%SYSTEM%/accessories/paint.exe -$F" } ,
					{ "gif" , "%SYSTEM%/accessories/paint.exe -$F" } ,
					{ "ico" , "%SYSTEM%/accessories/paint.exe -$F" }
				} } ,
				{ "fileicon" , {
					{ "%dir%" , "" }
				} } ,
				{ "internal" , {
					{ "correctShutdown" , "0" } ,
					{ "timeOnline" , "0" } ,
					{ "language" , language2string[(_language)PersonalData->language] } ,
				} }
			};
		
		// Write to disk!
		this->create();
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

// Static Map to cache file images
static _map<string,_pair<string,_bitmap>> extension2UrlAndimage;


_bitmap _registry::getFileTypeImage( const string& extension , _mimeType mimeType ) const 
{
	// Fetch Url of the Icon
	const string& iconUrl = _iniFile::readIndex( "fileicon" , extension );
	
	// If we already loaded the icon corresponding to a specific extension
	_pair<string,_bitmap>& cachedIcon = extension2UrlAndimage[extension];
	
	// If the Cached Icon is valid and the url has not changed since
	if( cachedIcon.second.isValid() && cachedIcon.first == iconUrl )
		return cachedIcon.second;
	
	if( iconUrl.empty() || iconUrl == "default" )
	{
		switch( mimeType )
		{
			case _mime::directory:
				return BMP_FolderIcon();
				
			case _mime::application_x_lua_bytecode:
				return BMP_LuaIcon();
				
			case _mime::application_octet_stream:
			case _mime::application_microsoft_installer:
				return BMP_ExeIcon();
				
			case _mime::text_plain:
				return BMP_TxtIcon();
				
			case _mime::text_xml:
				return BMP_XmlIcon();
				
			case _mime::text_x_ini:
				return BMP_IniIcon();
				
			case _mime::application_x_nintendo_ds_rom:
				return BMP_NdsIcon();
				
			case _mime::application_x_nintendo_gba_rom:
				return BMP_GbaIcon();
			
			case _mime::font_opentype:
			case _mime::font_truetype:
				return BMP_FontIcon();
			
			case _mime::audio_mpeg:
			case _mime::audio_wav:
			case _mime::audio_x_aiff:
			case _mime::audio_mid:
			case _mime::audio_x_mpegurl:
			case _mime::audio_x_mod:
				return BMP_MusicIcon();
			
			case _mime::image_jpeg:
				return BMP_JpegIcon();
				
			case _mime::image_png:
			case _mime::image_gif:
			case _mime::image_bmp:
				return BMP_ImageIcon();
				
			default:
				break;
		}
		return BMP_FileIcon();
	}
	
	// Add the corresponding bitmap to the cache
	cachedIcon.first = iconUrl;
	cachedIcon.second = _imageFile( iconUrl , false ).readBitmap(); // Read Image from File
	
	return cachedIcon.second;
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