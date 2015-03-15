#include <_controller/controller.registry.h>
#include <_controller/controller.filesystem.h>
#include <_dialog/dialog.runtimeerror.h>

#include <_type/type.time.h>
#include <_type/type.user.guest.h>
#include <_type/type.windows.h>

//! File-Images
#include <_resource/resource.icon.file.h>
#include <_resource/resource.icon.exe.h>
#include <_resource/resource.icon.txt.h>
#include <_resource/resource.icon.xml.h>
#include <_resource/resource.icon.nds.h>
#include <_resource/resource.icon.gba.h>
#include <_resource/resource.icon.ini.h>
#include <_resource/resource.icon.lua.h>
#include <_resource/resource.icon.folder.h>
#include <_resource/resource.icon.font.h>
#include <_resource/resource.icon.music.h>
#include <_resource/resource.icon.jpg.h>
#include <_resource/resource.icon.image.h>
#include <_resource/resource.icon.registry.h>
#include <_resource/resource.icon.archive.h>

bool _registryController::init()
{
	// Open registries
	fileTypeRegistry	= new _iniFile("%SYSTEM%/filetypes.reg");
	systemRegistry		= new _iniFile("%SYSTEM%/registry.reg");
	setUserRegistry( new _guestUser() );
	
	// Prepare them for the first use (if needed)
	if( fileTypeRegistry->isUsedFirstTime() )
		_registryController::prepareFileTypeRegistry();
	if( systemRegistry->isUsedFirstTime() )
		_registryController::prepareSystemRegistry();
	
	// Make sure they exists
	fileTypeRegistry->create();
	systemRegistry->create();
	
	// Fill Cache-Parameters
	safeBootMode		= systemRegistry->readIndexInt( "bootParams" , "safeBootMode" );
	correctShutDown		= systemRegistry->readIndexInt( "bootParams" , "correctShutDown" );
	systemStartTime		= _time::now();
	
	// Act, as if the system has not been shut down safely. If it did, that attribute will be changed after the successful shutdown
	systemRegistry->writeIndex( "bootParams" , "correctShutDown" , "0" );
	
	return true;
}

void _registryController::prepareFileTypeRegistry()
{
	fileTypeRegistry->setMap( {{
		{ "fileMapper" , {{
			{ "jpg"	, "%WINDIR%/accessories/paint.exe -\"$F\"" } ,
			{ "png"	, "%WINDIR%/accessories/paint.exe -\"$F\"" } ,
			{ "bmp"	, "%WINDIR%/accessories/paint.exe -\"$F\"" } ,
			{ "gif"	, "%WINDIR%/accessories/paint.exe -\"$F\"" } ,
			{ "ico"	, "%WINDIR%/accessories/paint.exe -\"$F\"" } ,
			{ "nds"	, "%SYSTEM%/rom_exec.exe -\"$F\"" } ,
			{ "*"	, "%SYSTEM%/progmapper.exe -\"$F\"" }
		}} }
		, { "fileIcon" , {{
			//{ "%dir%" , "" }
		}} }
	}} );
	fileTypeRegistry->flush();
}

void _registryController::prepareSystemRegistry()
{
	systemRegistry->setMap( {{
		{ "bootParams" , {{
			{ "safeBootMode" , "0" } ,
			{ "correctShutDown" , "1" } ,
			{ "timeOnline" , "0" } ,
		}} }
		, { "localization" , {{
			{ "language" , _windows::getDSLanguage().c_str() }
		}} }
		, { "gui" , {{
			{ "minDragDistance" , "9" } ,
			{ "maxClickPeriod" , "30" } ,
			{ "keyRepetitionDelay" , "30" } ,
			{ "keyRepetitionPause" , "6" } ,
			{ "maxDoubleClickPeriod" , "60" } ,
			{ "maxDoubleClickArea" , "6" } ,
			{ "rightClickDelay" , "45" } ,
			
			{ "fileObjectHeight" , "10" } ,
			{ "listItemHeight" , "9" } ,
			{ "counterHeight" , "16" } ,
			{ "selectHeight" , "10" } ,
			
			{ "startButtonTextColor" , "RGB(30,30,30)" } ,
			{ "fileExtensionVisible" , "1" } ,

		}} }
		, { "serviceMapper" , {{
		}} }
	}} );
	systemRegistry->flush();
}


void _registryController::indicateSafeShutdown(){
	systemRegistry->writeIndex( "bootParams" , "correctShutDown" , "1" );
}


void _registryController::end()
{
	// Delete currently logged in user
	setUserRegistry( nullptr );
	
	// Update time that windows ran
	_unixTime timeOnline = getSystemRegistry().readIndexInt( "bootParams" , "timeOnline" );
	timeOnline += _time::now().toUnixTime() - systemStartTime;						// Add difference between system end and system start
	getSystemRegistry().writeIndex( "bootParams" , "timeOnline" , int2string(timeOnline) );	// Write new time
	
	// Remove the safe boot flag
	systemRegistry->writeIndex( "bootParams" , "safeBootMode" , "0" );
	
	// Delete Registries
	systemRegistry = nullptr; // Flush the system registry
	fileTypeRegistry = nullptr; // Flush the filetype registry
	
	// Indicate that the computer was successfully shut down
	indicateSafeShutdown();
}

string	_registryController::getPackagePath( const string& package ){
	return fileTypeRegistry->readIndex( "serviceMapper" , package ).cpp_str();
}

void	_registryController::setPackagePath( const string& package , string path ){
	fileTypeRegistry->writeIndex( "serviceMapper" , package , move(path) );
}

string _registryController::getFileTypeHandler( const string& extension )
{
	string index = fileTypeRegistry->readIndex( "fileMapper" , extension ).cpp_str();
	
	// Read mapper that can read all types of files
	if( index.empty() )
		return fileTypeRegistry->readIndex( "fileMapper" , "*" ).cpp_str();
	
	return move(index);
}

_bitmap _registryController::getFileTypeImage( const string& extension , _mimeType mimeType )
{
	// Fetch Url of the Icon

	const string& iconUrl = fileTypeRegistry->readIndex( "fileIcon" , extension ).cpp_str();
	
	// If we already loaded the icon corresponding to a specific extension
	_pair<string,_bitmap>& cachedIcon = _registryController::extension2UrlAndimage[extension];
	
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
			
			case _mime::application_x_ms_registry:
				return BMP_RegistryIcon();
				
			case _mime::application_x_nintendo_ds_rom:
				return BMP_NdsIcon();
				
			case _mime::application_x_nintendo_gba_rom:
				return BMP_GbaIcon();
			
			case _mime::application_zip:
				return BMP_ArchiveIcon();
			
			case _mime::font_opentype:
			case _mime::font_truetype:
				return BMP_FontIcon();
			
			case _mime::audio_mpeg:
			case _mime::audio_wav:
			case _mime::audio_x_aiff:
			case _mime::audio_x_midi:
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

void _registryController::setFileTypeHandler( const string& extension , const string& command ){
	fileTypeRegistry->writeIndex( "fileMapper" , extension , command );
}

void _registryController::setUserRegistry( _uniquePtr<_user> newUser ){
	userRegistry = move(newUser);
	_filesystemController::setCurrentUserDir( userRegistry ? userRegistry->getHomeFolder() : "" );
}

_uniquePtr<_iniFile>				_registryController::systemRegistry;
_uniquePtr<_user>					_registryController::userRegistry;
_uniquePtr<_iniFile>				_registryController::fileTypeRegistry;
bool								_registryController::correctShutDown;
bool								_registryController::safeBootMode;
_unixTime							_registryController::systemStartTime;
_map<string,_pair<string,_bitmap>>	_registryController::extension2UrlAndimage;