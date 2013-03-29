#include "_type/type.binfile.h"
#include <stdio.h>

#include "strings_bin.h"

_binfile::operator string()
{
	if( this->isExisting() )
		return this->readString();
	
	// Test if this file is a built-in one
	string fn = this->getFileName();
	
	// Convert to lowerspace
	transform( fn.begin() , fn.end() , fn.begin() , ::tolower );
	
	if( fn == _direntry::replaceASSOCS( "%SYSTEM%/localizedstrings.ini") )
		return string( (const char*)strings_bin );

	return "";
}