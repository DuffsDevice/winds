#include "_type/type.font.h"
#include "_type/type.font.memory.h"
#include "_type/type.font.freetype.h"
#include "_type/type.direntry.h"

_length _font::getStringWidth( const _char* str , _u8 fontSize ) const
{
	if( !str || !*str )
		return 0;
	
	_u16 tempX = 0;
	_fontPtr font = this;
	
	do
	{
		_length width = font->getCharacterWidth( *str , fontSize );
		if( width )
			tempX += width + font->getLetterSpace();
		
	}while( *++str );
	
	return tempX;
}

#include "_resource/resource.font.arialblack.13.h"
#include "_resource/resource.font.couriernew.10.h"
#include "_resource/resource.font.system.7.h"
#include "_resource/resource.font.system.10.h"
#include "_resource/resource.font.systemsymbols.8.h"
#include "_resource/resource.font.handwriting.9.h"

const _vector<_memoryFont*> builtInFonts =
{
	new FONT_ArialBlack13()
	, new FONT_CourierNew10()
	, new FONT_System7()
	, new FONT_System10()
	, new FONT_SystemSymbols8()
	, new FONT_Handwriting9()
};

//! Receive a font, created from file
_fontPtr _font::fromFile( string path ) 
{
	_direntry d = _direntry( path );
	string fn = d.getFileName();
	
	// Convert to lowerspace
	transform( fn.begin() , fn.end() , fn.begin() , ::tolower );
	
	if( d.isExisting() )
		return new _freetypeFont( path );
	
	// If not existing, look, if 
	if( fn == _direntry::replaceASSOCS( "%SYSTEM%/arialblack13.ttf" ) )
		return builtInFonts[0];
	if( fn == _direntry::replaceASSOCS( "%SYSTEM%/couriernew10.ttf" ) )
		return builtInFonts[1];
	if( fn == _direntry::replaceASSOCS( "%SYSTEM%/system7.ttf" ) )
		return builtInFonts[2];
	if( fn == _direntry::replaceASSOCS( "%SYSTEM%/system10.ttf" ) )
		return builtInFonts[3];
	if( fn == _direntry::replaceASSOCS( "%SYSTEM%/systemsymbols8.ttf" ) )
		return builtInFonts[4];
	if( fn == _direntry::replaceASSOCS( "%SYSTEM%/handwriting9.ttf" ) )
		return builtInFonts[5];
	
	return nullptr;
}