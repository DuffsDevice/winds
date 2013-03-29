#include "_type/type.font.h"
#include "_type/type.memoryfont.h"
#include "_type/type.freetypefont.h"
#include "_type/type.direntry.h"

_font::_font( string newName ) 
	: name( newName )
{ }

_u16 _font::getStringWidth( string str , _u8 fontSize ) const {
	_u16 tempX = 0;
	for( const _char& ch : str )
		tempX += 1 + this->getCharacterWidth( ch , fontSize );
	return tempX;
}

string _font::getName() const {
	return this->name;
}

#include "_resource/FONT_ArialBlack13.h"
#include "_resource/FONT_CourierNew10.h"
#include "_resource/FONT_Tahoma7.h"

vector<_memoryfont*> builtInFonts =
{
	new FONT_ArialBlack13()
	, new FONT_CourierNew10()
	, new FONT_Tahoma7()
};

//! Receive a font, created from file
const _font* _font::fromFile( string path ) 
{
	_direntry d = _direntry( path );
	string fn = d.getFileName();
	
	// Convert to lowerspace
	transform( fn.begin() , fn.end() , fn.begin() , ::tolower );
	
	if( d.isExisting() )
		return new _freetypefont( path );
	
	// If not existing, look, if 
	if( fn == _direntry::replaceASSOCS( "%SYSTEM%/arialblack13.ttf" ) )
		return builtInFonts[0];
	if( fn == _direntry::replaceASSOCS( "%SYSTEM%/couriernew10.ttf" ) )
		return builtInFonts[1];
	if( fn == _direntry::replaceASSOCS( "%SYSTEM%/tahoma7.ttf" ) )
		return builtInFonts[2];
	
	return nullptr;
}