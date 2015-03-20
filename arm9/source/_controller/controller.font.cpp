#include <_controller/controller.font.h>
#include <_controller/controller.registry.h>
#include <_controller/controller.debug.h>
#include <_controller/controller.filesystem.h>
#include <_type/type.winds.h>

bool _fontController::init()
{
	//! Restore the default list of fonts
	registerFont( _font::fromFile("%SYSTEM%/systemascii7.ttf") );
	registerFont( _font::fromFile("%SYSTEM%/systemsymbols8.ttf") );
	registerFont( _font::fromFile("%SYSTEM%/system7.ttf") );
	registerFont( _font::fromFile("%SYSTEM%/system10.ttf") );
	registerFont( _font::fromFile("%SYSTEM%/arialblack13.ttf") );
	registerFont( _font::fromFile("%SYSTEM%/couriernew10.ttf") );
	registerFont( _font::fromFile("%SYSTEM%/handwriting9.ttf") );
	
	// Reload all fonts
	const auto& fonts = _registryController::getSystemRegistry().readSection("registeredFonts");
	for( const auto& font : fonts ) // Type must at least be a std::pair
		registerFont( _font::fromFile( font.second.cpp_str() ) );
	
	// Load standard Font and Size
	standardFont = nullptr;
	standardFont = _fontController::getFont( _registryController::getSystemRegistry().readIndex("appearence","standardFont").cpp_str() );
	standardFontSize = _registryController::getSystemRegistry().readIndexInt("appearence","standardFontSize");
	
	if( standardFont )
		return true;
	
	standardFont = _fontController::getFont("System7");
	
	return standardFont != nullptr;
}

void _fontController::end()
{
	_registryController::getSystemRegistry().writeIndex("appearence","standardFontSize",int2string(standardFontSize));
	_registryController::getSystemRegistry().writeIndex("appearence","standardFont",standardFont->getName());
	
	// Free registered fonts
	_fontController::registeredFonts.clear();
}

void _fontController::deleteFont( _fontPtr font )
{
	if( !font || font == _fontController::getStandardFont() )
		return;
	
	auto iter = std::find( _fontController::registeredFonts.begin() , _fontController::registeredFonts.end() , font );
	
	if( iter == _fontController::registeredFonts.end() )
		return;
	
	_fontController::registeredFonts.erase( iter );
}

_list<_fontHandle> _fontController::getRegisteredFontsAsHandles(){
	_list<_fontHandle> fonts;
	for( const _uniquePtr<_font>& ptr : _fontController::registeredFonts )
		fonts.emplace_back( ptr.get() );
	return fonts;
}

void _fontController::registerFont( _uniquePtr<_font> font ){
	if( font && !_fontController::isExistent( font->getName() ) ){ // Ensure font is valid and not already in list
		registeredFonts.push_back( {} );
		registeredFonts.back() = move(font);
	}
}

_fontPtr _fontController::getFont( _literal fontName )
{
	for( const _uniquePtr<_font>& ft : _fontController::registeredFonts )
		if( ft->getName() == fontName )
			return ft;
	return nullptr;
}

void _fontController::setStandardFont( _fontPtr font ){
	if( font && isExistent(font) )
		_fontController::standardFont = font;
}

bool _fontController::isExistent( _fontPtr font ){
	for( const _uniquePtr<_font>& ft : _fontController::registeredFonts )
		if( ft == font )
			return true;
	return false;
}

bool _fontController::isExistent( const string& fontName ){
	for( const _uniquePtr<_font>& ft : _fontController::registeredFonts )
		if( ft->getName() == fontName )
			return true;
	return false;
}

_fontList	_fontController::registeredFonts;
_fontPtr	_fontController::standardFont;
_fontSize	_fontController::standardFontSize;