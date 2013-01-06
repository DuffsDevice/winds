#ifndef _WIN_T_INI_
#define _WIN_T_INI_

#include <stdio.h>
#include <map>

#include "_type/type.h"

typedef map<string,map<string,string>> _iniStructure;

class _ini
{
	
	private:
		
		string input;
		string output;
		
		_iniStructure array;
		
	public:
		
		// Returns either -1 for success or the line number of error
		_s16 read();
		
		void write();
		
		_ini( string str ) : input( str ), output( "" ) { }
		
		_iniStructure& getMap();
		
		string&	getString();
		
		virtual ~_ini(){}
		
};


#endif