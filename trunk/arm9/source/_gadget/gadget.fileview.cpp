#include "_gadget/gadget.fileview.h"
#include "_type/type.system.h"

_fileView::_fileView( _optValue<_coord> x , _optValue<_coord> y
		, _optValue<_length> width , _optValue<_length> height , string path
		, _fileViewType viewType , _fileExtensionList allowedExtensions
		, _callback<_eventHandler>* eventHandler , bool singleClickToExecute , _style&& style
	) :
	_scrollArea( x , y , width , height , _scrollType::meta , _scrollType::meta , (_style&&)style )
	, directory( path )
	, viewType( viewType )
	, filemask( allowedExtensions.empty() ? nullptr : new _fileExtensionList( move(allowedExtensions) ) )
	, eventHandler( move(eventHandler) )
	, singleClickToExecute( singleClickToExecute ) 
{	
	// Set Real Type of gadget
	this->setType( _gadgetType::fileview );
	
	// Generate _fileObject's
	this->generateChildren();
}


void _fileView::setPath( const string& path )
{
	this->directory = _direntry( path );
	this->generateChildren();
}

_callbackReturn _fileView::eventForwarder( _event event )
{
	_fileView* fileView = (_fileView*)event.getGadget()->getParent();
	
	if( !fileView )
		return use_internal;
	
	// Use custom Handler
	if( fileView->eventHandler )
		return (*fileView->eventHandler)( move(event) );
	// Use default handler
	else if( ( event == _eventType::onMouseDblClick ) != fileView->singleClickToExecute )
		event.getGadget<_fileObject>()->execute();
	
	return use_internal;
}

void _fileView::generateChildren()
{
	// Remove all old children
	this->removeChildren( true );
	
	// Rewind the directory
	this->directory.rewindChildren();
	
	// Clear the filemask vector if it is empty
	if( this->filemask && this->filemask.data()->empty() )
		this->filemask = nullptr;
	
	switch( this->viewType )
	{
		case _fileViewType::symbol_big:
		{
			// Read Children of directory
			for( _literal str ; this->directory.readChild( str , this->filemask ) != false ; )
			{
				// Allocate Fileobject
				_fileObject* fo = new _fileObject( ignore , ignore , ignore , ignore , this->directory.getFileName() + str , this->viewType );
				
				auto cb = _gadgetHelpers::moveBesidePrecedent( _direction::right , 28 , 2 , false , 1 , 1 , false );
				fo->setInternalEventHandler( onParentSet , cb );
				
				// Add User-defined Handler
				fo->setUserEventHandler( onMouseClick , make_callback( &_fileView::eventForwarder ) );
				fo->setUserEventHandler( onMouseDblClick , make_callback( &_fileView::eventForwarder ) );
				
				this->addChild( fo );
			}
			break;
		}
		case _fileViewType::list:
		default:
			// Read Children of directory
			//_vector<string> names = { "Haloo.lnk" , "Resize.bmp" , "Halihalo.exe" , "Hallo/"};
			//for( string str : names )
			for( _literal str ; this->directory.readChild( str , this->filemask ) != false ; )
			{
				// Allocate Fileobject
				_fileObject* fo = new _fileObject( ignore , ignore , ignore , ignore  , this->directory.getFileName() + str , this->viewType );
				
				auto cb = _gadgetHelpers::moveBesidePrecedent( _direction::down , 30 , 2 , false , 1 , 1 , false );
				fo->setInternalEventHandler( onParentSet , cb );
				
				// Add User-defined Handler
				fo->setUserEventHandler( onMouseClick , make_callback( &_fileView::eventForwarder ) );
				fo->setUserEventHandler( onMouseDblClick , make_callback( &_fileView::eventForwarder ) );
				
				this->addChild( fo , true );
			}
			break;
	}
}

_fileView::~_fileView(){
	this->removeChildren( true );
}