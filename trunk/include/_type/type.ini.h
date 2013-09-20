#ifndef _WIN_T_INI_
#define _WIN_T_INI_

#include <stdio.h>
#include <map>

#include "_type/type.h"

typedef _map<string,_map<string,string>> _iniStructure;

class _ini
{
	
	private:
		
		string input;
		string output;
		
		_iniStructure array;
		
	public:
		
		//! Ctor
		_ini( string str ) : input( str ), output( "" ) { }
		
		//! Returns either -1 for success or the line number of error
		_s16 read();
		
		//! Encode the std::map structure to string
		void write();
		
		//! Get the c++ std::map representation after a previous call to ::read
		const _iniStructure& getMap(){
			return this->array;
		}
		
		//! Get a modifyable version of the std::map representation of the .ini file
		_iniStructure& getMutableMap(){
			return this->array;
		}
		
		//! Get the output encoded ini-string ready to be written into a file
		string&	getString(){
			return this->output;
		}
		
		//! Virtual Dtor
		virtual ~_ini(){}
		
		
		//! Allocate
		static _ini* fromFile( string path );
};


#endif