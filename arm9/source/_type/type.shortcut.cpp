//! Headers
#include "_type/type.direntry.shortcut.h"
#include "_type/type.system.h"

//! Graphics
#include "_resource/resource.image.shortcutoverlay.h"

_shortcut::_shortcut( string&& fn) :
	_direntry( move(fn) )
	, destination( nullptr )
{ }

const string& _shortcut::getDestination()
{
	if( this->destination )
		return *this->destination;
	
	_registry parser = _registry( this->readString() );
	
	this->destination = new string( parser.readIndex( "LocalShortcut" , "URL" ) );
	
	return *this->destination;
}

_bitmap _shortcut::getFileImage()
{
	static _u8 fOH = _system::getUser().fOH;
	
	if( this->image.isValid() )
		return this->image;
	
	//! Creaty my icon
	this->image = _bitmap( fOH , fOH );
	this->image.reset( NO_COLOR );
	
	_direntry fl = this->getDestination();
	
	if( fl.getFileName() != "" )
	{
		_constBitmap& icon = fl.getFileImage();
		this->image.copy( ( 10 - icon.getWidth() ) >> 1 , ( fOH - icon.getHeight() ) >> 1 , icon );
	}
	
	this->image.copy( 5 , 5 , BMP_ShortcutOverlay() );
	
	return this->image;
}

bool _shortcut::execute( _cmdArgs args ){
	return _direntry( this->getDestination() ).execute( move(args) );
}

_shortcut::~_shortcut()
{
	if( this->destination )
		delete this->destination;
}