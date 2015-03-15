#ifndef _WIN_T_DIRENTRYSHORTCUT_
#define _WIN_T_DIRENTRYSHORTCUT_

#include <_type/type.h>
#include <_type/type.direntry.h>
#include <_type/type.ini.h>
#include <_type/type.arguments.h>

class _shortcut : public _direntry {
	
	private:
		
		mutable _bitmap			image;
		mutable string*			destination;
		mutable _args*	fallbackArgs;
		mutable _args*	standardArgs;
		
		void readAttributes() const ;
		
	public:
		
		//! Ctor
		_shortcut( string&& fn );
		_shortcut( const string& fn ) :
			_shortcut( string(fn) )
		{}
		
		//! Get the Destination the shortcut pointes to
		const string& getDestination() const {
			this->readAttributes();
			return *this->destination;
		}
		
		//! Execute the thing behind the shortcut
		_programHandle execute( _args args = _args() );
		
		//! get the shortcuts extension, which is anyway null
		const string& getExtension() const {
			static string empty("");
			return empty;
		}
		
		//! Geth the shortcuts file-image
		_bitmap getFileImage() const ;
		
		//! Dtor
		~_shortcut();
};

#endif