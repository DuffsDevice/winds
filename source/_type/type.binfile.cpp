#include "_type/type.binfile.h"
#include <stdio.h>

#include "localizationText_bin.h"
#include "localizationMonth_bin.h"

_binfile::operator string()
{
	if( this->isExisting() )
		return this->readString();
	
	// Test if this file is a built-in one
	string fn = this->getFileName();
	
	// Convert to lowerspace
	transform( fn.begin() , fn.end() , fn.begin() , ::tolower );
	
	if( fn == _direntry::replaceASSOCS( "%SYSTEM%/localizationtext.ini") )
		return string( (const char*)localizationText_bin );
	if( fn == _direntry::replaceASSOCS( "%SYSTEM%/localizationmonth.ini") )
		return string( (const char*)localizationMonth_bin );

	return "";
}