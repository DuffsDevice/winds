// Check if already included
#ifndef _WIN_G_FILEVIEW_
#define _WIN_G_FILEVIEW_

#include <_type/type.gadget.h>
#include <_type/type.uniqueptr.h>
#include <_type/type.sound.static.h>
#include <_gadget/gadget.scrollArea.h>
#include <_gadget/gadget.fileobject.h>

class _fileView : public _scrollArea {
	
	private:
	
		_direntry								directory;
		_fileViewType							viewType;
		_uniquePtr<_fileExtensionList>			filemask;
		_uniquePtr<_callback<_eventHandler>>	eventHandler;
		bool									singleClickToExecute;
		
		// ContextMenu
		static _uniquePtr<_menu>	defaultMenu;
		static void					defaultMenuHandler( _u16 listIndex , _u16 entryIndex );
		void						initializeMenu();
		
		// Member function to forward all events to the 'eventHandler'
		static _callbackReturn eventForwarder( _event );
		
		// Generates all _fileObjects
		void generateChildren();
		
		//! Full Ctor
		_fileView( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string path , _fileViewType viewType , _fileExtensionList allowedExtensions , _callback<_eventHandler>* eventHandler , bool singleClickToExecute , _style&& style );
		
	public:
	
		//! Method to set the Path
		void setPath( const string& path );
		
		//! Method to get the Path
		string getPath(){ return this->directory.getFileName(); }
		
		//! Tell the _fileview to reduce 
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
		_fileView( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string path , _fileViewType viewType , _fileExtensionList allowedExtensions = {} , _style&& style = _style() ) :
			_fileView( move(x) , move(y) , move(width) , move(height) , move(path) , viewType , move(allowedExtensions) , nullptr , false , (_style&&)style )
		{}
		
		//! Ctor with eventHandler
		_fileView( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string path , _fileViewType viewType , _paramAlloc<_callback<_eventHandler>> eventHandler , _fileExtensionList allowedExtensions = {} , _style&& style = _style() ) :
			_fileView( move(x) , move(y) , move(width) , move(height) , move(path) , viewType , move(allowedExtensions) , eventHandler.get() , false , (_style&&)style )
		{}
		
		//! Ctor with singleClickToExecute flag
		_fileView( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string path , _fileViewType viewType , bool singleClickToExecute , _fileExtensionList allowedExtensions = {} , _style&& style = _style() ) :
			_fileView( move(x) , move(y) , move(width) , move(height) , move(path) , viewType , move(allowedExtensions) , nullptr , singleClickToExecute , (_style&&)style )
		{}
		
		//! Dtor
		~_fileView();
};

#endif