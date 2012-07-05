#include "_gadget/gadget.fileview.h"
#include "_type/type.system.h"

#define FAT_EMULATORf


_fileview::_fileview( _length width , _length height , _coord x , _coord y , string path , _fileviewType viewtype , _scrollType scrollTypeX , _scrollType scrollTypeY , _gadgetStyle style ) :
	_scrollArea( width , height , x , y , scrollTypeX , scrollTypeY , style ) 
	, directory( path )
	, viewType( viewtype )
{
	// Reset Bitamp
	this->bitmap->reset( ( 1 << 16 ) - 1 );
	
	// Generate _fileobject's
	this->generateChildren();
	
	// Refresh...
	this->refreshBitmap();
}

void _fileview::setPath( const string& path ){
	this->removeChildren( true );
	this->directory = _direntry( path );
	this->generateChildren();
}

void _fileview::generateChildren()
{
	int i = -_system_->_runtimeAttributes_->user->getIntAttr( "fileObjectHeight" );
	
	this->directory.rewindChildren();
	
	// Read Children of directory
	for( string str; this->directory.readChild( str ) != false ; )
	{
		//printf( "new %s\n" , str.c_str() );
		this->addChild( new _fileobject( 1 , ( i += _system_->_runtimeAttributes_->user->getIntAttr( "fileObjectHeight" ) + 1 ) , str , this->viewType ) );
	}
}

_fileview::~_fileview(){
	this->removeChildren( true );
}