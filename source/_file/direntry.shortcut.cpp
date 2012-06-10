#include "_file/direntry.shortcut.h"
#include "_type/type.system.h"

#include "_graphic/BMP_ShortcutOverlay.h"
_bitmap* icon_shortcut = new BMP_ShortcutOverlay();

_shortcut::_shortcut( string fn) : _direntry( fn ) , destination( nullptr ) , image( nullptr )
{ }

_shortcut::~_shortcut(){
	if( this->image )
		delete this->image;
}

_direntry _shortcut::getDestination(){
	if( this->destination.getFileName() != "" )
		return this->destination;
	_ini parser( this->readString() );
	if( parser.read() )
		this->destination = _direntry(parser.getMap()["LocalShortcut"]["URL"]);
	return this->destination;
}

const _bitmap* _shortcut::getFileImage(){
	if( this->image )
		return this->image;
	
	//! Creaty my icon
	this->image = new _bitmap( _system_->_runtimeAttributes_->fileObjectHeight , _system_->_runtimeAttributes_->fileObjectHeight );
	this->image->reset( NO_COLOR );
	
	_direntry fl = this->getDestination();
	if( fl.getFileName() != "" )
	{
		const _bitmap* icon = fl.getFileImage();
		this->image->copy( 5 - ( icon->getWidth() >> 1 ) , ( _system_->_runtimeAttributes_->fileObjectHeight >> 1 ) - ( icon->getHeight() >> 1 ) , icon );
	}
	this->image->copy( 5 , 5 , icon_shortcut );
	return  this->image;
}

const string _shortcut::getExtension() const {
	return "";
}