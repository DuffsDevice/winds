#ifndef _WIN_T_INI_FILE_
#define _WIN_T_INI_FILE_

#include "_type/type.h"
#include "_type/type.ini.h"
#include "_type/type.direntry.h"

class _iniFile : public _ini , public _direntry
{
	private:
		
		//! Flag indicating whether the registry file existed before this _iniFile was allocated
		const bool firstTimeUse;
		
	public:
		
		//! Ctor
		_iniFile( const string& filename );
		
		//! Flush the content to disk
		void flush();
		
		//! Check whether the registry file existed before this _iniFile was allocated
		bool isUsedFirstTime(){ return firstTimeUse; }
		
		//! Virtual Dtor
		virtual ~_iniFile(){
			this->flush();
		}
};

#endif
	