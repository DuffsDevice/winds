#include "_gadget/gadget.fileview.h"
#include "_type/type.system.h"

#define FAT_EMULATORf


_fileview::_fileview( _length width , _length height , _coord x , _coord y , string path , _fileviewType viewtype , _scrollType scrollTypeX , _scrollType scrollTypeY , _style style ) :
	_scrollArea( width , height , x , y , scrollTypeX , scrollTypeY , style ) 
	, directory( path )
	, viewType( viewtype )
{
	// Set Real Type!
	this->setType( _gadgetType::fileview );
	
	// Generate _fileobject's
	this->generateChildren();
	
	// Refresh...
	this->refreshBitmap();
}


void _fileview::setPath( const string& path )
{
	this->directory = _direntry( path );
	this->generateChildren();
	
	// Trigger 'onChange'-Event
	this->triggerEvent( onChange );
}


void _fileview::generateChildren()
{
	this->removeChildren( true );
	
	int fileObjectHeight = _system::_runtimeAttributes_->user->fOH;
	int i = -fileObjectHeight;
	
	this->directory.rewindChildren();
	
	// Read Children of directory
	for( string str; this->directory.readChild( str ) != false ; )
		this->addChild( new _fileobject( 1 , ( i += fileObjectHeight + 1 ) , this->directory.getFileName() + str , this->viewType ) );
	
	//this->addChild( new _fileobject( 1 , ( i += fileObjectHeight + 1 ) , "Helloooo.lua" , this->viewType ) );
}


_fileview::~_fileview(){
	this->removeChildren( true );
}