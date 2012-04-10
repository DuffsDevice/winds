#ifndef _WIN_T_INI_
#define _WIN_T_INI_

#include <stdio.h>
#include <string>
#include <map>
#include <sstream>
using namespace std;

#include "_type/type.h"

class _ini{
	
	private:
		
		static void trim( string& );
		
		istringstream str;
		
		map<string,string> array;
		
	public:
		
		bool parse();
		
		_ini( string str ) : str( str ) { }
		
		map<string,string>& getMap();
		
};


#endif