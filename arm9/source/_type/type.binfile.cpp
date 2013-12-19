#include <stdio.h>
#include "_type/type.binfile.h"

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
	{
		string str = (const _char*)localizationText_bin;
		str.resize( localizationText_bin_size );
		return move( str );
	}
	else if( fn == _direntry::replaceASSOCS( "%SYSTEM%/localizationmonth.ini") )
	{
		string str = (const _char*)localizationMonth_bin;
		str.resize( localizationMonth_bin_size );
		return move( str );
	}

	return "";
}