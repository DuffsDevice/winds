#include "_gadget/gadget.fileview.h"
#include "_type/type.system.h"

_fileview::_fileview( _length width , _length height , _coord x , _coord y , string path , _fileviewType viewtype ,
			_scrollType scrollTypeX , _scrollType scrollTypeY , bool singleClickToExecute , _style&& style ) :
	_scrollArea( width , height , x , y , scrollTypeX , scrollTypeY , (_style&&)style ) 
	, directory( path )
	, viewType( viewtype )
{
	// Set Real Type of gadget
	this->setType( _gadgetType::fileview );
	
	// Generate _fileobject's
	this->generateChildren( singleClickToExecute );
}


void _fileview::setPath( const string& path )
{
	this->directory = _direntry( path );
	this->generateChildren( false );
}


void _fileview::generateChildren( bool singleClickToExecute )
{
	this->removeChildren( true );
	
	this->directory.rewindChildren();
	
	switch( this->viewType )
	{
		case _fileviewType::symbol_big:
		{
			//_length curX = 1;
			//_length curY = 1;
			// Read Children of directory
			for( string str; this->directory.readChild( str ) != false ; )
			{
				// Allocate Fileobject
				_fileobject* fo = new _fileobject( ignore , ignore , ignore , ignore , this->directory.getFileName() + str , this->viewType , singleClickToExecute );
				
				auto cb = _gadgetHelpers::moveBesidePrecedent( _dimension::vertical , 28 , 2 );
				fo->setInternalEventHandler( onParentSet , cb );
				fo->setInternalEventHandler( onParentResize , cb );
				fo->setInternalEventHandler( onPreSet , cb );
				
				this->addChild( fo );
			}
			break;
		}
		case _fileviewType::list:
		default:
			// Read Children of directory
			//_vector<string> names = { "Haloo.txt" , "Resize.lnk" , "Halihalo.exe" };
			//for( string str : names )
			for( string str; this->directory.readChild( str ) != false ; )
			{
				// Allocate Fileobject
				_fileobject* fo = new _fileobject( ignore , ignore , ignore , ignore  , this->directory.getFileName() + str , this->viewType , singleClickToExecute );
				
				auto cb = _gadgetHelpers::moveBesidePrecedent( _dimension::vertical , 30 , 2 );
				fo->setInternalEventHandler( onParentSet , cb );
				//fo->setInternalEventHandler( onPreSet , cb );
				
				this->addChild( fo , true );
			}
			break;
	}
}

_fileview::~_fileview(){
	this->removeChildren( true );
}