#ifndef _WIN_T_CWDCHANGER_
#define _WIN_T_CWDCHANGER_

#include "_type/type.h"
#include "_type/type.direntry.h"
#include "_type/type.flexptr.h"

namespace unistd{
#include <unistd.h>
}

class _cwdChanger
{
	private:
		
		flex_ptr<string> oldCwd;
		
		static string getCWD(){
			_char val[PATH_MAX];
			unistd::getcwd( val , PATH_MAX );
			return val;
		}
		
		static void setCWD( const string& dir ){
			unistd::chdir( dir.c_str() );
		}
		
		//! Ctor
		_cwdChanger( const _cwdChanger& ){}
	
	public:
		
		//! Ctor
		_cwdChanger( const string& newDir ) :
			oldCwd( getCWD() )
		{
			this->setCWD( _direntry::replaceASSOCS( newDir ) );
		}
		
		//! Move Ctor
		_cwdChanger( _cwdChanger&& other ) :
			oldCwd( move( other.oldCwd ) )
		{}
		
		//! Dtor
		~_cwdChanger(){
			if( oldCwd )
				this->setCWD( *oldCwd );
		}
};

#endif