#ifndef _WIN_T_CWDCHANGER_
#define _WIN_T_CWDCHANGER_

#include <_type/type.h>
#include <_type/type.direntry.h>
#include <_type/type.uniqueptr.h>

class _cwdChanger
{
	private:
		
		_uniquePtr<string>	oldCwd;
		
		// Function to set/get currenly working directory
		static string		getCWD();
		static void			setCWD( const string& dir );
		
		//! Ctor
		_cwdChanger( const _cwdChanger& ){}
	
	public:
		
		//! Ctor
		_cwdChanger( const string& newDir );
		
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