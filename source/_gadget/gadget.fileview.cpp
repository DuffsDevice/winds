#include "_gadget/gadget.fileview.h"
#include "_type/type.system.h"

#define FAT_EMULATORd


_fileview::_fileview( _length width , _length height , _coord x , _coord y , _directory* dir , _fileviewType viewtype , _gadgetStyle style ) :
	_gadget( fileview , width , height , x , y , style ) , directory( dir ) , viewType( viewtype )
{
	// Reset Bitamp
	this->bitmap->reset( ( 1 << 16 ) - 1 );
	
	// Read Children of directory
	#ifndef FAT_EMULATOR
	dir->readChildren();
	_directory* d = dir;
	#else
	deque<_file*>* d = new deque<_file*>{ new _file("Textdatei.txt") , new _file("Fat:/XML-File.xml") , new _file("Fat:/Executable_file.exe") , new _directory("Fat:/Unknown") };
	#endif
	
	int i = -_system_->_runtimeAttributes_->fileObjectHeight;
	
	for( _file* file : *d )
		this->addChild( new _fileobject( 1 , ( i += _system_->_runtimeAttributes_->fileObjectHeight + 1 ) , file , this->viewType ) );
	
	// Refresh...
	this->refreshBitmap();
}