#include "_type/type.binfile.h"
#include "_controller/controller.filesystem.h"
#include <stdio.h>

#include "ini_localization_text_ini.h"
#include "ini_localization_month_ini.h"

_binFile::operator string()
{
	if( this->isExisting() )
		return this->readString();
	
	// Test if this file is a built-in one
	string fn = this->getFileName();
	
	// Convert to lowerspace
	transform( fn.begin() , fn.end() , fn.begin() , ::tolower );
	
	if( fn == _filesystemController::replaceAssocDirs( "%SYSTEM%/localizationtext.ini") )
	{
		string str = (const _char*)ini_localization_text_ini;
		str.resize( ini_localization_text_ini_size );
		return move( str );
	}
	else if( fn == _filesystemController::replaceAssocDirs( "%SYSTEM%/localizationmonth.ini") )
	{
		string str = (const _char*)ini_localization_month_ini;
		str.resize( ini_localization_month_ini_size );
		return move( str );
	}

	return "";
}