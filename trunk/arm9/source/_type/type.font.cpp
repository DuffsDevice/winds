#include <_type/type.font.h>
#include <_type/type.font.memory.h>
#include <_type/type.font.freetype.h>
#include <_type/type.direntry.h>
#include <_controller/controller.filesystem.h>

_length _font::getStringWidth( _literal str , _fontSize fontSize ) const
{
	if( !str || !*str )
		return 0;
	
	_u16 tempX = 0;
	_fontHandle font = this;
	
	do
	{
		_length width = font->getCharacterWidth( *str , fontSize );
		if( width )
			tempX += width + font->getLetterSpace();
		
	}while( *++str );
	
	return tempX;
}

_length _font::getStringWidth( _wliteral str , _fontSize fontSize ) const
{
	if( !str || !*str )
		return 0;
	
	_u16 tempX = 0;
	_fontHandle font = this;
	
	do
	{
		_length width = font->getCharacterWidth( *str , fontSize );
		if( width )
			tempX += width + font->getLetterSpace();
		
	}while( *++str );
	
	return tempX;
}

_length _font::getStringWidth( const wstring& str , _fontSize fontSize ) const
{
	if( str.empty() )
		return 0;
	
	_u16 tempX = 0;
	_fontHandle font = this;
	
	for( _wchar ch : str )
	{
		_length width = font->getCharacterWidth( ch , fontSize );
		if( width )
			tempX += width + font->getLetterSpace();
		
	}
	
	return tempX;
}

#include <_resource/resource.font.arialblack.13.h>
#include <_resource/resource.font.couriernew.10.h>
#include <_resource/resource.font.system.ascii.7.h>
#include <_resource/resource.font.system.symbols.8.h>
#include <_resource/resource.font.system.7.h>
#include <_resource/resource.font.system.10.h>
#include <_resource/resource.font.handwriting.9.h>

//! Receive a font, created from file
_uniquePtr<_font> _font::fromFile( const string& path ) 
{
	_direntry d = _direntry( path );
	string fn = d.getFileName();
	
	// Convert to lowerspace
	transform( fn.begin() , fn.end() , fn.begin() , ::tolower );
	
	if( d.isExisting() )
		return new _freetypeFont( path );
	
	// If not existing, look, if 
	if( fn == _filesystemController::replaceAssocDirs( "%SYSTEM%/arialblack13.ttf" ) )
		return new FONT_ArialBlack13();
	if( fn == _filesystemController::replaceAssocDirs( "%SYSTEM%/couriernew10.ttf" ) )
		return new FONT_CourierNew10();
	if( fn == _filesystemController::replaceAssocDirs( "%SYSTEM%/systemascii7.ttf" ) )
		return new FONT_SystemASCII7();
	if( fn == _filesystemController::replaceAssocDirs( "%SYSTEM%/system7.ttf" ) )
		return new FONT_System7();
	if( fn == _filesystemController::replaceAssocDirs( "%SYSTEM%/system10.ttf" ) )
		return new FONT_System10();
	if( fn == _filesystemController::replaceAssocDirs( "%SYSTEM%/systemsymbols8.ttf" ) )
		return new FONT_SystemSymbols8();
	if( fn == _filesystemController::replaceAssocDirs( "%SYSTEM%/handwriting9.ttf" ) )
		return new FONT_Handwriting9();
	
	return nullptr;
}