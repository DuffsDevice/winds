#ifndef _WIN_T_BINARYFILE_
#define _WIN_T_BINARYFILE_

// This Class is mainly useless except for the fact that it can read built-in binary-files

#include "_type/type.h"
#include "_type/type.direntry.h"

class _binfile : _direntry
{
	public:
		
		_binfile( string path ) : _direntry( path ) { }
		
		operator string();
};

#endif