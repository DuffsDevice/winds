#ifndef _WIN_T_DIRENTRYSHORTCUT_
#define _WIN_T_DIRENTRYSHORTCUT_

#include "_type/type.h"
#include "_type/type.direntry.h"
#include "_type/type.ini.h"

class _shortcut : public _direntry {
	
	private:
		
		mutable _bitmap	image;
		string*			destination;
		
	public:
		
		//! Ctor
		_shortcut( string&& fn );
		_shortcut( const string& fn ) :
			_shortcut( string(fn) )
		{}
		
		//! Get the Destination the shortcut pointes to
		const string& getDestination();
		
		//! execute the thing behind the shortcut
		bool execute( _cmdArgs args = _cmdArgs() );
		
		//! get the shortcuts extension, which is anyway null
		const ssstring& getExtension() const {
			static ssstring empty("");
			return empty;
		}
		
		//! Geth the shortcuts file-image
		_bitmap getFileImage();
		
		//! Dtor
		~_shortcut();
};

#endif