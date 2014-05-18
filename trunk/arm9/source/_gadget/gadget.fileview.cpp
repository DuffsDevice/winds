#include "_gadget/gadget.fileview.h"
#include "_type/type.gadget.helpers.h"
#include "_type/type.windows.soundbank.h"

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
	
	_windowsSoundBank::play( _windowsSound::navigation );
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
	if( this->filemask && this->filemask->empty() )
		this->filemask = nullptr;
	
	switch( this->viewType )
	{
		case _fileViewType::symbol:
		{
			auto cb = _gadgetHelpers::moveBesidePrecedent( _direction::right , 4 , 4 , true , 1 , 1 , false );
			
			// Read Children of directory
			//for( string str : { "Haloo.lnk" , "2013-10-05 16.55.56.jpg" , "Halihalo.exe" , "Hallo/"} )
			for( _literal str ; this->directory.readChild( str , this->filemask ) != false ; )
			{
				// Allocate Fileobject
				_fileObject* fo = new _fileObject( ignore , ignore , ignore , ignore , this->directory.getFileName() + str , this->viewType );
				
				fo->setInternalEventHandler( onParentAdd , cb );
				
				// Add User-defined Handler
				fo->setUserEventHandler( onMouseClick , make_callback( &_fileView::eventForwarder ) );
				fo->setUserEventHandler( onMouseDblClick , make_callback( &_fileView::eventForwarder ) );
				
				this->addChild( fo , true );
			}
			break;
		}
		case _fileViewType::list:
		default:
		{
			auto cb = _gadgetHelpers::moveBesidePrecedent( _direction::down , 30 , 2 , false , 1 , 1 , false );
			
			// Read Children of directory
			//for( string str : { "Haloo.drp" , "2013-10-05 16.55.56.jpg" , "Halihalo.exe" , "Hallo/"} )
			for( _literal str ; this->directory.readChild( str , this->filemask ) != false ; )
			{
				// Allocate Fileobject
				_fileObject* fo = new _fileObject( ignore , ignore , ignore , ignore  , this->directory.getFileName() + str , this->viewType );
				
				fo->setInternalEventHandler( onParentAdd , cb );
				
				// Add User-defined Handler
				fo->setUserEventHandler( onMouseClick , make_callback( &_fileView::eventForwarder ) );
				fo->setUserEventHandler( onMouseDblClick , make_callback( &_fileView::eventForwarder ) );
				
				this->addChild( fo , true );
			}
			break;
		}
	}
}

_fileView::~_fileView(){
	this->removeChildren( true );
}