//! Headers
#include "_type/type.shortcut.h"
#include "_type/type.system.h"

//! Graphics
#include "_resource/BMP_ShortcutOverlay.h"

_bitmap icon_shortcut = BMP_ShortcutOverlay();

_shortcut::_shortcut( string fn) : _direntry( fn ) , destination( nullptr )
{ }

_direntry _shortcut::getDestination()
{
	if( this->destination.getFileName() != "" )
		return this->destination;
	
	_ini parser = _ini( this->readString() );
	
	if( parser.read() )
		this->destination = _direntry( parser.getMap().at( "LocalShortcut" ).at( "URL" ) );
	
	return this->destination;
}

_bitmap _shortcut::getFileImage()
{
	static _u8 fOH = _system::getUser()->fOH;
	
	if( this->image.isValid() )
		return this->image;
	
	//! Creaty my icon
	this->image = _bitmap( fOH , fOH );
	this->image.reset( NO_COLOR );
	
	_direntry fl = this->getDestination();
	
	if( fl.getFileName() != "" )
	{
		_bitmap icon = fl.getFileImage();
		this->image.copy( ( 10 - icon.getWidth() ) >> 1 , ( fOH - icon.getHeight() ) >> 1 , icon );
	}
	
	this->image.copy( 5 , 5 , icon_shortcut );
	
	return  this->image;
}

const string _shortcut::getExtension() const {
	return "";
}