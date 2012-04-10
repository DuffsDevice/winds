#include "_file/file.shortcut.h"
#include "_type/type.system.h"

#include "_graphic/BMP_ShortcutOverlay.h"
_bitmap* icon_shortcut = new BMP_ShortcutOverlay();

_shortcut::_shortcut( string fn) : _file( fn ) , destination( nullptr ) , image( nullptr )
{ }

_shortcut::~_shortcut(){
	if( this->destination )
		delete this->destination;
	if( this->image )
		delete this->image;
}

_file* _shortcut::getDestination(){
	if( this->destination )
		return this->destination;
	_ini parser( this->readString() );
	if( parser.parse() )
		this->destination = new _file(parser.getMap()["LocalShortcut:URL"]);
	return this->destination;
}

const _bitmap* _shortcut::getFileImage(){
	if( this->image )
		return this->image;
	
	//! Creaty my icon
	this->image = new _bitmap( _system_->_runtimeAttributes_->fileObjectHeight , _system_->_runtimeAttributes_->fileObjectHeight );
	this->image->reset( NO_COLOR );
	
	_file* fl = this->getDestination();
	if( fl ){
		const _bitmap* icon = fl->getFileImage();
		this->image->copy( 5 - ( icon->getWidth() >> 1 ) , ( _system_->_runtimeAttributes_->fileObjectHeight >> 1 ) - ( icon->getHeight() >> 1 ) , icon );
	}
	this->image->copy( 5 , 5 , icon_shortcut );
	return  this->image;
}

const string _shortcut::getExtension() const {
	return "";
}