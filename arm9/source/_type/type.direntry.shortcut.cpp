#include <_type/type.direntry.shortcut.h>
#include <_controller/controller.gui.h>

// Shortcut-Icon
#include <_resource/resource.image.shortcutoverlay.h>

_shortcut::_shortcut( string&& fn) :
	_direntry( move(fn) )
	, destination( nullptr )
	, fallbackArgs( nullptr )
	, standardArgs( nullptr )
{}

void _shortcut::readAttributes() const 
{
	if( !this->destination )
	{
		_ini parser = _ini( this->readString() );
		
		// Read Destination
		this->destination = new string( parser.readIndex( "LocalShortcut" , "URL" ) );
		
		// Read Fallback Arguments
		const string& fallbackArgsString = parser.readIndex( "LocalShortcut" , "FallbackArgs" );
		if( !fallbackArgsString.empty() )
			this->fallbackArgs = new _args( fallbackArgsString.c_str() );
		
		// Read standard arguments
		const string& standardArgsString = parser.readIndex( "LocalShortcut" , "StandardArgs" );
		if( !standardArgsString.empty() )
			this->standardArgs = new _args( standardArgsString.c_str() );
	}
}

_bitmap _shortcut::getFileImage() const 
{
	_u8 fOH = _guiController::getFileObjectHeight();
	
	if( this->image.isValid() )
		return this->image;
	
	//! Creaty my icon
	this->image = _bitmap( fOH , fOH );
	this->image.reset( _color::transparent );
	
	_direntry fl = this->getDestination();
	
	if( fl.getFileName() != "" ){
		_constBitmap& icon = fl.getFileImage();
		this->image.copy( ( fOH - icon.getWidth() ) >> 1 , ( fOH - icon.getHeight() ) >> 1 , icon );
	}
	
	this->image.copy( 0 , 5 , BMP_ShortcutOverlay() );
	
	return this->image;
}

bool _shortcut::execute( _args args )
{
	const string& dest = this->getDestination();
	
	// If args are empty, replace with fallback args
	if( args.empty() && this->fallbackArgs )
		args = *this->fallbackArgs;
	
	// Prepend standard arguments if they are available
	if( this->standardArgs )
		args.insert( args.begin() , this->standardArgs->begin() , this->standardArgs->end() );
	
	return _direntry(dest).execute( move(args) );
}

_shortcut::~_shortcut(){
	if( this->destination )
		delete this->destination;
}