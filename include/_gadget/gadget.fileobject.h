// Check if already included
#ifndef _WIN_G_FILEOBJECT_
#define _WIN_G_FILEOBJECT_

#include "_type/type.gadget.h"
#include "_type/type.direntry.h"
#include "_type/type.runtimeAttributes.h"
#include "_gadget/gadget.label.h"

enum _fileviewType{
	detail = 0,
	list = 1,
	symbol_small = 2,
	symbol_big = 3
};

class _fileobject : public _gadget {
	
	private:
	
		// My Data...
		_direntry		file;
		_fileviewType	viewType;
		
		bool 			pressed;
		
		static _callbackReturn refreshHandler( _event event );
		static _callbackReturn dragHandler( _event event );
		static _callbackReturn focusHandler( _event event );
		static _callbackReturn doubleClickHandler( _event event );
		
	public:
	
		// Ctor
		_fileobject( _coord x , _coord y , string dir , _fileviewType viewtype = _fileviewType::list , _style&& style = _style() );
		
		// Dtor
		~_fileobject();
		
};

#endif