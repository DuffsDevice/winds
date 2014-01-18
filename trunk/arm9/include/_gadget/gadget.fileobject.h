// Check if already included
#ifndef _WIN_G_FILEOBJECT_
#define _WIN_G_FILEOBJECT_

#include "_type/type.gadget.h"
#include "_type/type.direntry.h"
#include "_type/type.runtimeAttributes.h"
#include "_gadget/gadget.label.h"

enum class _fileViewType : _u8{
	detail = 0,
	list = 1,
	symbol_small = 2,
	symbol_big = 3
};

class _fileObject : public _gadget {
	
	private:
	
		// My Data...
		_direntry*		file;
		_fileViewType	viewType;
		
		bool 			pressed;
		
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn dragHandler( _event );
		static _callbackReturn focusHandler( _event );
		static _callbackReturn updateHandler( _event );
		
	public:
	
		//! Ctor
		_fileObject( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , const string& dir , _fileViewType viewtype = _fileViewType::list , _style&& style = _style() );
		
		//! Dtor
		~_fileObject();
		
		//! Get the _direntry instance that this _filobject refers to
		const _direntry& getDirentry(){
			return *this->file;
		}
		
		//! Execute this file
		void execute( _cmdArgs args = _cmdArgs() , bool openInNewWindow = false );
};

#endif