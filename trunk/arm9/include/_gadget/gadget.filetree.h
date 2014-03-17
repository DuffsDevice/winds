// Check if already included
#ifndef _WIN_G_FILETREE_
#define _WIN_G_FILETREE_

#include "_type/type.gadget.h"
#include "_type/type.flexptr.h"
#include "_gadget/gadget.scrollArea.h"
#include "_gadget/gadget.fileobject.h"
#include "_gadget/gadget.treeview.h"

class _fileTree : public _treeView
{
	private:
	
		_direntry							directory;
		flex_ptr<_fileExtensionList>		filemask;
		flex_ptr<_callback<_eventHandler>>	eventHandler;
		bool								singleClickToExecute : 1;
		bool								folderOnly : 1;
		
		// Member function to forward all events to the 'eventHandler'
		_callbackReturn folderOpenHandler( _event );
		static _callbackReturn eventForwarder( _event );
		
		// Generates all _fileObjects
		void generateChildren();
		
		// Helps to build the tree
		void appendNodesOfDirectory( _gadget* rootNode , _direntry path );
		
		//! Full Ctor
		_fileTree( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string path , _fileExtensionList allowedExtensions , _callback<_eventHandler>* eventHandler , bool folderOnly , bool singleClickToExecute , _style&& style );
		
	public:
	
		//! Method to set the Path
		void setPath( const string& path );
		
		//! Method to get the Path
		string getPath(){ return this->directory.getFileName(); }
		
		//! Tell the _fileTree to reduce 
		void setFileMask( _paramAlloc<_fileExtensionList> allowedExtensions ){
			filemask = allowedExtensions.get();
			this->generateChildren();
		}
		
		//! Allow all extensions
		void removeFileMask(){
			filemask = nullptr;
			this->generateChildren();
		}
		
		//! Set eventHandler to handle onMouseClick, onMouseDblClick
		void setEventHandler( _paramAlloc<_callback<_eventHandler>> eventHandler ){
			this->eventHandler = eventHandler.get();
		}
		
		//! Remove the currently assigned eventHandler
		void removeEventHandler(){
			this->eventHandler = nullptr;
		}
		
		//! Simple Ctor
		_fileTree( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string path , bool folderOnly = false , _fileExtensionList allowedExtensions = {} , bool singleClickToExecute = false , _style&& style = _style() ) :
			_fileTree( move(x) , move(y) , move(width) , move(height) , move(path) , move(allowedExtensions) , nullptr , folderOnly , singleClickToExecute , (_style&&)style )
		{}
		
		//! Ctor with eventHandler
		_fileTree( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string path , _paramAlloc<_callback<_eventHandler>> eventHandler , bool folderOnly = false , _fileExtensionList allowedExtensions = {} , bool singleClickToExecute = false , _style&& style = _style() ) :
			_fileTree( move(x) , move(y) , move(width) , move(height) , move(path) , move(allowedExtensions) , eventHandler.get() , folderOnly , singleClickToExecute , (_style&&)style )
		{}
};

#endif