#include "_gadget/gadget.fileview.h"
#include "_type/type.system.h"

_fileview::_fileview( _length width , _length height , _coord x , _coord y , string path , _fileviewType viewtype ,
			_scrollType scrollTypeX , _scrollType scrollTypeY , bool singleClickToExecute , _style&& style ) :
	_scrollArea( width , height , x , y , scrollTypeX , scrollTypeY , (_style&&)style ) 
	, directory( path )
	, viewType( viewtype )
{
	// Set Real Type!
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
	
	int fileObjectHeight = _system::getUser()->fOH;
	int i = -fileObjectHeight;
	
	this->directory.rewindChildren();
	
	switch( this->viewType )
	{
		case _fileviewType::symbol_big:
		{
			_length curX = 1;
			_length curY = 1;
			// Read Children of directory
			for( string str; this->directory.readChild( str ) != false ; )
			{
				_fileobject* fo = new _fileobject( curX , curY , this->directory.getFileName() + str , this->viewType , singleClickToExecute );
				curX += fo->getWidth() + 2;
				if( fo->getDimensions().getX2() >= this->getWidth() )
				{
					curX = 0;
					curY += fo->getHeight() + 1;
					fo->moveTo( curX , curY );
				}
				this->addChild( fo );
			}
			break;
		}
		case _fileviewType::list:
		default:
			// Read Children of directory
			for( string str; this->directory.readChild( str ) != false ; )
				this->addChild( new _fileobject( 1 , ( i += fileObjectHeight + 1 ) , this->directory.getFileName() + str , this->viewType , singleClickToExecute ) );
			this->addChild( new _fileobject( 1 , ( i += fileObjectHeight + 1 ) , this->directory.getFileName() + "hello.txt" , this->viewType , singleClickToExecute ) );
			this->addChild( new _fileobject( 1 , ( i += fileObjectHeight + 1 ) , this->directory.getFileName() + "text.lnk" , this->viewType , singleClickToExecute ) );
			this->addChild( new _fileobject( 1 , ( i += fileObjectHeight + 1 ) , this->directory.getFileName() + "hello.txt" , this->viewType , singleClickToExecute ) );
			this->addChild( new _fileobject( 1 , ( i += fileObjectHeight + 1 ) , this->directory.getFileName() + "text.lnk" , this->viewType , singleClickToExecute ) );
			this->addChild( new _fileobject( 1 , ( i += fileObjectHeight + 1 ) , this->directory.getFileName() + "hello.txt" , this->viewType , singleClickToExecute ) );
			this->addChild( new _fileobject( 1 , ( i += fileObjectHeight + 1 ) , this->directory.getFileName() + "text.lnk" , this->viewType , singleClickToExecute ) );
			this->addChild( new _fileobject( 1 , ( i += fileObjectHeight + 1 ) , this->directory.getFileName() + "hello.txt" , this->viewType , singleClickToExecute ) );
			this->addChild( new _fileobject( 1 , ( i += fileObjectHeight + 1 ) , this->directory.getFileName() + "text.lnk" , this->viewType , singleClickToExecute ) );
			this->addChild( new _fileobject( 1 , ( i += fileObjectHeight + 1 ) , this->directory.getFileName() + "hello.txt" , this->viewType , singleClickToExecute ) );
			this->addChild( new _fileobject( 1 , ( i += fileObjectHeight + 1 ) , this->directory.getFileName() + "text.lnk" , this->viewType , singleClickToExecute ) );
			this->addChild( new _fileobject( 1 , ( i += fileObjectHeight + 1 ) , this->directory.getFileName() + "hello.txt" , this->viewType , singleClickToExecute ) );
			this->addChild( new _fileobject( 1 , ( i += fileObjectHeight + 1 ) , this->directory.getFileName() + "text.lnk" , this->viewType , singleClickToExecute ) );
			this->addChild( new _fileobject( 1 , ( i += fileObjectHeight + 1 ) , this->directory.getFileName() + "hello.txt" , this->viewType , singleClickToExecute ) );
			this->addChild( new _fileobject( 1 , ( i += fileObjectHeight + 1 ) , this->directory.getFileName() + "text.lnk" , this->viewType , singleClickToExecute ) );
			this->addChild( new _fileobject( 1 , ( i += fileObjectHeight + 1 ) , this->directory.getFileName() + "hello.txt" , this->viewType , singleClickToExecute ) );
			this->addChild( new _fileobject( 1 , ( i += fileObjectHeight + 1 ) , this->directory.getFileName() + "text.lnk" , this->viewType , singleClickToExecute ) );
			this->addChild( new _fileobject( 1 , ( i += fileObjectHeight + 1 ) , this->directory.getFileName() + "hello.txt" , this->viewType , singleClickToExecute ) );
			this->addChild( new _fileobject( 1 , ( i += fileObjectHeight + 1 ) , this->directory.getFileName() + "text.lnk" , this->viewType , singleClickToExecute ) );
			this->addChild( new _fileobject( 1 , ( i += fileObjectHeight + 1 ) , this->directory.getFileName() + "hello.txt" , this->viewType , singleClickToExecute ) );
			this->addChild( new _fileobject( 1 , ( i += fileObjectHeight + 1 ) , this->directory.getFileName() + "text.lnk" , this->viewType , singleClickToExecute ) );
			break;
	}
}


_fileview::~_fileview(){
	this->removeChildren( true );
}