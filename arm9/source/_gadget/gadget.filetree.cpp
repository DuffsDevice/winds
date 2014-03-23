#include "_gadget/gadget.filetree.h"
#include "_type/type.gadget.helpers.h"

_fileTree::_fileTree( _optValue<_coord> x , _optValue<_coord> y
		, _optValue<_length> width , _optValue<_length> height , string path
		, _fileExtensionList allowedExtensions
		, _callback<_eventHandler>* eventHandler , bool folderOnly , bool singleClickToExecute , _style&& style
	) :
	_treeView( x , y , width , height , {} , _scrollType::meta , _scrollType::meta , (_style&&)style )
	, directory( path )
	, filemask( allowedExtensions.empty() ? nullptr : new _fileExtensionList( move(allowedExtensions) ) )
	, eventHandler( move(eventHandler) )
	, singleClickToExecute( singleClickToExecute )
	, folderOnly( folderOnly )
{	
	// Set Real Type of gadget
	this->setType( _gadgetType::filetree );
	
	// Generate _fileObject's
	this->generateChildren();
}


void _fileTree::setPath( const string& path )
{
	this->directory = _direntry( path );
	this->generateChildren();
}

_callbackReturn _fileTree::folderOpenHandler( _event event )
{
	_fileObject* curNode = event.getGadget<_fileObject>();
	
	// Check if we loaded the children already
	if( _treeView::hasChildren( curNode ) )
		return use_internal;
	
	_gadget* nextNode = curNode;
	
	const _direntry& entry = curNode->getDirentry();
	
	//for( string str : { "NewDir/" , "NewDir2/" } )
	for( _literal str ; this->folderOnly ? entry.readChildFolderOnly( str ) : entry.readChild( str , this->filemask ) ; )
	{
		// Allocate Fileobject
		_fileObject* fo = new _fileObject( ignore , ignore , ignore , ignore , entry.getFileName() + str , _fileViewType::list , _style::storeInt( - 1 - abs(curNode->getStyle().val) ) );
		
		if( fo->getDirentry().isDirectory() )
			fo->setUserEventHandler( onOpen , make_callback( this , &_fileTree::folderOpenHandler ) );
		
		// Add User-defined Handler
		fo->setUserEventHandler( onMouseClick , make_callback( &_fileTree::eventForwarder ) );
		fo->setUserEventHandler( onMouseDblClick , make_callback( &_fileTree::eventForwarder ) );
		
		this->addChild( fo , true , nextNode );
		
		// Try to append the next node before the current node
		nextNode = fo;
	}
	
	return use_internal;
}

_callbackReturn _fileTree::eventForwarder( _event event )
{
	_fileTree* fileTree = (_fileTree*)event.getGadget()->getParent();
	
	if( !fileTree )
		return use_internal;
	
	// Use custom Handler
	if( fileTree->eventHandler )
		return (*fileTree->eventHandler)( move(event) );
	// Use default handler
	else if( ( event == _eventType::onMouseDblClick ) != fileTree->singleClickToExecute )
		event.getGadget<_fileObject>()->execute();
	
	return use_internal;
}

void _fileTree::generateChildren()
{
	// Rewind the directory
	this->directory.rewindChildren();
	
	// Clear the filemask vector if it is empty
	if( this->filemask && this->filemask.data()->empty() )
		this->filemask = nullptr;
	
	_vector<_treeViewNode> children;
	
	// Read Children of directory
	//for( string str : { "Haloo.lnk" , "Hallo/" , "2013.jpg" , "Halihalo.exe" } )
	for( _literal str ; this->folderOnly ? this->directory.readChildFolderOnly( str ) : this->directory.readChild( str , this->filemask ) ; )
	{
		// Allocate Fileobject
		_fileObject* fo = new _fileObject( ignore , ignore , ignore , ignore , this->directory.getFileName() + str , _fileViewType::list );
		
		if( fo->getDirentry().isDirectory() )
			fo->setUserEventHandler( onOpen , make_callback( this , &_fileTree::folderOpenHandler ) );
		
		// Add User-defined Handler
		fo->setUserEventHandler( onMouseClick , make_callback( &_fileTree::eventForwarder ) );
		fo->setUserEventHandler( onMouseDblClick , make_callback( &_fileTree::eventForwarder ) );
		
		children.emplace_back( move(fo) );
	}
	
	this->setTree( move(children) );
}