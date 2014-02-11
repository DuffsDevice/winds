#include <stdio.h>
#include "_type/type.binfile.h"

#include "ini_localization_text_bin.h"
#include "ini_localization_month_bin.h"

_binFile::operator string()
{
	if( this->isExisting() )
		return this->readString();
	
	// Test if this file is a built-in one
	string fn = this->getFileName();
	
	// Convert to lowerspace
	transform( fn.begin() , fn.end() , fn.begin() , ::tolower );
	
	if( fn == _direntry::replaceASSOCS( "%SYSTEM%/localizationtext.ini") )
	{
		string str = (const _char*)ini_localization_text_bin;
		str.resize( ini_localization_text_bin_size );
		return move( str );
	}
	else if( fn == _direntry::replaceASSOCS( "%SYSTEM%/localizationmonth.ini") )
	{
		string str = (const _char*)ini_localization_month_bin;
		str.resize( ini_localization_month_bin_size );
		return move( str );
	}

	return "";
}