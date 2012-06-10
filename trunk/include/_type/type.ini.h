#ifndef _WIN_T_INI_
#define _WIN_T_INI_

#include <stdio.h>
#include <map>
#include <sstream>
using namespace std;

#include "_type/type.h"

typedef map<string,map<string,string>> _iniStructure;

class _ini
{
	
	private:
		
		static void trim( string& );
		
		istringstream input;
		string output;
		
		_iniStructure array;
		
	public:
		
		bool read();
		
		void write();
		
		_ini( string str ) : input( str ), output( "" ) { }
		
		_iniStructure& getMap();
		
		string&	getString();
		
};


#endif