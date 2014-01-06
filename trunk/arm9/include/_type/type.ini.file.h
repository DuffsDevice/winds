#ifndef _WIN_T_INIFILE_
#define _WIN_T_INIFILE_

#include "_type/type.h"
#include "_type/type.ini.h"
#include "_type/type.direntry.h"

class _iniFile : public _ini , protected _direntry
{
	protected:
		
		bool creation; // Flag whether the registry was created this time
		
	public:
		
		//! Ctor
		_iniFile( const string& filename );
		
		//! Flush the content to disk
		void flush();
		
		//! Virtual Dtor
		virtual ~_iniFile(){
			this->flush();
		}
};

#endif
	