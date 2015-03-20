#include <_gadget/gadget.fileview.h>
#include <_type/type.gadget.helpers.h>
#include <_type/type.windows.h>
#include <_type/type.windows.soundbank.h>
#include <_controller/controller.localization.h>

void _fileView::initializeMenu()
{
	if( !_fileView::defaultMenu )
	{
		_fileView::defaultMenu = new _menu(
			{
				{ 1 , _localizationController::getBuiltInString("lbl_refresh") } ,
				{ 2 , _menu::divider } ,
				{ 3 , _menuEntry( _localizationController::getBuiltInString("lbl_new") , 1 ) } ,
				{ 4 , _menu::divider } ,
				{ 5 , _localizationController::getBuiltInString("lbl_properties") }
			}
		);
		_fileView::defaultMenu->setList( 1 ,
			{
				{ 1 , _localizationController::getBuiltInString("lbl_file") } ,
				{ 2 , _localizationController::getBuiltInString("lbl_folder") }
			}
		);
		_fileView::defaultMenu->addHandler( _menuHandlerRule() , make_callback( &_fileView::defaultMenuHandler ) );
	}
}

_fileView::_fileView( _optValue<_coord> x , _optValue<_coord> y
		, _optValue<_length> width , _optValue<_length> height , string path
		, _fileViewType viewType , _fileExtensionList allowedExtensions
		, _callback<_eventHandler>* eventHandler , bool singleClickToExecute , _style&& style
	) :
	_scrollArea( x , y , width , height , _scrollType::meta , _scrollType::meta , style | _style::rightClickable | _style::noTransparentParts )
	, directory( path )
	, viewType( viewType )
	, filemask( allowedExtensions.empty() ? nullptr : new _fileExtensionList( move(allowedExtensions) ) )
	, eventHandler( move(eventHandler) )
	, singleClickToExecute( singleClickToExecute )
{
	this->initializeMenu();
	
	// Set Real Type of gadget
	this->setType( _gadgetType::fileview );
	
	// Set ContextMenu
	this->setInternalEventHandler( onMouseRightClick , _gadgetHelpers::openContextMenu( _fileView::defaultMenu.get() ) );
	
	// Generate _fileObject's
	this->generateChildren();
}


void _fileView::setPath( const string& path , bool preventClick )
{
	this->directory = _direntry( path );
	this->generateChildren();
	
	if( !preventClick )
		_windowsSoundBank::play( _windowsSound::navigation );
}

_callbackReturn _fileView::eventForwarder( _event event )
{
	_fileView* fileView = (_fileView*)event.getGadget<_fileObject>()->getParent();
	
	if( !fileView )
		return use_internal;
	
	// Use custom Handler
	if( fileView->eventHandler ){
		_callbackReturn ret = (*fileView->eventHandler)( move(event) );
		if( ret != use_internal )
			return ret;
	}
	// Use default handler
	if( ( event == _eventType::onMouseDblClick ) != fileView->singleClickToExecute )
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
			//for( string str : { "Haloo.ini" , "2013-10-05 16.55.56.jpg" , "Halihalo.exe" , "Hallo/" } )
			for( _literal str ; this->directory.readChild( str , this->filemask ) != false ; )
			{
				// Allocate Fileobject
				_fileObject* fo = new _fileObject( ignore , ignore , ignore , ignore  , this->directory.getFileName() + str , this->viewType , _style::rightClickable );
				
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

void _fileView::defaultMenuHandler( _u16 listIndex , _u16 entryIndex )
{
	_fileView* fileView = (_fileView*)_gadgetHelpers::openContextMenu::getCurrentSubject();
	
	if( !fileView || fileView->getType() != _gadgetType::fileview )
		return;
	
	if( listIndex == 0 )
	{
		switch( entryIndex )
		{
			case 1: // Refresh
				fileView->generateChildren();
				break;
			case 5: // Properties
				_winds::execute("%SYSTEM%/filedetail.exe -\"" + fileView->directory.getFileName() + "\"" );
				break;
		}
	}
	else if( listIndex == 1 ) // New...
	{
		switch( entryIndex )
		{
			case 1: // File
				printf("Making File!");
				break;
			case 2: // Folder
				printf("Making Folder!");
				break;
		}
	}
}

_uniquePtr<_menu> 				_fileView::defaultMenu;