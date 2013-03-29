#ifdef sdg
#include "_type/type.direntry.builtinContent.h"
#include "_type/type.progLua.h"
#include "_type/type.progC.h"


//! Content that is built-in
#include "program_bin.h"
#include "strings_bin.h"
#include "_resource/PROG_Explorer.h"
#include "_resource/FONT_ArialBlack13.h"
#include "_resource/FONT_CourierNew10.h"
#include "_resource/FONT_Tahoma7.h"

#include "_resource/BMP_FileIcon.h"
#include "_resource/BMP_ExeIcon.h"
#include "_resource/BMP_TxtIcon.h"
#include "_resource/BMP_XmlIcon.h"
#include "_resource/BMP_NdsIcon.h"
#include "_resource/BMP_GbaIcon.h"
#include "_resource/BMP_IniIcon.h"
#include "_resource/BMP_LuaIcon.h"
#include "_resource/BMP_FolderIcon.h"
#include "_resource/BMP_FontIcon.h"
#include "_resource/BMP_MusicIcon.h"
#include "_resource/BMP_JpegIcon.h"
#include "_resource/BMP_ImageIcon.h"

//! Other bitmaps
#include "_resource/BMP_WindowHeader.h"
#include "_resource/BMP_ShortcutOverlay.h"
#include "_resource/BMP_Checkboxes.h"
#include "_resource/BMP_DefaultUserIcon.h"
#include "_resource/BMP_WindowsWallpaper.h"


//! Method to resolve identifier from a given filepath
const string builtinPath = "/windows/system32/";

string path2identifier( string path )
{
	// Convert path to lowercase
	transform( path.begin() , path.end() , path.begin() , ::tolower );
	
	if( path.substr( 0 , 17 ) == builtinPath )
		return path.substr( 17 );
	
	return "";
}


_program* getBuiltIn::program( string path )
{
	path = path2identifier( path );
	if( path.empty() )
		return nullptr;
	
	if( path == "explorer.exe" )
		return new PROG_Explorer();
	if( path == "sampleprogram.exe" )
		return new _progLua( (const char*)program_bin );
	
	return nullptr;
}

map<string,_bitmap*> builtInBitmaps = 
{
	{ "files/file.bmp" , new BMP_FileIcon() }
	, { "files/exe.bmp" , new BMP_ExeIcon() }
	, { "files/txt.bmp" , new BMP_TxtIcon() }
	, { "files/xml.bmp" , new BMP_XmlIcon() }
	, { "files/nds.bmp" , new BMP_NdsIcon() }
	, { "files/gba.bmp" , new BMP_GbaIcon() }
	, { "files/ini.bmp" , new BMP_IniIcon() }
	, { "files/lua.bmp" , new BMP_LuaIcon() }
	, { "files/folder.bmp" , new BMP_FolderIcon() }
	, { "files/font.bmp" , new BMP_FontIcon() }
	, { "files/music.bmp" , new BMP_MusicIcon() }
	, { "files/jpeg.bmp" , new BMP_JpegIcon() }
	, { "files/image.bmp" , new BMP_ImageIcon() }
	, { "ui/windowheader.bmp" , new BMP_WindowHeader() }
	, { "ui/windowheaderblurred.bmp" , new BMP_WindowHeaderBlurred() }
	, { "ui/checkboxChecked.bmp" , new BMP_CheckboxChecked() }
	, { "ui/checkboxSemi.bmp" , new BMP_CheckboxSemi() }
	, { "ui/checkboxUnchecked.bmp" , new BMP_CheckboxUnchecked() }
	, { "ui/shortcutoverlay.bmp" , new BMP_ShortcutOverlay() }
	, { "logos/default.bmp" , new BMP_DefaultUserIcon() }
	, { "wallpaper.bmp" , new BMP_WindowsWallpaper() }
};




const _bitmap* getBuiltIn::bitmap( string path )
{
	path = path2identifier( path );
	if( path.empty() )
		return nullptr;
	
	return builtInBitmaps[ path ];
}

const char* getBuiltIn::binary( string path )
{
	path = path2identifier( path );
	if( path.empty() )
		return nullptr;
	
	if( path == "localizedstrings.ini" )
		return (const char*)strings_bin;
	
	return nullptr;
}

const _memoryfont* getBuiltIn::font( string path )
{
	path = path2identifier( path );
	if( path.empty() )
		return nullptr;
	
	return builtInFonts[ path ];
}
#endif