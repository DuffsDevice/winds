#include "_gadget/gadget.fileview.h"
#include "_type/type.system.h"

_fileView::_fileView( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string path , _fileViewType viewtype ,
			_scrollType scrollTypeX , _scrollType scrollTypeY , bool singleClickToExecute , _style&& style ) :
	_scrollArea( x , y , width , height , scrollTypeX , scrollTypeY , (_style&&)style )
	, directory( path )
	, viewType( viewtype )
{	
	// Set Real Type of gadget
	this->setType( _gadgetType::fileview );
	
	// Generate _fileObject's
	this->generateChildren( singleClickToExecute );
}


void _fileView::setPath( const string& path )
{
	this->directory = _direntry( path );
	this->generateChildren( false );
}


void _fileView::generateChildren( bool singleClickToExecute )
{
	this->removeChildren( true );
	
	this->directory.rewindChildren();
	
	switch( this->viewType )
	{
		case _fileViewType::symbol_big:
		{
			//_length curX = 1;
			//_length curY = 1;
			// Read Children of directory
			for( string str; this->directory.readChild( str ) != false ; )
			{
				// Allocate Fileobject
				_fileObject* fo = new _fileObject( ignore , ignore , ignore , ignore , this->directory.getFileName() + str , this->viewType , singleClickToExecute );
				
				auto cb = _gadgetHelpers::moveBesidePrecedent( _dimension::vertical , 28 , 2 );
				fo->setInternalEventHandler( onParentSet , cb );
				fo->setInternalEventHandler( onParentResize , cb );
				fo->setInternalEventHandler( onPreSet , cb );
				
				this->addChild( fo );
			}
			break;
		}
		case _fileViewType::list:
		default:
			// Read Children of directory
			//_vector<string> names = { "Haloo.txt" , "Resize.lnk" , "Halihalo.exe" };
			//for( string str : names )
			for( string str; this->directory.readChild( str ) != false ; )
			{
				// Allocate Fileobject
				_fileObject* fo = new _fileObject( ignore , ignore , ignore , ignore  , this->directory.getFileName() + str , this->viewType , singleClickToExecute );
				
				auto cb = _gadgetHelpers::moveBesidePrecedent( _dimension::vertical , 30 , 2 );
				fo->setInternalEventHandler( onParentSet , cb );
				//fo->setInternalEventHandler( onPreSet , cb );
				
				this->addChild( fo , true );
			}
			break;
	}
}

_fileView::~_fileView(){
	this->removeChildren( true );
}