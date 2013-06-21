#ifndef _WIN_T_CWDCHANGER_
#define _WIN_T_CWDCHANGER_

#include "_type/type.h"
#include "_type/type.direntry.h"

namespace unistd{
#include <unistd.h>
}

class _cwdchanger
{
	private:
		
		string oldCwd;
		
		string getCWD(){
			_char val[PATH_MAX];			
			unistd::getcwd( val , PATH_MAX );
			return val;
		}
		
		void setCWD( const string& dir ){
			unistd::chdir( dir.c_str() );
		}
		
		//! Ctor
		_cwdchanger( const _cwdchanger& ){}
	
	public:
		
		//! Ctor
		_cwdchanger( const string& newDir ) :
			oldCwd( getCWD() )
		{
			setCWD( _direntry::replaceASSOCS( newDir ) );
		}
		
		//! Move Ctor
		_cwdchanger( _cwdchanger&& other ) :
			oldCwd( move( other.oldCwd ) )
		{
			other.oldCwd.clear();
		}
		
		//! Dtor
		~_cwdchanger(){
			if( !oldCwd.empty() )
				setCWD( oldCwd );
		}
};

#endif