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
		_direntry*		file;
		_fileviewType	viewType;
		
		bool 			pressed;
		
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn dragHandler( _event );
		static _callbackReturn focusHandler( _event );
		static _callbackReturn clickHandler( _event );
		static _callbackReturn updateHandler( _event );
		
	public:
	
		//! Ctor
		_fileobject( _optValue<_length> width , _optValue<_length> height , _optValue<_coord> x , _optValue<_coord> y , string&& dir , _fileviewType viewtype = _fileviewType::list , bool singleClickToExecute = false , _style&& style = _style() );
		_fileobject( _optValue<_length> width , _optValue<_length> height , _optValue<_coord> x , _optValue<_coord> y , const string& dir , _fileviewType viewtype = _fileviewType::list , bool singleClickToExecute = false , _style&& style = _style() ) :
			_fileobject( move(width) , move(height) , move(x) , move(y) , string( dir ) , viewType , singleClickToExecute , move(style) )
		{ }
		
		//! Dtor
		~_fileobject();
		
		//! Get the _direntry instance that this _filobject refers to
		const _direntry& getDirentry(){
			return *this->file;
		}
		
		//! Execute this file
		void execute( _cmdArgs&& args = _cmdArgs() , bool openInNewWindow = false );
};

#endif